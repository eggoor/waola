#pragma once

typedef struct point {
	int x;
	int y;
} point_t;

typedef struct two_d_size {
	int cx;
	int cy;
} two_d_size_t;

typedef struct rect {
	point_t left_top;
	point_t right_bottom;
} rect_t;


typedef struct corner {
	point_t point;
	point_t screen;
} corner_t;
