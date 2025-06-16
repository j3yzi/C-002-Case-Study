#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/employee.h"

EmployeeNode *employeeList = NULL;
const char* dataFile = "data/record.bin";

int main (void)
{
    employeeList = loadEmployeeDataFromFile(dataFile);
    
    if (employeeList == NULL) {
        printf("No previous records found or file error. Starting fresh.\n\n");
    } else {
        printf("Employee records loaded successfully.\n\n");
    }

    runMenuLoop();

    return 0;
}