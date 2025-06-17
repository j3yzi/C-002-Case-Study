@echo off
setlocal EnableDelayedExpansion

REM ===================================================================
REM Backend Test Compilation Script for Payroll System
REM This script compiles the backend test suite with all backend modules.
REM It's designed for developers working on backend modules to test them.
REM ===================================================================

REM Define PowerShell commands for colored output
set "CYAN=powershell write-host -foregroundcolor cyan -nonewline"
set "YELLOW=powershell write-host -foregroundcolor yellow -nonewline"
set "GREEN=powershell write-host -foregroundcolor green -nonewline"
set "RED=powershell write-host -foregroundcolor red -nonewline"

%CYAN% "Compiling Backend Test Suite..."
echo.

REM Define compiler and flags
set CC=gcc
set CFLAGS=-Wall -Wextra -std=c11 -g
set INCLUDE_DIR=../../include

REM Define source files
set TEST_SRC=backend_test.c
set BACKEND_SRCS=../backend/file_handler.c ../backend/list_manager.c ../backend/payroll_logic.c ../backend/globals.c
set FRONTEND_SRCS=../frontend/employee_io.c

REM Define output executable name
set TARGET=backend_test.exe

REM Display source files that will be compiled
echo.
%YELLOW% "Source files to be compiled:"
echo.
%YELLOW% "Test source: "
echo %TEST_SRC%
%YELLOW% "Backend sources: "
echo %BACKEND_SRCS%
%YELLOW% "Frontend sources: "
echo %FRONTEND_SRCS%
echo.

REM Create data directory if it doesn't exist
if not exist "..\..\data" mkdir "..\..\data"

REM --- Compilation Command ---
%CYAN% "Running compilation command..."
echo.
%CC% %CFLAGS% %TEST_SRC% %BACKEND_SRCS% %FRONTEND_SRCS% -I%INCLUDE_DIR% -o %TARGET%

IF %ERRORLEVEL% NEQ 0 (
    echo.
    %RED% "ERROR: Compilation failed!"
    echo.
) ELSE (
    echo.
    %GREEN% "Compilation successful! Executable: "
    echo %TARGET%
    echo.
    %CYAN% "Running backend tests..."
    echo.
    %TARGET%
)

pause
