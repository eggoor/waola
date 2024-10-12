#include "pch.h"

#include "wlist.h"
#include "wcont4r.h"
#include "whost.h"
#include "whost_view.h"
#include "wmac_addr.h"
#include "wsockaddr.h"

int main(void)
{
	wlog_set_level(wll_error);
	test_list();
	test_cont4r();
	test_wmac_addr();
	test_wsockaddr();
	test_whost();
	test_whost_view();
}
