#include "pch.h"

#include "waolavat.h"

int main(void)
{
	wlog_set_level(wll_error);
	test_creation();
	test_get_host_view_list();
	test_add();
}
