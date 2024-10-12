/** @file waolava.h
* Waola vault internal interface.
*
* In-memory storage for discovered hosts.
*/

#pragma once

#include "include/waolava.h"

void notify_subscribers(const waolava_t*,
	waolava_op_t, whost_view_t*);
