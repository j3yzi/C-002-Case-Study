#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "../../include/headers/apctxt.h"
#include "../../include/headers/state.h"
#include "../../include/headers/list.h"
#include "ui/menuio.h"

int main(void)
{   
    // Initialize the Windows terminal
    appInitWinTerm("CS002 - Academic Management System");
    
    // Initialize the global application state
    studentListCreated.isEnabled = false;
    universityEmployeeListCreated.isEnabled = false;
    
    // Create the lists (initially empty)
    list* studentList = NULL;
    list* universityEmployeeList = NULL;
    
    // Initialize the menu I/O system with the lists
    initMenuIO(studentList, universityEmployeeList);
    
    // Start the main menu loop
    int result = runMenuLoop();
    
    // Clean up resources before exit (if lists were created)
    // Note: Add proper cleanup functions when lists are implemented
    
    printf("Thank you for using the Academic Management System!\n");
    printf("Press any key to exit...");
    _getch();
    
    return result;
}