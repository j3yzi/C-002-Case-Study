@echo off
setlocal EnableDelayedExpansion

REM ===================================================================
REM Validation Test Compilation Script
REM This script compiles the validation test program.
REM ===================================================================

REM Define PowerShell commands for colored output
set "CYAN=powershell write-host -foregroundcolor cyan -nonewline"
set "YELLOW=powershell write-host -foregroundcolor yellow -nonewline"
set "GREEN=powershell write-host -foregroundcolor green -nonewline"
set "RED=powershell write-host -foregroundcolor red -nonewline"

%CYAN% "Compiling Validation Test..."
echo.

REM Define compiler and flags
set CC=gcc
set CFLAGS=-Wall -Wextra -std=c11 -g
set INCLUDE_DIR=../include

REM Define source files
set TEST_SRC=test_validation.c
set LIB_SRCS=lib/validation.c

REM Define output executable name
set TARGET=test_validation.exe

REM Display source files that will be compiled
echo.
%YELLOW% "Source files to be compiled:"
echo.
%YELLOW% "Test source: "
echo %TEST_SRC%
%YELLOW% "Library sources: "
echo %LIB_SRCS%
echo.

REM --- Compilation Command ---
%CYAN% "Running compilation command..."
echo.
%CC% %CFLAGS% %TEST_SRC% %LIB_SRCS% -I%INCLUDE_DIR% -o %TARGET%

IF %ERRORLEVEL% NEQ 0 (
    echo.
    %RED% "ERROR: Compilation failed!"
    echo.
) ELSE (
    echo.
    %GREEN% "Compilation successful! Executable: "
    echo %TARGET%
    echo.
    %CYAN% "Running validation test..."
    echo.
    %TARGET%
)

pause
