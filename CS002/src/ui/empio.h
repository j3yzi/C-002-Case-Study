#ifndef EMPIO_H
#define EMPIO_H

#include "../../../include/models/employee.h"

/**
 * @brief Prompts the user for all new employee data and validates it.
 * @param newEmployee A pointer to an allocated Employee struct to be filled.
 * @return Returns 0 on success, -1 on failure (e.g., invalid name).
 */
int getEmployeeDataFromUser(Employee* newEmployee);

/**
 * @brief Prompts the user to search for an employee and displays results.
 * @param employeeList Pointer to the employee list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleSearchEmployee(const list* employeeList);

/**
 * @brief Prompts the user to edit an existing employee.
 * @param employeeList Pointer to the employee list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleEditEmployee(list* employeeList);

/**
 * @brief Prompts the user to delete an employee.
 * @param employeeList Pointer to the employee list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleDeleteEmployee(list* employeeList);

/**
 * @brief Prompts the user for an employee number to search.
 * @param buffer Buffer to store the employee number.
 * @param bufferSize Size of the buffer.
 * @return Returns 0 on success, -1 on cancel.
 */
int getEmployeeNumberFromUser(char* buffer, int bufferSize);

/**
 * @brief Prompts the user for employee data updates, allowing them to keep existing values.
 * @param employee A pointer to the employee to edit (contains current values).
 * @return Returns 0 on success, -1 on failure (e.g., invalid name).
 */
int editEmployeeDataFromUser(Employee* employee);

#endif 