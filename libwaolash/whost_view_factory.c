#include "pch.h"

#include <whost_view_factory.h>

whost_view_t* whost_view_make_impl(whost_t* const host);

whost_view_t* wmake_host_view(whost_t* const host)
{
	return whost_view_make_impl(host);
}
