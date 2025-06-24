@echo off
setlocal EnableDelayedExpansion
:: Compile script for CS002 project

:: Create bin directory if it doesn't exist
if not exist bin mkdir bin

:: Define PowerShell commands for colored output
set "CYAN=powershell write-host -foregroundcolor cyan -nonewline"
set "YELLOW=powershell write-host -foregroundcolor yellow -nonewline"
set "GREEN=powershell write-host -foregroundcolor green -nonewline"
set "RED=powershell write-host -foregroundcolor red -nonewline"

%CYAN% "Compiling the project..."
echo.

:: Define compiler and flags
set CC=C:\msys64\ucrt64\bin\gcc.exe
set CFLAGS=-g -Wall -I../include/headers

:: Dynamically scan for source files
set "MAIN_SRC=src/main.c"
set "UI_SRCS="
set "MODULE_SRCS="
set "MODEL_SRCS=../include/models/employee.c"
set "LIB_SRCS="

:: Scan UI sources
for %%F in (src\ui\*.c) do (
    if not defined UI_SRCS (
        set "UI_SRCS=%%F"
    ) else (
        set "UI_SRCS=!UI_SRCS! %%F"
    )
)

:: Scan module sources
for %%F in (src\modules\*.c) do (
    if not defined MODULE_SRCS (
        set "MODULE_SRCS=%%F"
    ) else (
        set "MODULE_SRCS=!MODULE_SRCS! %%F"
    )
)

:: Scan library sources
for %%F in (..\include\src\*.c) do (
    if not defined LIB_SRCS (
        set "LIB_SRCS=%%F"
    ) else (
        set "LIB_SRCS=!LIB_SRCS! %%F"
    )
)

:: Display source files that will be compiled
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

%YELLOW% "Model sources: "
if defined MODEL_SRCS (
    echo %MODEL_SRCS%
) else (
    echo No model sources found.
)

%YELLOW% "Library sources: "
if defined LIB_SRCS (
    echo %LIB_SRCS%
) else (
    echo No library sources found.
)
echo.

:: Define output executable name
set TARGET=bin\project_cs002.exe

:: --- Compilation Command ---
%CYAN% "Running compilation command..."
echo.
%CC% %CFLAGS% %MAIN_SRC% %UI_SRCS% %MODULE_SRCS% %MODEL_SRCS% %LIB_SRCS% -o %TARGET%

IF %ERRORLEVEL% NEQ 0 (
    echo.
    %RED% "ERROR: Compilation failed!"
    echo.
) ELSE (
    echo.
    %GREEN% "Compilation successful! Executable: "
    echo %TARGET%
    echo.
    
    :: Optionally run the program
    :: Remove the "rem" from the next line to run the program after successful compilation
    rem %TARGET%
)

pause
