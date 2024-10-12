#include "pch.h"

#include "PlatformHelper.h"

using namespace WaolaCli;

typedef BOOL(WINAPI* PFN_ISWOW64PROCESS2) (HANDLE, USHORT*, USHORT*);

bool PlatformHelper::Is32BitProcess::get()
{
	USHORT processMachine = IMAGE_FILE_MACHINE_UNKNOWN;
	USHORT nativeMachine = IMAGE_FILE_MACHINE_UNKNOWN;

	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.
	
	auto moduleHandle = GetModuleHandle(TEXT("kernel32"));

	if (moduleHandle)
	{
		auto fnIsWow64Process2 = (PFN_ISWOW64PROCESS2)GetProcAddress(moduleHandle,
			"IsWow64Process2");

		if (fnIsWow64Process2)
		{
			fnIsWow64Process2(GetCurrentProcess(),
				&processMachine, &nativeMachine);
		}
	}

	return processMachine == IMAGE_FILE_MACHINE_I386;
}
