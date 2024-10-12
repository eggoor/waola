#include "pch.h"

#include "waoladi_fact4s.h"

wdiscoverer_net_t* wdiscoverer_net_make_impl(waoladi_t* const restrict di);

wdiscoverer_net_t* wmake_discoverer_net(waoladi_t* const restrict di)
{
	return wdiscoverer_net_make_impl(di);
}
