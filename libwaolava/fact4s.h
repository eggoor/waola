#pragma once

typedef struct fact4s
{
	whost_from_data_factory make_host_from_data;
	whost_view_factory make_host_view;
} waolava_fact4s_t;

waolava_fact4s_t* fact4s_make(const whost_from_data_factory host_from_data_factory,
	const whost_view_factory host_view_factory);

void fact4s_free(waolava_fact4s_t* restrict const fact4s);
