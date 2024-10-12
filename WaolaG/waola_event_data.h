#pragma once

#include "WaolagHost.h"

/**
 * \struct waola vault callback data.
 */
typedef struct waola_event_data {
	const void* caller;
	whost_view_t* hostView;
	waolava_op_t opCode;
	WaolagHost* host;
} waola_event_data_t;
