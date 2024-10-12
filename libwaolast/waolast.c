#include "pch.h"

#include "waolast_priv.h"

struct waolast
{
	void* data;
	wmutex_t* mutex;
	wmodule_t module;
	int op_code;
};

waolast_t* wstatus_make_impl(const wmutex_factory mutex_factory)
{
	waolast_t* status = walloc_s(sizeof(struct waolast));
	if (!status) {
		wlog_if_level(wll_warning, "No %zuB of free memory for status\n",
			sizeof(struct waolast));
		goto end;
	}

	status->mutex = (*mutex_factory)();
	if (!status->mutex) {
		wfree_s(status);
		status = NULL;
		wlog_if_level(wll_warning, "Unable to make status mutex\n");
		goto end;
	}

	status->data = NULL;
	status->module = wm_undefined;
	status->op_code = 0;

end:
	return status;
}

void wstatus_free(waolast_t* const self)
{
	wfree_s(self->data);
	wmutex_free(self->mutex);
	wfree_s(self);
}


const void* wstatus_get(const waolast_t* self,
	wmodule_t* module, int* op_code)
{
	const void* data = NULL;
	
	if (try_lock(self)) {
		*module = self->module;
		*op_code = self->op_code;
		data = self->data;
		release(self);
	}
	else {
		*module = wm_undefined;
	}

	return data;
}

void wstatus_set(waolast_t* self,
	wmodule_t module, int op_code, const void* const restrict data)
{
	if (try_lock(self)) {
		switch (module)
		{
			case wm_scanner:
				wstatus_scanner_set(self, op_code);
				break;
			case wm_discoverer:
				wstatus_discoverer_set(self, op_code, data);
				break;
			case wm_resolver:
				wstatus_resolver_set(self, op_code, data);
				break;
			default:
				wlog_if_level(wll_warning, "Invalid waola module identifier %d passed to %s\n",
					module, __func__);
		}
		release(self);
	}
}

static BOOL try_lock(const waolast_t* self)
{
	return wmutex_try_lock(self->mutex);
}

static void release(const waolast_t* self)
{
	wmutex_release(self->mutex);
}

static void wstatus_scanner_set(waolast_t* self,
	waolasc_opcode_t op_code)
{
	self->module = wm_scanner;
	self->op_code = op_code;
}

static void wstatus_discoverer_set(waolast_t* self,
	int op_code, const void* const restrict data)
{
	self->module = wm_discoverer;
	self->op_code = op_code;
	wfree_s(self->data);
	
	switch (op_code)
	{
		case wdi_scheduling_arp:
		case wdi_sending_arp:
		case wdi_got_arp:
		case wdi_arp_failed:
			self->data = walloc_s(sizeof(uint32_t));
			if (self->data) {
				*(uint32_t*)self->data = *(uint32_t*)data;
			}
			else {
				wlog_if_level(wll_warning, "No %zuB of free memory for IPv4 address\n",
					sizeof(uint32_t));
			}
			break;
		default:
			self->op_code = wdi_undefined;
			self->data = NULL;
			wlog_if_level(wll_warning, "Invalid operation code %d is passed to %s\n",
				op_code, __func__);
	}
}

static void wstatus_resolver_set(waolast_t* self,
	int op_code, const void* const restrict data)
{
	self->module = wm_resolver;
	self->op_code = op_code;
	wfree_s(self->data);
	
	switch (op_code)
	{
		case wre_getaddrinfo:
		case wre_getaddrinfo_failed:
		case wre_gotnameinfo:
		{
			self->data = wstrdup_s((const char*)data);
			break;
		}
		case wre_gotaddrinfo:
		case wre_getnameinfo:
		case wre_getnameinfo_failed:
		case wre_sending_arp:
		case wre_got_arp:
		case wre_arp_failed:
		{
			self->data = walloc_s(sizeof(uint32_t));
			if (self->data) {
				*(uint32_t*)self->data = *(uint32_t*)data;
			}
			else {
				wlog_if_level(wll_warning, "No %zuB of free memory for IPv4 address\n",
					sizeof(uint32_t));
			}
			break;
		}
		default:
			self->op_code = wre_undefined;
			self->data = NULL;
			wlog_if_level(wll_warning, "Invalid operation code %d is passed to %s\n",
				op_code, __func__);
	}
}
