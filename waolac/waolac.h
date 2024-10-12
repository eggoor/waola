/** @file waolac.h
* Waola console interface client.
*/

#pragma once

/**
 * Discovers all possible hosts against all active network interfaces.
 * Prints MAC address, IP address and hosname for each discovered host.
 *
 * @return 0 on success, otherwise error code.
 */
int discover(const wcont4r_factory cont4r_factory);

/**
 * Prints routine usage.
 *
 * @param argv0 executable path.
 *
 * @return EX_USAGE.
 */
int print_usage(const char* const restrict argv0);

/**
 * Extracts relative file name from full one.
 *
 * @param full_name file full name.
 *
 * @return relative file name.
 */
const char* get_filename(const char* const restrict full_name);


/**
 * Converts errno to exit code.
 *
 * @param rc error number.
 *
 * @return exit code.
 */
int errno_to_exit_code(int rc);
