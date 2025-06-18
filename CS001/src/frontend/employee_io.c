#include "../../include/employee.h"
#include "../../include/appctxt.h"

#include <windows.h>

EmployeeNode* getEmployeeDataFromUser(EmployeeNode** newEmployee, AppContext* appContext) { // tatanggap ng node from backend (createEmployeeList() ng list_manager.c)
    system("cls");
    
    if (appContext->isInitialList) {
        printf("Employee %d of %d\n", appContext->count + 1, MAX_EMPLOYEE_CREATION_COUNT);
    } else {
        printf("Employee %d of %d\n", appContext->count + 1, MAX_EMPLOYEE_RECORDS);
    }

    while (1) {
        printf("Employee Number: ");
        if (scanf("%10s", (*newEmployee)->employeeNumber) == 1) {
            while (getchar() != '\n');
            break;
        }
        printf("Error: Invalid employee number input\n");
        while (getchar() != '\n');
    }
    
    while (1) {
        printf("Employee Name: ");
        if (scanf("%15[^\n]", (*newEmployee)->employeeName) == 1) {
            while (getchar() != '\n');
            break;
        }
        printf("Error: Invalid employee name input\n");
        while (getchar() != '\n');
    }
    
    
    int statusInput;
    while (1) {
        printf("Employee Status (0 for Regular, 1 for Casual): ");
        if (scanf("%d", &statusInput) == 1 && (statusInput == 0 || statusInput == 1)) {
            while (getchar() != '\n');
            (*newEmployee)->statusCode = (EmployeeStatusCode)statusInput;
            break;
        }
        printf("Error: Invalid status code input\n");
        while (getchar() != '\n');
    }
    
    
    while (1) {
        printf("Employee Hours Worked: ");
        if (scanf("%d", &(*newEmployee)->hoursWorked) == 1 && (*newEmployee)->hoursWorked >= 0 && (*newEmployee)->hoursWorked <= 99) {
            while (getchar() != '\n');
            break;
        }
        printf("Error: Invalid hours worked input (must be 0-99)\n");
        while (getchar() != '\n');
    }

    while (1) {
        printf("Employee Basic Rate: ");
        if (scanf("%f", &(*newEmployee)->basicRate) == 1 && (*newEmployee)->basicRate >= 0 && (*newEmployee)->basicRate <= 999.99) {
            while (getchar() != '\n');
            break;
        }
        printf("Error: Invalid basic rate input (must be 0-999.99)\n");
        while (getchar() != '\n');
    }
    
    return *newEmployee;
    
    /* NOTE TO DEVELOPERS:
     * This function is responsible for collecting employee data from user input.
     * Pay calculations will be handled by the payroll_logic.c module.
     */
}


void displayReportHeader() {
    printf("\n");
    printf("                             ABC COMPANY\n");
    printf("                             Makati City\n");
    printf("Employee Number    |   Employee Name  |   Status   |   Hours Worked\n");
    printf("----------------------------------------------------------------\n");
}


int displayEmployeeRecord(AppContext* appContext) {
    if (appContext->head == NULL) {
        printf("No employee records found.\n");
        return -1;
    }

    displayReportHeader();

    appContext->current = appContext->head;

    while (appContext->current != NULL) {
        const char* statusText = (appContext->current->statusCode == STATUS_REGULAR) ? "Regular" : "Casual";
        
        printf("%-15s |   %-15s |   %-8s |   %d\n", 
               appContext->current->employeeNumber, 
               appContext->current->employeeName, 
               statusText, 
               appContext->current->hoursWorked);
        
        appContext->current = appContext->current->next;
    }

    appContext->current = NULL;
    printf("\n");

    return 0;
}
