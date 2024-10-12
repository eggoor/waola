/** @file wasync_callback.h
* Waola scanner asynchronous operation callback info.
*/

#pragma once

typedef int (*async_operation_finished_cb)(void*);

/**
 * \struct Waola asynchronous operation callback data.
 */
typedef struct async_operation_data {
	void* caller;
	async_operation_finished_cb cb;
} async_operation_data_t;
