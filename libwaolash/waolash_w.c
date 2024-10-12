#include "pch.h"

static WSADATA wsaData = { 0 };


int get_system_error_s()
{
	return GetLastError();
}

int get_socket_error_s()
{
	return WSAGetLastError();
}

int waola_prepare()
{
	int rc = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (rc) {
		wlog_if_level(wll_warning, "WSAStartup failed: %d\n", rc);
	}

	return rc;
}

int waola_finish()
{
	int rc = WSACleanup();

	if (rc) {
		wlog_if_level(wll_error, "WSACleanup failed: %d\n", rc);
	}

	return rc;
}
