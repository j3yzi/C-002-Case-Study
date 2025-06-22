#include <stdio.h>
#include <stdlib.h>
#include "../../include/headers/apctxt.h"
#include "../../include/headers/list.h"
#include "../../include/models/student.h"
#include "ui/menuio.h"

int main(void) {
    // Initialize the terminal
    appInitWinTerm("Student Records System");
    
    list* studentList = NULL;
    if (createList(&studentList, DOUBLY_CIRCULAR) != 0) {
        fprintf(stderr, "Failed to create student list\\n");
        return 1;
    }

    runStudentMenuLoop();

    // Remember to destroy the list and free memory
    // For now, we don't have a function to free student data.
    destroyList(&studentList, NULL); 

    return 0;
} 