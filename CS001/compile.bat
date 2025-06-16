@echo off
REM compile.bat - Batch script to compile the CS001 project

echo Compiling the project...

REM Define compiler and flags
set CC=gcc
set CFLAGS=-Wall -Wextra -std=c11 -g

REM Define include directory
set INCLUDE_DIR=include

REM Define source files individually
set FRONTEND_SRCS=src\frontend\employee_io.c
set BACKEND_SRCS=src\backend\file_handler.c src\backend\list_manager.c src\backend\payroll_logic.c
set MAIN_SRC=main.c

REM Define output executable name
set TARGET=project_payroll.exe

REM --- Compilation Command ---
%CC% %CFLAGS% %MAIN_SRC% %FRONTEND_SRCS% %BACKEND_SRCS% -I%INCLUDE_DIR% -o %TARGET%

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Compilation failed!
    echo.
) ELSE (
    echo.
    echo Compilation successful! Executable: %TARGET%
    echo.
)

REM Optional: Pause at the end so the user can see the output before the window closes
pause