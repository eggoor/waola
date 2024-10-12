#include "pch.h"

#include "include/wmem_s.h"

static int wsockaddr_compare_sockaddr_in(const struct sockaddr_in* l, const struct sockaddr_in* const restrict r);
static int wsockaddr_compare_sockaddr_in6(const struct sockaddr_in6* l, const struct sockaddr_in6* const restrict r);

int wsockaddr_get_len_s(sa_family_t sa_family)
{
	int sa_len = 0;

	switch (sa_family) {
	case AF_INET: {
		sa_len = sizeof(struct sockaddr_in);
		break;
	}
	case AF_INET6: {
		sa_len = sizeof(struct sockaddr_in6);
		break;
	}
	default: {
		wlog_if_level(wll_warning, "Address family %d is not suported\n",
			sa_family);
	}
	}

	return sa_len;
}

BOOL wsockaddr_addr_is_empty_s(const struct sockaddr* const restrict sa)
{
	static byte zero_addr[14] = { 0 };
	return !sa || memcmp(sa->sa_data, zero_addr, 14) == 0;
}

int wsockaddr_compare_s(const struct sockaddr* l, const struct sockaddr* const restrict r)
{
	int res = 0;

	if (l != r) {
		if (!l || !r) {
			res = l > r ? 1 : -1;
		}
		else {
			sa_family_t sa_family = l->sa_family;
			sa_family_t that_sa_family = r->sa_family;

			if (sa_family == that_sa_family) {
				switch (sa_family)
				{
				case AF_INET:
				{
					res = wsockaddr_compare_sockaddr_in((struct sockaddr_in*)l,
						(struct sockaddr_in*)r);
				}
				break;
				case AF_INET6:
				{
					res = wsockaddr_compare_sockaddr_in6((struct sockaddr_in6*)l,
						(struct sockaddr_in6*)r);
				}
				break;
				default:
				{
					size_t sa_size = sa_family == AF_INET6
						? sizeof(struct sockaddr_in6)
						: sizeof(struct sockaddr_in);

					res = memcmp(l, r, sa_size);
				}
				}
			}
			else {
				res = sa_family - that_sa_family;
			}
		}
	}

	return res;
}

int wsockaddr_compare_int_s(const struct sockaddr* const restrict l, uint32_t r)
{
	struct sockaddr_in sar = {
		.sin_family = AF_INET,
		.sin_port = 0,
		.sin_addr.s_addr = r
	};

	return wsockaddr_compare_s(l, (const struct sockaddr*)&sar);
}

struct sockaddr* wsockaddr_from_string_s(const char* const restrict ipAddr)
{
	struct sockaddr* sa = NULL;

	if (!ipAddr) {
		goto end;
	}

	if (ipAddr) {
		struct sockaddr_in sin = { 0 };
		struct sockaddr_in6 sin6 = { 0 };

		if (inet_pton(AF_INET, ipAddr, &sin.sin_addr) == 1) {
			sa = walloc_s(sizeof(struct sockaddr_in));
			if (!sa) {
				wlog_if_level(wll_warning, "No %zuB of free memory for socket sockaddr_in\n",
					sizeof(struct sockaddr_in));
				goto end;
			}
			memcpy(sa, &sin, sizeof(struct sockaddr_in));
			sa->sa_family = AF_INET;
		}
		else if (inet_pton(AF_INET6, ipAddr, &sin6.sin6_addr) == 1) {
			sa = walloc_s(sizeof(struct sockaddr_in6));
			if (!sa) {
				wlog_if_level(wll_warning, "No %zuB of free memory for sockaddr_in6\n",
					sizeof(struct sockaddr_in6));
				goto end;
			}
			memcpy(sa, &sin6, sizeof(struct sockaddr_in6));
			sa->sa_family = AF_INET6;
		}
	}

end:
	return sa;
}

const char* wsockaddr_to_string_s(const struct sockaddr* sa,
	char buffer[], socklen_t buf_size)
{
	const char* res = NULL;
	if (buf_size > 0) {
		buffer[0] = '\0';
	}

	if (sa) {
		switch (sa->sa_family) {
		case AF_INET: {
			struct sockaddr_in* sockaddrIn = (struct sockaddr_in*)sa;
			res = inet_ntop(AF_INET, &sockaddrIn->sin_addr, buffer, buf_size);
			break;
		}
		case AF_INET6: {
			struct sockaddr_in6* sockaddrIn6 = (struct sockaddr_in6*)sa;
			res = inet_ntop(AF_INET6, &sockaddrIn6->sin6_addr, buffer, buf_size);
			break;
		}
		}
	}

	return res;
}

const char* wsockaddr_int_to_string_s(uint32_t ip_addr,
	char buffer[], socklen_t buf_size)
{
	struct sockaddr_in sin = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = ip_addr
	};

	return wsockaddr_to_string_s((const struct sockaddr*)&sin, buffer, buf_size);
}

const char* wsockaddr_in_addr_to_string_s(struct in_addr in_addr,
	char buffer[], socklen_t buf_size)
{
	struct sockaddr_in sin = {
		.sin_family = AF_INET,
		.sin_addr = in_addr
	};

	return wsockaddr_to_string_s((const struct sockaddr*)&sin, buffer, buf_size);
}

struct sockaddr* wsockaddr_dup_s(const struct sockaddr* const restrict src)
{
	struct sockaddr* sa_dup = NULL;

	if (src) {
		const int sa_len = wsockaddr_get_len_s(src->sa_family);

		sa_dup = walloc_s(sa_len);

		if (!sa_dup) {
			wlog_if_level(wll_warning, "No %dB of free memory for socket address\n",
				sa_len);
			goto end;
		}

		memcpy(sa_dup, src, sa_len);
	}

end:
	return sa_dup;
}

uint32_t wsockaddr_to_int(const struct sockaddr* const restrict sa)
{
	uint32_t addr = 0;

	switch (sa->sa_family)
	{
	case AF_INET:
		addr = ((const struct sockaddr_in*)sa)->sin_addr.s_addr;
		break;
	default:
		wlog_if_level(wll_warning, "Unsupported socket address family %d passed to %s\n",
			sa->sa_family, __func__);
	}

	return addr;
}

int wsockaddr_compare_sockaddr_in(const struct sockaddr_in* l,
	const struct sockaddr_in* r)
{
	int res = 0;

	if (l && r) {
		const uint32_t addrL = ntohl(l->sin_addr.s_addr);
		const uint32_t addrR = ntohl(r->sin_addr.s_addr);

		res = addrL - addrR;
	}
	else {
		res = l > r ? 1 : -1;
	}

	return res;
}

static int wsockaddr_compare_sockaddr_in6(const struct sockaddr_in6* l, const struct sockaddr_in6* const restrict r)
{
	int res = 0;

	if (l != r) {
		if (l && r) {
			res = memcmp(l->sin6_addr.s6_addr, r->sin6_addr.s6_addr, sizeof(l->sin6_addr.s6_addr));
		}
		else {
			res = l > r ? 1 : -1;
		}
	}

	return res;
}
