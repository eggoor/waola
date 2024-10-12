#include "pch.h"

#include "waolavat.h"

void test_creation(void)
{
	waolava_t* vault = wmake_waolava_def();
	assert(vault);
	waolava_free(vault);
}

void test_get_host_view_list(void)
{
	waolava_t* vault = wmake_waolava_def();
	assert(waolava_is_empty(vault));
	const wcont4r_t* hosts = waolava_get_host_view_list(vault);
	assert(wcont4r_is_empty_s(hosts));
	waolava_free(vault);
}

void test_add(void)
{
	waolava_t* vault = wmake_waolava_def();
	assert(vault);

	whost_data_t* hd = whost_data_make_s(NULL, NULL,
		NULL, NULL, 0);

	const whost_view_t* hw = waolava_add(vault, hd);
	whost_data_free_s(hd);
	assert(!hw);
	assert(waolava_is_empty(vault));
	
	hd = whost_data_make_s("", "",
		NULL, NULL, 0);

	hw = waolava_add(vault, hd);
	whost_data_free_s(hd);
	assert(!hw);
	assert(waolava_is_empty(vault));

	hd = whost_data_make_s("router", "gateway",
		NULL, NULL, 0);

	hw = waolava_add(vault, hd);
	whost_data_free_s(hd);
	assert(hw);
	assert(!waolava_is_empty(vault));

	waolava_free(vault);
}
