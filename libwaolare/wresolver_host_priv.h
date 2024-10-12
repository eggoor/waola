#pragma once

#include "wresolver_host.h"

static void resolve_hostname(const wresolver_host_t* const restrict hr);
static int thread_callbak(const wthread_cbi_t* const cbi);
static void thread_finished_callback(const wthread_cbi_t* const cbi);
static void child_thread_finished_callback(const wthread_cbi_t* const cbi);

static uint32_t wresolver_host_lookup_in_arp_cache(const wresolver_host_t* self);

static int resolve_host(const wthread_cbi_t* const restrict cbi);
static int do_resolve_host(wresolver_host_t* self);
