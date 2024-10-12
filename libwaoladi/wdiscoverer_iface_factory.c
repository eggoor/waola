#include "pch.h"

#include "waoladi_fact4s.h"

wdiscoverer_iface_t* wdiscoverer_iface_make_impl(wthread_cbi_t* parent_cbi,
	wnet_iface_t* nif, const waolava_merge_host_cbi_t* const restrict mergeHostCbi);

wdiscoverer_iface_t* wmake_discoverer_iface(wthread_cbi_t* parent_cbi,
	wnet_iface_t* nif, const waolava_merge_host_cbi_t* const restrict mergeHostCbi)
{
	return wdiscoverer_iface_make_impl(parent_cbi, nif, mergeHostCbi);
}
