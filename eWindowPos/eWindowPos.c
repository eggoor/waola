/** @file eWindowPos.c
* Inscribes window rectangle into
* screen rectangle(s).
*/

#include "pch.h"

#include "include/eWindowPos.h"

#include "eWindowPos.h"

#define sqr(x) ((x)*(x))

EWINDOWPOS_API BOOL adjust_wnd_rect(rect_t* const rcWnd,
	const rect_t* const* const screens,
	const int screen_count,
	BOOL* const ext_adjusted)
{
	BOOL success = TRUE;

	if (ext_adjusted) {
		*ext_adjusted = FALSE;
	}

	corner_t cLt = {
		.point = {
			.x = rcWnd->left_top.x,
			.y = rcWnd->left_top.y
		},
		.screen = {
			.x = -1,
			.y = -1
			}
	};

	corner_t cRt = {
		.point = {
			.x = rcWnd->right_bottom.x,
			.y = rcWnd->left_top.y
		},
		 .screen.x = -1,
		.screen.y = -1,
	};

	corner_t cRb = {
		.point = {
			.x = rcWnd->right_bottom.x,
			.y = rcWnd->right_bottom.y
		},
		.screen = {
			.x = -1,
			.y = -1
			}
	};

	corner_t cLb = {
		.point = {
			.x = rcWnd->left_top.x,
			.y = rcWnd->right_bottom.y
		},
		.screen = {
			.x = -1,
			.y = -1
			}
	};

	success &= corner_ensure_screen(&cLt, screens, screen_count);
	success &= corner_ensure_screen(&cRt, screens, screen_count);
	success &= corner_ensure_screen(&cRb, screens, screen_count);
	success &= corner_ensure_screen(&cLb, screens, screen_count);

	if (!success) {
		goto end;
	}

	two_d_size_t wnd_size = {
		.cx = rcWnd->right_bottom.x - rcWnd->left_top.x,
		.cy = rcWnd->right_bottom.y - rcWnd->left_top.y
	};

	two_d_size_t sLt = calc_shift_lt(cLt.point, screens[cLt.screen.x], wnd_size);
	const two_d_size_t sRt = calc_shift_rt(cRt.point, screens[cRt.screen.x], wnd_size);
	two_d_size_t sRb = calc_shift_rb(cRb.point, screens[cRb.screen.x], wnd_size);
	const two_d_size_t sLb = calc_shift_lb(cLb.point, screens[cLb.screen.x], wnd_size);

	sLt.cx = max(sLt.cx, sLb.cx);
	sLt.cy = max(sLt.cy, sRt.cy);
	sRb.cx = min(sRb.cx, sRt.cx);
	sRb.cy = min(sRb.cy, sLb.cy);

	BOOL adjusted = FALSE;

	if (0 != sLt.cx || 0 != sLt.cy
		|| 0 != sRb.cx || 0 != sRb.cy) {
		adjusted = TRUE;
	}

	if (adjusted) {
		cLt.point.x += sLt.cx + sRb.cx;
		cLt.point.y += sLt.cy + sRb.cy;
		cRb.point.x += sRb.cx + sLt.cx;
		cRb.point.y += sRb.cy + sLt.cy;

		rcWnd->left_top.x = max(cLt.point.x, screens[cLt.screen.x]->left_top.x);
		rcWnd->left_top.y = max(cLt.point.y, screens[cLt.screen.y]->left_top.y);
		rcWnd->right_bottom.x = min(cRb.point.x, screens[cRb.screen.x]->right_bottom.x);
		rcWnd->right_bottom.y = min(cRb.point.y, screens[cRb.screen.y]->right_bottom.y);

		if (rcWnd->left_top.x >= screens[cRb.screen.x]->right_bottom.x
			|| rcWnd->left_top.y >= screens[cRb.screen.y]->right_bottom.y
			|| rcWnd->right_bottom.x <= screens[cLt.screen.x]->left_top.x
			|| rcWnd->right_bottom.y <= screens[cLt.screen.y]->left_top.y
			|| rcWnd->left_top.x >= rcWnd->right_bottom.x
			|| rcWnd->right_bottom.y <= rcWnd->left_top.y) {
			success = FALSE;
		}
	}

	if (ext_adjusted) {
		*ext_adjusted = adjusted;
	}

end:
	return success;
}

static BOOL coord_in_segmet(int a, int b, int c)
{
	return c >= a && c <= b;
}

static int calc_distance(int x1, int y1, int x2, int y2)
{
	return (int)sqrt(sqr(x2 - x1) + sqr(y2 - y1));
}

static int pt_calc_distance(point_t pt, int x2, int y2)
{
	return calc_distance(pt.x, pt.y, x2, y2);
}

static int find_closest_screen(point_t pt,
	const rect_t* const* const restrict screens,
	int screen_count)
{
	int screen = -1;

	int* distances = ealloc(sizeof(int) * screen_count * 4);
	if (!distances) {
		goto end;
	}

	for (int i = 0; i < screen_count * 4; i += 4) {
		screen = i / 4;
		distances[i] = pt_calc_distance(pt, screens[screen]->left_top.x,
			screens[screen]->left_top.y);
		distances[i + 1] = pt_calc_distance(pt, screens[screen]->right_bottom.x,
			screens[screen]->left_top.y);
		distances[i + 2] = pt_calc_distance(pt, screens[screen]->right_bottom.x,
			screens[screen]->right_bottom.y);
		distances[i + 3] = pt_calc_distance(pt, screens[screen]->left_top.x,
			screens[screen]->right_bottom.y);
	}

	int min_distance = INT_MAX;

	for (int i = screen_count * 4; i --> 0;) {
		if (min_distance > distances[i]) {
			min_distance = distances[i];
			screen = i / 4;
		}
	}

	efree(distances);

end:
	return screen;
}

static void corner_find_screen(corner_t* const restrict crnr,
	const rect_t* const* const restrict screens, int screen_sount)
{
	for (int i = screen_sount; i --> 0;) {
		const rect_t* screen = screens[i];
		if (coord_in_segmet(screen->left_top.x, screen->right_bottom.x, crnr->point.x))
		{
			crnr->screen.x = i;
		}

		if (coord_in_segmet(screen->left_top.y, screen->right_bottom.y, crnr->point.y))
		{
			crnr->screen.y = i;
		}

		if (crnr->screen.x != -1 && crnr->screen.y != -1) {
			break;
		}
	}
}

static BOOL corner_ensure_screen(corner_t* const restrict crnr,
	const rect_t* const* const restrict screens, int screen_sount)
{
	corner_find_screen(crnr, screens, screen_sount);

	if (-1 != crnr->screen.x && -1 != crnr->screen.y) {
		return TRUE;
	}

	int xScreen = find_closest_screen(crnr->point, screens, screen_sount);

	if (-1 != xScreen) {
		crnr->screen.x = crnr->screen.y = xScreen;
	}

	return -1 != xScreen;
}

static two_d_size_t calc_shift_lt(const point_t lt,
	const rect_t* screen, const two_d_size_t wnd_size)
{
	two_d_size_t shift = { 0 };

	if (pt_in_rect(screen, lt)) {
		return shift;
	}

	if (lt.x > screen->right_bottom.x) {
		shift.cx = screen->right_bottom.x - lt.x - wnd_size.cx;
	}

	if (lt.x + shift.cx < screen->left_top.x) {
		shift.cx += screen->left_top.x - (lt.x + shift.cx);
	}

	if (lt.y > screen->right_bottom.y) {
		shift.cy = screen->right_bottom.y - lt.y - wnd_size.cy;
	}

	if (lt.y + shift.cy < screen->left_top.y) {
		shift.cy += screen->left_top.y - (lt.y + shift.cy);
	}

	return shift;
}

static two_d_size_t calc_shift_rt(const point_t rt,
	const rect_t* screen, const two_d_size_t wnd_size)
{
	two_d_size_t shift = { 0 };

	if (pt_in_rect(screen, rt)) {
		return shift;
	}

	if (rt.x < screen->left_top.x) {
		shift.cx = screen->left_top.x - rt.x + wnd_size.cx;
	}

	if (rt.x + shift.cx > screen->right_bottom.x) {
		shift.cx += screen->right_bottom.x - (rt.x + shift.cx);
	}

	if (rt.y > screen->right_bottom.y) {
		shift.cy = screen->right_bottom.y - rt.y - wnd_size.cy;
	}

	if (rt.y + shift.cy < screen->left_top.y) {
		shift.cy += screen->left_top.y - (rt.y + shift.cy);
	}

	return shift;
}

static two_d_size_t calc_shift_rb(const point_t rb,
	const rect_t* screen, const two_d_size_t wnd_size)
{
	two_d_size_t shift = { 0 };

	if (pt_in_rect(screen, rb)) {
		return shift;
	}

	if (rb.x < screen->left_top.x) {
		shift.cx = screen->left_top.x - rb.x + wnd_size.cx;
	}

	if (rb.x + shift.cx > screen->right_bottom.x) {
		shift.cx += screen->right_bottom.x - (rb.x + shift.cx);
	}

	if (rb.y < screen->left_top.y) {
		shift.cy = screen->left_top.y - rb.y + wnd_size.cy;
	}

	if (rb.y + shift.cy > screen->right_bottom.y) {
		shift.cy += screen->right_bottom.y - (rb.y + shift.cy);
	}

	return shift;
}

static two_d_size_t calc_shift_lb(const point_t lb,
	const rect_t* screen, const two_d_size_t wnd_size)
{
	two_d_size_t shift = { 0 };

	if (pt_in_rect(screen, lb)) {
		return shift;
	}

	if (lb.x > screen->right_bottom.x) {
		shift.cx = screen->right_bottom.x - lb.x - wnd_size.cx;
	}

	if (lb.x + shift.cx < screen->left_top.x) {
		shift.cx += screen->left_top.x - (lb.x + shift.cx);
	}

	if (lb.y < screen->left_top.y) {
		shift.cy = screen->left_top.y - lb.y + wnd_size.cy;
	}

	if (lb.y + shift.cy > screen->right_bottom.y) {
		shift.cy += screen->right_bottom.y - (lb.y + shift.cy);
	}

	return shift;
}

static BOOL pt_in_rect(const rect_t* const restrict rc,
	const point_t pt)
{
	return pt.x >= rc->left_top.x
		&& pt.x <= rc->right_bottom.x
		&& pt.y >= rc->left_top.y
		&& pt.y <= rc->right_bottom.y;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
	LPVOID lpReserved)
{
	return TRUE;
}

