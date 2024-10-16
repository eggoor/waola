#include "pch.h"

#include "include/libwaolash.h"

#include "wmac_addr.h"

void wmac_addr_free_s(byte* const restrict mac_addr)
{
	wfree_s(mac_addr);
}

byte* wmac_addr_dup_s(const byte* const restrict src)
{
	byte* dst = walloc_s(ETHER_ADDR_LEN);

	if (!dst) {
		wlog_if_level(wll_warning, "No %dB of free memory for MAC address",
			ETHER_ADDR_LEN);
		goto end;
	}

	memcpy(dst, src, ETHER_ADDR_LEN);

end:
	return dst;
}

byte* wmac_addr_from_string_s(const char* restrict str)
{
	byte* macAddress = NULL;

	if (!str) {
		goto end;
	}

	const char* str_copy = str;

	macAddress = walloc_s(ETHER_ADDR_LEN);

	if (!macAddress) {
		wlog_if_level(wll_warning, "No %zuB of free memory for mac address\n",
			ETHER_ADDR_LEN);
		goto end;
	}

	int byteCounter = 0;

	while (*str && byteCounter < ETHER_ADDR_LEN) {
		if (!is_hex_digit_char(*str) || !is_hex_digit_char(*(str + 1))) {
			break;
		}
		macAddress[byteCounter++] = make_byte(*str, *(str + 1));
		str += 2;
		while (*str && is_mac_addr_delimeter(*str)) {
			++str;
		}
	}

	if (ETHER_ADDR_LEN != byteCounter) {
		wlog_if_level(wll_warning, "%s: rovided string '%s' isn't recognized as a MAC address; ignored\n",
			__func__, str_copy);
		wfree_s(macAddress);
		macAddress = NULL;
	}

end:
	return macAddress;
}

BOOL wmac_addr_is_empty_s(const byte* const restrict mac)
{
	static byte emptyMac[ETHER_ADDR_LEN] = { 0 };
	return !mac || memcmp(mac, emptyMac, ETHER_ADDR_LEN) == 0;
}

const char* wmac_addr_to_string_s(const byte* const restrict mac_addr,
	char* const restrict buffer)
{
	static const size_t buf_size = 3 * ETHER_ADDR_LEN;

	char* currChar = buffer;

	for (int i = 0; i < ETHER_ADDR_LEN; ++i) {
		sprintf_s(currChar, buf_size - i * 3, "%02X", mac_addr[i]);
		currChar += 2;
		*currChar++ = ':';
	}

	*(currChar - 1) = '\0';
	
	return buffer;
}

int wmac_addr_compare_s(const byte* const restrict l, const byte* const restrict r)
{
	int res = 0;

	if (l != r) {
		if (l && r) {
			res = memcmp(l, r, ETHER_ADDR_LEN);
		}
		else {
			res = l > r ? 1 : -1;
		}
	}

	return res;
}

BOOL is_mac_addr(const char* str)
{
	int byteCounter = 0;

	if (!str) {
		goto end;
	}

	while (*str&& byteCounter < ETHER_ADDR_LEN) {
		if (!is_hex_digit_char(*str) || !is_hex_digit_char(*(str + 1))) {
			break;
		}
		++byteCounter;
		str += 2;
		while (*str && is_mac_addr_delimeter(*str)) {
			++str;
		}
	}

end:
	return ETHER_ADDR_LEN == byteCounter;
}

BOOL is_mac_addr_delimeter(const char ch)
{
	static const char macAddressDelimeters[] = "-:";

	for (size_t i = ARRSIZE(macAddressDelimeters); i --> 0;) {
		if (ch == macAddressDelimeters[i]) {
			return TRUE;
		}
	}

	return FALSE;
}

static const int hextable[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1, 0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,10,11,12,13,14,15,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

BOOL is_hex_digit_char(char ch)
{
	return hextable[ch] != -1;
}

byte make_byte(char hi, char lo)
{
	return (hextable[hi] << 4) | hextable[lo];
}
