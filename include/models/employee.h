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

#define EMPLOYEE_NUMBER_LEN 11
#define EMPLOYEE_NAME_LEN 16

#define MAX_EMPLOYEE_CREATION_COUNT 5
#define MAX_EMPLOYEE_RECORDS 50

#define PAYROLL_FILE_NAME "payroll.dat"
#define REGULAR_HOURS 160.0f
#define OVERTIME_RATE 0.5f

typedef struct {
    char employeeNumber[EMPLOYEE_NUMBER_LEN];
    char employeeName[EMPLOYEE_NAME_LEN];
} PersonalInfo;
typedef enum {
    STATUS_REGULAR, // R
    STATUS_CASUAL // C
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
int saveEmployeeDataToFile(const list* employeeList, const char *filename);
list* loadEmployeeDataFromFile(const char* filename, ListType listType);


int createEmployee(Employee* newEmployeeData, list** l);
void deleteEmployee(Employee** employee);
int createEmployeeList(list** employeeList);
void freeEmployee(void* employeeData);

// -- END --

#endif