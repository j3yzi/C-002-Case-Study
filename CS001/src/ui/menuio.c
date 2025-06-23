#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "menuio.h"
#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/state.h"
#include "../../../include/models/employee.h"
#include "../../../include/headers/list.h"
#include "empio.h"
#include "../modules/payroll.h"

#define HIGHLIGHT_TEXT_COLOR 9
#define HIGHLIGHT_BG_COLOR 0
#define TEXT_COLOR 7
#define BG_COLOR 0
#define DISABLED_TEXT_COLOR 8
#define DISABLED_BG_COLOR 0

// File-static pointer to the employee list
static list* g_employeeList = NULL;

void initMenuIO(list* employeeList) {
    g_employeeList = employeeList;
}

void handleCreateEmployeeList() {
    system("cls");
    printf("--- Create Employee List ---\n");
    printf("You will now enter data for %d employees.\n\n", maxEmployeeCreationCount);
    
    int successCount = 0;
    for (int i = 0; i < maxEmployeeCreationCount; i++) {
        printf("=== Employee %d of %d ===\n", i + 1, maxEmployeeCreationCount);
        
        Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
        if (!newEmployee) {
            printf("Memory allocation failed for employee %d!\n", i + 1);
            continue;
        }
        
        if (getEmployeeDataFromUser(newEmployee) == 0) {
            if (createEmployee(newEmployee, &g_employeeList) == 0) {
                printf("Employee %d added successfully!\n\n", i + 1);
                successCount++;
            } else {
                printf("Failed to add employee %d to list.\n\n", i + 1);
                free(newEmployee);
            }
        } else {
            printf("Employee %d creation cancelled.\n\n", i + 1);
            free(newEmployee);
        }
        
        if (i < maxEmployeeCreationCount - 1) {
            printf("Press any key to continue to next employee...");
            _getch();
            system("cls");
        }
    }
    
    employeeListCreated.isEnabled = true;
    printf("Employee list creation complete! %d employees added successfully.\n", successCount);
    printf("Press any key to continue...");
    _getch();
}

void handleAddEmployee() {
    system("cls");
    printf("--- Add New Employee ---\n");

    Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
    if (!newEmployee) {
        printf("Memory allocation failed!\n");
        _getch();
        return;
    }

    if (getEmployeeDataFromUser(newEmployee) == 0) { // 0 is success
        if (createEmployee(newEmployee, &g_employeeList) != 0) {
            printf("Error: Failed to add employee to the list.\n");
            free(newEmployee); // Clean up if add fails
        } else {
            printf("\nEmployee added successfully!\n");
        }
    } else {
        printf("\nAdd employee cancelled.\n");
        free(newEmployee); // Clean up if user cancelled
    }

    printf("Press any key to continue...");
    _getch();
}

void handleDisplayPayroll() {
    system("cls");
    printf("--- Payroll Report ---\n\n");
    printf("%-10s | %-15s | %-8s | %-12s | %-12s | %-10s | %-12s\n", "Emp. No.", "Employee Name", "Status", "Basic Pay", "Overtime Pay", "Deductions", "Net Pay");
    printf("--------------------------------------------------------------------------------------------------\n");

    if (g_employeeList == NULL || g_employeeList->head == NULL) {
        printf("No employees to display.\n");
    } else {
        node* current = g_employeeList->head;
        do {
            Employee* emp = (Employee*)current->data;
            calculatePayroll(emp);
            char statusStr[10];
            switch (emp->employment.status) {
                case statusRegular: strcpy(statusStr, "Regular"); break;
                case statusCasual: strcpy(statusStr, "Casual"); break;
                default: strcpy(statusStr, "N/A"); break;
            }
            printf("%-10s | %-15s | %-8s | %-12.2f | %-12.2f | %-10.2f | %-12.2f\n", emp->personal.employeeNumber, emp->personal.name.fullName, statusStr, emp->payroll.basicPay, emp->payroll.overtimePay, emp->payroll.deductions, emp->payroll.netPay);
            current = current->next;
        } while (current != g_employeeList->head);
    }
    printf("\nPress any key to continue...");
    _getch();
}

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
    {'1', "Create Employee List", false, true, 9,0,7,0,8,0, &handleCreateEmployeeList},
    {'2', "Add an Employee", true, false, 9,0,7,0,8,0, &handleAddEmployee},
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
    if (g_employeeList != NULL && g_employeeList->head != NULL) {
        mainMenu.options[1].isDisabled = false; // Edit
        mainMenu.options[2].isDisabled = false; // Search
        mainMenu.options[3].isDisabled = false; // Delete
        mainMenu.options[5].isDisabled = false; // Export
        mainMenu.options[6].isDisabled = false; // Display
    }
}

int menuLoop() {
    char mainMenuChoice;
    do {
        checkStates();
        mainMenuChoice = initMenu(&mainMenu);
        
        switch (mainMenuChoice) {
            case '1': {
                char subMenuChoice;
                do {
                    checkStates();
                    subMenuChoice = initMenu(&addEmployeeSubMenu);
                    // The initMenu function calls the onSelect callback
                } while (subMenuChoice != '3');
                break;
            }
            case '2':
                handleEditEmployee(g_employeeList);
                break;
            case '3':
                handleSearchEmployee(g_employeeList);
                break;
            case '4':
                handleDeleteEmployee(g_employeeList);
                break;
            case '5':
            case '6':
                // These options are not implemented yet
                winTermClearScreen();
                printf("--- Option '%c' is not implemented yet ---\n", mainMenuChoice);
                printf("Press any key to continue...");
                _getch();
                break;
            case '7':
                handleDisplayPayroll(); // Call directly
            case '8':
                winTermClearScreen();
                printf("Exiting application...\n");
                break;
            default:
                // Unknown option, just continue
                break;
        }
    } while (mainMenuChoice != '8');
    return 0;
}
