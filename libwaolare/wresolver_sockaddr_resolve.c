#include "pch.h"

#include "wresolver_sockaddr_resolve.h"

int wresolver_sockaddr_resolve(whost_view_t* hw, waolast_t* const restrict status)
{
	int rc = 0;
	const char* hostname = whost_view_get_hostname_s(hw);

	if (wstr_is_empty_s(hostname)) {
		rc = EINVAL;
		goto end;
	}

	rc = waola_prepare();
	if (rc) {
		goto end;
	}

	struct addrinfo* addr_info = { 0 };

	wstatus_set(status, wm_resolver, wre_getaddrinfo, hostname);

	rc = getaddrinfo(hostname, NULL, NULL, &addr_info);

	if (NO_ERROR == rc) {
		for (struct addrinfo* p = addr_info; p; p = addr_info->ai_next) {
			if (p->ai_addr && AF_INET == p->ai_addr->sa_family) {
				const uint32_t ip_addr = wsockaddr_to_int(p->ai_addr);
				if (wlog_get_level() >= wll_verbose) {
					char ip_addr_str[INET_ADDRSTRLEN];
					wsockaddr_int_to_string_s(ip_addr, ip_addr_str, INET_ADDRSTRLEN);
					wlog(wll_verbose, "\t%s: Successfully resolved hostname '%s' to IP address %s\n",
						whost_view_get_host_id_s(hw), hostname, ip_addr_str);
				}
				
				whost_view_set_sockarrd_s(hw, p->ai_addr);
				
				wstatus_set(status, wm_resolver, wre_gotaddrinfo, &ip_addr);
				
				break;
			}
		}
	}
	else {
		wstatus_set(status, wm_resolver, wre_getaddrinfo_failed, hostname);
		wlog_if_level((EAI_NONAME != rc && EAI_MEMORY != rc) ? wll_warning : wll_debug,
			"getaddrinfo('%s'): %s (%d)\n", hostname, gai_strerror(rc), rc);
	}

	freeaddrinfo(addr_info);

	rc = waola_finish();

end:
	wlog_if_level(rc ? wll_warning : wll_debug,
		"%s exiting with '%s' (%d)\n",
		__func__, wstrerror_s(rc), rc);

	return rc;
}
