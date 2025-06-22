#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/headers/apctxt.h"
#include "../../include/models/employee.h"
#include "../../include/headers/state.h"
#include "ui/menuio.h"

// Initialize configuration

int main(void)
{   
    list* listOfEmplyoees = createList(DOUBLY_CIRCULAR);

    Employee* employeeOne = (Employee*)malloc(sizeof(Employee));

    strcpy(employeeOne->personal.employeeName, "Marc Celis");
    strcpy(employeeOne->personal.employeeNumber, "9213489");

    addNode(&listOfEmplyoees, employeeOne);

    printf("%s\n", ((Employee*)(listOfEmplyoees->head->data))->personal.employeeName);
    printf("%s\n", ((Employee*)(listOfEmplyoees->head->next->data))->personal.employeeName);
    printf("%s\n", ((Employee*)(listOfEmplyoees->head->prev->data))->personal.employeeName);

    printf("%s\n", ((Employee*)(listOfEmplyoees->tail->data))->personal.employeeName);

    int runMenu = menuLoop();
    

    return 0;
}