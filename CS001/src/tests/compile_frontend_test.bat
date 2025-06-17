@echo off
setlocal EnableDelayedExpansion

REM ===================================================================
REM Frontend Test Compilation Script for Payroll System
REM This script compiles the frontend test suite with all frontend modules.
REM It's designed for developers working on frontend modules to test them.
REM ===================================================================

REM Define PowerShell commands for colored output
set "CYAN=powershell write-host -foregroundcolor cyan -nonewline"
set "YELLOW=powershell write-host -foregroundcolor yellow -nonewline"
set "GREEN=powershell write-host -foregroundcolor green -nonewline"
set "RED=powershell write-host -foregroundcolor red -nonewline"

%CYAN% "Compiling Frontend Test Suite..."
echo.

REM Define compiler and flags
set CC=gcc
set CFLAGS=-Wall -Wextra -std=c11 -g
set INCLUDE_DIR=../../include

REM Define source files
set TEST_SRC=frontend_test.c
set FRONTEND_SRCS=../frontend/employee_io.c ../frontend/menu_io.c

REM Define output executable name
set TARGET=frontend_test.exe

REM Display source files that will be compiled
echo.
%YELLOW% "Source files to be compiled:"
echo.
%YELLOW% "Test source: "
echo %TEST_SRC%
%YELLOW% "Frontend sources: "
echo %FRONTEND_SRCS%
echo.

REM --- Compilation Command ---
%CYAN% "Running compilation command..."
echo.
%CC% %CFLAGS% %TEST_SRC% %FRONTEND_SRCS% -I%INCLUDE_DIR% -o %TARGET%

IF %ERRORLEVEL% NEQ 0 (
    echo.
    %RED% "ERROR: Compilation failed!"
    echo.
) ELSE (
    echo.
    %GREEN% "Compilation successful! Executable: "
    echo %TARGET%
    echo.
    %CYAN% "Running frontend tests..."
    echo.
    %TARGET%
)

pause
