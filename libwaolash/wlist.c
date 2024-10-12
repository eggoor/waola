#include "pch.h"

#include <wlog.h>

#include "include/wmem_s.h"
#include "include/wlist_s.h"

struct wlist_item
{
	wlist_item_t* prev;
	wlist_item_t* next;
	void* value;
};

struct wlist
{
	wlist_item_t* head;
	wlist_item_t* tail;
	wlist_item_t* curr;
};

wlist_t* wlist_make_s(void)
{
	wlist_t* list = NULL;
	
	list = walloc_s(sizeof(struct wlist));

	if (!list) {
		wlog_if_level(wll_warning, "No %zuB of free memory for list\n",
			sizeof(struct wlist));
		goto end;
	}

	list->head = list->curr = list->tail = NULL;

end:
	return list;
}

void wlist_free_s(wlist_t* const restrict list)
{
	if (!list) {
		return;
	}

	wlist_item_t* curr = list->head;

	while (curr) {
		wlist_item_t* next = curr->next;
		wfree_s(curr);
		curr = next;
	}

	wfree_s(list);
}

BOOL wlist_add_tail_s(wlist_t* list, void* const restrict value)
{
	if (!list || !value) {
		return FALSE;
	}

	wlist_item_t* nli = walloc_s(sizeof(struct wlist_item));

	if (nli) {
		nli->value = value;
		nli->prev = list->tail;
		nli->next = NULL;

		if (!list->head) {
			list->head = nli;
		}
		else {
			list->tail->next = nli;
		}

		list->tail = nli;
	}

	return nli != NULL;
}

BOOL wlist_is_empty_s(const wlist_t* const restrict list)
{
	return list == NULL || list->head == NULL;
}

void* wlist_get_first_s(const wlist_t* const restrict list)
{
	void* value = NULL;

	if (list) {
		((wlist_t*)list)->curr = list->head;
		if (list->curr) {
			value = list->curr->value;
		}
	}

	return value;
}

void* wlist_get_next_s(const wlist_t* const restrict list)
{
	void* value = NULL;

	if (list && list->curr) {
		((wlist_t*)list)->curr = list->curr->next;
		if (list->curr) {
			value = list->curr->value;
		}
	}

	return value;
}

void wlist_reverse_s(wlist_t* const restrict list)
{
	if (!list) {
		return;
	}

	wlist_item_t* curr = list->tail;

	while (curr) {
		wlist_item_t* tmp = curr->prev;
		curr->prev = curr->next;
		curr->next = tmp;
		curr = curr->next;
	}

	curr = list->head;
	list->head = list->tail;
	list->tail = curr;
}

void wlist_append_s(wlist_t* acceptor, const wlist_t* const restrict donor)
{
	if (!donor) {
		return;
	}

	void* item = wlist_get_first_s(donor);

	while (item) {
		wlist_add_tail_s(acceptor, item);
		item = wlist_get_next_s(donor);
	}
}

BOOL wlist_delete_s(wlist_t* list, const void* const restrict value)
{
	BOOL deleted = FALSE;

	if (list) {
		wlist_item_t* curr = list->head;

		while (curr) {
			if (curr->value == value) {
				break;
			}
			curr = curr->next;
		}

		if (curr) {
			if (curr->prev) {
				curr->prev->next = curr->next;
			}
			if (curr->next) {
				curr->next->prev = curr->prev;
			}
			if (curr == list->head) {
				list->head = curr->next;
			}
			if (curr == list->tail) {
				list->tail = curr->prev;
			}
			if (list->curr == curr) {
				list->curr = curr->prev ? curr->prev : list->head;
			}
			wfree_s(curr);
			deleted = TRUE;
		}
	}

	return deleted;
}
