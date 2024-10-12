/** @file wnet_iface_u.c
* Unices-specific implementation of Waola network interface wrapper.
*/

#include "pch.h"

#include "warper_u.h"

#include "wnet_iface.h"

int wnet_iface_prepare_for_arp(wnet_iface_t* self)
{
	warper_t* warper = wnet_iface_get_arper(self);
	int rc = warper_setup_communication_device(warper, self);

	if (0 == rc) {
		warper_prepare_req_buffer(warper, self);
	}
	else {
		wlog_if_level(wll_warning, "Setup communication device failed\n");
	}

	return rc;
}
