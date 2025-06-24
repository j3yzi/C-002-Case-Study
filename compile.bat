@echo off
echo Compiling PUP Information Management System...

:: Compile the PUP Information Management System with all modules
gcc -o bin/PUP-InformationManagementSystem.exe ^
    src/main.c ^
    src/ui/menuio.c ^
    src/ui/empio.c ^
    src/ui/stuio.c ^
    src/modules/data.c ^
    src/modules/payroll.c ^
    include/models/employee.c ^    include/models/student.c ^
    include/src/appwnd.c ^
    include/src/auth.c ^
    include/src/lismgr.c ^
    include/src/lisops.c ^
    include/src/menu.c ^
    include/src/state.c ^
    include/src/validation.c ^
    -Iinclude ^
    -Iinclude/headers ^
    -Iinclude/models ^
    -Iinclude/src ^
    -Wall -Wextra -std=c99

if %errorlevel% == 0 (
    echo Compilation successful!
    echo PUP Information Management System executable created: bin/PUP-InformationManagementSystem.exe
    echo.
    echo System Features:
    echo - Employee Management with Payroll Calculations
    echo - Student Records Management with Academic Tracking
    echo - Multiple List Management for both Employee and Student records
    echo - Individual List Saving/Loading with Custom Names
    echo - Comprehensive Reporting and Statistics
    echo - Advanced Data Validation and Error Handling
    echo.
    echo You can now run: bin/PUP-InformationManagementSystem.exe
) else (
    echo Compilation failed! Check the errors above.
)

pause 