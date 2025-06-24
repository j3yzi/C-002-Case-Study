@echo off
echo Compiling CS002 - Dual Management System...

:: Compile the CS002 dual management system with all modules
gcc -o bin/CS002.exe ^
    src/main.c ^
    src/ui/menuio.c ^
    src/ui/empio.c ^
    src/ui/stuio.c ^
    src/modules/data.c ^
    src/modules/payroll.c ^
    ../include/models/employee.c ^
    ../include/models/student.c ^
    ../include/src/appwnd.c ^
    ../include/src/auth.c ^
    ../include/src/config.c ^
    ../include/src/lismgr.c ^
    ../include/src/lisops.c ^
    ../include/src/menu.c ^
    ../include/src/state.c ^
    ../include/src/validation.c ^
    -I../include ^
    -I../include/headers ^
    -I../include/models ^
    -I../include/src ^
    -Wall -Wextra -std=c99

if %errorlevel% == 0 (
    echo Compilation successful!
    echo CS002 Dual Management System executable created: bin/CS002.exe
    echo.
    echo System Features:
    echo - Multiple Employee List Management
    echo - Multiple Student List Management  
    echo - Individual List Saving/Loading
    echo - Comprehensive Reporting
    echo - Advanced Data Validation
    echo.
    echo You can now run: bin/CS002.exe
) else (
    echo Compilation failed! Check the errors above.
)

pause
