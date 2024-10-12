#pragma once

typedef struct TPermanentStorageCbd
{
	HKEY hHosts;
	LPCTSTR pszHostId;
	HDPA hostDataList;
} PermanentStorageCbd;

typedef void (*PermanentStorageCb)(PermanentStorageCbd* const restrict cbd);
