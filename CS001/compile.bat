@echo off
setlocal EnableDelayedExpansion
REM compile.bat - Batch script to compile the CS001 project

REM Define PowerShell commands for colored output
set "CYAN=powershell write-host -foregroundcolor cyan -nonewline"
set "YELLOW=powershell write-host -foregroundcolor yellow -nonewline"
set "GREEN=powershell write-host -foregroundcolor green -nonewline"
set "RED=powershell write-host -foregroundcolor red -nonewline"

%CYAN% "Compiling the project..."
echo.

REM Define compiler and flags
set CC=gcc
set CFLAGS=-Wall -Wextra -std=c11 -g

REM Define include directory
set INCLUDE_DIR=include

REM Define source files individually
set UI_SRCS=src\ui\empio.c src\ui\menuio.c
set MODULE_SRCS=src\modules\data.c src\modules\employee.c src\modules\payroll.c 
set LIB_SRCS=src\lib\lismgr.c src\lib\lisops.c src\lib\validation.c src\lib\menu.c src\lib\appwnd.c
set MAIN_SRC=src\main.c

REM Define output executable name
set TARGET=project_payroll.exe

REM Display source files that will be compiled
echo.
%YELLOW% "Source files to be compiled:"
echo.
%YELLOW% "Main source: "
echo %MAIN_SRC%
%YELLOW% "UI sources: "
echo %UI_SRCS%
%YELLOW% "Module sources: "
echo %MODULE_SRCS%
%YELLOW% "Library sources: "
echo %LIB_SRCS%
echo.

REM --- Compilation Command ---
%CYAN% "Running compilation command..."
echo.
%CC% %CFLAGS% %MAIN_SRC% %UI_SRCS% %MODULE_SRCS% %LIB_SRCS% -I%INCLUDE_DIR% -o %TARGET%

IF %ERRORLEVEL% NEQ 0 (
    echo.
    %RED% "ERROR: Compilation failed!"
    echo.
) ELSE (
    echo.
    %GREEN% "Compilation successful! Executable: "
    echo %TARGET%
    echo.
)

pause