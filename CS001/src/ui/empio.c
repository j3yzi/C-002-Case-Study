#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "empio.h"
#include "../../../include/headers/apctxt.h"
#include "../../../include/models/employee.h"

int getEmployeeDataFromUser(Employee* newEmployee) {
    memset(newEmployee, 0, sizeof(Employee));

    int isNameValid = 0;
    while (!isNameValid) {
        printf("\n--- Enter Employee Name ---\n");
        appFormField nameFields[] = {
            { "Enter First Name: ", newEmployee->personal.name.firstName, employeeFirstNameLen, IV_MAX_LEN, {.rangeInt = {.max = employeeFirstNameLen - 1}} },
            { "Enter Middle Name (optional): ", newEmployee->personal.name.middleName, employeeMiddleNameLen, IV_OPTIONAL, {.rangeInt = {.max = employeeMiddleNameLen - 1}} },
            { "Enter Last Name: ", newEmployee->personal.name.lastName, employeeLastNameLen, IV_MAX_LEN, {.rangeInt = {.max = employeeLastNameLen - 1}} }
        };
        appGetValidatedInput(nameFields, 3);

        isNameValid = composeEmployeeName(&newEmployee->personal.name);
        if (!isNameValid) {
            printf("\n[Error] Invalid name combination. It must be less than %d chars, or exactly %d with a middle name.\n", employeeNameLen, employeeNameLen - 1);
            printf("Press any key to try again, or ESC to cancel.");
            if (_getch() == 27) return -1; // ESC key
        }
    }

    printf("\n--- Enter Employment Details ---\n");
    char statusInput[2];
    char hoursBuffer[10], rateBuffer[20];
    appFormField otherFields[] = {
        { "Enter Employee Number: ", newEmployee->personal.employeeNumber, employeeNumberLen, IV_MAX_LEN, {.rangeInt = {.max = employeeNumberLen - 1}} },
        { "Enter Status (R/C): ", statusInput, 2, IV_CHOICES, {.choices = {.choices = (const char*[]){"R", "C"}, .count = 2}} },
        { "Enter Hours Worked: ", hoursBuffer, 10, IV_RANGE_INT, {.rangeInt = {.min = 0, .max = 400}} },
        { "Enter Basic Rate: ", rateBuffer, 20, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0}} }
    };
    appGetValidatedInput(otherFields, 4);

    newEmployee->employment.status = (strcmp(statusInput, "R") == 0) ? statusRegular : statusCasual;
    newEmployee->employment.hoursWorked = atoi(hoursBuffer);
    newEmployee->employment.basicRate = atof(rateBuffer);

    return 0; // Success
}

/**
 * @brief Prompts the user for an employee number to search.
 * @param buffer Buffer to store the employee number.
 * @param bufferSize Size of the buffer.
 * @return Returns 0 on success, -1 on cancel.
 */
int getEmployeeNumberFromUser(char* buffer, int bufferSize) {
    printf("Enter Employee Number: ");
    appFormField field = { "Employee Number: ", buffer, bufferSize, IV_MAX_LEN, {.rangeInt = {.max = bufferSize - 1}} };
    appGetValidatedInput(&field, 1);
    return 0;
}

/**
 * @brief Prompts the user to search for an employee and displays results.
 * @param employeeList Pointer to the employee list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleSearchEmployee(const list* employeeList) {
    system("cls");
    printf("--- Search Employee ---\n");
    printf("1. Search by Employee Number\n");
    printf("2. Search by Name\n");
    printf("3. Display All Employees\n");
    printf("Enter choice (1-3): ");
    
    char choice = _getch();
    printf("%c\n\n", choice);
    
    switch (choice) {
        case '1': {
            char empNumber[employeeNumberLen];
            if (getEmployeeNumberFromUser(empNumber, employeeNumberLen) == 0) {
                Employee* emp = searchEmployeeByNumber(employeeList, empNumber);
                if (emp) {
                    displayEmployeeDetails(emp);
                } else {
                    printf("Employee with number '%s' not found.\n", empNumber);
                }
            }
            break;
        }
        case '2': {
            char fullName[employeeNameLen];
            printf("Enter Full Name: ");
            appFormField field = { "Full Name: ", fullName, employeeNameLen, IV_MAX_LEN, {.rangeInt = {.max = employeeNameLen - 1}} };
            appGetValidatedInput(&field, 1);
            
            Employee* emp = searchEmployeeByName(employeeList, fullName);
            if (emp) {
                displayEmployeeDetails(emp);
            } else {
                printf("Employee with name '%s' not found.\n", fullName);
            }
            break;
        }
        case '3':
            displayAllEmployees(employeeList);
            break;
        default:
            printf("Invalid choice.\n");
            return -1;
    }
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Prompts the user to edit an existing employee.
 * @param employeeList Pointer to the employee list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleEditEmployee(list* employeeList) {
    system("cls");
    printf("--- Edit Employee ---\n");
    
    char empNumber[employeeNumberLen];
    if (getEmployeeNumberFromUser(empNumber, employeeNumberLen) != 0) {
        return -1;
    }
    
    Employee* existingEmp = searchEmployeeByNumber(employeeList, empNumber);
    if (!existingEmp) {
        printf("Employee with number '%s' not found.\n", empNumber);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("\nCurrent Employee Information:\n");
    displayEmployeeDetails(existingEmp);
    
    Employee newData;
    memcpy(&newData, existingEmp, sizeof(Employee)); // Start with current data
    
    if (editEmployeeDataFromUser(&newData) == 0) {
        if (updateEmployeeData(existingEmp, &newData) == 0) {
            printf("\nEmployee updated successfully!\n");
        } else {
            printf("Failed to update employee.\n");
        }
    } else {
        printf("Edit cancelled.\n");
    }
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Prompts the user to delete an employee.
 * @param employeeList Pointer to the employee list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleDeleteEmployee(list* employeeList) {
    system("cls");
    printf("--- Delete Employee ---\n");
    
    char empNumber[employeeNumberLen];
    if (getEmployeeNumberFromUser(empNumber, employeeNumberLen) != 0) {
        return -1;
    }
    
    Employee* emp = searchEmployeeByNumber(employeeList, empNumber);
    if (!emp) {
        printf("Employee with number '%s' not found.\n", empNumber);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("\nEmployee to delete:\n");
    displayEmployeeDetails(emp);
    
    printf("Are you sure you want to delete this employee? (Y/N): ");
    char confirm = _getch();
    printf("%c\n", confirm);
    
    if (confirm == 'Y' || confirm == 'y') {
        if (removeEmployeeFromList(employeeList, empNumber) == 0) {
            printf("Employee deleted successfully!\n");
        } else {
            printf("Failed to delete employee.\n");
        }
    } else {
        printf("Delete cancelled.\n");
    }
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Prompts the user for employee data updates, allowing them to keep existing values.
 * @param employee A pointer to the employee to edit (contains current values).
 * @return Returns 0 on success, -1 on failure (e.g., invalid name).
 */
int editEmployeeDataFromUser(Employee* employee) {
    Employee backup;
    memcpy(&backup, employee, sizeof(Employee)); // Backup original data
    
    printf("\n--- Edit Employee Name ---\n");
    printf("Current: First='%s', Middle='%s', Last='%s'\n", 
           employee->personal.name.firstName, 
           employee->personal.name.middleName,
           employee->personal.name.lastName);
    printf("Press Enter to keep current value, or type new value:\n\n");
    
    // Edit name fields with current values as defaults
    char tempFirst[employeeFirstNameLen];
    char tempMiddle[employeeMiddleNameLen]; 
    char tempLast[employeeLastNameLen];
    
    strcpy(tempFirst, employee->personal.name.firstName);
    strcpy(tempMiddle, employee->personal.name.middleName);
    strcpy(tempLast, employee->personal.name.lastName);
    
    printf("First Name [%s]: ", employee->personal.name.firstName);
    if (fgets(tempFirst, employeeFirstNameLen, stdin)) {
        // Remove newline and check if user entered something
        tempFirst[strcspn(tempFirst, "\n")] = 0;
        if (strlen(tempFirst) > 0) {
            strcpy(employee->personal.name.firstName, tempFirst);
        }
    }
    
    printf("Middle Name [%s]: ", employee->personal.name.middleName);
    if (fgets(tempMiddle, employeeMiddleNameLen, stdin)) {
        tempMiddle[strcspn(tempMiddle, "\n")] = 0;
        if (strlen(tempMiddle) > 0) {
            strcpy(employee->personal.name.middleName, tempMiddle);
        }
    }
    
    printf("Last Name [%s]: ", employee->personal.name.lastName);
    if (fgets(tempLast, employeeLastNameLen, stdin)) {
        tempLast[strcspn(tempLast, "\n")] = 0;
        if (strlen(tempLast) > 0) {
            strcpy(employee->personal.name.lastName, tempLast);
        }
    }
    
    // Validate the new name combination
    if (!composeEmployeeName(&employee->personal.name)) {
        printf("\n[Error] Invalid name combination. Reverting to original values.\n");
        memcpy(&employee->personal.name, &backup.personal.name, sizeof(EmployeeName));
        return -1;
    }
    
    printf("\n--- Edit Employment Details ---\n");
    printf("Current: Number='%s', Status='%s', Hours=%d, Rate=%.2f\n",
           employee->personal.employeeNumber,
           (employee->employment.status == statusRegular) ? "Regular" : "Casual",
           employee->employment.hoursWorked,
           employee->employment.basicRate);
    printf("Press Enter to keep current value, or type new value:\n\n");
    
    char tempBuffer[50];
    
    // Employee Number
    printf("Employee Number [%s]: ", employee->personal.employeeNumber);
    if (fgets(tempBuffer, sizeof(tempBuffer), stdin)) {
        tempBuffer[strcspn(tempBuffer, "\n")] = 0;
        if (strlen(tempBuffer) > 0 && strlen(tempBuffer) < employeeNumberLen) {
            strcpy(employee->personal.employeeNumber, tempBuffer);
        }
    }
    
    // Status
    printf("Status (R/C) [%s]: ", (employee->employment.status == statusRegular) ? "R" : "C");
    if (fgets(tempBuffer, sizeof(tempBuffer), stdin)) {
        tempBuffer[strcspn(tempBuffer, "\n")] = 0;
        if (strlen(tempBuffer) > 0) {
            if (tempBuffer[0] == 'R' || tempBuffer[0] == 'r') {
                employee->employment.status = statusRegular;
            } else if (tempBuffer[0] == 'C' || tempBuffer[0] == 'c') {
                employee->employment.status = statusCasual;
            }
        }
    }
    
    // Hours Worked
    printf("Hours Worked [%d]: ", employee->employment.hoursWorked);
    if (fgets(tempBuffer, sizeof(tempBuffer), stdin)) {
        tempBuffer[strcspn(tempBuffer, "\n")] = 0;
        if (strlen(tempBuffer) > 0) {
            int newHours = atoi(tempBuffer);
            if (newHours >= 0 && newHours <= 400) {
                employee->employment.hoursWorked = newHours;
            }
        }
    }
    
    // Basic Rate
    printf("Basic Rate [%.2f]: ", employee->employment.basicRate);
    if (fgets(tempBuffer, sizeof(tempBuffer), stdin)) {
        tempBuffer[strcspn(tempBuffer, "\n")] = 0;
        if (strlen(tempBuffer) > 0) {
            float newRate = atof(tempBuffer);
            if (newRate >= 0.0) {
                employee->employment.basicRate = newRate;
            }
        }
    }
    
    return 0; // Success
}