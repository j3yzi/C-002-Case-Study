@echo off
setlocal EnableDelayedExpansion
REM compile.bat - Batch script to compile the CS003 project

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

REM Create bin directory if it doesn't exist
if not exist bin mkdir bin

REM Define include directory
set INCLUDE_DIR=..\include

REM Dynamically scan for source files
set "MAIN_SRC=src\main.c"
set "UI_SRCS="
set "MODULE_SRCS="
set "LIB_SRCS="

REM Scan UI sources
for %%F in (src\ui\*.c) do (
    if not defined UI_SRCS (
        set "UI_SRCS=%%F"
    ) else (
        set "UI_SRCS=!UI_SRCS! %%F"
    )
)

REM Scan module sources
for %%F in (src\modules\*.c) do (
    if not defined MODULE_SRCS (
        set "MODULE_SRCS=%%F"
    ) else (
        set "MODULE_SRCS=!MODULE_SRCS! %%F"
    )
)

REM Scan library sources
for %%F in (..\include\src\*.c) do (
    if not defined LIB_SRCS (
        set "LIB_SRCS=%%F"
    ) else (
        set "LIB_SRCS=!LIB_SRCS! %%F"
    )
)

REM Define output executable name
set TARGET=bin\project_student_records.exe

REM Display source files that will be compiled
echo.
%YELLOW% "Source files to be compiled:"
echo.
%YELLOW% "Main source: "
echo %MAIN_SRC%

%YELLOW% "UI sources: "
if defined UI_SRCS (
    echo %UI_SRCS%
) else (
    echo No UI sources found.
)

%YELLOW% "Module sources: "
if defined MODULE_SRCS (
    echo %MODULE_SRCS%
) else (
    echo No module sources found.
)

%YELLOW% "Library sources: "
if defined LIB_SRCS (
    echo %LIB_SRCS%
) else (
    echo No library sources found.
)
echo.

REM --- Compilation Command ---
%CYAN% "Running compilation command..."
echo.
%CC% %CFLAGS% %MAIN_SRC% %UI_SRCS% %MODULE_SRCS% %LIB_SRCS% -I%INCLUDE_DIR%\headers -I%INCLUDE_DIR%\models -o %TARGET%

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