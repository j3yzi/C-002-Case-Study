#include "../../include/employee.h"

EmployeeNode* getEmployeeDataFromUser(EmployeeNode** newEmployee) { // tatanggap ng node from backend (createEmployeeList() ng list_manager.c)
    printf("Employee Number: ");
    scanf("%s", (*newEmployee)->employeeNumber);
    
    // (...) Continue
    
    // Return the populated node
    return *newEmployee;
    
    /* NOTE TO DEVELOPERS:
     * This function is responsible for collecting employee data from user input.
     * Pay calculations will be handled by the payroll_logic.c module.
     */
}


void displayReportHeader() {
    //                             ABC COMPANY
    //                             Makati City
    // Employee Number    |   Employee Name  | ...
}


void displayEmployeeRecord(EmployeeNode* employee) {
    if (employee == NULL) {
        printf("Error: NULL employee record\n");
        return;
    }

    // 1234567890    Juan Dela Cruz    Status   Basic Salary
}