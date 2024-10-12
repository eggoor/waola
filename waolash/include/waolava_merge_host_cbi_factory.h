/** @file waolava_merge_host_cbi_factory.h
* Waola vault merge host callback info factory.
*/

#pragma once

#include "waolava_merge_host_cbi_t.h"

typedef waolava_merge_host_cbi_t* (*waolava_merge_host_cbi_factory)(waolava_t* const,
	waolava_merge_host_callback);

waolava_merge_host_cbi_t* wmake_waolava_merge_host_cbi(waolava_t* vault,
	waolava_merge_host_callback cb);

void waolava_callback_info_free(waolava_merge_host_cbi_t* const cbi);
