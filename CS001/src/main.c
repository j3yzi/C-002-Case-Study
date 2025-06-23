#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/headers/apctxt.h"
#include "../../include/headers/state.h"
#include "../../include/models/employee.h"
#include "../../include/headers/list.h"
#include "ui/menuio.h"

int main(void)
{   
    // Initialize the Windows terminal
    appInitWinTerm("CS001 - Employee Payroll Management System");
    
    // Initialize the global application state
    employeeListCreated.isEnabled = false;
    
    // Create the employee list
    list* employeeList = NULL;
    if (createEmployeeList(&employeeList) != 0) {
        fprintf(stderr, "Failed to create employee list\n");
        printf("Press any key to exit...");
        _getch();
        return 1;
    }

    // Initialize the menu I/O system with the employee list
    initMenuIO(employeeList);
    
    // Start the main menu loop
    int result = menuLoop();
    
    // Clean up resources before exit
    if (employeeList != NULL) {
        destroyList(&employeeList, freeEmployee);
    }

    printf("Thank you for using the Employee Payroll Management System!\n");
    printf("Press any key to exit...");
    _getch();
    
    return result;
}