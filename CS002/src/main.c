#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "../../include/headers/apctxt.h"
#include "../../include/headers/state.h"
#include "../../include/headers/list.h"
#include "../../include/models/employee.h"
#include "../../include/models/student.h"
#include "ui/menuio.h"

int main(void)
{   
    // Initialize the Windows terminal
    appInitWinTerm("CS002 - Dual Management System");
    
    // Initialize the multi-list management system
    initMultiListManager();
    
    // Start the main menu loop
    int result = menuLoop();
    
    // Clean up all resources before exit
    cleanupMultiListManager();
    
    printf("Press any key to exit...");
    _getch();
    
    return result;
}