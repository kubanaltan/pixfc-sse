:: @echo off

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
if NOT EXIST "%VS100COMNTOOLS%\vsvars32.bat" (
		echo Cannot find Visual Studio 2010
		exit /B 1
) else (
	call "%VS100COMNTOOLS%\vsvars32.bat"
	set cmake_gen=Visual Studio 10
)


:: Check the directory we are given
if NOT EXIST "%1" (
	echo %1 does not exist
	exit /B 1
)


::
:: Set variables
::
set destdir=PixFC-SSE-%2
set build_arch=32
set build_dir="%1\build\temp"
set archive_name="%destdir%_win32"
if EXIST "C:\Program Files\7-Zip\7z.exe" (
	set zip_cmd="C:\Program Files\7-Zip\7z.exe"
) else (
	if NOT EXIST "C:\Program Files (x86)\7-Zip\7z.exe" (
		echo "Cant find 7-zip"
		exit /B 1
	)
	set zip_cmd="C:\Program Files (x86)\7-Zip\7z.exe"
)

:do_build
:: Remove previous build dir
if EXIST "%build_dir%" (
	rmdir /S /Q "%build_dir%"
)
mkdir "%build_dir%"

:: Go to pixfc build dir
pushd "%build_dir%"

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
copy /B "%build_dir%\src\Release\pixfc-sse.lib" "%destdir%"
mkdir "%destdir%\include"
mkdir "%destdir%\include\win"
copy /A "%1\include\pixfc-sse.h" "%destdir%\include"
copy /A "%1\include\win\stdint.h" "%destdir%\include\win"
copy /A README.win "%destdir%\README"
copy /A COPYING "%destdir%\"
copy /A "%1\example.c" "%destdir%\"
copy /A "%1\Changelog" "%destdir%\"
copy /B "%build_dir%\tools\Release\time_conversions.exe" "%destdir%\"

:: create zip file
if EXIST "%archive_name%.zip" (
	del "%archive_name%.zip"
)
%zip_cmd% a -tzip "%archive_name%.zip" "%destdir%"

:: Delete temp dir
if EXIST %destdir% (
	rmdir /S /Q %destdir%
)

:: Check if we need to build in 64-bit or if we are done
if "%build_arch%"=="32" (
	set build_arch=64
	set cmake_gen=%cmake_gen% Win64
	set archive_name="PixFC-SSE-%2_win64"
	goto do_build
)
