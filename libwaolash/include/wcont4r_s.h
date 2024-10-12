/** @file wcont4r_s.h
* Waola container interface.
*/

#pragma once

#include <waolash_t.h>

void wcont4r_free_s(wcont4r_t* const restrict cont);

BOOL wcont4r_add_s(wcont4r_t* cont, void* const restrict value);
BOOL wcont4r_is_empty_s(const wcont4r_t* const restrict cont);
void* wcont4r_get_first_s(const wcont4r_t* const restrict cont);
void* wcont4r_get_next_s(const wcont4r_t* const restrict cont);
void wcont4r_reverse_s(wcont4r_t* const restrict cont);
BOOL wcont4r_delete_s(wcont4r_t* cont, const void* const restrict value);
