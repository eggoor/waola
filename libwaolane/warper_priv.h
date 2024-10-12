/** @file warper_priv.h
* Private declarations of Waola ARP client.
*/

#pragma once

#include "warper_u.h"

static int thread_callbak(const wthread_cbi_t* const cbi);
static void thread_finished_callback(const wthread_cbi_t* const cbi);
