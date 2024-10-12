/** @file waolava_factory.h
* Waola vault factory.
*/

#pragma once

#include "waolava_merge_host_cbi_factory.h"
#include "waolava_on_host_modified_cbi_factory.h"
#include "wmutex_factory.h"

typedef waolava_t* (*waolava_factory)(const wcont4r_factory cont4r_factory,
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
	const waolava_merge_host_cbi_factory waolava_merge_host_cbi_factory);

waolava_t* wmake_waolava_def(void);

void waolava_free(waolava_t* const self);
