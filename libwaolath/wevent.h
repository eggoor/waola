#pragma once

#include "include/wevent.h"

int wevent_do_make_impl(void** const restrict ppe);
void wevent_do_free_impl(void* const restrict e);

BOOL wevent_wait_impl(void* const restrict e, int timeout_ms);
void wevent_set_impl(void* const restrict e);
