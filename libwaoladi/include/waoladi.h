/** @file waoladi.h
* 
* Waola network scanner (aka discoverer) public interface.
*/

#pragma once

/*
Include in precompiled header:

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <WS2tcpip.h>
#endif
*/

#include <waoladi_t.h>

#if __cplusplus
extern "C" {
#endif

/**
 * Discovers all possible hosts against all active network interfaces.
 * Notifices subscribed clients about each discovered host
 * with vault's callback.
 *
 * @param self discoverer interface pointer.
 * @param async_cbi If not null, operation will be
 * performed asynchronously, provided callback function
 * will be called back when operation finished.
 *
 * @return 0 on success, otherwise exit code.
 */
int waoladi_discover(waoladi_t* self,
	const async_operation_data_t* const restrict async_cbi);
void waoladi_cancel_task_s(waoladi_t* self);

#if __cplusplus
}   // Extern C
#endif
