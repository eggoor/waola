#include "pch.h"

#include "wcont4r.h"

void test_cont4r(void)
{
	do_test_cont4r();
}

void do_test_cont4r(void)
{
	assert(wmake_cont4r_s(wct_undefined) == NULL);
	assert(wmake_cont4r_s(wct_list + 1) == NULL);
	wcont4r_t* cont4r = wmake_cont4r_s(wct_list);
	assert(cont4r);
	assert(wcont4r_is_empty_s(cont4r));

	int arr[] = { 4, 3, 2, 1, 0 };
	for (int i = ARRSIZE(arr); i --> 0;) {
		wcont4r_add_s(cont4r, arr + i);
	}

	int* n = wcont4r_get_first_s(cont4r);
	for (int i = 0; i < ARRSIZE(arr); ++i) {
		assert(*n == i);
		n = wcont4r_get_next_s(cont4r);
	}

	wcont4r_reverse_s(cont4r);

	n = wcont4r_get_first_s(cont4r);

	for (int i = ARRSIZE(arr); i --> 0;) {
		assert(*n == i);
		n = wcont4r_get_next_s(cont4r);
	}

	wcont4r_free_s(cont4r);
}
