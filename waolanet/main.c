#include "pch.h"

#include "wnet_iface.h"
#include "wnet_iface_prov.h"

int main(void)
{
	wlog_set_level(wll_error);
	test_wnet_iface();
	test_wnet_iface_prov();
}
