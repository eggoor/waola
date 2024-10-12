/** @file libwaolasc.h
* Waola scanner internal interface.
*/

#pragma once
#include "include/libwaolasc.h"

static BOOL merge_subscriber(const waolasc_t*,
	const waolasc_subscribe_data_t*);
static void notify_subscribers(const waolasc_t* const scanner);
static int do_discover(waolasc_t* self, BOOL async);
static int on_discovery_finished(void* const context);
static int do_refresh(waolasc_t* self, BOOL async);
static int on_refresh_finished(void* const context);
static BOOL register_task(waolasc_t* self, wtask_t task);
static BOOL unregister_task(waolasc_t* self, wtask_t task);
