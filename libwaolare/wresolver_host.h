#pragma once

typedef struct wresolver_host
{
#if defined(DEBUG) || defined(_DEBUG)
	const char* id;
#endif
	wthread_cbi_t* parentCbi;
	wthread_cbi_t* threadCbi;
	whost_view_t* hostView;
	waolast_t* status;
	wthread_t* thread;
} wresolver_host_t;

void wresolver_host_wait_finished(wresolver_host_t* self);
whost_view_t* wresolver_host_get_host_view(const wresolver_host_t* self);
wresolver_hosts_t* wresolver_host_get_parent(const wresolver_host_t* self);
int wresolver_host_finish_resolve(wresolver_host_t* self);
