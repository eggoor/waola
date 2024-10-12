#pragma once

#if __cplusplus
extern "C" {
#endif

typedef struct wlist_item wlist_item_t;
typedef struct wlist wlist_t;

wlist_t* wlist_make_s(void);
void wlist_free_s(wlist_t* const restrict list);

BOOL wlist_add_tail_s(wlist_t* list, void* const restrict value);
BOOL wlist_delete_s(wlist_t* list, const void* const restrict value);
BOOL wlist_is_empty_s(const wlist_t* const restrict list);
void* wlist_get_first_s(const wlist_t* const restrict list);
void* wlist_get_next_s(const wlist_t* const restrict list);
void wlist_reverse_s(wlist_t* const restrict list);
void wlist_append_s(wlist_t* acceptor, const wlist_t* const restrict donor);

#if __cplusplus
}   // Extern C
#endif
