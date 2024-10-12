#pragma once

#include "include/eWindowPos.h"

static BOOL corner_ensure_screen(corner_t* const restrict c,
	const rect_t* const* const restrict screens, int sc);
static void corner_find_screen(corner_t* const restrict cs,
	const rect_t* const* const restrict screens, int sc);
static int find_closest_screen(point_t pt,
	const rect_t* const* const restrict screens, int sc);
static BOOL pt_in_rect(const rect_t* const restrict rc,
	const point_t pt);

static two_d_size_t calc_shift_lt(const point_t lt,
	const rect_t* screen, const two_d_size_t wnd_size);
static two_d_size_t calc_shift_rt(const point_t rt,
	const rect_t* screen, const two_d_size_t wnd_size);
static two_d_size_t calc_shift_rb(const point_t rb,
	const rect_t* screen, const two_d_size_t wnd_size);
static two_d_size_t calc_shift_lb(const point_t lb,
	const rect_t* screen, const two_d_size_t wnd_size);
