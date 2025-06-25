#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "empio.h"
#include "../../include/headers/apctxt.h"
#include "../../include/models/employee.h"

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
            { "Enter First Name: ", newEmployee->personal.name.firstName, employeeFirstNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeFirstNameLen - 1}} },
            { "Enter Middle Name (optional): ", newEmployee->personal.name.middleName, employeeMiddleNameLen, IV_OPTIONAL_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeMiddleNameLen - 1}} },
            { "Enter Last Name: ", newEmployee->personal.name.lastName, employeeLastNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeLastNameLen - 1}} }
        };
        appGetValidatedInput(nameFields, 3);        isNameValid = composeEmployeeName(&newEmployee->personal.name);
        if (!isNameValid) {
            printf("\n[Error] Name is too long to format properly. First and last names must each be\n");
            printf("        less than %d characters to fit in the %d character full name format.\n", employeeNameLen - 5, employeeNameLen);
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
    
    if (!appYesNoPrompt("Confirm this information?")) {
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
    fflush(stdout);
    
    if (fgets(buffer, bufferSize, stdin) == NULL) {
        return -1; // Handle EOF or read error
    }

    // Remove newline character if present
    char* newline = strchr(buffer, '\n');
    if (newline) {
        *newline = '\0';
    } else {
        // Clear any remaining input from the buffer if it was too long
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    return 0;
}

/**
 * @brief Prompts the user to search for an employee and displays results.
 * @param employeeList Pointer to the employee list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleSearchEmployee(const list* employeeList) {
    winTermClearScreen();
    
    Menu searchMenu = {1, "Search Employee", (MenuOption[]){
        {'1', "Search by Employee Number", "Find employee using their ID number", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Search by Name", "Find employee by first or last name", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Display All Employees", "Show complete list of all employees", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Back to Main Menu", "Return to the employee management menu", false, false, 9, 0, 7, 0, 8, 0, NULL}
    }, 4};
    
    char choice = initMenu(&searchMenu);
    
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
            appFormField field = { "Enter Full Name: ", fullName, employeeNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeNameLen - 1}} };
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

    while (1) {
        winTermClearScreen();
        printf("=== Delete Employee ===\n\n");
        printf("Enter the employee number to delete (or type 'back' to cancel):\n");
        
        char empNumber[employeeNumberLen];
        getEmployeeNumberFromUser(empNumber, employeeNumberLen);

        if (strcmp(empNumber, "back") == 0) {
            printf("\nOperation Cancelled.\n");
            printf("Press any key to continue...");
            _getch();
            return 0;
        }
        
        Employee* emp = searchEmployeeByNumber(employeeList, empNumber);
        if (emp) {
            printf("\n=== Employee to Delete ===\n");
            displayEmployeeDetails(emp);
            
            printf("\n⚠️  WARNING: This action cannot be undone!\n");
            if (appYesNoPrompt("Are you sure you want to delete this employee?")) {
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
            return 0; // Exit loop
        } else {
            printf("\n❌ Employee with number '%s' was not found.\n", empNumber);
            printf("\nWhat would you like to do?\n");
            printf("1. Try again\n");
            printf("2. View all employees\n");
            printf("3. Back to Employee Menu\n");
            printf("\nSelect an option (1-3): ");
            
            char choice = _getch();
            printf("%c\n", choice);
            
            switch (choice) {
                case '1':
                    continue; // Loop to try again
                case '2':
                    winTermClearScreen();
                    printf("=== All Employees ===\n\n");
                    displayAllEmployees(employeeList);
                    printf("\nPress any key to continue...");
                    _getch();
                    continue; // Loop back to delete prompt
                case '3':
                default:
                    return 0; // Exit to employee menu
            }
        }
    }
}

/**
 * @brief Prompts the user for employee data updates using appFormField validation.
 * @param employee A pointer to the employee to edit (contains current values).
 * @return Returns 0 on success, -1 on failure (e.g., invalid name).
 */
int editEmployeeDataFromUser(Employee* employee) {
    Employee backup;
    memcpy(&backup, employee, sizeof(Employee)); // Backup original data

    // Create a menu for editing options
    char menuTitle[100];
    sprintf(menuTitle, "Edit Employee: %s", employee->personal.employeeNumber);
    
    winTermClearScreen();
    printf("====================================\n");
    printf("%s\n", menuTitle);
    printf("====================================\n\n");
    
    printf("Current Employee Information:\n");
    printf("Name: %s\n", employee->personal.name.fullName);
    printf("  First Name: %s\n", employee->personal.name.firstName);
    printf("  Middle Name: %s\n", employee->personal.name.middleName);
    printf("  Last Name: %s\n", employee->personal.name.lastName);
    printf("Employee Number: %s\n", employee->personal.employeeNumber);
    printf("Status: %s\n", (employee->employment.status == statusRegular) ? "Regular" : "Casual");
    printf("Hours Worked: %d\n", employee->employment.hoursWorked);
    printf("Basic Rate: %.2f\n\n", employee->employment.basicRate);
    
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf(">> Edit Name\n");
    printf(" Edit Employee Number\n");
    printf(" Edit Employment Status\n");
    printf(" Edit Hours Worked\n");
    printf(" Edit Basic Rate\n");
    printf(" Edit All Fields\n");
    printf(" Cancel\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\nSelect an option (1-7): ");
    
    char choice = _getch();
    printf("%c\n", choice);
    
    switch (choice) {
        case '1': {
            // Edit name
            printf("=== Edit Name ===\n");
            char firstNamePrompt[128], middleNamePrompt[128], lastNamePrompt[128];
            sprintf(firstNamePrompt, "Enter First Name [%s]: ", employee->personal.name.firstName);
            sprintf(middleNamePrompt, "Enter Middle Name (optional) [%s]: ", employee->personal.name.middleName);
            sprintf(lastNamePrompt, "Enter Last Name [%s]: ", employee->personal.name.lastName);
            
            appFormField nameFields[] = {
                { firstNamePrompt, employee->personal.name.firstName, employeeFirstNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeFirstNameLen - 1}} },
                { middleNamePrompt, employee->personal.name.middleName, employeeMiddleNameLen, IV_OPTIONAL_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeMiddleNameLen - 1}} },
                { lastNamePrompt, employee->personal.name.lastName, employeeLastNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeLastNameLen - 1}} }
            };appGetValidatedInput(nameFields, 3);
            
            if (!composeEmployeeName(&employee->personal.name)) {
                printf("\n[Error] Name is too long to format properly. First and last names must each be\n");
                printf("        less than %d characters to fit in the %d character full name format. Changes reverted.\n", employeeNameLen - 5, employeeNameLen);
                memcpy(&employee->personal.name, &backup.personal.name, sizeof(EmployeeName));
                return -1;
            }
            break;
        }
        case '2': {
            // Edit employee number
            printf("=== Edit Employee Number ===\n");
            char numberPrompt[64];
            sprintf(numberPrompt, "Enter Employee Number [%s]: ", employee->personal.employeeNumber);
            appFormField field = { numberPrompt, employee->personal.employeeNumber, employeeNumberLen, IV_MAX_LEN, {.rangeInt = {.max = employeeNumberLen - 1}} };
            appGetValidatedInput(&field, 1);
            break;
        }
        case '3': {
            // Edit status
            printf("=== Edit Employment Status ===\n");
            char statusInput[3];
            char statusPrompt[64];
            sprintf(statusPrompt, "Enter Status (R/C) [%c]: ", 
                   employee->employment.status == statusRegular ? 'R' : 'C');
            
            statusInput[0] = (employee->employment.status == statusRegular) ? 'R' : 'C';
            statusInput[1] = '\0';
            
            appFormField field = { statusPrompt, statusInput, 3, IV_CHOICES, {.choices = {.choices = (const char*[]){"R", "C", "r", "c"}, .count = 4}} };
            appGetValidatedInput(&field, 1);
            employee->employment.status = (statusInput[0] == 'R' || statusInput[0] == 'r') ? statusRegular : statusCasual;
            break;
        }
        case '4': {
            // Edit hours worked
            printf("=== Edit Hours Worked ===\n");
            char hoursBuffer[10];
            char hoursPrompt[64];
            
            sprintf(hoursPrompt, "Enter Hours Worked (0-400) [%d]: ", 
                   employee->employment.hoursWorked);
            sprintf(hoursBuffer, "%d", employee->employment.hoursWorked);
            
            appFormField field = { hoursPrompt, hoursBuffer, 10, IV_RANGE_INT, {.rangeInt = {.min = 0, .max = 400}} };
            appGetValidatedInput(&field, 1);
            employee->employment.hoursWorked = atoi(hoursBuffer);
            break;
        }
        case '5': {
            // Edit basic rate
            printf("=== Edit Basic Rate ===\n");
            char rateBuffer[20];
            char ratePrompt[64];
            
            sprintf(ratePrompt, "Enter Basic Rate (minimum 0.00) [%.2f]: ", 
                   employee->employment.basicRate);
            sprintf(rateBuffer, "%.2f", employee->employment.basicRate);
            
            appFormField field = { ratePrompt, rateBuffer, 20, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 10000.0}} };
            appGetValidatedInput(&field, 1);
            employee->employment.basicRate = (float)atof(rateBuffer);
            break;
        }
        case '6': {
            // Edit all fields
            printf("=== Edit All Fields ===\n\n");
            // Name fields
            printf("--- Name Information ---\n");
            char firstNamePrompt[128], middleNamePrompt[128], lastNamePrompt[128];
            sprintf(firstNamePrompt, "Enter First Name [%s]: ", employee->personal.name.firstName);
            sprintf(middleNamePrompt, "Enter Middle Name (optional) [%s]: ", employee->personal.name.middleName);
            sprintf(lastNamePrompt, "Enter Last Name [%s]: ", employee->personal.name.lastName);
            
            appFormField nameFields[] = {
                { firstNamePrompt, employee->personal.name.firstName, employeeFirstNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeFirstNameLen - 1}} },
                { middleNamePrompt, employee->personal.name.middleName, employeeMiddleNameLen, IV_OPTIONAL_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeMiddleNameLen - 1}} },
                { lastNamePrompt, employee->personal.name.lastName, employeeLastNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeLastNameLen - 1}} }
            };appGetValidatedInput(nameFields, 3);
            
            if (!composeEmployeeName(&employee->personal.name)) {
                printf("\n[Error] Name is too long to format properly. First and last names must each be\n");
                printf("        less than %d characters to fit in the %d character full name format. Changes reverted.\n", employeeNameLen - 5, employeeNameLen);
                memcpy(&employee->personal.name, &backup.personal.name, sizeof(EmployeeName));
                return -1;
            }
            
            // Employment fields
            printf("\n--- Employment Information ---\n");
            char numberPrompt[64], statusPrompt[64], hoursPrompt[64], ratePrompt[64];
            char statusInput[3], hoursBuffer[10], rateBuffer[20];
            
            sprintf(numberPrompt, "Enter Employee Number [%s]: ", employee->personal.employeeNumber);
            sprintf(statusPrompt, "Enter Status (R/C) [%c]: ", 
                   employee->employment.status == statusRegular ? 'R' : 'C');
            sprintf(hoursPrompt, "Enter Hours Worked (0-400) [%d]: ", 
                   employee->employment.hoursWorked);
            sprintf(ratePrompt, "Enter Basic Rate (minimum 0.00) [%.2f]: ", 
                   employee->employment.basicRate);
            
            // Initialize buffers with current values
            sprintf(hoursBuffer, "%d", employee->employment.hoursWorked);
            sprintf(rateBuffer, "%.2f", employee->employment.basicRate);
            statusInput[0] = (employee->employment.status == statusRegular) ? 'R' : 'C';
            statusInput[1] = '\0';
            
            appFormField employmentFields[] = {
                { numberPrompt, employee->personal.employeeNumber, employeeNumberLen, IV_MAX_LEN, {.rangeInt = {.max = employeeNumberLen - 1}} },
                { statusPrompt, statusInput, 3, IV_CHOICES, {.choices = {.choices = (const char*[]){"R", "C", "r", "c"}, .count = 4}} },
                { hoursPrompt, hoursBuffer, 10, IV_RANGE_INT, {.rangeInt = {.min = 0, .max = 400}} },
                { ratePrompt, rateBuffer, 20, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 10000.0}} }
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
    
    if (!appYesNoPrompt("Confirm these changes?")) {
        printf("Changes cancelled. Reverting to original values.\n");
        memcpy(employee, &backup, sizeof(Employee));
        return -1;
    }

    return 0; // Success
} 