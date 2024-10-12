#include "pch.h"

#include <wthread_factory.h>

wthread_t* wthread_make_impl(wthread_cbi_t* const restrict cbi);

wthread_t* wmake_thread(wthread_cbi_t* const restrict cbi)
{
	return wthread_make_impl(cbi);
}
