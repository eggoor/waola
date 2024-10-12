#include "pch.h"

#include "wmac_addr.h"

void test_wmac_addr(void)
{
	assert(wmac_addr_from_string_s(NULL) == NULL);
	assert(wmac_addr_from_string_s("") == NULL);
	
	byte* mac_addr = wmac_addr_from_string_s("Lorem ipsum");
	assert(NULL == mac_addr);
	assert(wmac_addr_is_empty_s(mac_addr) == TRUE);
	
	byte* mac_addr2 = wmac_addr_from_string_s("01.23.45.67.89.AB");
	assert(NULL == mac_addr2);

	mac_addr = wmac_addr_from_string_s("01:23:45:67:89:AB");
	assert(mac_addr);

	assert(wmac_addr_compare_s(mac_addr, mac_addr2) > 0);

	mac_addr2 = wmac_addr_from_string_s("01-23-45-67-89-AB");
	assert(mac_addr2);
	assert(wmac_addr_compare_s(mac_addr, mac_addr2) == 0);

	wmac_addr_free_s(mac_addr2);

	mac_addr2 = wmac_addr_from_string_s("0123456789AB");
	assert(mac_addr2);
	assert(wmac_addr_compare_s(mac_addr, mac_addr2) == 0);

	wmac_addr_free_s(mac_addr2);
	wmac_addr_free_s(mac_addr);
}
