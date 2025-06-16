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
set FRONTEND_SRCS=src\frontend\employee_io.c src\frontend\menu_io.c
set BACKEND_SRCS=src\backend\file_handler.c src\backend\list_manager.c src\backend\payroll_logic.c
set MAIN_SRC=main.c

REM Define output executable name
set TARGET=project_payroll.exe

REM Display source files that will be compiled
echo.
%YELLOW% "Source files to be compiled:"
echo.
%YELLOW% "Main source: "
echo %MAIN_SRC%
%YELLOW% "Frontend sources: "
echo %FRONTEND_SRCS%
%YELLOW% "Backend sources: "
echo %BACKEND_SRCS%
echo.

REM --- Compilation Command ---
%CYAN% "Running compilation command..."
echo.
%CC% %CFLAGS% %MAIN_SRC% %FRONTEND_SRCS% %BACKEND_SRCS% -I%INCLUDE_DIR% -o %TARGET%

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