#include "menuio.h"
#include "empio.h"
#include "../modules/data.h"
#include "../modules/payroll.h"
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
static appState employeeListCreated = {false};

// Function prototypes
static void handleAddEmployeeMenu();
static int handleCreateEmployeeList();
static int handleAddEmployee();
static int handleExportEmployeeData();
static int handleLoadEmployeeData();
static int handlePayrollReport();

// Menu definitions
Menu mainMenu = {1,"Main Menu", (MenuOption[]){
    {'1', "Add/Create Employee Options", false, true, 9,0,7,0,8,0, NULL},
    {'2', "Edit Employee Data", true, false, 9,0,7,0,8,0, NULL},
    {'3', "Search Employee", true, false, 9,0,7,0,8,0, NULL},
    {'4', "Delete Employee", true, false, 9,0,7,0,8,0, NULL},
    {'5', "Load Employee From File", false, false, 9,0,7,0,8,0, NULL},
    {'6', "Export Employee Data To CSV", true, false, 9,0,7,0,8,0, NULL},
    {'7', "Display Payroll Report", true, false, 9,0,7,0,8,0, NULL},
    {'8', "Exit", false, false, 9,0,7,0,8,0, NULL}}, 8};
    
Menu addEmployeeSubMenu = {2, "Add Employee Sub Menu", (MenuOption[]){
    {'1', "Create Employee List", false, true, 9,0,7,0,8,0, NULL},
    {'2', "Add an Employee", true, false, 9,0,7,0,8,0, NULL},
    {'3', "Back to Main Menu", false, false, 9,0,7,0,8,0, NULL}}, 3};

void checkStates() {
    if (employeeListCreated.isEnabled) {
        addEmployeeSubMenu.options[0].isDisabled = true;
        addEmployeeSubMenu.options[1].isDisabled = false;
        mainMenu.options[1].isDisabled = false;
        mainMenu.options[2].isDisabled = false;
        mainMenu.options[3].isDisabled = false;
        mainMenu.options[5].isDisabled = false;
        mainMenu.options[6].isDisabled = false;
    }
    // Also check if list has nodes for options that require it
    if (employeeList != NULL && employeeList->head != NULL) {
        mainMenu.options[1].isDisabled = false; // Edit
        mainMenu.options[2].isDisabled = false; // Search
        mainMenu.options[3].isDisabled = false; // Delete
        mainMenu.options[5].isDisabled = false; // Export
        mainMenu.options[6].isDisabled = false; // Display
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

int menuLoop() {
    char mainMenuChoice;
    do {
        checkStates();
        system("cls");
        mainMenuChoice = initMenu(&mainMenu);
        
        switch(mainMenuChoice) {
            case '1':
                handleAddEmployeeMenu();
                break;
            case '2':
                if (!mainMenu.options[1].isDisabled) {
                    handleEditEmployee(employeeList);
                }
                break;
            case '3':
                if (!mainMenu.options[2].isDisabled) {
                    handleSearchEmployee(employeeList);
                }
                break;
            case '4':
                if (!mainMenu.options[3].isDisabled) {
                    handleDeleteEmployee(employeeList);
                }
                break;
            case '5':
                handleLoadEmployeeData();
                break;
            case '6':
                if (!mainMenu.options[5].isDisabled) {
                    handleExportEmployeeData();
                }
                break;
            case '7':
                if (!mainMenu.options[6].isDisabled) {
                    handlePayrollReport();
                }
                break;
            case '8':
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (true);
    return 0;
}

/**
 * @brief Handles the Add Employee submenu.
 */
static void handleAddEmployeeMenu() {
    char choice;
    
    do {
        checkStates();
        system("cls");
        choice = initMenu(&addEmployeeSubMenu);
        
        switch(choice) {
            case '1':
                if (!addEmployeeSubMenu.options[0].isDisabled) {
                    handleCreateEmployeeList();
                }
                break;
            case '2':
                if (!addEmployeeSubMenu.options[1].isDisabled) {
                    handleAddEmployee();
                }
                break;
            case '3':
                return;
            default:
                printf("Invalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (choice != '3');
}

/**
 * @brief Handles creating a new employee list.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleCreateEmployeeList() {
    if (employeeListCreated.isEnabled) {
        printf("Employee list already exists!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    if (createEmployeeList(&employeeList) != 0) {
        printf("Failed to create employee list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    employeeListCreated.isEnabled = true;
    printf("Employee list created successfully!\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles adding a new employee.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleAddEmployee() {
    if (!employeeListCreated.isEnabled) {
        printf("Employee list not created yet!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    system("cls");
    printf("--- Add New Employee ---\n");
    
    Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
    if (!newEmployee) {
        printf("Memory allocation failed!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
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
    
    printf("\nEmployee added successfully!\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles exporting employee data to a CSV file.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleExportEmployeeData() {
    if (!employeeListCreated.isEnabled || !employeeList || !employeeList->head) {
        printf("No employee data to export!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    system("cls");
    printf("--- Export Employee Data to CSV ---\n");
    
    char filename[100];
    printf("Enter filename for CSV export (e.g., employees.csv): ");
    scanf("%99s", filename);
    getchar(); // Clear newline
    
    // TODO: Implement actual CSV export functionality
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file for writing!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Write CSV header
    fprintf(file, "Employee Number,Full Name,First Name,Middle Name,Last Name,Status,Hours Worked,Basic Rate,Basic Pay,Overtime Pay,Deductions,Net Pay\n");
    
    // Write employee data
    node* current = employeeList->head;
    if (current) {
        do {
            current = current->next;
            if (current != employeeList->head) {
                Employee* emp = (Employee*)current->data;
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
            }
        } while (current != employeeList->head);
    }
    
    fclose(file);
    printf("Data exported to %s successfully!\n", filename);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles loading employee data from a file.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleLoadEmployeeData() {
    system("cls");
    printf("--- Load Employee Data ---\n");
    
    char filename[100];
    printf("Enter filename to load from: ");
    scanf("%99s", filename);
    getchar(); // Clear newline
    
    // If list already exists, confirm overwrite
    if (employeeListCreated.isEnabled && employeeList && employeeList->head) {
        printf("Warning: This will replace existing employee data. Continue? (Y/N): ");
        char confirm = toupper(_getch());
        printf("%c\n", confirm);
        
        if (confirm != 'Y') {
            printf("Operation cancelled.\n");
            printf("Press any key to continue...");
            _getch();
            return -1;
        }
        
        // Clean up old list
        destroyList(&employeeList, freeEmployee);
    }
    
    // Load new employee data
    list* newList = loadEmployeeDataFromFile(filename, SINGLY);
    if (!newList) {
        printf("Failed to load employee data from file!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    employeeList = newList;
    employeeListCreated.isEnabled = true;
    
    printf("Employee data loaded successfully!\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles displaying the payroll report.
 * @return Returns 0 on success, -1 on failure.
 */
static int handlePayrollReport() {
    if (!employeeListCreated.isEnabled || !employeeList || !employeeList->head) {
        printf("No employee data to display!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    system("cls");
    printf("=== Payroll Report ===\n\n");
    
    node* current = employeeList->head;
    int count = 0;
    
    double totalBasicPay = 0.0;
    double totalOvertimePay = 0.0;
    double totalDeductions = 0.0;
    double totalNetPay = 0.0;
    
    printf("%-12s | %-15s | %-8s | %-10s | %-10s | %-10s | %-10s | %-10s\n",
           "Emp. Number", "Employee Name", "Status", "Basic Pay", "Overtime", "Deductions", "Net Pay", "Hours");
    printf("----------------------------------------------------------------------------------------------\n");
    
    do {
        current = current->next;
        if (current != employeeList->head) {
            Employee* emp = (Employee*)current->data;
            count++;
            
            // Calculate payroll if not already calculated
            if (emp->payroll.netPay == 0.0) {
                calculatePayroll(emp);
            }
            
            printf("%-12s | %-15s | %-8s | %10.2f | %10.2f | %10.2f | %10.2f | %10d\n",
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
    } while (current != employeeList->head);
    
    printf("----------------------------------------------------------------------------------------------\n");
    printf("Totals:                           | %10.2f | %10.2f | %10.2f | %10.2f |\n",
           totalBasicPay, totalOvertimePay, totalDeductions, totalNetPay);
    printf("----------------------------------------------------------------------------------------------\n");
    printf("Total employees: %d\n\n", count);
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}
