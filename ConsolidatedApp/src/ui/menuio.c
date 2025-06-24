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
    {'4', "Exit", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 4};

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
                displaySystemStatistics();
                break;
            case '4':
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
    
    printf("\nSYSTEM CAPABILITIES:\n");
    printf("- Multi-list management (up to 10 lists per type)\n");
    printf("- Employee payroll calculations and reporting\n");
    printf("- Student academic records and grade tracking\n");
    printf("- Individual list saving with custom names\n");
    printf("- Comprehensive reporting and statistics\n");
    printf("- Data validation and error handling\n");
    
    printf("\nPress any key to continue...");
    _getch();
}

/**
 * @brief Employee management submenu
 */
int runEmployeeManagement(void) {
    char choice;
    Menu employeeMenu = {1, "Employee Management", (MenuOption[]){
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
        winTermClearScreen();
        printf("=== Employee Management ===\n");
        if (empManager.activeEmployeeList >= 0) {
            printf("Active List: %s (%d employees)\n\n", 
                   empManager.employeeListNames[empManager.activeEmployeeList],
                   empManager.employeeLists[empManager.activeEmployeeList] ? 
                   empManager.employeeLists[empManager.activeEmployeeList]->size : 0);
        } else {
            printf("No active employee list\n\n");
        }
        
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
            case '4':
                if (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList]) {
                    // Call function from empio.c
                    extern int handleEditEmployee(list* employeeList);
                    handleEditEmployee(empManager.employeeLists[empManager.activeEmployeeList]);
                } else {
                    printf("\nNo active employee list!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
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
    Menu studentMenu = {1, "Student Management", (MenuOption[]){
        {'1', "Create Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Switch Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Add Student", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Edit Student", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'5', "Delete Student", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'6', "Search Student", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'7', "Display All Students", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'8', "Student Report", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'9', "Save Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'A', "Load Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'B', "Back to Main Menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 11};
    
    do {
        winTermClearScreen();
        printf("=== Student Management ===\n");
        if (stuManager.activeStudentList >= 0) {
            printf("Active List: %s (%d students)\n\n", 
                   stuManager.studentListNames[stuManager.activeStudentList],
                   stuManager.studentLists[stuManager.activeStudentList] ? 
                   stuManager.studentLists[stuManager.activeStudentList]->size : 0);
        } else {
            printf("No active student list\n\n");
        }
        
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
                handleStudentReport();
                break;
            case '9':
                handleSaveStudentList();
                break;
            case 'A':
            case 'a':
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

int handleStudentReport(void) {
    winTermClearScreen();
    printf("=== Student Report ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Generating student report for: %s\n\n", stuManager.studentListNames[stuManager.activeStudentList]);
    
    // Generate the student report file
    char reportFilePath[512];
    int reportResult = generateStudentReportFile(stuManager.studentLists[stuManager.activeStudentList], reportFilePath, sizeof(reportFilePath));
    
    if (reportResult > 0) {
        printf("Successfully generated student report!\n");
        printf("Report saved to: %s\n", reportFilePath);
        printf("Processed %d students\n", reportResult);
    } else {
        printf("Failed to generate student report.\n");
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

int handleSwitchEmployeeList(void) {
    winTermClearScreen();
    printf("=== Switch Employee List ===\n\n");
    
    if (empManager.employeeListCount == 0) {
        printf("No employee lists available!\n");
        printf("Create an employee list first.\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    if (empManager.employeeListCount == 1) {
        printf("Only one employee list available: %s\n", empManager.employeeListNames[0]);
        printf("It is already active.\n");
        printf("Press any key to continue...");
        _getch();
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
    printf("\nEnter the number of the list to switch to (1-%d): ", empManager.employeeListCount);
    
    char input[10];
    if (!fgets(input, sizeof(input), stdin)) {
        return -1;
    }
    
    int choice = atoi(input) - 1; // Convert to 0-based index
    
    if (choice < 0 || choice >= empManager.employeeListCount) {
        printf("Invalid choice! Please enter a number between 1 and %d.\n", empManager.employeeListCount);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    if (choice == empManager.activeEmployeeList) {
        printf("List '%s' is already active!\n", empManager.employeeListNames[choice]);
        printf("Press any key to continue...");
        _getch();
        return 0;
    }
    
    empManager.activeEmployeeList = choice;
    int listSize = empManager.employeeLists[choice] ? empManager.employeeLists[choice]->size : 0;
    
    printf("Successfully switched to employee list: %s\n", empManager.employeeListNames[choice]);
    printf("This list contains %d employees.\n", listSize);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

int handleSwitchStudentList(void) {
    winTermClearScreen();
    printf("=== Switch Student List ===\n\n");
    
    if (stuManager.studentListCount == 0) {
        printf("No student lists available!\n");
        printf("Create a student list first.\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    if (stuManager.studentListCount == 1) {
        printf("Only one student list available: %s\n", stuManager.studentListNames[0]);
        printf("It is already active.\n");
        printf("Press any key to continue...");
        _getch();
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
    printf("\nEnter the number of the list to switch to (1-%d): ", stuManager.studentListCount);
    
    char input[10];
    if (!fgets(input, sizeof(input), stdin)) {
        return -1;
    }
    
    int choice = atoi(input) - 1; // Convert to 0-based index
    
    if (choice < 0 || choice >= stuManager.studentListCount) {
        printf("Invalid choice! Please enter a number between 1 and %d.\n", stuManager.studentListCount);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    if (choice == stuManager.activeStudentList) {
        printf("List '%s' is already active!\n", stuManager.studentListNames[choice]);
        printf("Press any key to continue...");
        _getch();
        return 0;
    }
    
    stuManager.activeStudentList = choice;
    int listSize = stuManager.studentLists[choice] ? stuManager.studentLists[choice]->size : 0;
    
    printf("Successfully switched to student list: %s\n", stuManager.studentListNames[choice]);
    printf("This list contains %d students.\n", listSize);
    printf("Press any key to continue...");
    _getch();
    return 0;
} 