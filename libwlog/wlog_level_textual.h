/** @file wlog_level_textual.h
* Textual log level converter.
*/

#pragma once

#include "include/wlog_level.h"

#if __cplusplus
extern "C" {
#endif

wlog_level_t wlog_level_from_string(char* const restrict log_level_str);

#if __cplusplus
}   // Extern C
#endif
