#include "pch.h"

#include "waolare_fact4s.h"

wresolver_host_t* wresolver_host_make_impl(wthread_cbi_t* parent_cbi,
	whost_view_t* hw, const waolava_on_host_modified_cbi_t* host_modified_cbi,
	waolast_t* status);

wresolver_host_t* wmake_resolver_host(wthread_cbi_t* parent_cbi,
	whost_view_t* hw, const waolava_on_host_modified_cbi_t* host_modified_cbi,
	waolast_t* status)
{
	return wresolver_host_make_impl(parent_cbi, hw, host_modified_cbi, status);
}
