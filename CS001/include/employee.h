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

/**
 * Global variables for managing the employee linked list.
 * 
 * The 'extern' keyword indicates that these variables are declared here,
 * but their memory allocation and definition are in another source file.
 * This allows multiple source files to access the same global variables.
 */
// Global variables for the employee linked list
extern EmployeeNode* empHead;
extern EmployeeNode* empCurr;
extern EmployeeNode* empTail;


// FUNCTIONS PROTOTYPES HERE
// -- START --

// ### FRONTEND ###
EmployeeNode* getEmployeeDataFromUser(EmployeeNode** newEmployee);
void displayReportHeader();
void displayEmployeeRecord();

void printMenu();
int runMenuLoop();

// ### BACKEND ###
void calculatePayroll();
int exportEmployeeDataToFile(EmployeeNode* head, const char *filename);
EmployeeNode* loadEmployeeDataFromFile(const char* filename);

EmployeeNode* createEmployeeNode();
int createEmployeeList();
void addEmployeeNode();
void freeEmployeeList();

// Payroll calculation functions
void calculateNetPay();
void calculateBasicPay();
void calculateDeductions();
void calculateOvertimePay();

// (optional)
// int getEmployeeCount();
// EmployeeNode* findEmployeeByNumber();
// void deleteEmployeeNode();

// -- END --

#endif