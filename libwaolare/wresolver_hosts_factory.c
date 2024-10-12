#include "pch.h"

#include "waolare_fact4s.h"

wresolver_hosts_t* wresolver_hosts_make_impl(const waolare_t* const restrict parent);

wresolver_hosts_t* wmake_resolver_hosts(const waolare_t* const restrict parent)
{
	return wresolver_hosts_make_impl(parent);
}
