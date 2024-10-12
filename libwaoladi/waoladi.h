/** @file waoladi.h
*
* Waola network scanner (aka discoverer) internal interface.
*/

#pragma once

#include "include/waoladi.h"

#include "waoladi_fact4s.h"

const waoladi_fact4s_t* waoladi_get_fact4s(const waoladi_t* self);
const waolava_merge_host_cbi_t* waoladi_get_merge_host_cbi(const waoladi_t* self);
wthread_counter_t* waoladi_get_thread_counter(const waoladi_t* self);
waolast_t* waoladi_get_status(const waoladi_t* self);
int waoladi_on_discovery_finished(void* const restrict context, int rc);
wnet_iface_prov_t* waoladi_get_iface_prov(const waoladi_t* self);
