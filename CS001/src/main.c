#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/employee.h"
#include "../include/apctxt.h"
#include "../include/list.h"

// Initialize configuration
AppConfig config;

int main(void)
{   
    Employee employee;
    // Buffers to hold string input for non-string fields
    char statusStr[2];
    char hoursStr[4];
    char rateStr[8];

    config.appName = "Payroll Management System";
    config.appVersion = "1.0.0";
    config.license = "MIT License";
    config.creator = "GROUP 1 - BSIT 1-2";

    // appFormField employeeForm[] = {
    //     {"\033[36mEmployee Number: \033[0m", employee.personal.employeeNumber, EMPLOYEE_NUMBER_LEN, IV_MAX_LEN_CHARS, {
    //         .maxLengthChars = {.maxLength = EMPLOYEE_NUMBER_LEN - 1}
    //     }},

    //     {"\033[36mEmployee Name: \033[0m", employee.personal.employeeName, EMPLOYEE_NAME_LEN, IV_MAX_LEN_CHARS, {
    //         .maxLengthChars = {.maxLength = EMPLOYEE_NAME_LEN - 1}
    //     }},

    //     // Corrected fields to use string buffers and appropriate validation
    //     {"\033[36mEmployee Status (0 for Regular, 1 for Casual): \033[0m", statusStr, sizeof(statusStr), IV_CHOICES, {.choices = {.choices = (const char*[]){"0", "1"}, .count = 2}}},

    //     {"\033[36mHours Worked: \033[0m", hoursStr, sizeof(hoursStr), IV_RANGE_INT, {.rangeInt = {0, 99}}},

    //     {"\033[36mBasic Rate: \033[0m", rateStr, sizeof(rateStr), IV_RANGE_FLT, {.rangeFloat = {0.0f, 999.99f}}}
    // };

    // appGetValidatedInput(employeeForm, 5);

    // // Convert string buffers to the correct types and assign to the employee struct
    // employee.employment.status = (EmployeeStatus)atoi(statusStr);
    // employee.employment.hoursWorked = atoi(hoursStr);
    // employee.employment.basicRate = atof(rateStr);
    
    // // --- Verification ---
    // // Print the collected data to confirm it's stored correctly.
    // printf("\n--- Employee Data Received ---\n");
    // printf("Number: %s\n", employee.personal.employeeNumber);
    // printf("Name: %s\n", employee.personal.employeeName);
    // printf("Status: %s\n", employee.employment.status == STATUS_REGULAR ? "Regular" : "Casual");
    // printf("Hours Worked: %d\n", employee.employment.hoursWorked);
    // printf("Basic Rate: %.2f\n", employee.employment.basicRate);
    // // ---------------------
    
    appInitWinTerm(config.appName);
    int initializeMenuLoop = menuLoop();

    system("cls");
    printf("Thank you for using the program!\n");

    return 0;
}