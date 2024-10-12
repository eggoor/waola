/** @file waolast_priv.h
* Waola status provider private declarations.
*/

#pragma once

#include "waolast.h"

/**
 * Tries to lock provider's mutex.
 *
 * @param self status provider interface pointer.
 *
 * @return TRUE if provider's mutex successfully locked.
 */
static BOOL try_lock(const waolast_t* self);

/**
 * Releases provider's mutex.
 *
 * @param self status provider interface pointer.
 */
static void release(const waolast_t* self);
