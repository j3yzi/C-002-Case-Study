#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "empio.h"
#include "../../../include/headers/apctxt.h"
#include "../../../include/models/employee.h"

/**
 * @brief Prompts the user for all new employee data and validates it.
 * @param newEmployee A pointer to an allocated Employee struct to be filled.
 * @return Returns 0 on success, -1 on failure (e.g., invalid name).
 */
int getEmployeeDataFromUser(Employee* newEmployee) {
    // Clear the employee structure
    memset(newEmployee, 0, sizeof(Employee));

    int isNameValid = 0;
    while (!isNameValid) {
        winTermClearScreen();
        printf("=== Enter Employee Name ===\n\n");
        
        appFormField nameFields[] = {
            { "Enter First Name: ", newEmployee->personal.name.firstName, employeeFirstNameLen, IV_MAX_LEN, {.rangeInt = {.max = employeeFirstNameLen - 1}} },
            { "Enter Middle Name (optional): ", newEmployee->personal.name.middleName, employeeMiddleNameLen, IV_OPTIONAL, {.rangeInt = {.max = employeeMiddleNameLen - 1}} },
            { "Enter Last Name: ", newEmployee->personal.name.lastName, employeeLastNameLen, IV_MAX_LEN, {.rangeInt = {.max = employeeLastNameLen - 1}} }
        };
        appGetValidatedInput(nameFields, 3);

        isNameValid = composeEmployeeName(&newEmployee->personal.name);
        if (!isNameValid) {
            printf("\n[Error] Invalid name combination. It must be less than %d chars, or exactly %d with a middle name.\n", employeeNameLen, employeeNameLen - 1);
            printf("Press any key to try again, or ESC to cancel...");
            if (_getch() == 27) return -1; // ESC key
        }
    }

    winTermClearScreen();
    printf("=== Enter Employment Details ===\n\n");
    
    char statusInput[3];
    char hoursBuffer[10], rateBuffer[20];
    
    appFormField employmentFields[] = {
        { "Enter Employee Number: ", newEmployee->personal.employeeNumber, employeeNumberLen, IV_MAX_LEN, {.rangeInt = {.max = employeeNumberLen - 1}} },
        { "Enter Status (R/C): ", statusInput, 3, IV_CHOICES, {.choices = {.choices = (const char*[]){"R", "C", "r", "c"}, .count = 4}} },
        { "Enter Hours Worked (0-400): ", hoursBuffer, 10, IV_RANGE_INT, {.rangeInt = {.min = 0, .max = 400}} },
        { "Enter Basic Rate (minimum 0.00): ", rateBuffer, 20, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 10000.0}} }
    };
    appGetValidatedInput(employmentFields, 4);

    // Convert string inputs to appropriate data types
    newEmployee->employment.status = (statusInput[0] == 'R' || statusInput[0] == 'r') ? statusRegular : statusCasual;
    newEmployee->employment.hoursWorked = atoi(hoursBuffer);
    newEmployee->employment.basicRate = (float)atof(rateBuffer);

    printf("\n=== Employee Information Summary ===\n");
    printf("Name: %s\n", newEmployee->personal.name.fullName);
    printf("Employee Number: %s\n", newEmployee->personal.employeeNumber);
    printf("Status: %s\n", (newEmployee->employment.status == statusRegular) ? "Regular" : "Casual");
    printf("Hours Worked: %d\n", newEmployee->employment.hoursWorked);
    printf("Basic Rate: %.2f\n", newEmployee->employment.basicRate);
    
    printf("\nConfirm this information? (Y/N): ");
    char confirm = _getch();
    printf("%c\n", confirm);
    
    if (confirm != 'Y' && confirm != 'y') {
        printf("Employee creation cancelled.\n");
        return -1;
    }

    return 0; // Success
}

/**
 * @brief Prompts the user for an employee number to search.
 * @param buffer Buffer to store the employee number.
 * @param bufferSize Size of the buffer.
 * @return Returns 0 on success, -1 on cancel.
 */
int getEmployeeNumberFromUser(char* buffer, int bufferSize) {
    appFormField field = { "Enter Employee Number: ", buffer, bufferSize, IV_MAX_LEN, {.rangeInt = {.max = bufferSize - 1}} };
    appGetValidatedInput(&field, 1);
    return 0;
}

/**
 * @brief Prompts the user to search for an employee and displays results.
 * @param employeeList Pointer to the employee list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleSearchEmployee(const list* employeeList) {
    winTermClearScreen();
    printf("=== Search Employee ===\n\n");
    printf("1. Search by Employee Number\n");
    printf("2. Search by Name\n");
    printf("3. Display All Employees\n");
    printf("4. Back to Main Menu\n");
    printf("\nEnter choice (1-4): ");
    
    char choice = _getch();
    printf("%c\n\n", choice);
    
    switch (choice) {
        case '1': {
            char empNumber[employeeNumberLen];
            printf("=== Search by Employee Number ===\n");
            if (getEmployeeNumberFromUser(empNumber, employeeNumberLen) == 0) {
                Employee* emp = searchEmployeeByNumber(employeeList, empNumber);
                if (emp) {
                    printf("\n=== Employee Found ===\n");
                    displayEmployeeDetails(emp);
                } else {
                    printf("\nEmployee with number '%s' not found.\n", empNumber);
                }
            }
            break;
        }
        case '2': {
            char fullName[employeeNameLen];
            printf("=== Search by Name ===\n");
            appFormField field = { "Enter Full Name: ", fullName, employeeNameLen, IV_MAX_LEN, {.rangeInt = {.max = employeeNameLen - 1}} };
            appGetValidatedInput(&field, 1);
            
            Employee* emp = searchEmployeeByName(employeeList, fullName);
            if (emp) {
                printf("\n=== Employee Found ===\n");
                displayEmployeeDetails(emp);
            } else {
                printf("\nEmployee with name '%s' not found.\n", fullName);
            }
            break;
        }
        case '3':
            winTermClearScreen();
            printf("=== Display All Employees ===\n");
            displayAllEmployees(employeeList);
            break;
        case '4':
            return 0; // Back to main menu
        default:
            printf("Invalid choice.\n");
            break;
    }
    
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Prompts the user to edit an existing employee.
 * @param employeeList Pointer to the employee list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleEditEmployee(list* employeeList) {
    winTermClearScreen();
    printf("=== Edit Employee ===\n\n");
    
    char empNumber[employeeNumberLen];
    if (getEmployeeNumberFromUser(empNumber, employeeNumberLen) != 0) {
        return -1;
    }
    
    Employee* existingEmp = searchEmployeeByNumber(employeeList, empNumber);
    if (!existingEmp) {
        printf("\nEmployee with number '%s' not found.\n", empNumber);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("\n=== Current Employee Information ===\n");
    displayEmployeeDetails(existingEmp);
    
    Employee newData;
    memcpy(&newData, existingEmp, sizeof(Employee)); // Start with current data
    
    if (editEmployeeDataFromUser(&newData) == 0) {
        if (updateEmployeeData(existingEmp, &newData) == 0) {
            printf("\nEmployee updated successfully!\n");
            printf("\n=== Updated Employee Information ===\n");
            displayEmployeeDetails(existingEmp);
        } else {
            printf("Failed to update employee.\n");
        }
    } else {
        printf("Edit cancelled.\n");
    }
    
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Prompts the user to delete an employee.
 * @param employeeList Pointer to the employee list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleDeleteEmployee(list* employeeList) {
    winTermClearScreen();
    printf("=== Delete Employee ===\n\n");
    
    // Validate employee list first
    if (!employeeList) {
        printf("Error: Employee list is not available.\n");
        printf("Press any key to continue...");
        _getch();
        return 0; // Return 0 to continue program, not exit
    }
    
    if (!employeeList->head || employeeList->size == 0) {
        printf("No employees available to delete.\n");
        printf("Please add some employees first.\n");
        printf("Press any key to continue...");
        _getch();
        return 0; // Return 0 to continue program
    }
    
    char empNumber[employeeNumberLen];
    printf("Enter the employee number you want to delete:\n");
    if (getEmployeeNumberFromUser(empNumber, employeeNumberLen) != 0) {
        printf("Operation cancelled.\n");
        printf("Press any key to continue...");
        _getch();
        return 0; // Return 0 to continue program
    }
    
    Employee* emp = searchEmployeeByNumber(employeeList, empNumber);
    if (!emp) {
        printf("\n❌ Employee with number '%s' was not found.\n", empNumber);
        printf("\nWould you like to:\n");
        printf("1. Try a different employee number\n");
        printf("2. View all employees\n");
        printf("3. Return to main menu\n");
        printf("\nEnter choice (1-3): ");
        
        char choice = _getch();
        printf("%c\n", choice);
        
        switch (choice) {
            case '1':
                // Recursive call to try again
                return handleDeleteEmployee(employeeList);
            case '2':
                printf("\n=== Current Employees ===\n");
                displayAllEmployees(employeeList);
                printf("\nPress any key to continue...");
                _getch();
                return 0;
            case '3':
            default:
                printf("Returning to main menu...\n");
                printf("Press any key to continue...");
                _getch();
                return 0;
        }
    }
    
    printf("\n=== Employee to Delete ===\n");
    displayEmployeeDetails(emp);
    
    printf("\n⚠️  WARNING: This action cannot be undone!\n");
    printf("Are you sure you want to delete this employee? (Y/N): ");
    char confirm = _getch();
    printf("%c\n", confirm);
    
    if (confirm == 'Y' || confirm == 'y') {
        printf("\nDeleting employee...\n");
        if (removeEmployeeFromList(employeeList, empNumber) == 0) {
            printf("✅ Employee '%s' deleted successfully!\n", empNumber);
            printf("Employee count is now: %d\n", employeeList->size);
        } else {
            printf("❌ Failed to delete employee from the system.\n");
            printf("The employee may have already been removed.\n");
        }
    } else {
        printf("Delete operation cancelled.\n");
        printf("Employee '%s' was not deleted.\n", empNumber);
    }
    
    printf("\nPress any key to continue...");
    _getch();
    return 0; // Always return 0 to continue program execution
}

/**
 * @brief Prompts the user for employee data updates using appFormField validation.
 * @param employee A pointer to the employee to edit (contains current values).
 * @return Returns 0 on success, -1 on failure (e.g., invalid name).
 */
int editEmployeeDataFromUser(Employee* employee) {
    Employee backup;
    memcpy(&backup, employee, sizeof(Employee)); // Backup original data
    
    winTermClearScreen();
    printf("=== Edit Employee Data ===\n\n");
    printf("Current Employee Information:\n");
    printf("Name: %s %s %s\n", employee->personal.name.firstName, 
           employee->personal.name.middleName, employee->personal.name.lastName);
    printf("Employee Number: %s\n", employee->personal.employeeNumber);
    printf("Status: %s\n", (employee->employment.status == statusRegular) ? "Regular" : "Casual");
    printf("Hours Worked: %d\n", employee->employment.hoursWorked);
    printf("Basic Rate: %.2f\n\n", employee->employment.basicRate);
    
    printf("Choose what to edit:\n");
    printf("1. Name\n");
    printf("2. Employee Number\n");
    printf("3. Employment Status\n");
    printf("4. Hours Worked\n");
    printf("5. Basic Rate\n");
    printf("6. Edit All Fields\n");
    printf("7. Cancel\n");
    printf("\nEnter choice (1-7): ");
    
    char choice = _getch();
    printf("%c\n\n", choice);
    
    switch (choice) {
        case '1': {
            // Edit name
            printf("=== Edit Name ===\n");
            appFormField nameFields[] = {
                { "Enter First Name: ", employee->personal.name.firstName, employeeFirstNameLen, IV_MAX_LEN, {.rangeInt = {.max = employeeFirstNameLen - 1}} },
                { "Enter Middle Name (optional): ", employee->personal.name.middleName, employeeMiddleNameLen, IV_OPTIONAL, {.rangeInt = {.max = employeeMiddleNameLen - 1}} },
                { "Enter Last Name: ", employee->personal.name.lastName, employeeLastNameLen, IV_MAX_LEN, {.rangeInt = {.max = employeeLastNameLen - 1}} }
            };
            appGetValidatedInput(nameFields, 3);
            
            if (!composeEmployeeName(&employee->personal.name)) {
                printf("\n[Error] Invalid name combination. Changes reverted.\n");
                memcpy(&employee->personal.name, &backup.personal.name, sizeof(EmployeeName));
                return -1;
            }
            break;
        }
        case '2': {
            // Edit employee number
            printf("=== Edit Employee Number ===\n");
            appFormField field = { "Enter Employee Number: ", employee->personal.employeeNumber, employeeNumberLen, IV_MAX_LEN, {.rangeInt = {.max = employeeNumberLen - 1}} };
            appGetValidatedInput(&field, 1);
            break;
        }
        case '3': {
            // Edit status
            printf("=== Edit Employment Status ===\n");
            char statusInput[3];
            appFormField field = { "Enter Status (R/C): ", statusInput, 3, IV_CHOICES, {.choices = {.choices = (const char*[]){"R", "C", "r", "c"}, .count = 4}} };
            appGetValidatedInput(&field, 1);
            employee->employment.status = (statusInput[0] == 'R' || statusInput[0] == 'r') ? statusRegular : statusCasual;
            break;
        }
        case '4': {
            // Edit hours worked
            printf("=== Edit Hours Worked ===\n");
            char hoursBuffer[10];
            appFormField field = { "Enter Hours Worked (0-400): ", hoursBuffer, 10, IV_RANGE_INT, {.rangeInt = {.min = 0, .max = 400}} };
            appGetValidatedInput(&field, 1);
            employee->employment.hoursWorked = atoi(hoursBuffer);
            break;
        }
        case '5': {
            // Edit basic rate
            printf("=== Edit Basic Rate ===\n");
            char rateBuffer[20];
            appFormField field = { "Enter Basic Rate (minimum 0.00): ", rateBuffer, 20, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 10000.0}} };
            appGetValidatedInput(&field, 1);
            employee->employment.basicRate = (float)atof(rateBuffer);
            break;
        }
        case '6': {
            // Edit all fields
            printf("=== Edit All Fields ===\n\n");
            
            // Name fields
            printf("--- Name Information ---\n");
            appFormField nameFields[] = {
                { "Enter First Name: ", employee->personal.name.firstName, employeeFirstNameLen, IV_MAX_LEN, {.rangeInt = {.max = employeeFirstNameLen - 1}} },
                { "Enter Middle Name (optional): ", employee->personal.name.middleName, employeeMiddleNameLen, IV_OPTIONAL, {.rangeInt = {.max = employeeMiddleNameLen - 1}} },
                { "Enter Last Name: ", employee->personal.name.lastName, employeeLastNameLen, IV_MAX_LEN, {.rangeInt = {.max = employeeLastNameLen - 1}} }
            };
            appGetValidatedInput(nameFields, 3);
            
            if (!composeEmployeeName(&employee->personal.name)) {
                printf("\n[Error] Invalid name combination. Changes reverted.\n");
                memcpy(&employee->personal.name, &backup.personal.name, sizeof(EmployeeName));
                return -1;
            }
            
            // Employment fields
            printf("\n--- Employment Information ---\n");
            char statusInput[3], hoursBuffer[10], rateBuffer[20];
            appFormField employmentFields[] = {
                { "Enter Employee Number: ", employee->personal.employeeNumber, employeeNumberLen, IV_MAX_LEN, {.rangeInt = {.max = employeeNumberLen - 1}} },
                { "Enter Status (R/C): ", statusInput, 3, IV_CHOICES, {.choices = {.choices = (const char*[]){"R", "C", "r", "c"}, .count = 4}} },
                { "Enter Hours Worked (0-400): ", hoursBuffer, 10, IV_RANGE_INT, {.rangeInt = {.min = 0, .max = 400}} },
                { "Enter Basic Rate (minimum 0.00): ", rateBuffer, 20, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 10000.0}} }
            };
            appGetValidatedInput(employmentFields, 4);
            
            employee->employment.status = (statusInput[0] == 'R' || statusInput[0] == 'r') ? statusRegular : statusCasual;
            employee->employment.hoursWorked = atoi(hoursBuffer);
            employee->employment.basicRate = (float)atof(rateBuffer);
            break;
        }
        case '7':
            printf("Edit cancelled.\n");
            return -1;
        default:
            printf("Invalid choice. Edit cancelled.\n");
            return -1;
    }
    
    printf("\n=== Updated Information ===\n");
    printf("Name: %s\n", employee->personal.name.fullName);
    printf("Employee Number: %s\n", employee->personal.employeeNumber);
    printf("Status: %s\n", (employee->employment.status == statusRegular) ? "Regular" : "Casual");
    printf("Hours Worked: %d\n", employee->employment.hoursWorked);
    printf("Basic Rate: %.2f\n", employee->employment.basicRate);
    
    printf("\nConfirm these changes? (Y/N): ");
    char confirm = _getch();
    printf("%c\n", confirm);
    
    if (confirm != 'Y' && confirm != 'y') {
        printf("Changes cancelled. Reverting to original values.\n");
        memcpy(employee, &backup, sizeof(Employee));
        return -1;
    }

    return 0; // Success
}