#pragma once

#include "wdiscoverer_host.h"

static int thread_callbak(const wthread_cbi_t* const cbi);
static void thread_finished_callback(const wthread_cbi_t* const cbi);
static int discover_host(wdiscoverer_host_t* self);
static int resolve_hostname(wdiscoverer_host_t* self);
static const wdiscoverer_net_t* get_discoverer_net(wdiscoverer_host_t* self);
static void wdiscoverer_host_make_host(wdiscoverer_host_t* self);
static void wdiscoverer_host_add_to_vault(wdiscoverer_host_t* self);
