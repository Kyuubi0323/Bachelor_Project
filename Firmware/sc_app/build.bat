@echo off

set CURDIR=%~dp0
set TOPDIR=%1
set MODULE=%2
set option=%3


if "%CROSS_TOOL_PATH%"=="" (
    set CROSS_TOOL_PATH=%CURDIR%..\sc_tool
)

set MAKE_PATH=%CROSS_TOOL_PATH%\gnumake;

if "%COMPILE_ENV_SET_FLAG%"==""  echo --------------set compile tools path------------------- && set PATH=%PATH%;%MAKE_PATH% && set COMPILE_ENV_SET_FLAG=done

if "%option%" == "clean" (
	gnumake -f Makefile clean TOP_DIR=" %CURDIR%\"
) else (
	if not exist %TOPDIR%\sc_sdk_images\%MODULE%\simcom_lib.lib (
		echo the lib of %MODULE% has not exist...
		goto FAIL
	)
	copy /y %TOPDIR%\sc_sdk_images\%MODULE%\simcom_lib.lib %CURDIR%\simcom_lib\libs\simcom_lib.lib
	gnumake -f Makefile TOP_DIR=" %CURDIR%\" DFLAG+="-D%project_macro% -D%project_macro_BT% -D%project_macro_GNSS% -D%project_macro_FS_OLD% -D%project_macro_OVERSEASGNSS%"
)


:END
goto:eof


:FAIL

echo ***************************************
echo              build fail
echo ***************************************

exit /b 1
