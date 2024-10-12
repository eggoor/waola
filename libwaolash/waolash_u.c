#include "pch.h"

#include "include/libwaolash.h"

int get_system_error_s(void)
{
	return errno;
}

int get_socket_error_s(void)
{
	return errno;
}

int waola_prepare(void)
{
	return 0;
}

int waola_finish(void)
{
	return 0;
}

int closesocket(int s)
{
	return close(s);
}
