#include "menuio.h"
#include "empio.h"
#include "../modules/data.h"
#include "../modules/payroll.h"
#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/state.h"
#include "../../../include/models/employee.h"
#include "../../../include/headers/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define HIGHLIGHT_TEXT_COLOR 9
#define HIGHLIGHT_BG_COLOR 0
#define TEXT_COLOR 7
#define BG_COLOR 0
#define DISABLED_TEXT_COLOR 8
#define DISABLED_BG_COLOR 0

// Global variables
static list* employeeList = NULL;

// Function prototypes
static void handleAddEmployeeMenu(void);
static int handleCreateEmployeeList(void);
static int handleAddEmployee(void);
static int handleExportEmployeeData(void);
static int handleLoadEmployeeData(void);
static int handlePayrollReport(void);
static void showDisabledMessage(const char* action);

// Menu definitions
Menu mainMenu = {1, "Main Menu", (MenuOption[]){
    {'1', "Add/Create Employee Options", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Edit Employee Data", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "Search Employee", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'4', "Delete Employee", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'5', "Load Employee From File", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'6', "Export Employee Data To CSV", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'7', "Display Payroll Report", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'8', "Exit", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 8};
    
Menu addEmployeeSubMenu = {2, "Add Employee Sub Menu", (MenuOption[]){
    {'1', "Create Employee List", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Add an Employee", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "Back to Main Menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 3};

/**
 * @brief Updates menu option states based on current application state
 */
void checkStates(void) {
    // Check if employee list has been created
    if (employeeListCreated.isEnabled) {
        // Enable "Add an Employee" option
        addEmployeeSubMenu.options[1].isDisabled = false;
        
        // Check if list has employees for options that require data
        if (employeeList != NULL && employeeList->head != NULL && employeeList->size > 0) {
            mainMenu.options[1].isDisabled = false; // Edit Employee Data
            mainMenu.options[2].isDisabled = false; // Search Employee
            mainMenu.options[3].isDisabled = false; // Delete Employee
            mainMenu.options[5].isDisabled = false; // Export Employee Data To CSV
            mainMenu.options[6].isDisabled = false; // Display Payroll Report
        } else {
            // List exists but is empty
            mainMenu.options[1].isDisabled = true;
            mainMenu.options[2].isDisabled = true;
            mainMenu.options[3].isDisabled = true;
            mainMenu.options[5].isDisabled = true;
            mainMenu.options[6].isDisabled = true;
        }
        
        // Disable "Create Employee List" since it already exists
        addEmployeeSubMenu.options[0].isDisabled = true;
    } else {
        // Employee list not created yet
        mainMenu.options[1].isDisabled = true;
        mainMenu.options[2].isDisabled = true;
        mainMenu.options[3].isDisabled = true;
        mainMenu.options[5].isDisabled = true;
        mainMenu.options[6].isDisabled = true;
        
        addEmployeeSubMenu.options[0].isDisabled = false; // Enable "Create Employee List"
        addEmployeeSubMenu.options[1].isDisabled = true;  // Disable "Add an Employee"
    }
}

/**
 * @brief Initializes the menu I/O module with the employee list.
 * @param list Pointer to the employee list.
 */
void initMenuIO(list* list) {
    employeeList = list;
    if (employeeList != NULL) {
        employeeListCreated.isEnabled = true;
    }
}

/**
 * @brief Main menu loop that handles user input and menu navigation
 * @return Returns 0 on normal exit, other values on error
 */
int menuLoop(void) {
    char mainMenuChoice;
    
    do {
        checkStates();
        winTermClearScreen();
        mainMenuChoice = initMenu(&mainMenu);
        
        switch(mainMenuChoice) {
            case '1':
                handleAddEmployeeMenu();
                break;
            case '2':
                if (!mainMenu.options[1].isDisabled) {
                    handleEditEmployee(employeeList);
                } else {
                    showDisabledMessage("edit employees");
                }
                break;
            case '3':
                if (!mainMenu.options[2].isDisabled) {
                    handleSearchEmployee(employeeList);
                } else {
                    showDisabledMessage("search employees");
                }
                break;
            case '4':
                if (!mainMenu.options[3].isDisabled) {
                    handleDeleteEmployee(employeeList);
                } else {
                    showDisabledMessage("delete employees");
                }
                break;
            case '5':
                handleLoadEmployeeData();
                break;
            case '6':
                if (!mainMenu.options[5].isDisabled) {
                    handleExportEmployeeData();
                } else {
                    showDisabledMessage("export employee data");
                }
                break;
            case '7':
                if (!mainMenu.options[6].isDisabled) {
                    handlePayrollReport();
                } else {
                    showDisabledMessage("display payroll report");
                }
                break;
            case '8':
                printf("\nExiting program...\n");
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
 * @brief Handles the Add Employee submenu.
 */
static void handleAddEmployeeMenu(void) {
    char choice;
    
    do {
        checkStates();
        winTermClearScreen();
        choice = initMenu(&addEmployeeSubMenu);
        
        switch(choice) {
            case '1':
                if (!addEmployeeSubMenu.options[0].isDisabled) {
                    handleCreateEmployeeList();
                } else {
                    printf("\nEmployee list already exists!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '2':
                if (!addEmployeeSubMenu.options[1].isDisabled) {
                    handleAddEmployee();
                } else {
                    printf("\nPlease create an employee list first!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '3':
                return; // Back to main menu
            default:
                printf("\nInvalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (choice != '3');
}

/**
 * @brief Handles creating a new employee list and populating it with initial employees.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleCreateEmployeeList(void) {
    winTermClearScreen();
    printf("=== Create Employee List ===\n\n");
    
    if (employeeListCreated.isEnabled) {
        printf("Employee list already exists!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Create the list if it doesn't exist
    if (employeeList == NULL) {
        if (createEmployeeList(&employeeList) != 0) {
            printf("Failed to create employee list!\n");
            printf("Press any key to continue...");
            _getch();
            return -1;
        }
    }
    
    printf("Employee list created successfully!\n");
    printf("Now let's add %d employees to initialize the list.\n\n", maxEmployeeCreationCount);
    
    int employeesCreated = 0;
    for (int i = 0; i < maxEmployeeCreationCount; i++) {
        printf("=== Adding Employee %d of %d ===\n\n", i + 1, maxEmployeeCreationCount);
        
        Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
        if (!newEmployee) {
            printf("Memory allocation failed for employee %d!\n", i + 1);
            printf("Press any key to continue...");
            _getch();
            continue;
        }
        
        // Initialize employee data
        memset(newEmployee, 0, sizeof(Employee));
        
        if (getEmployeeDataFromUser(newEmployee) != 0) {
            printf("Employee %d creation cancelled or failed.\n", i + 1);
            free(newEmployee);
            
            printf("Do you want to continue with remaining employees? (Y/N): ");
            char choice = _getch();
            printf("%c\n", choice);
            
            if (choice == 'N' || choice == 'n') {
                break;
            }
            continue;
        }
        
        // Calculate payroll information
        calculatePayroll(newEmployee);
        
        if (createEmployee(newEmployee, &employeeList) != 0) {
            printf("Failed to add employee %d to the list!\n", i + 1);
            free(newEmployee);
            printf("Press any key to continue...");
            _getch();
            continue;
        }
        
        employeesCreated++;
        printf("\nEmployee %d '%s' added successfully!\n", i + 1, newEmployee->personal.name.fullName);
        
        if (i < maxEmployeeCreationCount - 1) {
            printf("Press any key to add the next employee...");
            _getch();
        }
    }
    
    if (employeesCreated > 0) {
        employeeListCreated.isEnabled = true;
        printf("\n=== Employee List Creation Complete ===\n");
        printf("Successfully created %d out of %d employees.\n", employeesCreated, maxEmployeeCreationCount);
        printf("You can now add more employees or perform other operations.\n");
    } else {
        printf("\n=== No Employees Created ===\n");
        printf("Employee list was created but no employees were added.\n");
        printf("You can add employees using the 'Add an Employee' option later.\n");
        employeeListCreated.isEnabled = true; // Still mark as created even if empty
    }
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles adding a new employee.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleAddEmployee(void) {
    winTermClearScreen();
    printf("=== Add New Employee ===\n\n");
    
    if (!employeeListCreated.isEnabled || employeeList == NULL) {
        printf("Employee list not created yet!\n");
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
    
    if (createEmployee(newEmployee, &employeeList) != 0) {
        printf("Failed to add employee to list!\n");
        free(newEmployee);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("\nEmployee '%s' added successfully!\n", newEmployee->personal.name.fullName);
    printf("Employee Number: %s\n", newEmployee->personal.employeeNumber);
    printf("Net Pay: %.2f\n", newEmployee->payroll.netPay);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles exporting employee data to a CSV file.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleExportEmployeeData(void) {
    winTermClearScreen();
    printf("=== Export Employee Data to CSV ===\n\n");
    
    if (!employeeListCreated.isEnabled || !employeeList || !employeeList->head || employeeList->size == 0) {
        printf("No employee data to export!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    char filename[100];
    printf("Enter filename for CSV export (e.g., employees.csv): ");
    if (scanf("%99s", filename) != 1) {
        printf("Invalid filename!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    getchar(); // Clear newline
    
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file '%s' for writing!\n", filename);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Write CSV header
    fprintf(file, "Employee Number,Full Name,First Name,Middle Name,Last Name,Status,Hours Worked,Basic Rate,Basic Pay,Overtime Pay,Deductions,Net Pay\n");
    
    // Write employee data
    node* current = employeeList->head;
    int count = 0;
    
    if (current != NULL) {
        do {
            Employee* emp = (Employee*)current->data;
            if (emp != NULL) {
                fprintf(file, "%s,%s,%s,%s,%s,%s,%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                    emp->personal.employeeNumber,
                    emp->personal.name.fullName,
                    emp->personal.name.firstName,
                    emp->personal.name.middleName,
                    emp->personal.name.lastName,
                    (emp->employment.status == statusRegular) ? "Regular" : "Casual",
                    emp->employment.hoursWorked,
                    emp->employment.basicRate,
                    emp->payroll.basicPay,
                    emp->payroll.overtimePay,
                    emp->payroll.deductions,
                    emp->payroll.netPay);
                count++;
            }
            current = current->next;
        } while (current != employeeList->head && current != NULL);
    }
    
    fclose(file);
    printf("Successfully exported %d employee records to '%s'!\n", count, filename);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles loading employee data from a file.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleLoadEmployeeData(void) {
    winTermClearScreen();
    printf("=== Load Employee Data ===\n\n");
    
    char filename[100];
    printf("Enter filename to load from: ");
    if (scanf("%99s", filename) != 1) {
        printf("Invalid filename!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    getchar(); // Clear newline
    
    // If list already exists and has data, confirm overwrite
    if (employeeListCreated.isEnabled && employeeList && employeeList->head && employeeList->size > 0) {
        printf("Warning: This will replace existing employee data. Continue? (Y/N): ");
        char confirm = _getch();
        printf("%c\n", confirm);
        
        if (confirm != 'Y' && confirm != 'y') {
            printf("Operation cancelled.\n");
            printf("Press any key to continue...");
            _getch();
            return -1;
        }
        
        // Clean up old list
        clearList(employeeList, freeEmployee);
    }
    
    // Load new employee data
    list* newList = loadEmployeeDataFromFile(filename, SINGLY);
    if (!newList) {
        printf("Failed to load employee data from file '%s'!\n", filename);
        printf("Make sure the file exists and is in the correct format.\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Replace current list
    if (employeeList != NULL && employeeList != newList) {
        destroyList(&employeeList, freeEmployee);
    }
    employeeList = newList;
    employeeListCreated.isEnabled = true;
    
    printf("Employee data loaded successfully from '%s'!\n", filename);
    printf("Loaded %d employee records.\n", employeeList->size);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles displaying the payroll report.
 * @return Returns 0 on success, -1 on failure.
 */
static int handlePayrollReport(void) {
    winTermClearScreen();
    printf("=== Employee Payroll Report ===\n\n");
    
    if (!employeeListCreated.isEnabled || !employeeList || !employeeList->head || employeeList->size == 0) {
        printf("No employee data to display!\n");
        printf("Please create an employee list and add some employees first.\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    node* current = employeeList->head;
    int count = 0;
    
    double totalBasicPay = 0.0;
    double totalOvertimePay = 0.0;
    double totalDeductions = 0.0;
    double totalNetPay = 0.0;
    
    printf("%-12s | %-20s | %-8s | %-10s | %-10s | %-10s | %-10s | %-6s\n",
           "Emp. Number", "Employee Name", "Status", "Basic Pay", "Overtime", "Deductions", "Net Pay", "Hours");
    printf("------------------------------------------------------------------------------------------------------\n");
    
    if (current != NULL) {
        do {
            Employee* emp = (Employee*)current->data;
            if (emp != NULL) {
                count++;
                
                // Recalculate payroll if needed
                if (emp->payroll.netPay == 0.0) {
                    calculatePayroll(emp);
                }
                
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
    printf("Total employees: %d\n\n", count);
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Shows a message when a disabled option is selected
 * @param action The action that cannot be performed
 */
static void showDisabledMessage(const char* action) {
    printf("\nCannot %s: ", action);
    if (!employeeListCreated.isEnabled) {
        printf("Please create an employee list first.\n");
    } else if (employeeList == NULL || employeeList->size == 0) {
        printf("No employees in the list. Please add some employees first.\n");
    } else {
        printf("This option is currently unavailable.\n");
    }
    printf("Press any key to continue...");
    _getch();
}
