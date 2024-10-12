#include "pch.h"

#include "fact4s.h"

waolava_fact4s_t* fact4s_make(const whost_from_data_factory host_from_data_factory,
	const whost_view_factory host_view_factory)
{
	waolava_fact4s_t* fact4s = walloc_s(sizeof(struct fact4s));

	if (!fact4s) {
		wlog_if_level(wll_warning, "No %zuB of free memory for struct fact4s\n",
			sizeof(struct fact4s));
		goto end;
	}

	fact4s->make_host_from_data = host_from_data_factory;
	fact4s->make_host_view = host_view_factory;

end:
	return fact4s;
}

void fact4s_free(waolava_fact4s_t* restrict const fact4s)
{
	wfree_s(fact4s);
}
