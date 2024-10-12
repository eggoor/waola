#include "pch.h"

#include "stopwatch.h"
#include "threads.h"

static void callbackSimple(const waolava_cbi_t* const cbi);

void testThreads(void)
{
	stopwatch_t* sw = stopwatch_make();
	static waolava_subscribe_data_t host_cbi = {
		.cb = callbackSimple
	};
	waolasc_t* scanner = wmake_waolasc_def_s(NULL, &host_cbi);
	
	stopwatch_start(sw);

	for (uint32_t tc = 8192; tc >= 1; tc /= 16)
	{
		printf("%u: ", tc);
		stopwatch_restart(sw);
		waolasc_set_max_threads_s(scanner, tc);
		waolasc_discover_s(scanner);
		unsigned long long int elapsed_ns = stopwatch_lap(sw);
		const wcont4r_t* host_list = waolasc_get_host_list_s(scanner);
		
		int n = 0;
		whost_view_t* hw = wcont4r_get_first_s(host_list);
		while (hw) {
			++n;
			waolasc_delete_host_s(scanner, hw);
			hw = wcont4r_get_next_s(host_list);
		}
		
		printf(" (%d)\n", n);
		print_elapsed(elapsed_ns);
		waolasc_free_s(scanner);
		scanner = wmake_waolasc_def_s(NULL, &host_cbi);
	}

	waolasc_unsubscribe_vault_s(scanner, &host_cbi);
	waolasc_free_s(scanner);
	stopwatch_free(sw);
}

static void callbackSimple(const waolava_cbi_t* const cbi)
{	
	printf(wva_added == cbi->opCode ? "+" : wva_modified == cbi->opCode ? "~" : "-");
}
