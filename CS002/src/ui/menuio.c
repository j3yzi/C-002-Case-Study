#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "menuio.h"
#include "empio.h"
#include "stuio.h"
#include "../modules/data.h"
#include "../modules/payroll.h"
#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/state.h"
#include "../../../include/models/employee.h"
#include "../../../include/models/student.h"
#include "../../../include/headers/list.h"

#define HIGHLIGHT_TEXT_COLOR 9
#define HIGHLIGHT_BG_COLOR 0
#define TEXT_COLOR 7
#define BG_COLOR 0
#define DISABLED_TEXT_COLOR 8
#define DISABLED_BG_COLOR 0

// Global managers
static EmployeeManager empManager;
static StudentManager stuManager;

// Menu definitions
Menu mainMenu = {1, "CS002 - Dual Management System", (MenuOption[]){
    {'1', "Employee Management", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Student Management", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "System Statistics", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'4', "Exit", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 4};

Menu employeeMenu = {2, "Employee Management", (MenuOption[]){
    {'1', "Create New Employee List", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Switch Active List", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "Add Employee to Active List", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'4', "Edit Employee", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'5', "Delete Employee", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'6', "Search Employee", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'7', "Display All Employees", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'8', "Generate Payroll Report", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'9', "Save Active List", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'A', "Load Employee List", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'B', "Back to Main Menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 11};

Menu studentMenu = {3, "Student Management", (MenuOption[]){
    {'1', "Create New Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Switch Active List", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "Add Student to Active List", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'4', "Edit Student", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'5', "Delete Student", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'6', "Search Student", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'7', "Display All Students", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'8', "Compute Final Grades", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'9', "Sort by Grade", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'A', "Generate Academic Report", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'B', "Save Active List", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'C', "Load Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'D', "Back to Main Menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 13};

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
    
    printf("Multi-list management system initialized!\n");
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
    
    printf("Multi-list management system cleaned up!\n");
}

/**
 * @brief Updates menu option states based on current application state
 */
void checkStates(void) {
    // Employee menu states
    if (empManager.employeeListCount > 0) {
        employeeMenu.options[1].isDisabled = false; // Switch Active List
        
        if (empManager.activeEmployeeList >= 0 && 
            empManager.employeeLists[empManager.activeEmployeeList] &&
            empManager.employeeLists[empManager.activeEmployeeList]->size > 0) {
            // Enable all employee operations
            for (int i = 2; i < 9; i++) {
                employeeMenu.options[i].isDisabled = false;
            }
        } else {
            // Enable only add employee, disable others
            employeeMenu.options[2].isDisabled = false; // Add Employee
            for (int i = 3; i < 9; i++) {
                employeeMenu.options[i].isDisabled = true;
            }
        }
    } else {
        // No employee lists, disable all except create
        for (int i = 1; i < 9; i++) {
            employeeMenu.options[i].isDisabled = true;
        }
    }
    
    // Student menu states
    if (stuManager.studentListCount > 0) {
        studentMenu.options[1].isDisabled = false; // Switch Active List
        
        if (stuManager.activeStudentList >= 0 && 
            stuManager.studentLists[stuManager.activeStudentList] &&
            stuManager.studentLists[stuManager.activeStudentList]->size > 0) {
            // Enable all student operations
            for (int i = 2; i < 12; i++) {
                studentMenu.options[i].isDisabled = false;
            }
        } else {
            // Enable only add student, disable others
            studentMenu.options[2].isDisabled = false; // Add Student
            for (int i = 3; i < 12; i++) {
                studentMenu.options[i].isDisabled = true;
            }
        }
    } else {
        // No student lists, disable all except create
        for (int i = 1; i < 12; i++) {
            studentMenu.options[i].isDisabled = true;
        }
    }
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
        printf("=== CS002 Dual Management System ===\n");
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
                displaySystemStatistics();
                break;
            case '4':
                printf("\nExiting CS002 Dual Management System...\n");
                return 0;
            default:
                printf("\nInvalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (true);
    
    return 0;
}

/**
 * @brief Runs the employee management submenu
 * @return 0 on success, -1 on failure
 */
int runEmployeeManagement(void) {
    char choice;
    
    do {
        checkStates();
        winTermClearScreen();
        
        printf("=== Employee Management ===\n");
        printf("Lists: %d | Active: %s | Records: %d\n\n", 
               empManager.employeeListCount,
               (empManager.activeEmployeeList >= 0) ? 
               empManager.employeeListNames[empManager.activeEmployeeList] : "None",
               (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList]) ?
               empManager.employeeLists[empManager.activeEmployeeList]->size : 0);
        
        choice = initMenu(&employeeMenu);
        
        switch(choice) {
            case '1':
                handleCreateEmployeeList();
                break;
            case '2':
                if (!employeeMenu.options[1].isDisabled) {
                    handleSwitchEmployeeList();
                } else {
                    printf("\nNo employee lists available to switch!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '3':
                if (!employeeMenu.options[2].isDisabled) {
                    handleAddEmployee();
                } else {
                    printf("\nPlease create an employee list first!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '4':
                if (!employeeMenu.options[3].isDisabled) {
                    handleEditEmployee(empManager.employeeLists[empManager.activeEmployeeList]);
                } else {
                    printf("\nNo employees to edit!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '5':
                if (!employeeMenu.options[4].isDisabled) {
                    handleDeleteEmployee(empManager.employeeLists[empManager.activeEmployeeList]);
                } else {
                    printf("\nNo employees to delete!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '6':
                if (!employeeMenu.options[5].isDisabled) {
                    handleSearchEmployee(empManager.employeeLists[empManager.activeEmployeeList]);
                } else {
                    printf("\nNo employees to search!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '7':
                if (!employeeMenu.options[6].isDisabled) {
                    handleDisplayAllEmployees();
                } else {
                    printf("\nNo employees to display!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '8':
                if (!employeeMenu.options[7].isDisabled) {
                    handlePayrollReport();
                } else {
                    printf("\nNo employee data for payroll report!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '9':
                if (!employeeMenu.options[8].isDisabled) {
                    handleSaveEmployeeList();
                } else {
                    printf("\nNo employee data to save!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case 'A':
            case 'a':
                handleLoadEmployeeList();
                break;
            case 'B':
            case 'b':
                return 0; // Back to main menu
            default:
                printf("\nInvalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (true);
    
    return 0;
}

/**
 * @brief Runs the student management submenu
 * @return 0 on success, -1 on failure
 */
int runStudentManagement(void) {
    char choice;
    
    do {
        checkStates();
        winTermClearScreen();
        
        printf("=== Student Management ===\n");
        printf("Lists: %d | Active: %s | Records: %d\n\n", 
               stuManager.studentListCount,
               (stuManager.activeStudentList >= 0) ? 
               stuManager.studentListNames[stuManager.activeStudentList] : "None",
               (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]) ?
               stuManager.studentLists[stuManager.activeStudentList]->size : 0);
        
        choice = initMenu(&studentMenu);
        
        switch(choice) {
            case '1':
                handleCreateStudentList();
                break;
            case '2':
                if (!studentMenu.options[1].isDisabled) {
                    handleSwitchStudentList();
                } else {
                    printf("\nNo student lists available to switch!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '3':
                if (!studentMenu.options[2].isDisabled) {
                    handleAddStudent();
                } else {
                    printf("\nPlease create a student list first!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '4':
                if (!studentMenu.options[3].isDisabled) {
                    handleEditStudent(stuManager.studentLists[stuManager.activeStudentList]);
                } else {
                    printf("\nNo students to edit!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '5':
                if (!studentMenu.options[4].isDisabled) {
                    handleDeleteStudent(stuManager.studentLists[stuManager.activeStudentList]);
                } else {
                    printf("\nNo students to delete!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '6':
                if (!studentMenu.options[5].isDisabled) {
                    handleSearchStudent(stuManager.studentLists[stuManager.activeStudentList]);
                } else {
                    printf("\nNo students to search!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '7':
                if (!studentMenu.options[6].isDisabled) {
                    handleDisplayAllStudents();
                } else {
                    printf("\nNo students to display!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '8':
                if (!studentMenu.options[7].isDisabled) {
                    handleComputeFinalGrades();
                } else {
                    printf("\nNo students for grade computation!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '9':
                if (!studentMenu.options[8].isDisabled) {
                    handleSortStudentsByGrade();
                } else {
                    printf("\nNo students to sort!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case 'A':
            case 'a':
                if (!studentMenu.options[9].isDisabled) {
                    handleStudentReport();
                } else {
                    printf("\nNo student data for report!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case 'B':
            case 'b':
                if (!studentMenu.options[10].isDisabled) {
                    handleSaveStudentList();
                } else {
                    printf("\nNo student data to save!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case 'C':
            case 'c':
                handleLoadStudentList();
                break;
            case 'D':
            case 'd':
                return 0; // Back to main menu
            default:
                printf("\nInvalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (true);
    
    return 0;
}

// Employee management implementations
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
    printf("Enter name for this employee list: ");
    if (!fgets(listName, sizeof(listName), stdin)) {
        return -1;
    }
    listName[strcspn(listName, "\n")] = 0; // Remove newline
    
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

int handleSwitchEmployeeList(void) {
    winTermClearScreen();
    printf("=== Switch Active Employee List ===\n\n");
    
    if (empManager.employeeListCount == 0) {
        printf("No employee lists available!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Available Employee Lists:\n");
    for (int i = 0; i < empManager.employeeListCount; i++) {
        printf("%d. %s (%d employees) %s\n", i + 1, 
               empManager.employeeListNames[i],
               empManager.employeeLists[i] ? empManager.employeeLists[i]->size : 0,
               (i == empManager.activeEmployeeList) ? "[ACTIVE]" : "");
    }
    
    printf("\nSelect list to activate (1-%d): ", empManager.employeeListCount);
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    getchar(); // Clear newline
    
    if (choice < 1 || choice > empManager.employeeListCount) {
        printf("Invalid choice!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    empManager.activeEmployeeList = choice - 1;
    printf("Active employee list switched to: %s\n", empManager.employeeListNames[empManager.activeEmployeeList]);
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

int handleDisplayAllEmployees(void) {
    winTermClearScreen();
    printf("=== Display All Employees ===\n\n");
    
    if (empManager.activeEmployeeList < 0 || !empManager.employeeLists[empManager.activeEmployeeList]) {
        printf("No active employee list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Active List: %s\n", empManager.employeeListNames[empManager.activeEmployeeList]);
    displayAllEmployees(empManager.employeeLists[empManager.activeEmployeeList]);
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

int handlePayrollReport(void) {
    winTermClearScreen();
    printf("=== Payroll Report ===\n\n");
    
    if (empManager.activeEmployeeList < 0 || !empManager.employeeLists[empManager.activeEmployeeList]) {
        printf("No active employee list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Generating payroll report for: %s\n\n", empManager.employeeListNames[empManager.activeEmployeeList]);
    
    // Generate the payroll report file
    char reportFilePath[512];
    int reportResult = generatePayrollReportFile(empManager.employeeLists[empManager.activeEmployeeList], reportFilePath, sizeof(reportFilePath));
    
    if (reportResult > 0) {
        printf("Successfully generated payroll report!\n");
        printf("Report saved to: %s\n", reportFilePath);
        printf("Processed %d employees\n", reportResult);
    } else {
        printf("Failed to generate payroll report.\n");
    }
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

int handleSaveEmployeeList(void) {
    winTermClearScreen();
    printf("=== Save Employee List ===\n\n");
    
    if (empManager.activeEmployeeList < 0 || !empManager.employeeLists[empManager.activeEmployeeList]) {
        printf("No active employee list to save!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Saving list: %s\n", empManager.employeeListNames[empManager.activeEmployeeList]);
    
    // Show existing data files
    printf("\nExisting employee data files:\n");
    listEmployeeDataFiles();
    printf("\n");
    
    char filename[100];
    printf("Enter filename (will be saved as 'employee_LISTNAME_TIMESTAMP.dat'): ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        return -1;
    }
    filename[strcspn(filename, "\n")] = 0; // Remove newline
    
    // Use the custom save function
    int savedCount = saveListWithCustomName(empManager.employeeLists[empManager.activeEmployeeList], 
                                           filename, "employee");
    
    if (savedCount >= 0) {
        printf("Successfully saved %d employee records!\n", savedCount);
        printf("Data saved to data directory with timestamp.\n");
    } else {
        printf("Failed to save employee list.\n");
    }
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

int handleLoadEmployeeList(void) {
    winTermClearScreen();
    printf("=== Load Employee List ===\n\n");
    
    // List available employee data files
    listEmployeeDataFiles();
    printf("\n");
    
    char filename[100];
    printf("Enter filename to load: ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        return -1;
    }
    filename[strcspn(filename, "\n")] = 0; // Remove newline
    
    char listName[50];
    printf("Enter name for this loaded list: ");
    if (!fgets(listName, sizeof(listName), stdin)) {
        return -1;
    }
    listName[strcspn(listName, "\n")] = 0; // Remove newline
    
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

// Student management implementations
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
    printf("Enter name for this student list: ");
    if (!fgets(listName, sizeof(listName), stdin)) {
        return -1;
    }
    listName[strcspn(listName, "\n")] = 0; // Remove newline
    
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

int handleSwitchStudentList(void) {
    winTermClearScreen();
    printf("=== Switch Active Student List ===\n\n");
    
    if (stuManager.studentListCount == 0) {
        printf("No student lists available!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Available Student Lists:\n");
    for (int i = 0; i < stuManager.studentListCount; i++) {
        printf("%d. %s (%d students) %s\n", i + 1, 
               stuManager.studentListNames[i],
               stuManager.studentLists[i] ? stuManager.studentLists[i]->size : 0,
               (i == stuManager.activeStudentList) ? "[ACTIVE]" : "");
    }
    
    printf("\nSelect list to activate (1-%d): ", stuManager.studentListCount);
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    getchar(); // Clear newline
    
    if (choice < 1 || choice > stuManager.studentListCount) {
        printf("Invalid choice!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    stuManager.activeStudentList = choice - 1;
    printf("Active student list switched to: %s\n", stuManager.studentListNames[stuManager.activeStudentList]);
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
    
    // Calculate final grade and remarks
    calculateFinalGrade(newStudent);
    
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

int handleDisplayAllStudents(void) {
    winTermClearScreen();
    printf("=== Display All Students ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Active List: %s\n", stuManager.studentListNames[stuManager.activeStudentList]);
    displayAllStudents(stuManager.studentLists[stuManager.activeStudentList]);
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

int handleComputeFinalGrades(void) {
    winTermClearScreen();
    printf("=== Compute Final Grades ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    list* activeList = stuManager.studentLists[stuManager.activeStudentList];
    node* current = activeList->head;
    int count = 0;
    
    printf("Recalculating grades for list: %s\n\n", stuManager.studentListNames[stuManager.activeStudentList]);
    
    if (current != NULL) {
        do {
            Student* student = (Student*)current->data;
            if (student != NULL) {
                count++;
                calculateFinalGrade(student);
                printf("%d. %s - Final Grade: %.2f (%s)\n", count, 
                       student->personal.name.fullName, 
                       student->academic.finalGrade, 
                       student->academic.remarks);
            }
            current = current->next;
        } while (current != activeList->head && current != NULL);
    }
    
    printf("\nSuccessfully computed grades for %d students.\n", count);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

int handleSortStudentsByGrade(void) {
    winTermClearScreen();
    printf("=== Sort Students by Grade ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Sorting students by final grade (highest to lowest)...\n\n");
    
    if (sortStudentsByGrade(stuManager.studentLists[stuManager.activeStudentList], 1) == 0) {
        printf("Students sorted successfully!\n\n");
        displayAllStudents(stuManager.studentLists[stuManager.activeStudentList]);
    } else {
        printf("Failed to sort students!\n");
    }
    
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

int handleStudentReport(void) {
    winTermClearScreen();
    printf("=== Student Academic Report ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Generating academic report for: %s\n\n", stuManager.studentListNames[stuManager.activeStudentList]);
    
    // Generate the student report file
    char reportFilePath[512];
    int reportResult = generateStudentReportFile(stuManager.studentLists[stuManager.activeStudentList], reportFilePath, sizeof(reportFilePath));
    
    if (reportResult > 0) {
        printf("Successfully generated academic report!\n");
        printf("Report saved to: %s\n", reportFilePath);
        printf("Processed %d students\n", reportResult);
    } else {
        printf("Failed to generate academic report.\n");
    }
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

int handleSaveStudentList(void) {
    winTermClearScreen();
    printf("=== Save Student List ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list to save!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Saving list: %s\n", stuManager.studentListNames[stuManager.activeStudentList]);
    
    // Show existing data files
    printf("\nExisting student data files:\n");
    listStudentDataFiles();
    printf("\n");
    
    char filename[100];
    printf("Enter filename (will be saved as 'student_LISTNAME_TIMESTAMP.dat'): ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        return -1;
    }
    filename[strcspn(filename, "\n")] = 0; // Remove newline
    
    // Use the custom save function
    int savedCount = saveListWithCustomName(stuManager.studentLists[stuManager.activeStudentList], 
                                           filename, "student");
    
    if (savedCount >= 0) {
        printf("Successfully saved %d student records!\n", savedCount);
        printf("Data saved to data directory with timestamp.\n");
    } else {
        printf("Failed to save student list.\n");
    }
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

int handleLoadStudentList(void) {
    winTermClearScreen();
    printf("=== Load Student List ===\n\n");
    
    // List available student data files
    listStudentDataFiles();
    printf("\n");
    
    char filename[100];
    printf("Enter filename to load: ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        return -1;
    }
    filename[strcspn(filename, "\n")] = 0; // Remove newline
    
    char listName[50];
    printf("Enter name for this loaded list: ");
    if (!fgets(listName, sizeof(listName), stdin)) {
        return -1;
    }
    listName[strcspn(listName, "\n")] = 0; // Remove newline
    
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

/**
 * @brief Displays system statistics
 */
void displaySystemStatistics(void) {
    winTermClearScreen();
    printf("=== CS002 System Statistics ===\n\n");
    
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
    printf("Total Employees: %d\n\n", totalEmployees);
    
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
    printf("Total Students: %d\n\n", totalStudents);
    
    printf("OVERALL STATISTICS:\n");
    printf("Total Records: %d\n", totalEmployees + totalStudents);
    printf("Total Lists: %d\n", empManager.employeeListCount + stuManager.studentListCount);
    
    printf("\nPress any key to continue...");
    _getch();
}

// CS002 Dual Management System - All functions implemented above