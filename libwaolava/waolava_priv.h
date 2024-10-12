/** @file waolava_priv.h
* Waola vault private declarations.
*
* In-memory storage for discovered hosts.
*/

#pragma once

#include "waolava.h"

static whost_view_t* merge_host(const waolava_t* self,
	whost_t* incoming_host, BOOL* acquired, BOOL* new_host);
static BOOL merge_subscriber(const waolava_t*,
	const waolava_subscribe_data_t*);
static void free_host_view_list(const waolava_t*);
static BOOL try_lock_host_list(const waolava_t*);
static BOOL try_lock_subscriber_list(const waolava_t*);
