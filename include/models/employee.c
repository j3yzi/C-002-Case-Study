#include "employee.h"
#include "../headers/list.h"
#include <stdio.h>
#include <stdlib.h>

// Forward declaration
int createEmployee(Employee* newEmployeeData, list** l);

/**
 * @brief Creates a new, empty list to store employees.
 * @param employeeList A double pointer to the list structure to be created.
 * @return Returns 0 on success, -1 on failure.
 */
int createEmployeeList(list** employeeList) {
    if (createList(employeeList, SINGLY) != 0) {
        return -1;
    }
    return 0;
}

/**
 * @brief Adds a new employee to the specified list.
 * @param newEmployeeData A pointer to the Employee struct to be added.
 * @param l A double pointer to the list where the employee will be added.
 * @return Returns 0 on success, -1 on failure.
 */
int createEmployee(Employee* newEmployeeData, list** l) {
    if (addNode(l, newEmployeeData) != 0) {
        return -1;
    }
    return 0;
}

/**
 * @brief Deletes an employee and frees the associated memory.
 * @brief This function frees the memory of the Employee struct and sets the original pointer to NULL
 * @brief to prevent dangling pointer issues.
 * @param employee A double pointer to the Employee struct to be deleted.
 */
void deleteEmployee(Employee** employee) {
    if (employee != NULL && *employee != NULL) {
        free(*employee);
        *employee = NULL;
    }
}

/**
 * @brief Frees the memory of an employee's data.
 * @brief This function is designed to be used as a callback (e.g., for destroyList)
 * @brief and takes a void pointer to the employee data.
 * @param employeeData A void pointer to the Employee data to be freed.
 */
void freeEmployee(void* employeeData) {
    if (employeeData != NULL) {
        free(employeeData);
    }
}