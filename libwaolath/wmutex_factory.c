#include "pch.h"

#include <wmutex_factory.h>

wmutex_t* wmutex_make_impl(void);

wmutex_t* wmake_mutex(void)
{
	return wmutex_make_impl();
}
