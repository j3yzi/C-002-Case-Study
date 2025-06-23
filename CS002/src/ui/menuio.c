#include "menuio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/state.h"
#include "../../../include/models/student.h"
#include "../../../include/models/employee.h"
#include "../../../include/headers/list.h"

// Define colors for the menu options for better readability
#define HIGHLIGHT_TEXT_COLOR 9 // Bright Blue
#define HIGHLIGHT_BG_COLOR 0   // Black
#define TEXT_COLOR 7           // White
#define BG_COLOR 0             // Black
#define DISABLED_TEXT_COLOR 8  // Gray
#define DISABLED_BG_COLOR 0    // Black

#define MAX_STUDENT_CREATION_COUNT 3
#define MAX_EMPLOYEE_CREATION_COUNT 3

// Global variables
static list* studentList = NULL;
static list* universityEmployeeList = NULL;

// Function prototypes
static void handleStudentManagement(void);
static void handleEmployeeManagement(void);
static void handleAdvancedOperations(void);
static int handleCreateStudentList(void);
static int handleCreateEmployeeList(void);
static int handleAddStudent(void);
static int handleAddEmployee(void);
static int handleEditStudent(void);
static int handleEditEmployee(void);
static int handleDeleteStudent(void);
static int handleDeleteEmployee(void);
static int handleSearchStudent(void);
static int handleSearchEmployee(void);
static int handleDisplayAllStudents(void);
static int handleDisplayAllEmployees(void);
static int handleMergeLists(void);
static int handleSortLists(void);
static int handleListStatistics(void);
static int handleExportData(void);
static int handleImportData(void);
static void showDisabledMessage(const char* action);

// Menu definitions
Menu mainMenu = {1, "Academic Management System", (MenuOption[]){
    {'1', "Manage Students", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Manage University Employees", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "Advanced List Operations", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'4', "Exit", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 4};

Menu studentMenu = {2, "Student Management", (MenuOption[]){
    {'1', "Create Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Add Student", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "Edit Student", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'4', "Delete Student", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'5', "Search Student", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'6', "Display All Students", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'7', "Back to Main Menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 7};

Menu employeeMenu = {3, "University Employee Management", (MenuOption[]){
    {'1', "Create Employee List", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Add Employee", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "Edit Employee", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'4', "Delete Employee", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'5', "Search Employee", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'6', "Display All Employees", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'7', "Back to Main Menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 7};

Menu advancedMenu = {4, "Advanced List Operations", (MenuOption[]){
    {'1', "Merge Student & Employee Lists", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Sort Lists", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "List Statistics", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'4', "Export Data", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'5', "Import Data", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'6', "Back to Main Menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 6};

/**
 * @brief Updates menu option states based on current application state
 */
void checkStates(void) {
    // Check student list state
    if (studentListCreated.isEnabled) {
        // Enable student operations if list has data
        if (studentList != NULL && studentList->head != NULL && studentList->size > 0) {
            studentMenu.options[1].isDisabled = false; // Add Student
            studentMenu.options[2].isDisabled = false; // Edit Student
            studentMenu.options[3].isDisabled = false; // Delete Student
            studentMenu.options[4].isDisabled = false; // Search Student
            studentMenu.options[5].isDisabled = false; // Display All Students
        } else {
            // List exists but is empty
            studentMenu.options[1].isDisabled = false; // Still allow adding
            studentMenu.options[2].isDisabled = true;
            studentMenu.options[3].isDisabled = true;
            studentMenu.options[4].isDisabled = true;
            studentMenu.options[5].isDisabled = true;
        }
        studentMenu.options[0].isDisabled = true; // Disable "Create Student List"
    } else {
        // Student list not created yet
        studentMenu.options[0].isDisabled = false; // Enable "Create Student List"
        studentMenu.options[1].isDisabled = true;
        studentMenu.options[2].isDisabled = true;
        studentMenu.options[3].isDisabled = true;
        studentMenu.options[4].isDisabled = true;
        studentMenu.options[5].isDisabled = true;
    }

    // Check employee list state
    if (universityEmployeeListCreated.isEnabled) {
        // Enable employee operations if list has data
        if (universityEmployeeList != NULL && universityEmployeeList->head != NULL && universityEmployeeList->size > 0) {
            employeeMenu.options[1].isDisabled = false; // Add Employee
            employeeMenu.options[2].isDisabled = false; // Edit Employee
            employeeMenu.options[3].isDisabled = false; // Delete Employee
            employeeMenu.options[4].isDisabled = false; // Search Employee
            employeeMenu.options[5].isDisabled = false; // Display All Employees
        } else {
            // List exists but is empty
            employeeMenu.options[1].isDisabled = false; // Still allow adding
            employeeMenu.options[2].isDisabled = true;
            employeeMenu.options[3].isDisabled = true;
            employeeMenu.options[4].isDisabled = true;
            employeeMenu.options[5].isDisabled = true;
        }
        employeeMenu.options[0].isDisabled = true; // Disable "Create Employee List"
    } else {
        // Employee list not created yet
        employeeMenu.options[0].isDisabled = false; // Enable "Create Employee List"
        employeeMenu.options[1].isDisabled = true;
        employeeMenu.options[2].isDisabled = true;
        employeeMenu.options[3].isDisabled = true;
        employeeMenu.options[4].isDisabled = true;
        employeeMenu.options[5].isDisabled = true;
    }

    // Check main menu state
    mainMenu.options[0].isDisabled = false; // Always allow student management
    mainMenu.options[1].isDisabled = false; // Always allow employee management

    // Advanced operations require at least one list to have data
    bool hasStudentData = studentListCreated.isEnabled && studentList && studentList->size > 0;
    bool hasEmployeeData = universityEmployeeListCreated.isEnabled && universityEmployeeList && universityEmployeeList->size > 0;
    
    if (hasStudentData || hasEmployeeData) {
        mainMenu.options[2].isDisabled = false; // Enable advanced operations
        
        // Configure advanced menu based on available data
        advancedMenu.options[0].isDisabled = !(hasStudentData && hasEmployeeData); // Merge requires both
        advancedMenu.options[1].isDisabled = false; // Sort
        advancedMenu.options[2].isDisabled = false; // Statistics
        advancedMenu.options[3].isDisabled = false; // Export
    } else {
        mainMenu.options[2].isDisabled = true; // Disable advanced operations
        advancedMenu.options[0].isDisabled = true;
        advancedMenu.options[1].isDisabled = true;
        advancedMenu.options[2].isDisabled = true;
        advancedMenu.options[3].isDisabled = true;
    }
}

/**
 * @brief Initializes the menu I/O module with the lists.
 * @param studentList Pointer to the student list.
 * @param employeeList Pointer to the employee list.
 */
void initMenuIO(list* students, list* employees) {
    studentList = students;
    universityEmployeeList = employees;
    
    if (studentList != NULL) {
        studentListCreated.isEnabled = true;
    }
    if (universityEmployeeList != NULL) {
        universityEmployeeListCreated.isEnabled = true;
    }
}

/**
 * @brief Main menu loop that handles user input and menu navigation
 * @return Returns 0 on normal exit, other values on error
 */
int runMenuLoop(void) {
    char mainMenuChoice;
    
    do {
        checkStates();
        winTermClearScreen();
        mainMenuChoice = initMenu(&mainMenu);
        
        switch(mainMenuChoice) {
            case '1':
                handleStudentManagement();
                break;
            case '2':
                handleEmployeeManagement();
                break;
            case '3':
                if (!mainMenu.options[2].isDisabled) {
                    handleAdvancedOperations();
                } else {
                    showDisabledMessage("perform advanced operations");
                }
                break;
            case '4':
                printf("\nExiting Academic Management System...\n");
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
 * @brief Handles the Student Management submenu.
 */
static void handleStudentManagement(void) {
    char choice;
    
    do {
        checkStates();
        winTermClearScreen();
        choice = initMenu(&studentMenu);
        
        switch(choice) {
            case '1':
                if (!studentMenu.options[0].isDisabled) {
                    handleCreateStudentList();
                } else {
                    showDisabledMessage("create student list - already exists");
                }
                break;
            case '2':
                if (!studentMenu.options[1].isDisabled) {
                    handleAddStudent();
                } else {
                    showDisabledMessage("add student");
                }
                break;
            case '3':
                if (!studentMenu.options[2].isDisabled) {
                    handleEditStudent();
                } else {
                    showDisabledMessage("edit student");
                }
                break;
            case '4':
                if (!studentMenu.options[3].isDisabled) {
                    handleDeleteStudent();
                } else {
                    showDisabledMessage("delete student");
                }
                break;
            case '5':
                if (!studentMenu.options[4].isDisabled) {
                    handleSearchStudent();
                } else {
                    showDisabledMessage("search student");
                }
                break;
            case '6':
                if (!studentMenu.options[5].isDisabled) {
                    handleDisplayAllStudents();
                } else {
                    showDisabledMessage("display students");
                }
                break;
            case '7':
                return; // Back to main menu
            default:
                printf("\nInvalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (choice != '7');
}

/**
 * @brief Handles the Employee Management submenu.
 */
static void handleEmployeeManagement(void) {
    char choice;
    
    do {
        checkStates();
        winTermClearScreen();
        choice = initMenu(&employeeMenu);
        
        switch(choice) {
            case '1':
                if (!employeeMenu.options[0].isDisabled) {
                    handleCreateEmployeeList();
                } else {
                    showDisabledMessage("create employee list - already exists");
                }
                break;
            case '2':
                if (!employeeMenu.options[1].isDisabled) {
                    handleAddEmployee();
                } else {
                    showDisabledMessage("add employee");
                }
                break;
            case '3':
                if (!employeeMenu.options[2].isDisabled) {
                    handleEditEmployee();
                } else {
                    showDisabledMessage("edit employee");
                }
                break;
            case '4':
                if (!employeeMenu.options[3].isDisabled) {
                    handleDeleteEmployee();
                } else {
                    showDisabledMessage("delete employee");
                }
                break;
            case '5':
                if (!employeeMenu.options[4].isDisabled) {
                    handleSearchEmployee();
                } else {
                    showDisabledMessage("search employee");
                }
                break;
            case '6':
                if (!employeeMenu.options[5].isDisabled) {
                    handleDisplayAllEmployees();
                } else {
                    showDisabledMessage("display employees");
                }
                break;
            case '7':
                return; // Back to main menu
            default:
                printf("\nInvalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (choice != '7');
}

/**
 * @brief Handles the Advanced Operations submenu.
 */
static void handleAdvancedOperations(void) {
    char choice;
    
    do {
        checkStates();
        winTermClearScreen();
        choice = initMenu(&advancedMenu);
        
        switch(choice) {
            case '1':
                if (!advancedMenu.options[0].isDisabled) {
                    handleMergeLists();
                } else {
                    showDisabledMessage("merge lists - need both student and employee data");
                }
                break;
            case '2':
                if (!advancedMenu.options[1].isDisabled) {
                    handleSortLists();
                } else {
                    showDisabledMessage("sort lists");
                }
                break;
            case '3':
                if (!advancedMenu.options[2].isDisabled) {
                    handleListStatistics();
                } else {
                    showDisabledMessage("view statistics");
                }
                break;
            case '4':
                if (!advancedMenu.options[3].isDisabled) {
                    handleExportData();
                } else {
                    showDisabledMessage("export data");
                }
                break;
            case '5':
                if (!advancedMenu.options[4].isDisabled) {
                    handleImportData();
                } else {
                    showDisabledMessage("import data");
                }
                break;
            case '6':
                return; // Back to main menu
            default:
                printf("\nInvalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (choice != '6');
}

// ===============================
// STUDENT MANAGEMENT FUNCTIONS
// ===============================

/**
 * @brief Handles creating a new student list and populating it with initial students.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleCreateStudentList(void) {
    winTermClearScreen();
    printf("=== Create Student List ===\n\n");
    
    if (studentListCreated.isEnabled) {
        printf("Student list already exists!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Student list created successfully!\n");
    printf("Now let's add %d students to initialize the list.\n\n", MAX_STUDENT_CREATION_COUNT);
    
    studentListCreated.isEnabled = true;
    printf("\n=== Student List Creation Complete ===\n");
    printf("Student list is ready for use!\n");
    printf("Use 'Add Student' to add individual students.\n");
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles adding a new student.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleAddStudent(void) {
    winTermClearScreen();
    printf("=== Add New Student ===\n\n");
    printf("This functionality will be implemented soon.\n");
    printf("Features to include:\n");
    printf("- Student number validation\n");
    printf("- Name validation (alphabetic only)\n");
    printf("- Program and year level selection\n");
    printf("- Academic information entry\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles editing an existing student.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleEditStudent(void) {
    winTermClearScreen();
    printf("=== Edit Student ===\n\n");
    printf("This functionality will be implemented soon.\n");
    printf("Features to include:\n");
    printf("- Search student by number or name\n");
    printf("- Edit personal information\n");
    printf("- Update academic standing\n");
    printf("- Grade management\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles deleting a student.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleDeleteStudent(void) {
    winTermClearScreen();
    printf("=== Delete Student ===\n\n");
    printf("This functionality will be implemented soon.\n");
    printf("Features to include:\n");
    printf("- Safe deletion with confirmation\n");
    printf("- Archive option before deletion\n");
    printf("- Cascade operations if needed\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles searching for students.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleSearchStudent(void) {
    winTermClearScreen();
    printf("=== Search Student ===\n\n");
    printf("This functionality will be implemented soon.\n");
    printf("Search options:\n");
    printf("1. By Student Number\n");
    printf("2. By Name (partial match)\n");
    printf("3. By Program\n");
    printf("4. By Year Level\n");
    printf("5. By Academic Standing\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles displaying all students.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleDisplayAllStudents(void) {
    winTermClearScreen();
    printf("=== Display All Students ===\n\n");
    printf("This functionality will be implemented soon.\n");
    printf("Display features:\n");
    printf("- Tabular format with all student info\n");
    printf("- Sorting options (name, number, program)\n");
    printf("- Filtering by year level or program\n");
    printf("- Academic performance summary\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

// ===============================
// EMPLOYEE MANAGEMENT FUNCTIONS  
// ===============================

/**
 * @brief Handles creating a new university employee list.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleCreateEmployeeList(void) {
    winTermClearScreen();
    printf("=== Create University Employee List ===\n\n");
    
    if (universityEmployeeListCreated.isEnabled) {
        printf("University employee list already exists!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("University employee list created successfully!\n");
    printf("Now let's add %d employees to initialize the list.\n\n", MAX_EMPLOYEE_CREATION_COUNT);
    
    universityEmployeeListCreated.isEnabled = true;
    printf("\n=== University Employee List Creation Complete ===\n");
    printf("Employee list is ready for use!\n");
    printf("Use 'Add Employee' to add individual employees.\n");
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles adding a new university employee.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleAddEmployee(void) {
    winTermClearScreen();
    printf("=== Add New University Employee ===\n\n");
    printf("This functionality will be implemented soon.\n");
    printf("Employee types:\n");
    printf("- Faculty (Professors, Instructors)\n");
    printf("- Administrative Staff\n");
    printf("- Support Staff\n");
    printf("- Research Personnel\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles editing an existing university employee.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleEditEmployee(void) {
    winTermClearScreen();
    printf("=== Edit University Employee ===\n\n");
    printf("This functionality will be implemented soon.\n");
    printf("Features to include:\n");
    printf("- Department transfers\n");
    printf("- Position updates\n");
    printf("- Salary adjustments\n");
    printf("- Contact information updates\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles deleting a university employee.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleDeleteEmployee(void) {
    winTermClearScreen();
    printf("=== Delete University Employee ===\n\n");
    printf("This functionality will be implemented soon.\n");
    printf("Features to include:\n");
    printf("- Employment termination process\n");
    printf("- Archive employee records\n");
    printf("- Transfer responsibilities\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles searching for university employees.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleSearchEmployee(void) {
    winTermClearScreen();
    printf("=== Search University Employee ===\n\n");
    printf("This functionality will be implemented soon.\n");
    printf("Search options:\n");
    printf("1. By Employee Number\n");
    printf("2. By Name\n");
    printf("3. By Department\n");
    printf("4. By Position/Title\n");
    printf("5. By Employment Status\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles displaying all university employees.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleDisplayAllEmployees(void) {
    winTermClearScreen();
    printf("=== Display All University Employees ===\n\n");
    printf("This functionality will be implemented soon.\n");
    printf("Display features:\n");
    printf("- Department-wise listing\n");
    printf("- Position hierarchy view\n");
    printf("- Payroll summary\n");
    printf("- Contact directory\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

// ===============================
// ADVANCED OPERATIONS FUNCTIONS
// ===============================

/**
 * @brief Handles merging student and employee lists for cross-referencing.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleMergeLists(void) {
    winTermClearScreen();
    printf("=== Merge Student & Employee Lists ===\n\n");
    printf("Advanced linked list operation: List merging\n\n");
    printf("This functionality will implement:\n");
    printf("- Cross-reference students and advisors\n");
    printf("- Find student-employee relationships\n");
    printf("- Generate combined reports\n");
    printf("- Create unified contact directory\n");
    printf("- Detect data inconsistencies\n");
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles sorting lists using various algorithms.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleSortLists(void) {
    winTermClearScreen();
    printf("=== Sort Lists ===\n\n");
    printf("Advanced linked list operation: Sorting\n\n");
    printf("Available sorting algorithms:\n");
    printf("1. Merge Sort (O(n log n))\n");
    printf("2. Quick Sort for linked lists\n");
    printf("3. Insertion Sort (for small lists)\n");
    printf("\nSort criteria:\n");
    printf("- Students: By name, number, GPA, year\n");
    printf("- Employees: By name, department, position, salary\n");
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles generating comprehensive list statistics.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleListStatistics(void) {
    winTermClearScreen();
    printf("=== List Statistics ===\n\n");
    printf("Advanced linked list operation: Statistical analysis\n\n");
    printf("Student Statistics:\n");
    printf("- Total enrollment by program\n");
    printf("- GPA distribution analysis\n");
    printf("- Year level demographics\n");
    printf("- Academic standing breakdown\n");
    printf("\nEmployee Statistics:\n");
    printf("- Department headcount\n");
    printf("- Position distribution\n");
    printf("- Salary range analysis\n");
    printf("- Employee-to-student ratios\n");
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles exporting data to various formats.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleExportData(void) {
    winTermClearScreen();
    printf("=== Export Data ===\n\n");
    printf("Export formats available:\n");
    printf("1. CSV files (Excel-compatible)\n");
    printf("2. JSON format (for web integration)\n");
    printf("3. Plain text reports\n");
    printf("4. Binary format (for backup)\n");
    printf("\nExport options:\n");
    printf("- Complete dataset\n");
    printf("- Filtered data by criteria\n");
    printf("- Statistical summaries only\n");
    printf("- Custom field selection\n");
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles importing data from external sources.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleImportData(void) {
    winTermClearScreen();
    printf("=== Import Data ===\n\n");
    printf("Import sources supported:\n");
    printf("1. CSV files (from Excel/Sheets)\n");
    printf("2. JSON files (from web systems)\n");
    printf("3. Text files (structured format)\n");
    printf("4. Binary backup files\n");
    printf("\nImport features:\n");
    printf("- Data validation during import\n");
    printf("- Duplicate detection and handling\n");
    printf("- Format conversion\n");
    printf("- Error reporting and recovery\n");
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Shows a message when a disabled option is selected
 * @param action The action that cannot be performed
 */
static void showDisabledMessage(const char* action) {
    printf("\nCannot %s: ", action);
    
    if (!studentListCreated.isEnabled && !universityEmployeeListCreated.isEnabled) {
        printf("Please create student and/or employee lists first.\n");
    } else if (!studentListCreated.isEnabled) {
        printf("Please create a student list first.\n");
    } else if (!universityEmployeeListCreated.isEnabled) {
        printf("Please create an employee list first.\n");
    } else if ((studentList == NULL || studentList->size == 0) && 
               (universityEmployeeList == NULL || universityEmployeeList->size == 0)) {
        printf("No data available. Please add students and/or employees first.\n");
    } else {
        printf("This option is currently unavailable.\n");
    }
    
    printf("Press any key to continue...");
    _getch();
}