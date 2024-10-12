/** @file waolava_on_host_modified_cbi_factory.h
* Waola vault host modified callback info factory.
*/

#pragma once

#include "waolava_on_host_modified_cbi_t.h"
#include "waolava_t.h"

typedef waolava_on_host_modified_cbi_t* (*waolava_on_host_modified_cbi_factory)(waolava_t*,
	waolava_on_host_modified_callback);

waolava_on_host_modified_cbi_t* wmake_waolava_on_host_modified_cbi(waolava_t* vault,
	waolava_on_host_modified_callback cb);

void waolava_host_modified_cbi_free(waolava_on_host_modified_cbi_t* const cbi);
