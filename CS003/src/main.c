#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "../../include/headers/apctxt.h"
#include "../../include/headers/state.h"
#include "../../include/headers/list.h"
#include "../../include/models/student.h"
#include "ui/menuio.h"

int main(void)
{   
    // Initialize the Windows terminal
    appInitWinTerm("CS003 - Student Records Management System");
    
    // Initialize the global application state
    studentListCreated.isEnabled = false;
    
    // Create the student list (initially empty)
    list* studentList = NULL;
    
    // Initialize the menu I/O system with the student list
    initMenuIO(studentList);
    
    // Start the main menu loop
    int result = runStudentMenuLoop();
    
    // Clean up resources before exit (if list was created)
    if (studentList != NULL) {
        destroyList(&studentList, freeStudent);
    }
    
    printf("Thank you for using the CS003 Student Records Management System!\n");
    printf("Press any key to exit...");
    _getch();
    
    return result;
} 