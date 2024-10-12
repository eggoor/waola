#pragma once

/*
Include in precompiled header:

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
#include <stddef.h>
*/

#if __cplusplus
extern "C" {
#endif

void* walloc_s(size_t bytes);
void wfree_s(void* const restrict p);
void* wrealloc_s(void* const restrict p, size_t bytes);

#ifdef _DEBUG
BOOL wheap_validate_s(void* const restrict p);
#endif

#if __cplusplus
}   // Extern C
#endif
