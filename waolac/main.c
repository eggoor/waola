#include "pch.h"

#include "waolac.h"

int main(int argc, char* argv[])
{
	wlog_try_set_level_from_env();
	
	int rc = argc > 1
		? waola_s(wmake_cont4r_s, wmake_mutex, wmake_thread_cbi,
			wmake_thread, wmake_net_iface, wmake_net_iface_prov,
			wmake_thread_counter, (const char**)argv + 1, (argc - 1))
		: discover(wmake_cont4r_s);
	
	return errno_to_exit_code(rc);
}
