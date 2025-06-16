// include/employee.h

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPLOYEE_NUMBER_LEN 11
#define EMPLOYEE_NAME_LEN 16
#define MAX_EMPLOYEES 5

typedef enum {
    STATUS_REGULAR, // R
    STATUS_CASUAL // C
} EmployeeStatusCode;

typedef struct EmployeeNode {
    char employeeNumber[EMPLOYEE_NUMBER_LEN]; // 10 characters 
    char employeeName[EMPLOYEE_NAME_LEN];     // 15 characters 
    
    EmployeeStatusCode statusCode;            // R or C 
    
    int hoursWorked;                          // max 99 
    
    float basicPay;                           // max 999999.99 
    float basicRate;                          // max 999.99 
    float deductions;                         // max 99999.99 
    float overtimePay;                        // Computed.]
    float netPay;                             // Computed.]
    
    struct EmployeeNode *next;
} EmployeeNode;

// FUNCTIONS PROTOTYPES HERE
// -- START --

// ### FRONTEND ###
EmployeeNode* getEmployeeDataFromUser();
void displayReportHeader();
void displayEmployeeRecord();

// ### BACKEND ###
void calculatePayroll();
int saveEmployeeDataToFile();
int loadEmployeeDataFromFile();

EmployeeNode* createEmployeeNode();

void addEmployeeNode();
void freeEmployeeList();

// (optional)
// int getEmployeeCount();
// EmployeeNode* findEmployeeByNumber();
// void deleteEmployeeNode();

// -- END --

#endif