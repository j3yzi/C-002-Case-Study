#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/headers/apctxt.h"
#include "../../include/models/employee.h"
#include "../../include/headers/state.h"
#include "../../include/headers/list.h"
#include "ui/menuio.h"

// Initialize configuration

int main(void)
{   
    list* listOfEmplyoees = NULL;
    if (createList(&listOfEmplyoees, DOUBLY_CIRCULAR) != 0) {
        fprintf(stderr, "Failed to create list\n");
        return 1;
    }

    Employee* employeeOne = (Employee*)malloc(sizeof(Employee));
    if (employeeOne == NULL) {
        fprintf(stderr, "Failed to allocate memory for employee\n");
        return 1;
    }

    strcpy(employeeOne->personal.employeeName, "Marc Celis");
    strcpy(employeeOne->personal.employeeNumber, "9213489");

    if (addNode(&listOfEmplyoees, employeeOne) != 0) {
        fprintf(stderr, "Failed to add node\n");
        // It's important to free the allocated memory for employeeOne if addNode fails.
        free(employeeOne);
        destroyList(listOfEmplyoees);
        return 1;
    }

    printf("%s\n", ((Employee*)(listOfEmplyoees->head->data))->personal.employeeName);
    printf("%s\n", ((Employee*)(listOfEmplyoees->head->next->data))->personal.employeeName);
    printf("%s\n", ((Employee*)(listOfEmplyoees->head->prev->data))->personal.employeeName);

    printf("%s\n", ((Employee*)(listOfEmplyoees->tail->data))->personal.employeeName);

    int runMenu = menuLoop();
    
    // Clean up list
    destroyList(&listOfEmplyoees, freeEmployee);

    return 0;
}