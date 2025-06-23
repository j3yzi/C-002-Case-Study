#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/headers/apctxt.h"
#include "../../include/models/employee.h"
#include "../../include/headers/list.h"
#include "ui/menuio.h"

int main(void)
{   
    appInitWinTerm("Payroll System");
    
    list* employeeList = NULL;
    if (createEmployeeList(&employeeList) != 0) {
        fprintf(stderr, "Failed to create list\n");
        return 1;
    }

    initMenuIO(employeeList);
    
    menuLoop();
    
    // Clean up list
    destroyList(&employeeList, freeEmployee);

    return 0;
}