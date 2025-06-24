// include/employee.h

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../headers/list.h"

// Forward declaration to avoid circular dependency
typedef struct AppContext AppContext;

#define employeeNumberLen 11
#define employeeNameLen 16

#define employeeFirstNameLen 32
#define employeeMiddleNameLen 32
#define employeeLastNameLen 32

#define maxEmployeeCreationCount 5
#define maxEmployeeRecords 50

#define payrollFileName "payroll.dat"
#define regularHours 160.0f
#define overtimeRate 0.5f

typedef struct {
    char firstName[employeeFirstNameLen];
    char middleName[employeeMiddleNameLen];
    char lastName[employeeLastNameLen];
    char fullName[employeeNameLen];
} EmployeeName;

typedef struct {
    char employeeNumber[employeeNumberLen];
    EmployeeName name;
} PersonalInfo;

typedef enum {
    statusRegular, // R
    statusCasual // C
} EmployeeStatus;

typedef struct {
    EmployeeStatus status;
    int hoursWorked;
    float basicRate;
} EmploymentInfo;

typedef struct {
    float basicPay;
    float overtimePay;
    float deductions;
    float netPay;
} PayrollInfo;

typedef struct {
    PersonalInfo personal;
    EmploymentInfo employment;
    PayrollInfo payroll;
} Employee;

// Function declarations
int composeEmployeeName(EmployeeName* name);
int saveEmployeeDataToFile(const list* employeeList, const char *filename);
list* loadEmployeeDataFromFile(const char* filename, ListType listType);

int createEmployee(Employee* newEmployeeData, list** l);
void deleteEmployee(Employee** employee);
int createEmployeeList(list** employeeList);
void freeEmployee(void* employeeData);

// Search operations
Employee* searchEmployeeByNumber(const list* employeeList, const char* employeeNumber);
Employee* searchEmployeeByName(const list* employeeList, const char* fullName);
int searchEmployeeIndexByNumber(const list* employeeList, const char* employeeNumber);

// Edit operations
int updateEmployeeData(Employee* employee, const Employee* newData);
int editEmployeeInList(list* employeeList, const char* employeeNumber, const Employee* newData);

// Remove operations
int removeEmployeeFromList(list* employeeList, const char* employeeNumber);

// Display operations
void displayEmployeeDetails(const Employee* employee);
void displayAllEmployees(const list* employeeList);

// Utility functions
int compareEmployeeByNumber(const void* emp1, const void* emp2);
int compareEmployeeByName(const void* emp1, const void* emp2);

// -- END --

#endif