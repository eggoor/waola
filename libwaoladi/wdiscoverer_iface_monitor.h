#pragma once

#include "wdiscoverer_iface.h"

static void cancel(wdiscoverer_iface_monitor_t* self);
static int thread_callbak(const wthread_cbi_t* const thread_cbi);
static int monitor_iface_discoverer(const wthread_cbi_t* const restrict thread_cbi);
static void thread_finished_callback(const wthread_cbi_t* const cbi);
