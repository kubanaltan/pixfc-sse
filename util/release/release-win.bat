@echo off

::
:: Carry out sanity checks first
::
:: Check arguments
if "%1"=="" (
	echo %0 pixfc dir version tag
	exit /B 1
)
if "%2"=="" (
	echo %0 pixfc dir version tag
	exit /B 1
)

:: Check the version of Visual Studio
if NOT EXIST "%VS90COMNTOOLS%\vsvars32.bat" (
	if NOT EXIST "%VS10COMNTOOLS%\vsvars32.bat" (
		echo Cannot find Visual Studio
		exit /B 1
	) else (
		set cmake_gen=Visual Studio 10
		call "%VS10COMNTOOLS%\vsvars32.bat"
	)
) else (
	call "%VS90COMNTOOLS%\vsvars32.bat"
	set cmake_gen=Visual Studio 9 2008
)

:: Check the directory we are given
if NOT EXIST "%1" (
	echo %1 does not exist
	exit /B 1
)


::
:: Set variables
::
set destdir=pixfc-sse-%2_win32
set build_arch=32

:do_build
:: Go to pixfc build dir
pushd "%1\build"
if NOT %ERRORLEVEL% EQU 0 (
	echo Error switching to build directory
	popd
	exit /B 1
)

:: Remove previous build dir
if EXIST temp (
	rmdir /S /Q temp
)
mkdir temp
cd temp

:: Build pixfc
cmake -G "%cmake_gen%" ..\..
if NOT %ERRORLEVEL% EQU 0 (
	echo CMake failed
	popd
	exit /B 1
)

devenv PixFC-SSE.sln /clean Release
devenv PixFC-SSE.sln /build Release
if NOT %ERRORLEVEL% EQU 0 (
	echo Build falied
	popd
	exit /B 1
)

:: Go back to where we were invoked from
popd

:: create temp dir
if EXIST %destdir% (
	rmdir /S /Q %destdir%
)
mkdir %destdir%

:: copy library and header files
copy /B "%1\build\temp\src\Release\pixfc-sse.lib" "%destdir%"
mkdir "%destdir%\include"
mkdir "%destdir%\include\win"
copy /A "%1\include\pixfc-sse.h" "%destdir%\include"
copy /A "%1\include\win\stdint.h" "%destdir%\include\win"
copy /A README.win "%destdir%\README"
copy /A COPYING "%destdir%\"
copy /A "%1\example.c" "%destdir%\"

:: create zip file
"C:\Program Files\7-Zip\7z.exe" a -tzip "%destdir%.zip" "%destdir%"

:: Delete temp dir
if EXIST %destdir% (
	rmdir /S /Q %destdir%
)

:: Check if we need to build in 64-bit or if we are done
if "%build_arch%"=="32" (
	set build_arch=64
	set destdir=pixfc-sse-%2_win64
	set cmake_gen=%cmake_gen% Win64
	goto do_build
)
