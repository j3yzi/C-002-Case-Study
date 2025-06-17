@echo off
setlocal EnableDelayedExpansion

REM ===================================================================
REM Complete System Test for Payroll System
REM This script compiles and links all modules into a single test executable
REM ===================================================================

REM Define PowerShell commands for colored output
set "CYAN=powershell write-host -foregroundcolor cyan -nonewline"
set "YELLOW=powershell write-host -foregroundcolor yellow -nonewline"
set "GREEN=powershell write-host -foregroundcolor green -nonewline"
set "RED=powershell write-host -foregroundcolor red -nonewline"

%CYAN% "Compiling Complete System Test..."
echo.

REM Define compiler and flags
set CC=gcc
set CFLAGS=-Wall -Wextra -std=c11 -g
set INCLUDE_DIR=../../include

REM Define test file that exercises the whole system
set TEST_SRC=system_test.c

REM Define all source files
set BACKEND_SRCS=../backend/file_handler.c ../backend/list_manager.c ../backend/payroll_logic.c
set FRONTEND_SRCS=../frontend/employee_io.c ../frontend/menu_io.c

REM Define output executable name
set TARGET=system_test.exe

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

REM --- Check if test source exists ---
if not exist %TEST_SRC% (
    echo.
    %YELLOW% "Creating system test source file..."
    echo.

    REM Create the system test file with basic tests for all modules
    echo // System test for the complete payroll application > %TEST_SRC%
    echo #include ^<stdio.h^> >> %TEST_SRC%
    echo #include ^<stdlib.h^> >> %TEST_SRC% 
    echo #include "../../include/employee.h" >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo // Global variable definitions >> %TEST_SRC%
    echo EmployeeNode* empHead = NULL; >> %TEST_SRC%
    echo EmployeeNode* empCurr = NULL; >> %TEST_SRC%
    echo EmployeeNode* empTail = NULL; >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo int main(void) { >> %TEST_SRC%
    echo     printf("=== Complete System Test ===\n"); >> %TEST_SRC%
    echo     printf("This test exercises all modules of the payroll system.\n\n"); >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo     // Test employee creation and list management >> %TEST_SRC%
    echo     printf("Testing employee node creation...\n"); >> %TEST_SRC%
    echo     EmployeeNode* testEmployee = createEmployeeNode(); >> %TEST_SRC%
    echo     if (testEmployee == NULL) { >> %TEST_SRC%
    echo         printf("Failed to create employee node!\n"); >> %TEST_SRC%
    echo         return 1; >> %TEST_SRC%
    echo     } >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo     // Initialize test employee with sample data >> %TEST_SRC%
    echo     strcpy(testEmployee-^>employeeNumber, "EMP-00001"); >> %TEST_SRC%
    echo     strcpy(testEmployee-^>employeeName, "Test Employee"); >> %TEST_SRC%
    echo     testEmployee-^>statusCode = STATUS_REGULAR; >> %TEST_SRC%
    echo     testEmployee-^>hoursWorked = 45; >> %TEST_SRC%
    echo     testEmployee-^>basicRate = 20.00; >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo     // Test payroll calculation >> %TEST_SRC%
    echo     printf("Testing payroll calculations...\n"); >> %TEST_SRC%
    echo     calculatePayroll(testEmployee); >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo     // Test display functions >> %TEST_SRC%
    echo     printf("Testing display functions...\n"); >> %TEST_SRC%
    echo     displayReportHeader(); >> %TEST_SRC%
    echo     displayEmployeeRecord(testEmployee); >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo     // Test file operations >> %TEST_SRC%
    echo     printf("\nTesting file operations...\n"); >> %TEST_SRC%
    echo     const char* testFile = "../../data/system_test.bin"; >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo     // Create a small list for testing >> %TEST_SRC%
    echo     empHead = testEmployee; >> %TEST_SRC%
    echo     empTail = testEmployee; >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo     // Save to file >> %TEST_SRC%
    echo     if (!exportEmployeeDataToFile(empHead, testFile)) { >> %TEST_SRC%
    echo         printf("Failed to save data to file!\n"); >> %TEST_SRC%
    echo         free(testEmployee); >> %TEST_SRC%
    echo         return 1; >> %TEST_SRC%
    echo     } >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo     // Free existing list >> %TEST_SRC%
    echo     free(testEmployee); >> %TEST_SRC%
    echo     empHead = empTail = NULL; >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo     // Load from file >> %TEST_SRC%
    echo     empHead = loadEmployeeDataFromFile(testFile); >> %TEST_SRC%
    echo     if (empHead == NULL) { >> %TEST_SRC%
    echo         printf("Failed to load data from file!\n"); >> %TEST_SRC%
    echo         return 1; >> %TEST_SRC%
    echo     } >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo     // Display loaded data >> %TEST_SRC%
    echo     printf("\nLoaded data from file:\n"); >> %TEST_SRC%
    echo     displayReportHeader(); >> %TEST_SRC%
    echo     displayEmployeeRecord(empHead); >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo     // Clean up >> %TEST_SRC%
    echo     free(empHead); >> %TEST_SRC%
    echo     empHead = NULL; >> %TEST_SRC%
    echo. >> %TEST_SRC%
    echo     printf("\n=== System Test Complete ===\n"); >> %TEST_SRC%
    echo     return 0; >> %TEST_SRC%
    echo } >> %TEST_SRC%
)

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
    %CYAN% "Running system test..."
    echo.
    %TARGET%
)

pause
