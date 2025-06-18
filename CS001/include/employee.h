// include/employee.h

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Forward declaration to avoid circular dependency
typedef struct AppContext AppContext;

#define EMPLOYEE_NUMBER_LEN 11
#define EMPLOYEE_NAME_LEN 16
#define MAX_EMPLOYEE_CREATION_COUNT 2
#define MAX_EMPLOYEE_RECORDS 50

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

// employee_io.c
EmployeeNode* getEmployeeDataFromUser(EmployeeNode** newEmployee, AppContext* appContext);
void displayReportHeader();
int displayEmployeeRecord(AppContext* appContext);

// menu_io.c
void printMenu();
int runMenuLoop(AppContext* appContext);

// ### BACKEND ###

// file_handler.c
int exportEmployeeDataToFile(EmployeeNode* head, const char *filename);
EmployeeNode* loadEmployeeDataFromFile(const char* filename);

// list_manager.c
int createEmployeeList(AppContext** appContext);
EmployeeNode* createEmployeeNode(AppContext** appContext);
void editEmployeeNode();
void deleteEmployeeNode();
void freeEmployeeList();
int getEmployeeCount();

// payroll_logic.c
void calculateBasicPay(EmployeeNode* employee);
void calculateOvertimePay(EmployeeNode* employee);
void calculateDeductions(EmployeeNode* employee);
void calculateNetPay(EmployeeNode* employee);

// (optional)
// int getEmployeeCount();
// EmployeeNode* findEmployeeByNumber();
// void deleteEmployeeNode();

// -- END --

#endif