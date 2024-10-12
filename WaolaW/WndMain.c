#include "pch.h"

#include "WaolaW.h"
#include "Log.h"
#include "WndMain.h"

HINSTANCE g_hInst;
HWND g_hwndMain;

LRESULT CALLBACK MainWndProc(const HWND, const UINT,
	const WPARAM, const LPARAM);
static ATOM MyRegisterClass(HINSTANCE hInst);
static int InitInstance(HINSTANCE hInst, int nCmdShow);

static WCHAR szTitle[DEF_STR_BUFFER_SIZE];
static WCHAR szWindowClass[DEF_STR_BUFFER_SIZE];

int APIENTRY wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	InitLog(hInst);
	wlog_if_level(wll_info, "Starting up…\n");

	g_hInst = hInst;

	InitCommonControls();

	LoadString(hInst, IDS_APP_TITLE, szTitle, DEF_STR_BUFFER_SIZE);
	LoadString(hInst, IDC_WAOLAW, szWindowClass, DEF_STR_BUFFER_SIZE);

	MyRegisterClass(hInst);

	int res = InitInstance(hInst, nCmdShow);
	if (res != 0)
	{
		wlog_if_level(wll_fatal, "InitInstance failed: %d\n", res);
		return res;
	}

	HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_WAOLAW));

	MSG msg = { 0 };

	wlog_if_level(wll_info, "Starting message pump…\n");
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	wlog_if_level(msg.wParam == 0 ? wll_info : wll_warning, "Exiting with code %d\n",
		(int)msg.wParam);

	return (int)msg.wParam;
}

static ATOM MyRegisterClass(HINSTANCE hInst)
{
	WNDCLASSEXW wcex = {
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = MainWndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInst,
		.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_WAOLAW)),
		.hCursor = LoadCursor(NULL, IDC_ARROW),
		.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
		.lpszMenuName = MAKEINTRESOURCEW(IDC_WAOLAW),
		.lpszClassName = szWindowClass,
		.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_WAOLAW))
	};

	return RegisterClassExW(&wcex);
}

static int InitInstance(HINSTANCE hInst, int nCmdShow)
{
	if (!WaolawCreate()) {
		wlog_if_level(wll_error, "WaolawCreate failed\n");
		TCHAR szMsg[DEF_STR_BUFFER_SIZE];
		LoadString(g_hInst, IDS_NOT_ENOUGH_MEMORY, szMsg, DEF_STR_BUFFER_SIZE);
		MessageBox(NULL, szMsg, TEXT("Error"), MB_ICONSTOP | MB_OK);
		return EX_SOFTWARE;
	}

	g_hwndMain = CreateWindowEx(0L, szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT,
		CW_USEDEFAULT, 800, 600, NULL, NULL, hInst, NULL);

	if (!g_hwndMain)
	{
		wlog_if_level(wll_error, "CreateWindowEx(szWindowClass) failed\n");
		HandleError(TRUE);
		return EX_OSERR;
	}

	ShowWindow(g_hwndMain, nCmdShow);
	UpdateWindow(g_hwndMain);

	return 0;
}
