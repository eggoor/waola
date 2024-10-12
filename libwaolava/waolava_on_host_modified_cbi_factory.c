#include "pch.h"

#include "include/waolava_on_host_modified_cbi.h"

waolava_on_host_modified_cbi_t* wmake_waolava_on_host_modified_cbi(waolava_t* vault,
	waolava_on_host_modified_callback cb)
{
	waolava_on_host_modified_cbi_t* cbi = walloc_s(sizeof(struct waolava_on_host_modified_cbi));

	if (!cbi) {
		wlog_if_level(wll_warning, "No %zuB of free memory for vault callbak info\n",
			sizeof(struct waolava_on_host_modified_cbi));
		goto end;
	}

	cbi->vault = vault;
	cbi->cb = cb;

end:
	return cbi;
}

void waolava_host_modified_cbi_free(waolava_on_host_modified_cbi_t* restrict const cbi)
{
	wfree_s(cbi);
}
