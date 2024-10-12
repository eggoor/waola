/** @file warper.h
* Internal declarations of Waola ARP client.
*/

#pragma once

#include "include/warper.h"

warper_t* warper_make_impl(const waolane_fact4s_t* fact4s,
	wthread_counter_t* tc);
int warper_free(warper_t* self);

int warper_start_listening(warper_t* self, warper_cb_data_t* const restrict cb_data);
void warper_wait_for_listening_finished(warper_t* self,
	BOOL join_thread);
int warper_get_errno(const warper_t* self);
BOOL warper_is_ready_for_arp(const warper_t* self);
BOOL warper_query_arp(const warper_t* self, uint32_t target_ip);
