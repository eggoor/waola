/** @file wdiscoverer_net.h
*
* Network discoverer.
*/

#pragma once

void wdiscoverer_net_free_s(wdiscoverer_net_t* self);

const waoladi_fact4s_t* wdiscoverer_net_get_fact4s(const wdiscoverer_net_t* self);
waolast_t* wdiscoverer_net_get_status(const wdiscoverer_net_t* self);

int wdiscoverer_net_discover(wdiscoverer_net_t* self, BOOL async);
void wdiscoverer_net_cancel(wdiscoverer_net_t* self);
BOOL wdiscoverer_net_is_task_cancelled(const wdiscoverer_net_t* self);

uint32_t wdiscoverer_net_get_thread_count(const wdiscoverer_net_t* self);
BOOL wdiscoverer_net_increment_thread_count(wdiscoverer_net_t* self);
void wdiscoverer_net_decrement_thread_count(wdiscoverer_net_t* self);
int wdiscoverer_net_get_cached_hosts(wdiscoverer_net_t* self);
