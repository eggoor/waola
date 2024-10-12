/** @file wdiscoverer_iface_platform.h
*
* Platform-specific declarations of network interface scanner.
*/

#include "wdiscoverer_iface.h"

static int begin_discovery(wdiscoverer_iface_t* self);
static int do_discover(wdiscoverer_iface_t* self);
static wdiscoverer_host_t* continue_discovery(wdiscoverer_iface_t* self,
	uint32_t target_ip_addr, const byte* target_mac_addr);
