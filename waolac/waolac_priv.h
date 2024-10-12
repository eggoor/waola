/** @file waolac_priv.h
* Waola console interface client private declarations.
*/

#pragma once

#include "waolac.h"

/**
 * Discovered host callback. Called by discovery engine
 * for each discovered host each time any new information
 * about the host is collected.
 *
 * @param cbi information about vault operation caused callback.
 */
static void host_callback(const waolava_cbi_t* const cbi);

/**
 * Prints MAC address, IP address and hostname of the
 * discovered host.
 *
 * @param hw model view of the discovered host.
 */
static void print_host(const whost_view_t* const hw);
