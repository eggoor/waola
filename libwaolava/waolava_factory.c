#include "pch.h"

#include "include/waolava.h"

#include "waolava.h"

waolava_t* waolava_make_impl(const wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory,
	const whost_from_data_factory host_from_data_factory,
	const whost_view_factory host_view_factory,
	const waolava_on_host_modified_cbi_factory waolava_on_host_modified_cbi_factory,
	const waolava_merge_host_cbi_factory waolava_merge_host_cbi_factory);

waolava_t* wmake_waolava(const wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory,
	const whost_from_data_factory host_from_data_factory,
	const whost_view_factory host_view_factory,
	const waolava_on_host_modified_cbi_factory waolava_on_host_modified_cbi_factory,
	const waolava_merge_host_cbi_factory waolava_merge_host_cbi_factory)
{
	return waolava_make_impl(cont4r_factory, mutex_factory,
		host_from_data_factory, host_view_factory,
		waolava_on_host_modified_cbi_factory,
		waolava_merge_host_cbi_factory);
}

waolava_t* wmake_waolava_def(void)
{
	return wmake_waolava(wmake_cont4r_s, wmake_mutex, wmake_host_from_data,
		wmake_host_view, wmake_waolava_on_host_modified_cbi,
		wmake_waolava_merge_host_cbi);
}
