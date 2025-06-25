#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "menuio.h"
#include "empio.h"
#include "stuio.h"
#include "../modules/data.h"
#include "../modules/payroll.h"
#include "../../include/headers/apctxt.h"
#include "../../include/headers/state.h"
#include "../../include/models/employee.h"
#include "../../include/models/student.h"
#include "../../include/headers/list.h"

// Global managers
EmployeeManager empManager;
StudentManager stuManager;

// Main menu definition
Menu mainMenu = {1, "PUP Information Management System", (MenuOption[]){
    {'1', "Employee Management", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Student Management", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "System Statistics", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'4', "Configuration Settings", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'5', "Exit", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 5
};

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
 * @brief Updates main menu option states
 */
void checkStates(void) {
    // Main menu options are always available
}

/**
 * @brief Generic function to check if an active list exists and has items
 * @param isActiveList Flag indicating if there's an active list
 * @param listSize Size of the active list (0 if no active list)
 * @param errorMessage Message to display if no active list
 * @return Returns 1 if list exists and operation can proceed, 0 otherwise
 */
static int checkActiveList(int isActiveList, int listSize, const char* errorMessage) {
    if (!isActiveList) {
        printf("\n%s\n", errorMessage ? errorMessage : "No active list!");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }
    return 1;
}

/**
 * @brief Updates menu option states based on list availability
 * @param menu Pointer to the menu to update
 * @param hasActiveList Flag indicating if there's an active list
 * @param hasItems Flag indicating if the active list has items
 * @param hasMultipleLists Flag indicating if there are multiple lists
 */
static void updateMenuStates(Menu* menu, int hasActiveList, int hasItems, int hasMultipleLists) {
    // Create List (1) - Always available
    menu->options[0].isDisabled = 0;
    
    // Switch List (2) - Only if multiple lists exist
    menu->options[1].isDisabled = !hasMultipleLists;
    
    // Add Item (3) - Only if there's an active list
    menu->options[2].isDisabled = !hasActiveList;
    
    // Edit Item (4) - Only if there are items in active list
    menu->options[3].isDisabled = !hasItems;
    
    // Delete Item (5) - Only if there are items in active list
    menu->options[4].isDisabled = !hasItems;
    
    // Search Item (6) - Only if there are items in active list
    menu->options[5].isDisabled = !hasItems;
    
    // Display All Items (7) - Only if there are items in active list
    menu->options[6].isDisabled = !hasItems;
    
    // Report (8) - Only if there are items in active list
    menu->options[7].isDisabled = !hasItems;
    
    // Save List (9) - Only if there's an active list with items
    menu->options[8].isDisabled = !hasItems;
    
    // Load List (A) - Always available
    menu->options[9].isDisabled = 0;
    
    // Back to Main Menu (B) - Always available
    menu->options[10].isDisabled = 0;
}

/**
 * @brief Updates employee menu option states based on current manager state
 * @param menu Pointer to the employee menu to update
 */
static void updateEmployeeMenuStates(Menu* menu) {
    int hasActiveList = (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList] != 0);
    int hasEmployees = hasActiveList && (empManager.employeeLists[empManager.activeEmployeeList]->size > 0);
    int hasMultipleLists = (empManager.employeeListCount > 1);
    
    updateMenuStates(menu, hasActiveList, hasEmployees, hasMultipleLists);
}

/**
 * @brief Updates student menu option states based on current manager state
 * @param menu Pointer to the student menu to update
 */
static void updateStudentMenuStates(Menu* menu) {
    int hasActiveList = (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList] != 0);
    int hasStudents = hasActiveList && (stuManager.studentLists[stuManager.activeStudentList]->size > 0);
    int hasMultipleStudents = hasActiveList && (stuManager.studentLists[stuManager.activeStudentList]->size > 1);
    int hasMultipleLists = (stuManager.studentListCount > 1);
    
    updateMenuStates(menu, hasActiveList, hasStudents, hasMultipleLists);
    
    // Update Sort Students option (menu option 8) - needs at least 2 students
    menu->options[7].isDisabled = !hasMultipleStudents;
}

/**
 * @brief Main menu loop
 * @return Returns 0 on normal exit, other values on error
 */
int menuLoop(void) {
    char choice;
    
    do {
        checkStates();
        winTermClearScreen();
        
        // Display system status
        printf("=== PUP Information Management System ===\n");
        printf("Employee Lists: %d | Active: %s\n", 
               empManager.employeeListCount,
               (empManager.activeEmployeeList >= 0) ? 
               empManager.employeeListNames[empManager.activeEmployeeList] : "None");
        printf("Student Lists: %d | Active: %s\n\n", 
               stuManager.studentListCount,
               (stuManager.activeStudentList >= 0) ? 
               stuManager.studentListNames[stuManager.activeStudentList] : "None");
        
        choice = initMenu(&mainMenu);
        
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
                printf("\nExiting PUP Information Management System...\n");
                return 0;
            default:
                printf("\nInvalid option. Press any key to continue...");
                _getch();
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
    printf("=== System Statistics ===\n\n");
    
    printf("EMPLOYEE MANAGEMENT:\n");
    printf("Total Employee Lists: %d\n", empManager.employeeListCount);
    int totalEmployees = 0;
    for (int i = 0; i < empManager.employeeListCount; i++) {
        if (empManager.employeeLists[i]) {
            int count = empManager.employeeLists[i]->size;
            totalEmployees += count;
            printf("  - %s: %d employees\n", empManager.employeeListNames[i], count);
        }
    }
    printf("Total Employees: %d\n", totalEmployees);
    
    if (empManager.activeEmployeeList >= 0) {
        printf("Active Employee List: %s\n", empManager.employeeListNames[empManager.activeEmployeeList]);
    } else {
        printf("Active Employee List: None\n");
    }
    printf("\n");
    
    printf("STUDENT MANAGEMENT:\n");
    printf("Total Student Lists: %d\n", stuManager.studentListCount);
    int totalStudents = 0;
    for (int i = 0; i < stuManager.studentListCount; i++) {
        if (stuManager.studentLists[i]) {
            int count = stuManager.studentLists[i]->size;
            totalStudents += count;
            printf("  - %s: %d students\n", stuManager.studentListNames[i], count);
        }
    }
    printf("Total Students: %d\n", totalStudents);
    
    if (stuManager.activeStudentList >= 0) {
        printf("Active Student List: %s\n", stuManager.studentListNames[stuManager.activeStudentList]);
    } else {
        printf("Active Student List: None\n");
    }
    printf("\n");
    
    printf("OVERALL STATISTICS:\n");
    printf("Total Records: %d\n", totalEmployees + totalStudents);
    printf("Total Lists: %d\n", empManager.employeeListCount + stuManager.studentListCount);
    
    printf("\nPress any key to continue...");
    _getch();
}

/**
 * @brief Employee management submenu
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
        {'1', "Create Employee List", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Switch Employee List", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Add Employee", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Edit Employee", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'5', "Delete Employee", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'6', "Search Employee", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'7', "Display All Employees", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'8', "Payroll Report", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'9', "Save Employee List", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'A', "Load Employee List", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'B', "Back to Main Menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 11};
    
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
        updateEmployeeMenuStates(&employeeMenu);
        
        choice = initMenu(&employeeMenu);
        
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
            case '5':
                if (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList]) {
                    // Call function from empio.c
                    extern int handleDeleteEmployee(list* employeeList);
                    handleDeleteEmployee(empManager.employeeLists[empManager.activeEmployeeList]);
                } else {
                    printf("\nNo active employee list!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '6':
                if (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList]) {
                    // Call function from empio.c
                    extern int handleSearchEmployee(const list* employeeList);
                    handleSearchEmployee(empManager.employeeLists[empManager.activeEmployeeList]);
                } else {
                    printf("\nNo active employee list!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '7':
                handleDisplayAllEmployees();
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
                printf("\nInvalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (1);
    
    return 0;
}

/**
 * @brief Student management submenu
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
        {'1', "Create Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Switch Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Add Student", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Edit Student", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'5', "Delete Student", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'6', "Search Student", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'7', "Display All Students", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'8', "Sort Students by Grade", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'9', "Student Report", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'S', "Save Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'L', "Load Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'B', "Back to Main Menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 12};
    
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
        updateStudentMenuStates(&studentMenu);
        
        choice = initMenu(&studentMenu);
        
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
            case '4':
                if (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]) {
                    // Call function from stuio.c
                    extern int handleEditStudent(list* studentList);
                    handleEditStudent(stuManager.studentLists[stuManager.activeStudentList]);
                } else {
                    printf("\nNo active student list!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '5':
                if (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]) {
                    // Call function from stuio.c
                    extern int handleDeleteStudent(list* studentList);
                    handleDeleteStudent(stuManager.studentLists[stuManager.activeStudentList]);
                } else {
                    printf("\nNo active student list!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '6':
                if (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]) {
                    // Call function from stuio.c
                    extern int handleSearchStudent(const list* studentList);
                    handleSearchStudent(stuManager.studentLists[stuManager.activeStudentList]);
                } else {
                    printf("\nNo active student list!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '7':
                handleDisplayAllStudents();
                break;
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
                printf("\nInvalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (1);
    
    return 0;
}

/**
 * @brief Creates a new employee list
 */
int handleCreateEmployeeList(void) {
    winTermClearScreen();
    printf("=== Create New Employee List ===\n\n");
    
    if (empManager.employeeListCount >= 10) {
        printf("Maximum number of employee lists (10) reached!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    char listName[50];
    appFormField field = { "Enter name for this employee list: ", listName, 50, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = 49}} };
    appGetValidatedInput(&field, 1);
    
    // Create new list
    list* newList = NULL;
    if (createEmployeeList(&newList) != 0) {
        printf("Failed to create employee list!\n");
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
    
    printf("Employee list '%s' created successfully!\n", listName);
    printf("This list is now active. Add employees to get started.\n");
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
    printf("=== Payroll Report ===\n\n");
    
    int hasActiveList = (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList]);
    if (!checkActiveList(hasActiveList, 0, "No active employee list!")) {
        return -1;
    }
    
    printf("Generating payroll report for: %s\n\n", empManager.employeeListNames[empManager.activeEmployeeList]);
    
    // Generate the payroll report file
    char reportFilePath[512];
    int reportResult = generatePayrollReportFile(empManager.employeeLists[empManager.activeEmployeeList], reportFilePath, sizeof(reportFilePath));
    
    if (reportResult > 0) {
        printf("Successfully generated payroll report!\n");
        printf("Report saved to: %s\n", reportFilePath);
        printf("Processed %d employees\n\n", reportResult);
        
        // Display the report content in the terminal
        printf("=== Employee Payroll Report ===\n");
        printf("%-12s | %-20s | %-8s | %-10s | %-10s | %-10s | %-10s | %-6s\n",
               "Emp. Number", "Employee Name", "Status", "Basic Pay", "Overtime", "Deductions", "Net Pay", "Hours");
        printf("------------------------------------------------------------------------------------------------------\n");
        
        list* employeeList = empManager.employeeLists[empManager.activeEmployeeList];
        node* current = employeeList->head;
        int count = 0;
        double totalBasicPay = 0.0;
        double totalOvertimePay = 0.0;
        double totalDeductions = 0.0;
        double totalNetPay = 0.0;
        
        if (current != NULL) {
            do {
                Employee* emp = (Employee*)current->data;
                if (emp != NULL) {
                    count++;
                    
                    printf("%-12s | %-20s | %-8s | %10.2f | %10.2f | %10.2f | %10.2f | %6d\n",
                           emp->personal.employeeNumber,
                           emp->personal.name.fullName,
                           (emp->employment.status == statusRegular) ? "Regular" : "Casual",
                           emp->payroll.basicPay,
                           emp->payroll.overtimePay,
                           emp->payroll.deductions,
                           emp->payroll.netPay,
                           emp->employment.hoursWorked);
                           
                    totalBasicPay += emp->payroll.basicPay;
                    totalOvertimePay += emp->payroll.overtimePay;
                    totalDeductions += emp->payroll.deductions;
                    totalNetPay += emp->payroll.netPay;
                }
                current = current->next;
            } while (current != employeeList->head && current != NULL);
        }
        
        printf("------------------------------------------------------------------------------------------------------\n");
        printf("%-32s | %10.2f | %10.2f | %10.2f | %10.2f |\n",
               "TOTALS:", totalBasicPay, totalOvertimePay, totalDeductions, totalNetPay);
        printf("------------------------------------------------------------------------------------------------------\n");
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
    printf("=== Load Employee List ===\n\n");
    
    // List available employee data files
    listEmployeeDataFiles();
    printf("\n");
    
    char filename[100];
    char listName[50];
    appFormField fields[] = {
        { "Enter filename to load: ", filename, 100, IV_MAX_LEN, {.rangeInt = {.min = 0, .max = 99}} },
        { "Enter name for this loaded list: ", listName, 50, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = 49}} }
    };
    appGetValidatedInput(fields, 2);
    
    // Load the data
    list* newList = loadListWithName(filename, "employee", SINGLY);
    if (!newList) {
        printf("Failed to load employee data from file '%s'!\n", filename);
        printf("Make sure the file exists and is in the correct format.\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Add to manager
    if (empManager.employeeListCount >= 10) {
        printf("Maximum number of employee lists reached!\n");
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
    
    printf("Employee list '%s' loaded successfully!\n", listName);
    printf("Loaded %d employee records.\n", newList->size);
    printf("This list is now active.\n");
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
    printf("=== Student Report ===\n\n");
    
    int hasActiveList = (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]);
    if (!checkActiveList(hasActiveList, 0, "No active student list!")) {
        return -1;
    }
    
    printf("Generating student report for: %s\n\n", stuManager.studentListNames[stuManager.activeStudentList]);
    
    // Generate the student report file
    char reportFilePath[512];
    int reportResult = generateStudentReportFile(stuManager.studentLists[stuManager.activeStudentList], reportFilePath, sizeof(reportFilePath));
    
    if (reportResult > 0) {
        printf("Successfully generated student report!\n");
        printf("Report saved to: %s\n", reportFilePath);
        printf("Processed %d students\n\n", reportResult);
        
        // Display the report content in the terminal
        printf("=== Student Academic Report ===\n");
        printf("%-12s | %-20s | %-8s | %-6s | %-6s | %-6s | %-6s | %-8s\n",
               "Student No.", "Student Name", "Program", "Year", "Prelim", "Midterm", "Final", "Remarks");
        printf("-------------------------------------------------------------------------------------\n");
        
        list* studentList = stuManager.studentLists[stuManager.activeStudentList];
        node* current = studentList->head;
        int count = 0;
        double totalFinalGrade = 0.0;
        int passedCount = 0;
        
        if (current != NULL) {
            do {
                Student* stu = (Student*)current->data;
                if (stu != NULL) {
                    count++;
                    
                    printf("%-12s | %-20s | %-8s | %6d | %6.2f | %6.2f | %6.2f | %-8s\n",
                           stu->personal.studentNumber,
                           stu->personal.name.fullName,
                           stu->personal.programCode,
                           stu->personal.yearLevel,
                           stu->academic.prelimGrade,
                           stu->academic.midtermGrade,
                           stu->academic.finalExamGrade,
                           stu->academic.remarks);
                           
                    totalFinalGrade += stu->academic.finalGrade;
                    if (strcmp(stu->academic.remarks, "Passed") == 0) {
                        passedCount++;
                    }
                }
                current = current->next;
            } while (current != studentList->head && current != NULL);
        }
        
        printf("-------------------------------------------------------------------------------------\n");
        printf("Total students: %d\n", count);
        printf("Average grade: %.2f\n", totalFinalGrade / count);
        printf("Passed: %d (%.1f%%)\n", passedCount, (passedCount * 100.0) / count);
        printf("Failed: %d (%.1f%%)\n", count - passedCount, ((count - passedCount) * 100.0) / count);
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
    printf("=== Load Student List ===\n\n");
    
    // List available student data files
    listStudentDataFiles();
    printf("\n");
    
    char filename[100];
    char listName[50];
    appFormField fields[] = {
        { "Enter filename to load: ", filename, 100, IV_MAX_LEN, {.rangeInt = {.min = 0, .max = 99}} },
        { "Enter name for this loaded list: ", listName, 50, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = 49}} }
    };
    appGetValidatedInput(fields, 2);
    
    // Load the data
    list* newList = loadListWithName(filename, "student", SINGLY);
    if (!newList) {
        printf("Failed to load student data from file '%s'!\n", filename);
        printf("Make sure the file exists and is in the correct format.\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Add to manager
    if (stuManager.studentListCount >= 10) {
        printf("Maximum number of student lists reached!\n");
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
    
    printf("Student list '%s' loaded successfully!\n", listName);
    printf("Loaded %d student records.\n", newList->size);
    printf("This list is now active.\n");
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
        {'1', "Update Payroll Settings", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Update Academic Settings", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Save Configuration", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Reset to Default Configuration", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'5', "Back to Main Menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 5
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
        
        choice = initMenu(&configMenu);
        
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
                printf("\nInvalid option. Press any key to continue...");
                _getch();
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
        { regularHoursPrompt, regularHoursStr, sizeof(regularHoursStr), IV_OPTIONAL, {0} },
        { overtimeRatePrompt, overtimeRateStr, sizeof(overtimeRateStr), IV_OPTIONAL, {0} }
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
        { passingGradePrompt, passingGradeStr, sizeof(passingGradeStr), IV_OPTIONAL, {0} },
        { minGradePrompt, minGradeStr, sizeof(minGradeStr), IV_OPTIONAL, {0} },
        { maxGradePrompt, maxGradeStr, sizeof(maxGradeStr), IV_OPTIONAL, {0} }
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