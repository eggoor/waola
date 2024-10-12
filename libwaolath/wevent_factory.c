#include "pch.h"

#include <wevent_factory.h>

wevent_t* wevent_make_impl(void);

wevent_t* wmake_event(void)
{
	return wevent_make_impl();
}
