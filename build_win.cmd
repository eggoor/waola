@echo off
if not "%1" == "x86" if not "%1" == "x64" (
	set ARCH=x64
) else (
	set ARCH=%1
)

echo Building ^for^ %ARCH% architecture

setlocal EnableDelayedExpansion

for /f "usebackq delims=#" %%a in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere" -latest -property installationPath`) do (
	set VsDevCmdFullName=%%a\Common7\Tools\VsDevCmd.bat
)

	if exist "%VsDevCmdFullName%" (
		if "%ARCH%" == "x86" (
			call "%VsDevCmdFullName%" -arch=x86
		) else (
			call "%VsDevCmdFullName%" -arch=amd64
		)
	) else if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\" (
		echo Initializing Visual Studio 2017...
		if "%ARCH%" == "x86" (
			call "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
			) else (
				call "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64
			)
		) else (if exist "%VS140COMNTOOLS%..\..\VC\" (
			echo Initializing Visual Studio 2015...
			if "%ARCH%" == "x86" (
				call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" x86 >NUL
			) else (
				call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" amd64 >NUL
			)
		) else if exist "%VS120COMNTOOLS%..\..\VC\" (
			echo Initializing Visual Studio 2013...
			if "%ARCH%" == "x86" (
				call "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" x86 >NUL
			) else (
				call "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" amd64 >NUL
			)
		) else (
			echo No Visual Studio found
			exit /b
		)
	)
)

if "%ARCH%" == "x86" (
	msbuild /t:restore /p:Configuration=Release Waola.sln
	msbuild /p:Configuration=Release Waola.sln
) else (
	msbuild /t:restore /p:Configuration=Release /p:Platform=x64 Waola.sln
	msbuild /p:Configuration=Release /p:Platform=x64 Waola.sln
)

endlocal
:end