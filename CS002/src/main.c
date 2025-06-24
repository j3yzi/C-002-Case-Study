#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "../../include/headers/apctxt.h"
#include "../../include/headers/state.h"
#include "../../include/headers/list.h"
#include "../../include/models/employee.h"
#include "ui/menuio.h"

int main(void)
{   
    // Initialize the Windows terminal
    appInitWinTerm("CS002 - Employee Management System");
    
    // Initialize the global application state
    employeeListCreated.isEnabled = false;
    
    // Create the employee list (initially empty)
    list* employeeList = NULL;
    
    // Initialize the menu I/O system with the employee list
    initMenuIO(employeeList);
    
    // Start the main menu loop
    int result = menuLoop();
    
    // Clean up resources before exit (if list was created)
    if (employeeList != NULL) {
        destroyList(&employeeList, free);
    }
    
    printf("Thank you for using the CS002 Employee Management System!\n");
    printf("Press any key to exit...");
    _getch();
    
    return result;
}