/** @file wdiscoverer_net_priv.h
*
* Private declarations of network scanner.
*/

#pragma once

#include "wdiscoverer_net.h"

static void child_thread_finished_callback(const wthread_cbi_t* const cbi);
static int on_discovery_finished(wdiscoverer_net_t* self);
