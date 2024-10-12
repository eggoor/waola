#include "pch.h"

#include "mem.h"

static void callbackHost(const waolava_cbi_t* const cbi);

#define MAX_HOSTS 256
static whost_view_t* hws[MAX_HOSTS];
static wmutex_t* mutex;
static size_t count;

void testMem(void)
{
	mutex = wmake_mutex();

	static waolava_subscribe_data_t host_cbi = {
		.cb = callbackHost
	};

	waolasc_t* scanner = wmake_waolasc_def_s(NULL, &host_cbi);

	for (int i = 0; i < 3; ++i) {
		printf("#%d:\n", i + 1);

		waolasc_discover_s(scanner);

		for (size_t j = 0; j < count; ++j) {
			const char* id = whost_view_get_host_id_s(hws[j]);
			printf("%zu) %s\n", j + 1, id ? id : "(null)");
		}

		for (size_t j = 0; j < count; ++j) {
			waolasc_delete_host_s(scanner, hws[j]);
		}

		memset(hws, 0, sizeof(hws) / sizeof(*hws));
		count = 0;

		printf("\n");
	}

	waolasc_unsubscribe_vault_s(scanner, &host_cbi);
	waolasc_free_s(scanner);
	wmutex_free(mutex);
}

static void callbackHost(const waolava_cbi_t* const cbi)
{
	const void* pN = whost_view_get_extra_data_s(cbi->hostView);

	if (!pN)
	{
		whost_view_set_extra_data_s(cbi->hostView, &count, sizeof(count));
		wmutex_lock(mutex, INFINITE);
		const char* ip_addr_str = whost_view_get_ip_addr_s(cbi->hostView);
		printf("+ %zu) %s\n", count + 1, ip_addr_str ? ip_addr_str : "(null)");

		if (MAX_HOSTS > count) {
			hws[count] = cbi->hostView;
			++count;
		}
		wmutex_release(mutex);
	}
}
