#include "pch.h"

#include "include/wresolver_hostname.h"

BOOL wresolver_sockaddr_name_resolve(const struct sockaddr* sa,
	char buff[], int ccb, waolast_t* status)
{
	if (!sa) {
		return FALSE;
	}

	const uint32_t ip_addr = wsockaddr_to_int(sa);
	wstatus_set(status, wm_resolver, wre_getnameinfo, &ip_addr);
	
	int rc = getnameinfo(sa, sizeof(struct sockaddr),
		buff, ccb, NULL, 0, 0);

	if (NO_ERROR == rc && !wstr_is_empty_s(buff)) {
		wstatus_set(status, wm_resolver, wre_gotnameinfo, buff);
	}
	else {
		wstatus_set(status, wm_resolver, wre_getnameinfo_failed, &ip_addr);
		
		if (wlog_get_level() >= wll_warning) {
			int socket_error = get_socket_error_s();
			wlog(wll_warning, "getnameinfo returned %d, socket_error %s (%d), hostname buffer == %s\n",
				rc, wstrerror_s(socket_error), socket_error, buff);
		}
	}
	
	return NO_ERROR == rc && !wstr_is_empty_s(buff);
}
