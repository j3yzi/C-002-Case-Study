#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

// Ensure NULL is defined
#ifndef NULL
#define NULL ((void*)0)
#endif

// Ensure boolean constants are defined
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#include "menuio.h"
#include "empio.h"
#include "stuio.h"
#include "../modules/data.h"
#include "../modules/payroll.h"
#include "../../include/headers/apctxt.h"
#include "../../include/headers/apclrs.h"
#include "../../include/headers/state.h"
#include "../../include/headers/interface.h"
#include "../../include/models/employee.h"
#include "../../include/models/student.h"
#include "../../include/headers/list.h"

// Global managers
EmployeeManager empManager;
StudentManager stuManager;

// Main menu definition
Menu mainMenu = {1, "PUP Information Management System", (MenuOption[]){
    {'1', "Employee Management", "Manage employee records and payroll information", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Student Management", "Handle student enrollment and academic records", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "Course Management", "Manage course information and academic programs", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'4', "System Statistics", "View system usage and performance statistics", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'5', "Configuration Settings", "Modify system configuration and settings", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'6', "Exit", "Close the application and return to system", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 6
};

/**
 * @brief Helper function to run a menu with the new interface system
 * @param menu Pointer to the menu to display
 * @return Returns the selected menu option character
 */
static char runMenuWithInterface(Menu* menu) {
    int selected = 0;
    int prevSelected = -1; // Initialize to -1 to force initial drawing
    int key;
    bool running = true;
    time_t lastTimeUpdate = 0;
    
    // Find first non-disabled option
    while (selected < menu->optionCount && menu->options[selected].isDisabled) {
        selected++;
    }
    if (selected >= menu->optionCount) selected = 0;
    
    // Get the console size
    int consoleWidth, consoleHeight;
    getConsoleSize(&consoleWidth, &consoleHeight);
    
    // Use constant width from interface.c
    int totalMenuNameWidth = 60; // MENU_HEADER_WIDTH from interface.c
    int paddingX = 5;
    int paddingY = 2;
    
    // Always make option box exactly 2/3 of the total width
    int totalOptionBoxWidth = (totalMenuNameWidth * 2) / 3;
    
    // Initial full menu draw
    winTermClearScreen();
    displayMenu(menu, selected);
    
    while (running) {
        // Only update what changed
        if (selected != prevSelected) {
            updateMenuSelection(menu, prevSelected, selected, consoleWidth, totalMenuNameWidth, paddingX, paddingY);
            prevSelected = selected;
        }
        
        // Update time every second
        time_t currentTime = time(NULL);
        if (currentTime > lastTimeUpdate) {
            updateInfoBoxTimeDate(consoleWidth, totalMenuNameWidth, totalOptionBoxWidth, paddingX, paddingY);
            lastTimeUpdate = currentTime;
        }
        
        // Check for keyboard input with timeout
        if (_kbhit()) {
            key = _getch();
            
            // Handle special keys (arrow keys)
            if (key == 0 || key == 224) {
                key = _getch(); // Get the actual key code
                if (key == 72) { // Up arrow
                    int oldSelected = selected;
                    do {
                        selected--;
                        if (selected < 0) selected = menu->optionCount - 1;
                    } while (menu->options[selected].isDisabled && selected != oldSelected);
                }
                else if (key == 80) { // Down arrow
                    int oldSelected = selected;
                    do {
                        selected++;
                        if (selected >= menu->optionCount) selected = 0;
                    } while (menu->options[selected].isDisabled && selected != oldSelected);
                }
            }
            else if (key == 13) { // Enter key
                if (!menu->options[selected].isDisabled) {
                    return menu->options[selected].key;
                } else {
                    // Show error message for disabled option
                    printf("\n\nOption '%c - %s' is currently disabled.\n", 
                           menu->options[selected].key, menu->options[selected].text);
                    printf("Please select a different option.\n");
                    printf("Press any key to continue...");
                    _getch();
                    // Force full redraw
                    winTermClearScreen();
                    displayMenu(menu, selected);
                }
            }
            else if (key == 27) { // Escape key
                // Find exit/back option (usually last, but could be different)
                for (int i = menu->optionCount - 1; i >= 0; i--) {
                    if (!menu->options[i].isDisabled) {
                        return menu->options[i].key;
                    }
                }
                return menu->options[menu->optionCount - 1].key; // Fallback to last option
            }
            else {
                // Check if key matches any menu option
                for (int i = 0; i < menu->optionCount; i++) {
                    if (key == menu->options[i].key || key == tolower(menu->options[i].key)) {
                        if (!menu->options[i].isDisabled) {
                            return menu->options[i].key;
                        } else {
                            // Show error message for disabled option
                            printf("\n\nOption '%c - %s' is currently disabled.\n", 
                                   menu->options[i].key, menu->options[i].text);
                            printf("Please select a different option.\n");
                            printf("Press any key to continue...");
                            _getch();
                            // Force full redraw
                            winTermClearScreen();
                            displayMenu(menu, selected);
                            break;
                        }
                    }
                }
            }
        }
        
        // Small delay to prevent CPU hogging
        Sleep(50);
    }
    
    // Find last non-disabled option as fallback
    for (int i = menu->optionCount - 1; i >= 0; i--) {
        if (!menu->options[i].isDisabled) {
            return menu->options[i].key;
        }
    }
    return menu->options[menu->optionCount - 1].key; // Final fallback
}

/**
 * @brief Gets the path to the configuration file
 * @param buffer Buffer to store the path
 * @param bufferSize Size of the buffer
 */
static void getConfigPath(char* buffer, size_t bufferSize) {
    if (!buffer || bufferSize < 1) return;
    
    // Get the full path of the executable
    char executablePath[512];
    GetModuleFileName(NULL, executablePath, sizeof(executablePath));
    
    // Find the last backslash to get the directory
    char* lastSlash = strrchr(executablePath, '\\');
    if (lastSlash != NULL) {
        *lastSlash = '\0'; // Remove the executable name, keep directory
        snprintf(buffer, bufferSize, "%s\\config.ini", executablePath);
    } else {
        // Fallback to current directory
        strncpy(buffer, "config.ini", bufferSize - 1);
        buffer[bufferSize - 1] = '\0';
    }
}

/**
 * @brief Initializes the multi-list management system
 */
void initMultiListManager(void) {
    // Initialize employee manager
    memset(&empManager, 0, sizeof(EmployeeManager));
    empManager.employeeListCount = 0;
    empManager.activeEmployeeList = -1;
    
    // Initialize student manager
    memset(&stuManager, 0, sizeof(StudentManager));
    stuManager.studentListCount = 0;
    stuManager.activeStudentList = -1;
    
    printf("PUP Information Management System initialized!\n");
}

/**
 * @brief Cleans up all allocated lists and resources
 */
void cleanupMultiListManager(void) {
    // Clean up employee lists
    for (int i = 0; i < empManager.employeeListCount; i++) {
        if (empManager.employeeLists[i]) {
            destroyList(&empManager.employeeLists[i], freeEmployee);
        }
    }
    
    // Clean up student lists
    for (int i = 0; i < stuManager.studentListCount; i++) {
        if (stuManager.studentLists[i]) {
            destroyList(&stuManager.studentLists[i], freeStudent);
        }
    }
    
    printf("PUP Information Management System cleaned up!\n");
}

/**
 * @brief Main menu loop
 * @return Returns 0 on normal exit, other values on error
 */
int menuLoop(void) {
    char choice;
    
    // Initialize console for proper display
    initConsole();
    
    do {
        checkMenuStates(&mainMenu);
        choice = runMenuWithInterface(&mainMenu);
        
        switch(choice) {
            case '1':
                runEmployeeManagement();
                break;
            case '2':
                runStudentManagement();
                break;
            case '3':
                runCourseManagement();
                break;
            case '4':
                displaySystemStatistics();
                break;
            case '5':
                runConfigurationManagement();
                break;
            case '6':
                winTermClearScreen();
                printf("\nExiting PUP Information Management System...\n");
                printf("Thank you for using the system!\n");
                return 0;
            default:
                // This should not happen as runMenuWithInterface handles invalid options
                break;
        }
    } while (1);
    
    return 0;
}

/**
 * @brief Displays comprehensive system statistics
 */
void displaySystemStatistics(void) {
    winTermClearScreen();
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║                         SYSTEM STATISTICS                        ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                   ║\n");
    printf("║  EMPLOYEE MANAGEMENT:                                             ║\n");
    printf("║    Total Employee Lists: %-41d║\n", empManager.employeeListCount);
    
    int totalEmployees = 0;
    for (int i = 0; i < empManager.employeeListCount; i++) {
        if (empManager.employeeLists[i]) {
            int count = empManager.employeeLists[i]->size;
            totalEmployees += count;
            printf("║      - %-30s: %-15d║\n", empManager.employeeListNames[i], count);
        }
    }
    printf("║    Total Employees: %-42d║\n", totalEmployees);
    
    if (empManager.activeEmployeeList >= 0) {
        printf("║    Active List: %-46s║\n", empManager.employeeListNames[empManager.activeEmployeeList]);
    } else {
        printf("║    Active List: %-46s║\n", "None");
    }
    printf("║                                                                   ║\n");
    
    printf("║  STUDENT MANAGEMENT:                                              ║\n");
    printf("║    Total Student Lists: %-42d║\n", stuManager.studentListCount);
    
    int totalStudents = 0;
    for (int i = 0; i < stuManager.studentListCount; i++) {
        if (stuManager.studentLists[i]) {
            int count = stuManager.studentLists[i]->size;
            totalStudents += count;
            printf("║      - %-30s: %-15d║\n", stuManager.studentListNames[i], count);
        }
    }
    printf("║    Total Students: %-43d║\n", totalStudents);
    
    if (stuManager.activeStudentList >= 0) {
        printf("║    Active List: %-46s║\n", stuManager.studentListNames[stuManager.activeStudentList]);
    } else {
        printf("║    Active List: %-46s║\n", "None");
    }
    printf("║                                                                   ║\n");
    
    printf("║  OVERALL STATISTICS:                                              ║\n");
    printf("║    Total Records: %-44d║\n", totalEmployees + totalStudents);
    printf("║    Total Lists: %-46d║\n", empManager.employeeListCount + stuManager.studentListCount);
    printf("║                                                                   ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    
    waitForKeypress("\nPress any key to continue...");
}

/**
 * @brief Employee management submenu
 * @return Returns 0 on normal exit, other values on error
 */
int runEmployeeManagement(void) {
    char choice;
    // Add active list info to menu title
    char menuTitle[100] = "Employee Management";
    
    if (empManager.activeEmployeeList >= 0) {
        sprintf(menuTitle, "Employee Management - %s (%d employees)", 
               empManager.employeeListNames[empManager.activeEmployeeList],
               empManager.employeeLists[empManager.activeEmployeeList] ? 
               empManager.employeeLists[empManager.activeEmployeeList]->size : 0);
    }
    
    Menu employeeMenu = {1, menuTitle, (MenuOption[]){
        {'1', "Create Employee List", "Initialize a new employee database", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Switch Employee List", "Change to a different employee list", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Add Employee", "Add a new employee to the current list", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Edit Employee", "Modify existing employee information", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'5', "Delete Employee", "Remove an employee from the database", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'6', "Search Employee", "Find employees by number or name", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'7', "Display All Employees", "Show complete list of all employees", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'T', "Employee Table View", "View employees in paginated table format", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'8', "Payroll Report", "Generate payroll calculations and reports", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'9', "Save Employee List", "Save current list to file", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'A', "Load Employee List", "Load employee data from saved file", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'B', "Back to Main Menu", "Return to the main system menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 12};
    
    do {
        // Update menu title with current active list info before displaying menu
        if (empManager.activeEmployeeList >= 0) {
            sprintf(menuTitle, "Employee Management - %s (%d employees)", 
                   empManager.employeeListNames[empManager.activeEmployeeList],
                   empManager.employeeLists[empManager.activeEmployeeList] ? 
                   empManager.employeeLists[empManager.activeEmployeeList]->size : 0);
        } else {
            strcpy(menuTitle, "Employee Management - No active list");
        }
        employeeMenu.name = menuTitle;
        
        // Update menu option states based on current manager state
        checkMenuStates(&employeeMenu);
        
        choice = runMenuWithInterface(&employeeMenu);
        
        switch(choice) {
            case '1':
                handleCreateEmployeeList();
                break;
            case '2':
                handleSwitchEmployeeList();
                break;
            case '3':
                handleAddEmployee();
                break;
            case '4': {
                extern int handleEditEmployee(list* employeeList);
                int hasActiveList = (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList]);
                if (checkActiveList(hasActiveList, 0, "No active employee list!")) {
                    handleEditEmployee(empManager.employeeLists[empManager.activeEmployeeList]);
                }
                break;
            }
            case '5': {
                int hasActiveList = (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList]);
                if (checkActiveList(hasActiveList, 0, "No active employee list!")) {
                    extern int handleDeleteEmployee(list* employeeList);
                    handleDeleteEmployee(empManager.employeeLists[empManager.activeEmployeeList]);
                }
                break;
            }
            case '6': {
                int hasActiveList = (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList]);
                if (checkActiveList(hasActiveList, 0, "No active employee list!")) {
                    extern int handleSearchEmployee(const list* employeeList);
                    handleSearchEmployee(empManager.employeeLists[empManager.activeEmployeeList]);
                }
                break;
            }
            case '7':
                handleDisplayAllEmployees();
                break;
            case 'T':
            case 't':
                handleDisplayEmployeeTable();
                break;
            case '8':
                handlePayrollReport();
                break;
            case '9':
                handleSaveEmployeeList();
                break;
            case 'A':
            case 'a':
                handleLoadEmployeeList();
                break;
            case 'B':
            case 'b':
                return 0; // Return to main menu
            default:
                break;
        }
    } while (1);
    
    return 0;
}

/**
 * @brief Student management submenu
 * @return Returns 0 on normal exit, other values on error
 */
int runStudentManagement(void) {
    char choice;
    // Add active list info to menu title
    char menuTitle[100] = "Student Management";
    
    if (stuManager.activeStudentList >= 0) {
        sprintf(menuTitle, "Student Management - %s (%d students)", 
               stuManager.studentListNames[stuManager.activeStudentList],
               stuManager.studentLists[stuManager.activeStudentList] ? 
               stuManager.studentLists[stuManager.activeStudentList]->size : 0);
    }
    
    Menu studentMenu = {1, menuTitle, (MenuOption[]){
        {'1', "Create Student List", "Initialize a new student database", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Switch Student List", "Change to a different student list", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Add Student", "Enroll a new student in the system", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Edit Student", "Modify existing student information", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'5', "Delete Student", "Remove a student from the database", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'6', "Search Student", "Find students by number or name", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'7', "Display All Students", "Show complete list of all students", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'T', "Student Table View", "View students in paginated table format", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'8', "Sort Students by Grade", "Arrange students by academic performance", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'9', "Student Report", "Generate academic reports and statistics", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'S', "Save Student List", "Save current list to file", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'L', "Load Student List", "Load student data from saved file", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'B', "Back to Main Menu", "Return to the main system menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 13};
    
    do {
        // Update menu title with current active list info before displaying menu
        if (stuManager.activeStudentList >= 0) {
            sprintf(menuTitle, "Student Management - %s (%d students)", 
                   stuManager.studentListNames[stuManager.activeStudentList],
                   stuManager.studentLists[stuManager.activeStudentList] ? 
                   stuManager.studentLists[stuManager.activeStudentList]->size : 0);
        } else {
            strcpy(menuTitle, "Student Management - No active list");
        }
        studentMenu.name = menuTitle;
        
        // Update menu option states based on current manager state
        checkMenuStates(&studentMenu);
        
        choice = runMenuWithInterface(&studentMenu);
        
        switch(choice) {
            case '1':
                handleCreateStudentList();
                break;
            case '2':
                handleSwitchStudentList();
                break;
            case '3':
                handleAddStudent();
                break;
            case '4': {
                int hasActiveList = (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]);
                if (checkActiveList(hasActiveList, 0, "No active student list!")) {
                    extern int handleEditStudent(list* studentList);
                    handleEditStudent(stuManager.studentLists[stuManager.activeStudentList]);
                }
                break;
            }
            case '5': {
                int hasActiveList = (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]);
                if (checkActiveList(hasActiveList, 0, "No active student list!")) {
                    extern int handleDeleteStudent(list* studentList);
                    handleDeleteStudent(stuManager.studentLists[stuManager.activeStudentList]);
                }
                break;
            }
            case '6': {
                int hasActiveList = (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]);
                if (checkActiveList(hasActiveList, 0, "No active student list!")) {
                    extern int handleSearchStudent(const list* studentList);
                    handleSearchStudent(stuManager.studentLists[stuManager.activeStudentList]);
                }
                break;
            }
            case '7':
                handleDisplayAllStudents();
                break;
            case 'T':
            case 't': {
                int hasActiveList = (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]);
                if (checkActiveList(hasActiveList, 0, "No active student list!")) {
                    extern int handleDisplayStudentTable(const list* studentList);
                    handleDisplayStudentTable(stuManager.studentLists[stuManager.activeStudentList]);
                }
                break;
            }
            case '8':
                handleSortStudentsByGrade();
                break;
            case '9':
                handleStudentReport();
                break;
            case 'S':
            case 's':
                handleSaveStudentList();
                break;
            case 'L':
            case 'l':
                handleLoadStudentList();
                break;
            case 'B':
            case 'b':
                return 0; // Return to main menu
            default:
                break;
        }
    } while (1);
    
    return 0;
}

/**
 * @brief Creates a new employee list
 * @return Returns 0 on success, other values on error
 */
int handleCreateEmployeeList(void) {
    winTermClearScreen();
    printf("%s", UI_HEADER);
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    
    // Calculate visible text length for centering
    char headerText[] = "Create New Employee List";
    int boxWidth = 69; // Total width including borders
    int borderSpace = 2; // Space for "║" on each side
    int availableSpace = boxWidth - borderSpace;
    int headerLen = strlen(headerText);
    int leftPadding = (availableSpace - headerLen) / 2;
    int rightPadding = availableSpace - headerLen - leftPadding;
    
    printf("║");
    for (int i = 0; i < leftPadding; i++) printf(" ");
    printf("%s%s%s", TXT_BOLD, headerText, TXT_RESET UI_HEADER);
    for (int i = 0; i < rightPadding; i++) printf(" ");
    printf("║\n");
    
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    printf("%s\n", TXT_RESET);
    
    if (empManager.employeeListCount >= 10) {
        printf("%s⚠️  Maximum number of employee lists (10) reached!%s\n", UI_WARNING, TXT_RESET);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    char listName[50];
    appFormField field = { "📝 Enter name for this employee list: ", listName, 50, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = 49}} };
    appGetValidatedInput(&field, 1);
    
    // Create new list
    list* newList = NULL;
    if (createEmployeeList(&newList) != 0) {
        printf("%s❌ Failed to create employee list!%s\n", UI_ERROR, TXT_RESET);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Add to manager
    empManager.employeeLists[empManager.employeeListCount] = newList;
    strncpy(empManager.employeeListNames[empManager.employeeListCount], listName, 49);
    empManager.employeeListNames[empManager.employeeListCount][49] = '\0';
    empManager.activeEmployeeList = empManager.employeeListCount;
    empManager.employeeListCount++;
    
    printf("%s✅ Employee list '%s' created successfully!%s\n", UI_SUCCESS, listName, TXT_RESET);
    printf("%sThis list is now active. Add employees to get started.%s\n", UI_INFO, TXT_RESET);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

int handleAddEmployee(void) {
    winTermClearScreen();
    printf("=== Add Employee to Active List ===\n\n");
    
    if (empManager.activeEmployeeList < 0 || !empManager.employeeLists[empManager.activeEmployeeList]) {
        printf("No active employee list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
    if (!newEmployee) {
        printf("Memory allocation failed!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Initialize employee data
    memset(newEmployee, 0, sizeof(Employee));
    
    if (getEmployeeDataFromUser(newEmployee) != 0) {
        printf("Failed to get employee data. Operation cancelled.\n");
        free(newEmployee);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Calculate payroll information
    calculatePayroll(newEmployee);
    
    if (createEmployee(newEmployee, &empManager.employeeLists[empManager.activeEmployeeList]) != 0) {
        printf("Failed to add employee to list!\n");
        free(newEmployee);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("\nEmployee '%s %s' added successfully to list '%s'!\n", 
           newEmployee->personal.name.firstName, 
           newEmployee->personal.name.lastName,
           empManager.employeeListNames[empManager.activeEmployeeList]);
    printf("Employee Number: %s\n", newEmployee->personal.employeeNumber);
    printf("Net Pay: %.2f\n", newEmployee->payroll.netPay);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

// Note: handleEditEmployee, handleDeleteEmployee, and handleSearchEmployee are in empio.c
// handleEditStudent, handleDeleteStudent, and handleSearchStudent are in stuio.c

int handleDisplayAllEmployees(void) {
    winTermClearScreen();
    printf("=== Display All Employees ===\n\n");
    
    int hasActiveList = (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList]);
    if (!checkActiveList(hasActiveList, 0, "No active employee list!")) {
        return -1;
    }
    
    printf("Active List: %s\n", empManager.employeeListNames[empManager.activeEmployeeList]);
    displayAllEmployees(empManager.employeeLists[empManager.activeEmployeeList]);
    waitForKeypress("\nPress any key to continue...");
    return 0;
}

int handlePayrollReport(void) {
    winTermClearScreen();
    printf("%s", UI_HEADER);
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    
    // Calculate visible text length for centering
    char headerText2[] = "Payroll Report";
    int boxWidth = 69; // Total width including borders
    int borderSpace = 2; // Space for "║" on each side
    int availableSpace = boxWidth - borderSpace;
    int headerLen2 = strlen(headerText2);
    int leftPadding2 = (availableSpace - headerLen2) / 2;
    int rightPadding2 = availableSpace - headerLen2 - leftPadding2;
    
    printf("║");
    for (int i = 0; i < leftPadding2; i++) printf(" ");
    printf("%s%s%s", TXT_BOLD, headerText2, TXT_RESET UI_HEADER);
    for (int i = 0; i < rightPadding2; i++) printf(" ");
    printf("║\n");
    
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    printf("%s\n", TXT_RESET);
    
    int hasActiveList = (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList]);
    if (!checkActiveList(hasActiveList, 0, "No active employee list!")) {
        return -1;
    }
    
    printf("%s📊 Generating payroll report for: %s%s%s\n\n", UI_INFO, UI_HIGHLIGHT, empManager.employeeListNames[empManager.activeEmployeeList], TXT_RESET);
    
    // Generate the payroll report file
    char reportFilePath[512];
    int reportResult = generatePayrollReportFile(empManager.employeeLists[empManager.activeEmployeeList], reportFilePath, sizeof(reportFilePath));
    
    if (reportResult > 0) {
        printf("%s✅ Successfully generated payroll report!%s\n", UI_SUCCESS, TXT_RESET);
        printf("%sReport saved to: %s%s\n", UI_INFO, reportFilePath, TXT_RESET);
        printf("%sProcessed %d employees%s\n\n", UI_INFO, reportResult, TXT_RESET);
        
        // Display the report content in the terminal
        printf("%s", UI_HEADER);
        printf("╔═══════════════════════════════════════════════════════════════════╗\n");
        
        // Calculate visible text length for centering
        char headerText3[] = "Employee Payroll Report";
        int headerLen3 = strlen(headerText3);
        int leftPadding3 = (boxWidth - borderSpace - headerLen3) / 2;
        int rightPadding3 = boxWidth - borderSpace - headerLen3 - leftPadding3;
        
        printf("║");
        for (int i = 0; i < leftPadding3; i++) printf(" ");
        printf("%s%s%s", TXT_BOLD, headerText3, TXT_RESET UI_HEADER);
        for (int i = 0; i < rightPadding3; i++) printf(" ");
        printf("║\n");
        
        printf("╚═══════════════════════════════════════════════════════════════════╝\n");
        printf("%s", TXT_RESET);
    } else {
        printf("Failed to generate payroll report.\n");
    }
    
    waitForKeypress(NULL);
    return 0;
}

int handleSaveEmployeeList(void) {
    winTermClearScreen();
    printf("=== Save Employee List ===\n\n");
    
    int hasActiveList = (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList]);
    if (!checkActiveList(hasActiveList, 0, "No active employee list to save!")) {
        return -1;
    }
    
    printf("Saving list: %s\n", empManager.employeeListNames[empManager.activeEmployeeList]);
    
    // Show existing data files
    printf("\nExisting employee data files:\n");
    listEmployeeDataFiles();
    printf("\n");
    
    char filename[100];
    appFormField field = { "Enter filename (will be saved as 'employee_LISTNAME.dat'): ", filename, 100, IV_MAX_LEN, {.rangeInt = {.min = 0, .max = 99}} };
    appGetValidatedInput(&field, 1);
    
    // Use the custom save function
    int savedCount = saveListWithCustomName(empManager.employeeLists[empManager.activeEmployeeList], 
                                           filename, "employee");
    
    if (savedCount >= 0) {
        printf("Successfully saved %d employee records!\n", savedCount);
        printf("Data saved to data directory.\n");
    } else {
        printf("Failed to save employee list.\n");
    }
    
    waitForKeypress(NULL);
    return 0;
}

int handleLoadEmployeeList(void) {
    winTermClearScreen();
    printf("%s", UI_HEADER);
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    
    // Calculate visible text length for centering
    char headerText4[] = "Load Employee List";
    int boxWidth4 = 69; // Total width including borders
    int borderSpace4 = 2; // Space for "║" on each side
    int availableSpace4 = boxWidth4 - borderSpace4;
    int headerLen4 = strlen(headerText4);
    int leftPadding4 = (availableSpace4 - headerLen4) / 2;
    int rightPadding4 = availableSpace4 - headerLen4 - leftPadding4;
    
    printf("║");
    for (int i = 0; i < leftPadding4; i++) printf(" ");
    printf("%s%s%s", TXT_BOLD, headerText4, TXT_RESET UI_HEADER);
    for (int i = 0; i < rightPadding4; i++) printf(" ");
    printf("║\n");
    
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    printf("%s\n", TXT_RESET);
    
    // List available employee data files
    printf("%s📁 Available Employee Data Files:%s\n", UI_INFO, TXT_RESET);
    printf("─────────────────────────────────────\n");
    listEmployeeDataFiles();
    printf("─────────────────────────────────────\n\n");
    
    char filename[100];
    char listName[50];
    appFormField fields[] = {
        { "📂 Enter filename to load: ", filename, 100, IV_MAX_LEN, {.rangeInt = {.min = 0, .max = 99}} },
        { "📝 Enter name for this loaded list: ", listName, 50, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = 49}} }
    };
    appGetValidatedInput(fields, 2);
    
    // Load the data
    list* newList = loadListWithName(filename, "employee", SINGLY);
    if (!newList) {
        printf("%s❌ Failed to load employee data from file '%s'!%s\n", UI_ERROR, filename, TXT_RESET);
        printf("%sPlease make sure the file exists and is in the correct format.%s\n", UI_WARNING, TXT_RESET);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Add to manager
    if (empManager.employeeListCount >= 10) {
        printf("%s⚠️  Maximum number of employee lists reached!%s\n", UI_WARNING, TXT_RESET);
        destroyList(&newList, freeEmployee);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    empManager.employeeLists[empManager.employeeListCount] = newList;
    strncpy(empManager.employeeListNames[empManager.employeeListCount], listName, 49);
    empManager.employeeListNames[empManager.employeeListCount][49] = '\0';
    empManager.activeEmployeeList = empManager.employeeListCount;
    empManager.employeeListCount++;
    
    printf("%s✅ Employee list '%s' loaded successfully!%s\n", UI_SUCCESS, listName, TXT_RESET);
    printf("%s📊 Loaded %d employee records.%s\n", UI_INFO, newList->size, TXT_RESET);
    printf("%sThis list is now active.%s\n", UI_INFO, TXT_RESET);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

int handleCreateStudentList(void) {
    winTermClearScreen();
    printf("=== Create New Student List ===\n\n");
    
    if (stuManager.studentListCount >= 10) {
        printf("Maximum number of student lists (10) reached!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    char listName[50];
    appFormField field = { "Enter name for this student list: ", listName, 50, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = 49}} };
    appGetValidatedInput(&field, 1);
    
    // Create new list
    list* newList = NULL;
    if (createStudentList(&newList) != 0) {
        printf("Failed to create student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Add to manager
    stuManager.studentLists[stuManager.studentListCount] = newList;
    strncpy(stuManager.studentListNames[stuManager.studentListCount], listName, 49);
    stuManager.studentListNames[stuManager.studentListCount][49] = '\0';
    stuManager.activeStudentList = stuManager.studentListCount;
    stuManager.studentListCount++;
    
    printf("Student list '%s' created successfully!\n", listName);
    printf("This list is now active. Add students to get started.\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

int handleAddStudent(void) {
    winTermClearScreen();
    printf("=== Add Student to Active List ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (!newStudent) {
        printf("Memory allocation failed!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Initialize student data
    memset(newStudent, 0, sizeof(Student));
    
    if (getStudentDataFromUser(newStudent) != 0) {
        printf("Failed to get student data. Operation cancelled.\n");
        free(newStudent);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    if (createStudent(newStudent, &stuManager.studentLists[stuManager.activeStudentList]) != 0) {
        printf("Failed to add student to list!\n");
        free(newStudent);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("\nStudent '%s %s' added successfully to list '%s'!\n", 
           newStudent->personal.name.firstName, 
           newStudent->personal.name.lastName,
           stuManager.studentListNames[stuManager.activeStudentList]);
    printf("Student Number: %s\n", newStudent->personal.studentNumber);
    printf("Final Grade: %.2f (%s)\n", newStudent->academic.finalGrade, newStudent->academic.remarks);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

// Student edit, delete, and search functions are implemented in stuio.c

int handleDisplayAllStudents(void) {
    winTermClearScreen();
    printf("=== Display All Students ===\n\n");
    
    int hasActiveList = (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]);
    if (!checkActiveList(hasActiveList, 0, "No active student list!")) {
        return -1;
    }
    
    printf("Active List: %s\n", stuManager.studentListNames[stuManager.activeStudentList]);
    displayAllStudents(stuManager.studentLists[stuManager.activeStudentList]);
    waitForKeypress("\nPress any key to continue...");
    return 0;
}

int handleSortStudentsByGrade(void) {
    winTermClearScreen();
    printf("=== Sort Students by Grade ===\n\n");
    
    int hasActiveList = (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]);
    if (!checkActiveList(hasActiveList, 0, "No active student list!")) {
        return -1;
    }
    
    // Check if there are enough students to sort
    list* studentList = stuManager.studentLists[stuManager.activeStudentList];
    if (studentList->size <= 1) {
        printf("Not enough students to sort. Need at least 2 students.\n");
        waitForKeypress(NULL);
        return 0;
    }
    
    printf("Sorting students by final grade (descending order)...\n\n");
    
    // Sort the students using the existing sortStudentsByGrade function
    // The second parameter is set to 1 for descending order
    if (sortStudentsByGrade(studentList, 1)) {
        printf("Students sorted successfully!\n");
        printf("\nSorted student list:\n");
        displayAllStudents(studentList);
    } else {
        printf("Failed to sort students.\n");
    }
    
    waitForKeypress(NULL);
    return 0;
}

int handleStudentReport(void) {
    winTermClearScreen();
    printf("%s", UI_HEADER);
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    
    // Calculate visible text length for centering
    char headerText5[] = "Student Report";
    int boxWidth5 = 69; // Total width including borders
    int borderSpace5 = 2; // Space for "║" on each side
    int availableSpace5 = boxWidth5 - borderSpace5;
    int headerLen5 = strlen(headerText5);
    int leftPadding5 = (availableSpace5 - headerLen5) / 2;
    int rightPadding5 = availableSpace5 - headerLen5 - leftPadding5;
    
    printf("║");
    for (int i = 0; i < leftPadding5; i++) printf(" ");
    printf("%s%s%s", TXT_BOLD, headerText5, TXT_RESET UI_HEADER);
    for (int i = 0; i < rightPadding5; i++) printf(" ");
    printf("║\n");
    
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    printf("%s\n", TXT_RESET);
    
    int hasActiveList = (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]);
    if (!checkActiveList(hasActiveList, 0, "No active student list!")) {
        return -1;
    }
    
    printf("%s📊 Generating student report for: %s%s%s\n\n", UI_INFO, UI_HIGHLIGHT, stuManager.studentListNames[stuManager.activeStudentList], TXT_RESET);
    
    // Generate the student report file
    char reportFilePath[512];
    int reportResult = generateStudentReportFile(stuManager.studentLists[stuManager.activeStudentList], reportFilePath, sizeof(reportFilePath));
    
    if (reportResult > 0) {
        printf("%s✅ Successfully generated student report!%s\n", UI_SUCCESS, TXT_RESET);
        printf("%sReport saved to: %s%s\n", UI_INFO, reportFilePath, TXT_RESET);
        printf("%sProcessed %d students%s\n\n", UI_INFO, reportResult, TXT_RESET);
        
        // Display the report content in the terminal
        printf("%s", UI_HEADER);
        printf("╔═══════════════════════════════════════════════════════════════════╗\n");
        
        // Calculate visible text length for centering
        char headerText6[] = "Student Academic Report";
        int headerLen6 = strlen(headerText6);
        int leftPadding6 = (boxWidth5 - borderSpace5 - headerLen6) / 2;
        int rightPadding6 = boxWidth5 - borderSpace5 - headerLen6 - leftPadding6;
        
        printf("║");
        for (int i = 0; i < leftPadding6; i++) printf(" ");
        printf("%s%s%s", TXT_BOLD, headerText6, TXT_RESET UI_HEADER);
        for (int i = 0; i < rightPadding6; i++) printf(" ");
        printf("║\n");
        
        printf("╚═══════════════════════════════════════════════════════════════════╝\n");
        printf("%s", TXT_RESET);
    } else {
        printf("Failed to generate student report.\n");
    }
    
    waitForKeypress(NULL);
    return 0;
}

int handleSaveStudentList(void) {
    winTermClearScreen();
    printf("=== Save Student List ===\n\n");
    
    int hasActiveList = (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]);
    if (!checkActiveList(hasActiveList, 0, "No active student list to save!")) {
        return -1;
    }
    
    printf("Saving list: %s\n", stuManager.studentListNames[stuManager.activeStudentList]);
    
    // Show existing data files
    printf("\nExisting student data files:\n");
    listStudentDataFiles();
    printf("\n");
    
    char filename[100];
    appFormField field = { "Enter filename (will be saved as 'student_LISTNAME.dat'): ", filename, 100, IV_MAX_LEN, {.rangeInt = {.min = 0, .max = 99}} };
    appGetValidatedInput(&field, 1);
    
    // Use the custom save function
    int savedCount = saveListWithCustomName(stuManager.studentLists[stuManager.activeStudentList], 
                                           filename, "student");
    
    if (savedCount >= 0) {
        printf("Successfully saved %d student records!\n", savedCount);
        printf("Data saved to data directory.\n");
    } else {
        printf("Failed to save student list.\n");
    }
    
    waitForKeypress(NULL);
    return 0;
}

int handleLoadStudentList(void) {
    winTermClearScreen();
    printf("%s", UI_HEADER);
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    
    // Calculate visible text length for centering
    char headerText7[] = "Load Student List";
    int boxWidth7 = 69; // Total width including borders
    int borderSpace7 = 2; // Space for "║" on each side
    int availableSpace7 = boxWidth7 - borderSpace7;
    int headerLen7 = strlen(headerText7);
    int leftPadding7 = (availableSpace7 - headerLen7) / 2;
    int rightPadding7 = availableSpace7 - headerLen7 - leftPadding7;
    
    printf("║");
    for (int i = 0; i < leftPadding7; i++) printf(" ");
    printf("%s%s%s", TXT_BOLD, headerText7, TXT_RESET UI_HEADER);
    for (int i = 0; i < rightPadding7; i++) printf(" ");
    printf("║\n");
    
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    printf("%s\n", TXT_RESET);
    
    // List available student data files
    printf("%s📁 Available Student Data Files:%s\n", UI_INFO, TXT_RESET);
    printf("─────────────────────────────────────\n");
    listStudentDataFiles();
    printf("─────────────────────────────────────\n\n");
    
    char filename[100];
    char listName[50];
    appFormField fields[] = {
        { "📂 Enter filename to load: ", filename, 100, IV_MAX_LEN, {.rangeInt = {.min = 0, .max = 99}} },
        { "📝 Enter name for this loaded list: ", listName, 50, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = 49}} }
    };
    appGetValidatedInput(fields, 2);
    
    // Load the data
    list* newList = loadListWithName(filename, "student", SINGLY);
    if (!newList) {
        printf("%s❌ Failed to load student data from file '%s'!%s\n", UI_ERROR, filename, TXT_RESET);
        printf("%sPlease make sure the file exists and is in the correct format.%s\n", UI_WARNING, TXT_RESET);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Add to manager
    if (stuManager.studentListCount >= 10) {
        printf("%s⚠️  Maximum number of student lists reached!%s\n", UI_WARNING, TXT_RESET);
        destroyList(&newList, freeStudent);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    stuManager.studentLists[stuManager.studentListCount] = newList;
    strncpy(stuManager.studentListNames[stuManager.studentListCount], listName, 49);
    stuManager.studentListNames[stuManager.studentListCount][49] = '\0';
    stuManager.activeStudentList = stuManager.studentListCount;
    stuManager.studentListCount++;
    
    printf("%s✅ Student list '%s' loaded successfully!%s\n", UI_SUCCESS, listName, TXT_RESET);
    printf("%s📊 Loaded %d student records.%s\n", UI_INFO, newList->size, TXT_RESET);
    printf("%sThis list is now active.%s\n", UI_INFO, TXT_RESET);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

int handleSwitchEmployeeList(void) {
    winTermClearScreen();
    printf("=== Switch Employee List ===\n\n");
    
    if (empManager.employeeListCount == 0) {
        printf("No employee lists available!\n");
        printf("Create an employee list first.\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    if (empManager.employeeListCount == 1) {
        printf("Only one employee list available: %s\n", empManager.employeeListNames[0]);
        printf("It is already active.\n");
        waitForKeypress(NULL);
        return 0;
    }
    
    printf("Available Employee Lists:\n");
    printf("=========================\n");
    for (int i = 0; i < empManager.employeeListCount; i++) {
        char activeMarker = (i == empManager.activeEmployeeList) ? '*' : ' ';
        int listSize = empManager.employeeLists[i] ? empManager.employeeLists[i]->size : 0;
        printf("%c %d. %s (%d employees)\n", 
               activeMarker, i + 1, empManager.employeeListNames[i], listSize);
    }
    
    printf("\n* = Currently Active List\n");
    char input[10];
    char prompt[100];
    sprintf(prompt, "Enter the number of the list to switch to (1-%d): ", empManager.employeeListCount);
    appFormField field = { prompt, input, 10, IV_RANGE_INT, {.rangeInt = {.min = 1, .max = empManager.employeeListCount}} };
    appGetValidatedInput(&field, 1);
    
    int choice = atoi(input) - 1; // Convert to 0-based index
    
    if (choice < 0 || choice >= empManager.employeeListCount) {
        printf("Invalid choice! Please enter a number between 1 and %d.\n", empManager.employeeListCount);
        waitForKeypress(NULL);
        return -1;
    }
    
    if (choice == empManager.activeEmployeeList) {
        printf("List '%s' is already active!\n", empManager.employeeListNames[choice]);
        waitForKeypress(NULL);
        return 0;
    }
    
    empManager.activeEmployeeList = choice;
    int listSize = empManager.employeeLists[choice] ? empManager.employeeLists[choice]->size : 0;
    
    printf("Successfully switched to employee list: %s\n", empManager.employeeListNames[choice]);
    printf("This list contains %d employees.\n", listSize);
    waitForKeypress(NULL);
    return 0;
}

int handleSwitchStudentList(void) {
    winTermClearScreen();
    printf("=== Switch Student List ===\n\n");
    
    if (stuManager.studentListCount == 0) {
        printf("No student lists available!\n");
        printf("Create a student list first.\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    if (stuManager.studentListCount == 1) {
        printf("Only one student list available: %s\n", stuManager.studentListNames[0]);
        printf("It is already active.\n");
        waitForKeypress(NULL);
        return 0;
    }
    
    printf("Available Student Lists:\n");
    printf("========================\n");
    for (int i = 0; i < stuManager.studentListCount; i++) {
        char activeMarker = (i == stuManager.activeStudentList) ? '*' : ' ';
        int listSize = stuManager.studentLists[i] ? stuManager.studentLists[i]->size : 0;
        printf("%c %d. %s (%d students)\n", 
               activeMarker, i + 1, stuManager.studentListNames[i], listSize);
    }
    
    printf("\n* = Currently Active List\n");
    char input[10];
    char prompt[100];
    sprintf(prompt, "Enter the number of the list to switch to (1-%d): ", stuManager.studentListCount);
    appFormField field = { prompt, input, 10, IV_RANGE_INT, {.rangeInt = {.min = 1, .max = stuManager.studentListCount}} };
    appGetValidatedInput(&field, 1);
    
    int choice = atoi(input) - 1; // Convert to 0-based index
    
    if (choice < 0 || choice >= stuManager.studentListCount) {
        printf("Invalid choice! Please enter a number between 1 and %d.\n", stuManager.studentListCount);
        waitForKeypress(NULL);
        return -1;
    }
    
    if (choice == stuManager.activeStudentList) {
        printf("List '%s' is already active!\n", stuManager.studentListNames[choice]);
        waitForKeypress(NULL);
        return 0;
    }
    
    stuManager.activeStudentList = choice;
    int listSize = stuManager.studentLists[choice] ? stuManager.studentLists[choice]->size : 0;
    
    printf("Successfully switched to student list: %s\n", stuManager.studentListNames[choice]);
    printf("This list contains %d students.\n", listSize);
    waitForKeypress(NULL);
    return 0;
}

/**
 * @brief Configuration management menu
 * @return Returns 0 on normal exit, other values on error
 */
int runConfigurationManagement(void) {
    char choice;
    char configPath[600];
    
    // Get the configuration file path
    getConfigPath(configPath, sizeof(configPath));
    
    Menu configMenu = {1, "Configuration Settings", (MenuOption[]){
        {'1', "Update Payroll Settings", "Modify payroll calculation parameters", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Update Academic Settings", "Change grading and academic thresholds", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Save Configuration", "Save current settings to configuration file", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Reset to Default Configuration", "Restore all settings to factory defaults", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'5', "Back to Main Menu", "Return to the main system menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 5
    };
    
    do {
        winTermClearScreen();
        
        // Display current configuration
        printf("=== Current Configuration Settings ===\n");
        printf("Payroll Settings:\n");
        printf("  Regular Hours: %.1f\n", g_config.regularHours);
        printf("  Overtime Rate: %.1f\n", g_config.overtimeRate);
        printf("\nAcademic Settings:\n");
        printf("  Passing Grade: %.1f\n", g_config.passingGrade);
        printf("  Min Grade: %.1f\n", g_config.minGrade);
        printf("  Max Grade: %.1f\n\n", g_config.maxGrade);
        
        choice = runMenuWithInterface(&configMenu);
        
        switch(choice) {
            case '1':
                handleUpdatePayrollSettings();
                break;
            case '2':
                handleUpdateAcademicSettings();
                break;
            case '3':
                handleSaveConfiguration(configPath);
                break;
            case '4':
                handleResetConfiguration(configPath);
                break;
            case '5':
                return 0;
            default:
                break;
        }
    } while (1);
    
    return 0;
}

/**
 * @brief Handles updating payroll configuration settings
 * @return Returns 0 on success, other values on error
 */
int handleUpdatePayrollSettings(void) {
    char regularHoursStr[50] = "";
    char overtimeRateStr[50] = "";
    bool configChanged = false;
    
    winTermClearScreen();
    printf("=== Update Payroll Settings ===\n\n");
    
    // Prepare prompts with current values
    char regularHoursPrompt[100];
    char overtimeRatePrompt[100];
    sprintf(regularHoursPrompt, "Regular Hours (40-240, current: %.1f): ", g_config.regularHours);
    sprintf(overtimeRatePrompt, "Overtime Rate (0.1-2.0, current: %.1f): ", g_config.overtimeRate);
    
    // Setup form fields with validation
    appFormField fields[] = {
        { regularHoursPrompt, regularHoursStr, sizeof(regularHoursStr), IV_OPTIONAL, {{0}} },
        { overtimeRatePrompt, overtimeRateStr, sizeof(overtimeRateStr), IV_OPTIONAL, {{0}} }
    };
    
    // Get validated input
    appGetValidatedInput(fields, 2);
    
    // Process regular hours if provided
    if (strlen(regularHoursStr) > 0) {
        float newRegularHours = (float)atof(regularHoursStr);
        
        // Additional validation
        if (newRegularHours >= 40.0f && newRegularHours <= 240.0f) {
            g_config.regularHours = newRegularHours;
            configChanged = true;
            printf("Regular Hours updated to %.1f\n", g_config.regularHours);
        } else {
            printf("\nInvalid input! Regular Hours must be between 40 and 240.\n");
            waitForKeypress("Press any key to continue...");
            return -1;
        }
    }
    
    // Process overtime rate if provided
    if (strlen(overtimeRateStr) > 0) {
        float newOvertimeRate = (float)atof(overtimeRateStr);
        
        // Additional validation
        if (newOvertimeRate >= 0.1f && newOvertimeRate <= 2.0f) {
            g_config.overtimeRate = newOvertimeRate;
            configChanged = true;
            printf("Overtime Rate updated to %.1f\n", g_config.overtimeRate);
        } else {
            printf("\nInvalid input! Overtime Rate must be between 0.1 and 2.0.\n");
            waitForKeypress("Press any key to continue...");
            return -1;
        }
    }
    
    if (configChanged) {
        printf("\nPayroll settings updated successfully!\n");
        
        // Ask if user wants to save changes
        if (appYesNoPrompt("\nDo you want to save these changes to the configuration file?")) {
            // Get the directory where the executable is located
            char configPath[600];
            getConfigPath(configPath, sizeof(configPath));
            
            if (saveConfig(configPath) == 0) {
                printf("\nConfiguration saved successfully to: %s\n", configPath);
            } else {
                printf("\nError saving configuration to: %s\n", configPath);
            }
        } else {
            printf("\nChanges are applied but not saved to the configuration file.\n");
            printf("You can save them later from the Configuration Settings menu.\n");
        }
    } else {
        printf("\nNo changes were made to the payroll settings.\n");
    }
    
    waitForKeypress("\nPress any key to continue...");
    return 0;
}

/**
 * @brief Handles updating academic configuration settings
 * @return Returns 0 on success, other values on error
 */
int handleUpdateAcademicSettings(void) {
    char passingGradeStr[50] = "";
    char minGradeStr[50] = "";
    char maxGradeStr[50] = "";
    bool configChanged = false;
    
    winTermClearScreen();
    printf("=== Update Academic Settings ===\n\n");
    
    // Prepare prompts with current values
    char passingGradePrompt[100];
    char minGradePrompt[100];
    char maxGradePrompt[100];
    
    sprintf(passingGradePrompt, "Passing Grade (50-90, current: %.1f): ", g_config.passingGrade);
    sprintf(minGradePrompt, "Min Grade (0-50, current: %.1f): ", g_config.minGrade);
    sprintf(maxGradePrompt, "Max Grade (90-100, current: %.1f): ", g_config.maxGrade);
    
    // Setup form fields with validation
    appFormField fields[] = {
        { passingGradePrompt, passingGradeStr, sizeof(passingGradeStr), IV_OPTIONAL, {{0}} },
        { minGradePrompt, minGradeStr, sizeof(minGradeStr), IV_OPTIONAL, {{0}} },
        { maxGradePrompt, maxGradeStr, sizeof(maxGradeStr), IV_OPTIONAL, {{0}} }
    };
    
    // Get validated input
    appGetValidatedInput(fields, 3);
    
    // Process passing grade if provided
    if (strlen(passingGradeStr) > 0) {
        float newPassingGrade = (float)atof(passingGradeStr);
        
        // Additional validation
        if (newPassingGrade >= 50.0f && newPassingGrade <= 90.0f) {
            g_config.passingGrade = newPassingGrade;
            configChanged = true;
            printf("Passing Grade updated to %.1f\n", g_config.passingGrade);
        } else {
            printf("\nInvalid input! Passing Grade must be between 50 and 90.\n");
            waitForKeypress("Press any key to continue...");
            return -1;
        }
    }
    
    // Process min grade if provided
    if (strlen(minGradeStr) > 0) {
        float newMinGrade = (float)atof(minGradeStr);
        
        // Additional validation
        if (newMinGrade >= 0.0f && newMinGrade <= 50.0f) {
            g_config.minGrade = newMinGrade;
            configChanged = true;
            printf("Min Grade updated to %.1f\n", g_config.minGrade);
        } else {
            printf("\nInvalid input! Min Grade must be between 0 and 50.\n");
            waitForKeypress("Press any key to continue...");
            return -1;
        }
    }
    
    // Process max grade if provided
    if (strlen(maxGradeStr) > 0) {
        float newMaxGrade = (float)atof(maxGradeStr);
        
        // Additional validation
        if (newMaxGrade >= 90.0f && newMaxGrade <= 100.0f) {
            g_config.maxGrade = newMaxGrade;
            configChanged = true;
            printf("Max Grade updated to %.1f\n", g_config.maxGrade);
        } else {
            printf("\nInvalid input! Max Grade must be between 90 and 100.\n");
            waitForKeypress("Press any key to continue...");
            return -1;
        }
    }
    
    if (configChanged) {
        printf("\nAcademic settings updated successfully!\n");
        
        // Ask if user wants to save changes
        if (appYesNoPrompt("\nDo you want to save these changes to the configuration file?")) {
            char configPath[600];
            getConfigPath(configPath, sizeof(configPath));
            
            if (saveConfig(configPath) == 0) {
                printf("\nConfiguration saved successfully to: %s\n", configPath);
            } else {
                printf("\nError saving configuration to: %s\n", configPath);
            }
        } else {
            printf("\nChanges are applied but not saved to the configuration file.\n");
            printf("You can save them later from the Configuration Settings menu.\n");
        }
    } else {
        printf("\nNo changes were made to the academic settings.\n");
    }
    
    waitForKeypress("\nPress any key to continue...");
    return 0;
}

/**
 * @brief Handles saving the current configuration to file
 * @param configPath Path to the configuration file
 * @return Returns 0 on success, other values on error
 */
int handleSaveConfiguration(const char* configPath) {
    winTermClearScreen();
    printf("=== Save Configuration ===\n\n");
    
    if (saveConfig(configPath) == 0) {
        printf("Configuration saved successfully to: %s\n", configPath);
    } else {
        printf("Error saving configuration to: %s\n", configPath);
    }
    
    waitForKeypress("\nPress any key to continue...");
    return 0;
}

/**
 * @brief Handles resetting the configuration to default values
 * @param configPath Path to the configuration file
 * @return Returns 0 on success, other values on error
 */
int handleResetConfiguration(const char* configPath) {
    winTermClearScreen();
    printf("=== Reset Configuration ===\n\n");
    
    if (appYesNoPrompt("Are you sure you want to reset all configuration settings to default values?")) {
        setDefaultConfig();
        printf("\nConfiguration has been reset to default values.\n");
        
        if (appYesNoPrompt("\nDo you want to save these default values to the configuration file?")) {
            if (saveConfig(configPath) == 0) {
                printf("\nDefault configuration saved to: %s\n", configPath);
            } else {
                printf("\nWarning: Could not save default configuration to: %s\n", configPath);
            }
        } else {
            printf("\nDefault values are applied but not saved to the configuration file.\n");
            printf("You can save them later from the Configuration Settings menu.\n");
        }
    } else {
        printf("\nReset operation cancelled.\n");
    }
    
    waitForKeypress("\nPress any key to continue...");
    return 0;
} 