#include "pch.h"

#include "wevent.h"

struct wevent {
	void* eimpl;
};

wevent_t* wevent_make_impl(void)
{
	wevent_t* event = walloc_s(sizeof(struct wevent));

	if (!event) {
		wlog_if_level(wll_warning, "No %zuB of free memory for wevent\n",
			sizeof(struct wevent));
		goto end;
	}

	int rc = wevent_do_make_impl(&event->eimpl);
	if (rc) {
		wlog_if_level(wll_error, "%s: can't create event implementation\n",
			__func__);
		wfree_s(event);
		event = NULL;
	}

end:
	return event;
}

void wevent_free(wevent_t* const restrict event)
{
	wevent_do_free_impl(event->eimpl);
	wfree_s(event);
}

void wevent_wait(wevent_t* self, int timeout_ms)
{
	wevent_wait_impl(self->eimpl, timeout_ms);
}

void wevent_set(wevent_t* self)
{
	wevent_set_impl(self->eimpl);
}
