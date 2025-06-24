#ifndef EMPIO_H
#define EMPIO_H

#include "../../include/headers/list.h"
#include "../../include/models/employee.h"

// Employee data input functions
int getEmployeeDataFromUser(Employee* newEmployee);
int getEmployeeNumberFromUser(char* buffer, int bufferSize);
int editEmployeeDataFromUser(Employee* employee);

// Employee operations
int handleSearchEmployee(const list* employeeList);
int handleEditEmployee(list* employeeList);
int handleDeleteEmployee(list* employeeList);

#endif 