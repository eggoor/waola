/** @file warper_m.h
* Waola ARP client macOS-specific declarations.
*/

#pragma once

#include "warper_u.h"

size_t warper_get_buf_size(const warper_t* self);
void warper_set_buf_size(warper_t* self, size_t buf_size);

static int do_setup_communication_device(warper_t* self, const char* const ifname);
