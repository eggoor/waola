/** @file wdiscoverer_iface_priv.h
*
* Private declarations of network interface scanner.
*/

#pragma once

#include "wdiscoverer_iface.h"

static int wdiscoverer_iface_thread_callbak(const wthread_cbi_t* const cbi);
static void wdiscoverer_iface_thread_finished_callback(const wthread_cbi_t* const cbi);
static void wdiscoverer_iface_child_thread_finished_callback(const wthread_cbi_t* const cbi);
static void wdiscoverer_iface_free_discovery_scheduled_event(wdiscoverer_iface_t* self);
static void start_monitor_progress(wdiscoverer_iface_t* self);
