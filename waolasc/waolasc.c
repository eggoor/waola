/** @file waolasc.c
* Waola scanner implementation for dynamic linking.
*/

#include "pch.h"

#include "include/waolasc.h"

WAOLACOD_API void waolasc_set_log_level(wlog_level_t level)
{
	wlog_set_level(level);
}

WAOLACOD_API waolasc_t* wmake_waolasc_def(const waolasc_subscribe_data_t* state_cbi,
	const waolava_subscribe_data_t* vault_cbi)
{
	return wmake_waolasc_def_s(state_cbi, vault_cbi);
}

WAOLACOD_API void waolasc_free(waolasc_t* const self)
{
	waolasc_free_s(self);
}

WAOLACOD_API BOOL waolasc_subscribe_state(waolasc_t* self,
	const waolasc_subscribe_data_t* waolasc_cbi)
{
	return waolasc_subscribe_state_s(self, waolasc_cbi);
}

WAOLACOD_API BOOL waolasc_unsubscribe_state(waolasc_t* self,
	const waolasc_subscribe_data_t* waolasc_cbi)
{
	return waolasc_unsubscribe_state_s(self, waolasc_cbi);
}

WAOLACOD_API BOOL waolasc_subscribe_vault(waolasc_t* self,
	const waolava_subscribe_data_t* vault_cbi)
{
	return waolasc_subscribe_vault_s(self, vault_cbi);
}

WAOLACOD_API BOOL waolasc_unsubscribe_vault(waolasc_t* self,
	const waolava_subscribe_data_t* vault_cbi)
{
	return waolasc_unsubscribe_vault_s(self, vault_cbi);
}

WAOLACOD_API BOOL waolasc_is_empty(const waolasc_t* self)
{
	return waolasc_is_empty_s(self);
}

WAOLACOD_API wtask_t waolasc_get_current_tasks(const waolasc_t* self)
{
	return waolasc_get_current_tasks_s(self);
}

WAOLACOD_API void waolasc_delete_host(waolasc_t* self, whost_view_t* const restrict hw)
{
	waolasc_delete_host_s(self, hw);
}

WAOLACOD_API void waolasc_set_max_threads(waolasc_t* self, uint32_t max_threads)
{
	waolasc_set_max_threads_s(self, max_threads);
}

WAOLACOD_API int waolasc_discover(waolasc_t* self)
{
	return waolasc_discover_s(self);
}

WAOLACOD_API int waolasc_discover_async(waolasc_t* self)
{
	return waolasc_discover_async_s(self);
}

WAOLACOD_API int waolasc_refresh(waolasc_t* self)
{
	return waolasc_refresh_s(self);
}

WAOLACOD_API int waolasc_refresh_async(waolasc_t* self)
{
	return waolasc_refresh_async_s(self);
}

WAOLACOD_API void waolasc_refresh_host(waolasc_t* self,
	const whost_view_t* hw)
{
	waolasc_refresh_host_s(self, hw);
}

WAOLACOD_API void waolasc_add(waolasc_t* const self, const whost_data_t* const hd)
{
	waolasc_add_s(self, hd);
}

WAOLACOD_API void waolasc_batch_add(waolasc_t* self,
	whost_data_t* hosts[], size_t count)
{
	waolasc_batch_add_s(self, hosts, count);
}

WAOLACOD_API void waolasc_cancel_task(waolasc_t* self)
{
	waolasc_cancel_task_s(self);
}

WAOLACOD_API const void* waolasc_get_status(const waolasc_t* self,
	wmodule_t* module, int* op_code)
{
	return waolasc_get_status_s(self, module, op_code);
}

WAOLACOD_API const wcont4r_t* waolasc_get_host_list(const waolasc_t* self)
{
	return waolasc_get_host_list_s(self);
}

WAOLACOD_API waolasc_t* waolasc_make(const wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory,
	const wevent_factory event_factory,
	const wthread_counter_factory thread_counter_factory,
	const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory,
	const wnet_iface_prov_factory net_iface_prov_factory,
	const whost_factory host_factory,
	const whost_from_data_factory host_from_data_factory,
	const whost_view_factory host_view_factory,
	const waolava_factory waolava_factory,
	const waolava_on_host_modified_cbi_factory waolava_on_host_modified_cbi_factory,
	const waolava_merge_host_cbi_factory waolava_merge_host_cbi_factory,
	const waoladi_factory waoladi_factory,
	const wdiscoverer_net_factory discoverer_net_factory,
	const wdiscoverer_iface_factory discoverer_iface_factory,
	const wdiscoverer_iface_monitor_factory discoverer_iface_monitor_factory,
	const wdiscoverer_host_factory discoverer_host_factory,
	const waolare_factory waolare_factory,
	const wresolver_hosts_factory resolver_hosts_factory,
	const wresolver_host_factory resolver_host_factory,
	const waolast_factory status_factory,
	const waolasc_subscribe_data_t* state_cbi,
	const waolava_subscribe_data_t* vault_cbi)
{
	return waolasc_make_s(cont4r_factory, mutex_factory, event_factory,
		thread_counter_factory, thread_cbi_factory, thread_factory,
		net_iface_factory, net_iface_prov_factory, host_factory,
		host_from_data_factory, host_view_factory, waolava_factory,
		waolava_on_host_modified_cbi_factory, waolava_merge_host_cbi_factory,
		waoladi_factory, discoverer_net_factory, discoverer_iface_factory,
		discoverer_iface_monitor_factory, discoverer_host_factory,
		waolare_factory, resolver_hosts_factory, resolver_host_factory,
		status_factory, state_cbi, vault_cbi);
}
