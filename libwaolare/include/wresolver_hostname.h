/** @file wresolver_hostname.h
* Hostname resolver.
*
* Tries to resolve host socket address to hostname.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

BOOL wresolver_sockaddr_name_resolve(const struct sockaddr* sa,
	char buff[], int ccb, waolast_t* status);

#if __cplusplus
}   // Extern C
#endif
