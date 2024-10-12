#include "pch.h"

#include "waoladi_fact4s.h"

wdiscoverer_iface_monitor_t* wdiscoverer_iface_monitor_make_impl(const waoladi_fact4s_t* fact4s,
	wthread_cbi_t* parent_cbi);

wdiscoverer_iface_monitor_t* wmake_discoverer_iface_monitor(const waoladi_fact4s_t* fact4s,
	wthread_cbi_t* parent_cbi)
{
	return wdiscoverer_iface_monitor_make_impl(fact4s, parent_cbi);
}
