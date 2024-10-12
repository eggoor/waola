#include "pch.h"

#include <wthread_counter_factory.h>

wthread_counter_t* wthread_counter_make_impl(const wmutex_factory mutex_factory);

wthread_counter_t* wmake_thread_counter(const wmutex_factory mutex_factory)
{
	return wthread_counter_make_impl(mutex_factory);
}
