#include "pch.h"

#include "include/wsockaddr.h"

WAOLASHD_API const char* wsockaddr_int_to_string(uint32_t ip_addr,
	char buffer[], int buf_size)
{
	return wsockaddr_int_to_string_s(ip_addr, buffer, buf_size);
}
