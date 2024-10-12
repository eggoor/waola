#include "pch.h"

#include "include/libwaolash.h"

#include "include/wlist_s.h"
#include "include/wcont4r_s.h"

typedef BOOL (*pfn_add)(void*, void*);
typedef BOOL (*pfn_delete)(void*, const void*);
typedef BOOL (*pfn_is_empty)(const void*);
typedef void* (*pfn_get_first)(const void*);
typedef void* (*pfn_get_next)(const void*);
typedef void (*pfn_reverse)(void*);
typedef void (*pfn_free)(void*);

struct wcont4r
{
	BOOL(*add)(void* impl, void* const restrict value);
	BOOL(*delete)(void* impl, const void* const restrict value);
	BOOL(*is_empty)(const void* const restrict impl);
	void* (*get_first)(const void* const restrict impl);
	void* (*get_next)(const void* const restrict impl);
	void (*reverse)(void* const restrict impl);
	void (*free)(void* const restrict impl);
	
	void* impl;
};

wcont4r_t* wcont4r_list_make(wlist_t* const restrict list)
{
	wcont4r_t* cont = walloc_s(sizeof(struct wcont4r));

	if (!cont) {
		wlog_if_level(wll_warning, "No %zuB of free memory for a list container\n",
			sizeof(struct wcont4r));
		goto end;
	}
	
	cont->impl = list;
	cont->add = (pfn_add)wlist_add_tail_s;
	cont->delete = (pfn_delete)wlist_delete_s;
	cont->is_empty = (pfn_is_empty)wlist_is_empty_s;
	cont->get_first = (pfn_get_first)wlist_get_first_s;
	cont->get_next = (pfn_get_next)wlist_get_next_s;
	cont->reverse = (pfn_reverse)wlist_reverse_s;
	cont->free = (pfn_free)wlist_free_s;

end:
	return cont;
}

void wcont4r_free_s(wcont4r_t* const restrict cont)
{
	if (cont) {
		(*cont->free)(cont->impl);
		wfree_s(cont);
	}
}

BOOL wcont4r_add_s(wcont4r_t* cont, void* const restrict value)
{
	return cont
		? (*cont->add)(cont->impl, value)
		: FALSE;
}

BOOL wcont4r_is_empty_s(const wcont4r_t* const restrict cont)
{
	return cont && (*cont->is_empty)(cont->impl);
}

void* wcont4r_get_first_s(const wcont4r_t* const restrict cont)
{
	return cont
		? (*cont->get_first)(cont->impl)
		: NULL;
}

void* wcont4r_get_next_s(const wcont4r_t* const restrict cont)
{
	return cont
		? (*cont->get_next)(cont->impl)
		: NULL;
}

void wcont4r_reverse_s(wcont4r_t* const restrict cont)
{
	if (cont) {
		(*cont->reverse)(cont->impl);
	}
}

BOOL wcont4r_delete_s(wcont4r_t* cont, const void* const restrict value)
{
	return cont
		? (*cont->delete)(cont->impl, value)
		: FALSE;
}
