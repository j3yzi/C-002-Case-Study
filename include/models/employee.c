#include "employee.h"
#include "../headers/list.h"
#include <stdio.h>
#include <stdlib.h>

// Forward declaration
int createEmployee(Employee* newEmployeeData, list** l);

int createEmployeeList(list** employeeList) {
    if (createList(employeeList, SINGLY) != 0) {
        return -1;
    }
    return 0;
}

int createEmployee(Employee* newEmployeeData, list** l) {
    if (addNode(l, newEmployeeData) != 0) {
        return -1;
    }
    return 0;
}

void deleteEmployee(Employee* employee) {
    if (employee != NULL) {
        free(employee);
    }
}

void freeEmployee(void* employeeData) {
    if (employeeData != NULL) {
        free(employeeData);
    }
}