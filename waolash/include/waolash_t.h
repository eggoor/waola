/** @file waolash_t.h
* Forward declarations of Waola shared types.
*
* Until otherwise specified, for both passed as in-parameters and returned values:
* - ownership isn't transferrend for const pointers;
* - ownership is transferrend for non-const pointers, excepting 'self' one.
*/

#pragma once

/*
Include in precompiled header:

#include <time.h>
#include <stdint.h>
#include <sys/socket.h>
*/

#include "wdefs.h"
#include "wcont4r_type.h"
#include "whost_field.h"
#include "whost_op_result.h"
#include "whost_changed_field.h"

#if __cplusplus
extern "C" {
#endif

	/**
	 * \struct Waola container.
	 */
	typedef struct wcont4r wcont4r_t;

	/**
	 * \struct Waola host plain data without behavior.
	 */
	typedef struct whost_data whost_data_t;

	/**
	 * \struct Waola host.
	 */
	typedef struct whost whost_t;

	/**
	 * \struct Waola host view.
	 */
	typedef struct whost_view whost_view_t;

	/**
	 * \struct Waola asynchronous operation callback data.
	 */
	typedef struct async_operation_data async_operation_data_t;

	/**
	 * Pointer to container factory function.
	 *
	 * @param ct Container type.
	 *
	 * @return container interface pointer.
	 */
	typedef wcont4r_t* (*wcont4r_factory)(wcont_type_t ct);

	/**
	 * Pointer to host factory function.
	 *
	 * @param display_name Optional. The 'display' (aka 'friendly') name of the host.
	 * @param hostname Optional. Host name.
	 * @param sockaddr Optional. IP address.
	 * @param mac_addr Optional. MAC address.
	 * @param lso Time when the host has been last seen online.
	 *
	 * @return host interface pointer.
	 */
	typedef whost_t* (*whost_factory)(const char* display_name, const char* hostname,
		const struct sockaddr* sockaddr, const byte* const mac_addr, time_t lso);

	/**
	 * Pointer to host factory function to maid from host data.
	 *
	 * @param hd Host data.
	 *
	 * @return host interface pointer.
	 */
	typedef whost_t* (*whost_from_data_factory)(const whost_data_t* const hd);

	/**
	 * Pointer to host view factory function.
	 *
	 * @param host Host.
	 *
	 * @return host view interface pointer.
	 */
	typedef whost_view_t* (*whost_view_factory)(whost_t* const host);

#if __cplusplus
}   // Extern C
#endif
