#include "pch.h"

#include "wlist.h"

void test_list(void)
{
	do_test_list();
	test_list_reverse();
}

void test_list_reverse(void)
{
	wlist_t* lst = wlist_make_s();
	assert(wlist_is_empty_s(lst));

	int arr[] = { 4, 3, 2, 1, 0 };
	for (int i = ARRSIZE(arr); i --> 0;) {
		wlist_add_tail_s(lst, arr + i);
	}

	int* n = wlist_get_first_s(lst);
	for (int i = 0; i < ARRSIZE(arr); ++i) {
		assert(*n == i);
		n = wlist_get_next_s(lst);
	}

	wlist_reverse_s(lst);

	n = wlist_get_first_s(lst);

	for (int i = ARRSIZE(arr); i --> 0;) {
		assert(*n == i);
		n = wlist_get_next_s(lst);
	}

	wlist_free_s(lst);
}

void do_test_list(void)
{
	wlist_t* lst = wlist_make_s();
	assert(wlist_is_empty_s(lst));

	int arr[] = { 1, 2, 3, 4 };

	for (int i = ARRSIZE(arr); i --> 1;) {
		wlist_add_tail_s(lst, arr + i);
	}

	int n = ARRSIZE(arr) - 1;
	int* p = wlist_get_first_s(lst);

	while (p) {
		assert(p == arr + n && *p == arr[n--]);
		p = wlist_get_next_s(lst);
	}

	assert(0 == n);

	wlist_delete_s(lst, arr + 1);

	n = ARRSIZE(arr) - 1;
	p = wlist_get_first_s(lst);

	while (p) {
		assert(p == arr + n && *p == arr[n--]);
		p = wlist_get_next_s(lst);
	}

	assert(1 == n);

	wlist_add_tail_s(lst, arr);

	p = wlist_get_first_s(lst);
	assert(p == arr + 3 && *p == arr[3]);

	p = wlist_get_next_s(lst);
	assert(p == arr + 2 && *p == arr[2]);

	p = wlist_get_next_s(lst);
	assert(p == arr && *p == *arr);

	p = wlist_get_next_s(lst);
	assert(!p);

	wlist_delete_s(lst, arr);
	wlist_delete_s(lst, arr + 2);
	wlist_delete_s(lst, arr + 3);

	assert(wlist_is_empty_s(lst));

	wlist_free_s(lst);
}
