#include "pch.h"

#include "include/waolava_merge_host_cbi.h"

waolava_merge_host_cbi_t* wmake_waolava_merge_host_cbi(waolava_t* restrict const vault,
	waolava_merge_host_callback cb)
{
	waolava_merge_host_cbi_t* cbi = walloc_s(sizeof(struct waolava_merge_host_cbi));

	if (!cbi) {
		wlog_if_level(wll_warning, "No %zuB of free memory for vault callbak info\n",
			sizeof(struct waolava_merge_host_cbi));
		goto end;
	}

	cbi->vault = vault;
	cbi->cb = cb;

end:
	return cbi;
}

void waolava_callback_info_free(waolava_merge_host_cbi_t* restrict const cbi)
{
	wfree_s(cbi);
}
