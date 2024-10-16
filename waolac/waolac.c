/** @file waolaс.c
* Waola console interface client implementation.
*/

#include "pch.h"

#include "waolac_priv.h"

static wcont4r_t* hosts;

int discover(const wcont4r_factory cont4r_factory)
{
	int rc = -1;

	hosts = (*cont4r_factory)(wct_list);
	if (!hosts) {
		return EX_OSERR;
	}

	static waolava_subscribe_data_t host_cbi = {
		.cb = host_callback
	};

	waolasc_t* scanner = wmake_waolasc_def_s(NULL, &host_cbi);
	rc = waolasc_discover_s(scanner);

	const whost_view_t* hw = wcont4r_get_first_s(hosts);

	while (hw) {
		if (!whost_view_get_extra_data_s(hw)) {
			print_host(hw);
		}
		hw = wcont4r_get_next_s(hosts);
	}

	waolasc_free_s(scanner);
	wcont4r_free_s(hosts);

	return rc;
}

static void host_callback(const waolava_cbi_t* const restrict cbi)
{
	const BOOL printed = TRUE;

	if (whost_view_get_hostname_s(cbi->hostView)) {
		whost_view_set_extra_data_s(cbi->hostView,
			&printed, sizeof(printed));
		print_host(cbi->hostView);
	}
	else if (wva_added == cbi->opCode) {
		wcont4r_add_s(hosts, cbi->hostView);
	}
}

static void print_host(const whost_view_t* const restrict hw)
{
	const char* mac_addr = whost_view_get_mac_addr_s(hw);
	const char* ip_addr = whost_view_get_ip_addr_s(hw);
	const char* hostname = whost_view_get_hostname_s(hw);

	printf("%s\t%s\t\t%s\n", mac_addr ? mac_addr : "(null)",
		ip_addr ? ip_addr : "(null)", hostname ? hostname : "(null)");
}

int print_usage(const char* const restrict argv0)
{
	const char* filename = get_filename(argv0);
	printf("Usage: %s [<mac_addr>...]\n",
		filename ? filename : "(null)");

	return EX_USAGE;
}

const char* get_filename(const char* const restrict path)
{
	const char* filename = path;

	while (*++filename);
	while (filename != path
		&& '/' != *--filename
		&& '\\' != *filename);

	return filename == path ? filename : filename + 1;
}

int errno_to_exit_code(int rc)
{
	int exit_code;

	switch (rc)
	{
	case 0:
		exit_code = 0;
		break;
	case EPERM:
	case EACCES:
		exit_code = EX_NOPERM;
		break;
	case EINVAL:
	case ERANGE:
		exit_code = EX_DATAERR;
		break;
	case ENOMEM:
		exit_code = EX_OSERR;
		break;
	case EBUSY:
		exit_code = EX_TEMPFAIL;
	break;
	default:
		exit_code = EX_SOFTWARE;
	}

	return exit_code;
}
