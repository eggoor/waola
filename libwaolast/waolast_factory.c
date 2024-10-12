/** @file waolast_factory.c
* Waola status provider factory implementation.
*/

#include "pch.h"

#include "include/waolast.h"

waolast_t* wstatus_make_impl(const wmutex_factory mutex_factory);

waolast_t* wmake_waolast(const wmutex_factory mutex_factory)
{
	return wstatus_make_impl(mutex_factory);
}
