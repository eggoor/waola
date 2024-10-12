#pragma once

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "framework.h"

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include "sysexits.h"

#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <WS2tcpip.h>
#include <Shlobj_core.h>
#include <RichEdit.h>
#include <shellapi.h>

#ifndef _countof
#define _countof(a) (sizeof(a) / sizeof(*a))
#endif

#include <wlog.h>
#include <waolash.h>
#include <waoladi_opcode.h>
#include <waolare_opcode.h>
#include <waolasc.h>
#include <waola.h>
#include <eWindowPos.h>

void swap(const void* restrict* const restrict l,
	const void* restrict* const restrict r);
BOOL IsStringNullOrWhiteSpace(LPCTSTR restrict src);
LPTSTR StrDup(LPCTSTR restrict src);
void HandleError(BOOL messageBox);
void OpenURL(LPCTSTR pszUrl);

#define DEF_STR_BUFFER_SIZE 48
