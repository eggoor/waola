#include "pch.h"

#include "resource.h"

#include "Log.h"

#define LOG_LEVEL_DEF wll_warning

static FILE* logFile;

void InitLog(HINSTANCE hInst)
{
	wlog_level_t logLevel = wll_info;
	if (wlog_try_set_level_from_env()) {
		logLevel = wlog_get_level();
	}

	TCHAR szLogPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL,
		CSIDL_LOCAL_APPDATA, NULL, 0, szLogPath))) {
		size_t nLen = _tcslen(szLogPath);
		szLogPath[nLen++] = _T('\\');
		szLogPath[nLen++] = _T('\0');
		TCHAR szExePath[MAX_PATH];
		nLen = GetModuleFileName(NULL, szExePath, MAX_PATH);

		if (nLen > 0) {
			static WCHAR szAppTitle[DEF_STR_BUFFER_SIZE];
			if (LoadString(hInst, IDS_APP_TITLE, szAppTitle, DEF_STR_BUFFER_SIZE)) {
				nLen = _tcslen(szLogPath);
				_tcscat_s(szLogPath, MAX_PATH - nLen - 1, szAppTitle);
				nLen = _tcslen(szLogPath);
				szLogPath[nLen++] = _T('\\');
				szLogPath[nLen] = _T('\0');

				CreateDirectory(szLogPath, NULL);
			}

			LPCTSTR pFilenameEnd = szExePath + _tcslen(szExePath) - 1;
			while (pFilenameEnd > szExePath
				&& *pFilenameEnd != _T('.')) {
				--pFilenameEnd;
			}

			if (pFilenameEnd > szExePath) {
				LPCTSTR pFilenameBegin = pFilenameEnd - 1;
				while (pFilenameBegin > szExePath
					&& *pFilenameBegin != _T('\\')) {
					--pFilenameBegin;
				}

				if (pFilenameEnd > pFilenameBegin) {
					_tcsncat_s(szLogPath, MAX_PATH - _tcslen(szLogPath) - 1,
						pFilenameBegin + 1, pFilenameEnd - pFilenameBegin - 1);
					_tcsncat_s(szLogPath, MAX_PATH - _tcslen(szLogPath) - 1,
						_T(".log"), 4);

					if (_tfreopen_s(&logFile, szLogPath, _T("w"), stdout)
						|| _tfreopen_s(&logFile, _T("NUL"), _T("w"), stderr)
						|| _dup2(_fileno(stdout), _fileno(stderr))
						|| !SetStdHandle(STD_OUTPUT_HANDLE, (HANDLE)_get_osfhandle(_fileno(stdout)))
						|| !SetStdHandle(STD_ERROR_HANDLE, (HANDLE)_get_osfhandle(_fileno(stderr)))) {
						__debugbreak();
					}
				}
			}
		}
	}
	
	if (LOG_LEVEL_DEF != logLevel) {
		waolash_set_log_level(logLevel);
		waolasc_set_log_level(logLevel);
		wlog_set_level(logLevel);
	}
}

void ReleaseLog()
{
	fclose(logFile);
}
