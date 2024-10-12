#pragma once

#include "include/waolare.h"
#include "waolare_fact4s.h"

const waolare_fact4s_t* waolare_get_fact4s(const waolare_t* self);
wthread_counter_t* waolare_get_thread_counter(const waolare_t* self);
waolast_t* waolare_get_status(const waolare_t* self);
wnet_iface_prov_t* waolare_get_iface_prov(const waolare_t* self);
const waolava_on_host_modified_cbi_t* waolare_get_host_modified_cbi(const waolare_t* self);
BOOL waolare_get_is_async(const waolare_t* self);
void waolare_on_resolving_finished(waolare_t* const restrict context);
