#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "empio.h"
#include "../modules/payroll.h"
#include "../../include/headers/apctxt.h"
#include "../../include/headers/apclrs.h"
#include "../../include/headers/interface.h"
#include "../../include/models/employee.h"

/**
 * @brief Helper function to run a menu with the new interface system
 * @param menu Pointer to the menu to display
 * @return Returns the selected menu option character
 */
static char runMenuWithInterface(Menu* menu) {
    int selected = 0;
    int key;
    
    // Find first non-disabled option
    while (selected < menu->optionCount && menu->options[selected].isDisabled) {
        selected++;
    }
    if (selected >= menu->optionCount) selected = 0;
    
    while (1) {
        // Clear screen and display the menu
        winTermClearScreen();
        displayMenu(menu, selected);
        
        // Get user input
        key = _getch();
        
        // Handle special keys (arrow keys)
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72) { // Up arrow
                int oldSelected = selected;
                do {
                    selected--;
                    if (selected < 0) selected = menu->optionCount - 1;
                } while (menu->options[selected].isDisabled && selected != oldSelected);
            }
            else if (key == 80) { // Down arrow
                int oldSelected = selected;
                do {
                    selected++;
                    if (selected >= menu->optionCount) selected = 0;
                } while (menu->options[selected].isDisabled && selected != oldSelected);
            }
        }
        else if (key == 13) { // Enter key
            if (!menu->options[selected].isDisabled) {
                return menu->options[selected].key;
            }
        }
        else if (key == 27) { // Escape key
            return menu->options[menu->optionCount - 1].key; // Return last option (usually Exit/Back)
        }
        else {
            // Check if key matches any menu option
            for (int i = 0; i < menu->optionCount; i++) {
                if (key == menu->options[i].key && !menu->options[i].isDisabled) {
                    return menu->options[i].key;
                }
            }
        }
    }
}

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
        printf("%s", UI_HEADER);
        printf("╔═══════════════════════════════════════════════════════════════════╗\n");
        printf("║                  %sEnter Employee Name%s                   ║\n", TXT_BOLD, TXT_RESET UI_HEADER);
        printf("╚═══════════════════════════════════════════════════════════════════╝\n");
        printf("%s\n", TXT_RESET);
        
        appFormField nameFields[] = {
            { "👤 Enter First Name: ", newEmployee->personal.name.firstName, employeeFirstNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeFirstNameLen - 1}} },
            { "👥 Enter Middle Name (optional): ", newEmployee->personal.name.middleName, employeeMiddleNameLen, IV_OPTIONAL_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeMiddleNameLen - 1}} },
            { "👤 Enter Last Name: ", newEmployee->personal.name.lastName, employeeLastNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeLastNameLen - 1}} }
        };
        appGetValidatedInput(nameFields, 3);        
        
        isNameValid = composeEmployeeName(&newEmployee->personal.name);
        if (!isNameValid) {
            printf("\n%s[Error] Name is too long to format properly.%s\n", UI_WARNING, TXT_RESET);
            printf("%sFirst and last names must each be less than %d characters to fit in the %d character full name format.%s\n", 
                   UI_WARNING, employeeNameLen - 5, employeeNameLen, TXT_RESET);
            printf("\n%sPress any key to try again, or ESC to cancel...%s", UI_PROMPT, TXT_RESET);
            if (_getch() == 27) return -1; // ESC key
        }
    }

    winTermClearScreen();
    printf("%s", UI_HEADER);
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║               %sEnter Employment Details%s                ║\n", TXT_BOLD, TXT_RESET UI_HEADER);
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    printf("%s\n", TXT_RESET);
    
    char genderInput[3];
    char statusInput[3], hoursBuffer[4], basicRateBuffer[10];
    
    appFormField employmentFields[] = {
        { "🆔 Enter Employee Number (exactly 10 digits): ", newEmployee->personal.employeeNumber, employeeNumberLen, IV_EXACT_LEN, {.rangeInt = {.max = 10}} },
        { "💼 Enter Employment Status (R for Regular, C for Casual): ", statusInput, 3, IV_CHOICES, {.choices = {.choices = (const char*[]){"R", "C", "r", "c"}, .count = 4}} },
        { "⏰ Enter Hours Worked (0-744): ", hoursBuffer, 4, IV_RANGE_INT, {.rangeInt = {.min = 0, .max = 744}} },
        { "💰 Enter Basic Rate (per hour): ", basicRateBuffer, 10, IV_RANGE_FLT, {.rangeFloat = {.min = 0.01, .max = 999.99}} }
    };
    appGetValidatedInput(employmentFields, 4);

    // Convert string inputs to appropriate data types
    newEmployee->employment.status = (statusInput[0] == 'R' || statusInput[0] == 'r') ? statusRegular : statusCasual;
    newEmployee->employment.hoursWorked = atoi(hoursBuffer);
    newEmployee->employment.basicRate = (float)atof(basicRateBuffer);

    printf("\n%s", UI_INFO);
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║              %s💼 Employee Information Summary 💼%s              ║\n", TXT_BOLD, TXT_RESET UI_INFO);
    printf("╠═══════════════════════════════════════════════════════════════════╣\n");
    
    // Define box width (should match the border width)
    int boxWidth = 69; // Total width including borders "╔" and "╗"
    int borderSpace = 2; // Space taken by "║ " and " ║"
    int availableSpace = boxWidth - borderSpace;
    
    // Employee Name row
    char nameLabel[] = "👤 Name:";
    // When calculating content length, we need the visible length only (no ANSI codes)
    int labelVisibleLen = strlen(nameLabel);
    int nameVisibleLen = strlen(newEmployee->personal.name.fullName);
    int nameContentLen = labelVisibleLen + 1 + nameVisibleLen; // +1 for space after label
    int namePadding = availableSpace - nameContentLen;
    if (namePadding < 0) namePadding = 0;
    
    printf("║ %s%s%s %s", UI_HIGHLIGHT, nameLabel, TXT_RESET, newEmployee->personal.name.fullName);
    for (int i = 0; i < namePadding; i++) printf(" ");
    printf(" ║\n");
    
    // Employee Number row
    char empNumLabel[] = "🆔 Employee Number:";
    int empNumLabelLen = strlen(empNumLabel);
    int empNumValueLen = strlen(newEmployee->personal.employeeNumber);
    int empNumContentLen = empNumLabelLen + 1 + empNumValueLen;
    int empNumPadding = availableSpace - empNumContentLen;
    if (empNumPadding < 0) empNumPadding = 0;
    
    printf("║ %s%s%s %s", UI_HIGHLIGHT, empNumLabel, TXT_RESET, newEmployee->personal.employeeNumber);
    for (int i = 0; i < empNumPadding; i++) printf(" ");
    printf(" ║\n");
    
    // Status row
    char statusLabel[] = "💼 Status:";
    const char* statusText = (newEmployee->employment.status == statusRegular) ? "Regular" : "Casual";
    int statusLabelLen = strlen(statusLabel);
    int statusValueLen = strlen(statusText);
    int statusContentLen = statusLabelLen + 1 + statusValueLen;
    int statusPadding = availableSpace - statusContentLen;
    if (statusPadding < 0) statusPadding = 0;
    
    printf("║ %s%s%s %s", UI_HIGHLIGHT, statusLabel, TXT_RESET, statusText);
    for (int i = 0; i < statusPadding; i++) printf(" ");
    printf(" ║\n");
    
    // Hours Worked row
    char hoursLabel[] = "⏰ Hours Worked:";
    char hoursText[20];
    sprintf(hoursText, "%d", newEmployee->employment.hoursWorked);
    int hoursLabelLen = strlen(hoursLabel);
    int hoursValueLen = strlen(hoursText);
    int hoursContentLen = hoursLabelLen + 1 + hoursValueLen;
    int hoursPadding = availableSpace - hoursContentLen;
    if (hoursPadding < 0) hoursPadding = 0;
    
    printf("║ %s%s%s %s", UI_HIGHLIGHT, hoursLabel, TXT_RESET, hoursText);
    for (int i = 0; i < hoursPadding; i++) printf(" ");
    printf(" ║\n");
    
    // Basic Rate row
    char rateLabel[] = "💰 Basic Rate:";
    char rateText[30];
    sprintf(rateText, "$%.2f", newEmployee->employment.basicRate);
    int rateLabelLen = strlen(rateLabel);
    int rateValueLen = strlen(rateText);
    int rateContentLen = rateLabelLen + 1 + rateValueLen;
    int ratePadding = availableSpace - rateContentLen;
    if (ratePadding < 0) ratePadding = 0;
    
    printf("║ %s%s%s %s", UI_HIGHLIGHT, rateLabel, TXT_RESET, rateText);
    for (int i = 0; i < ratePadding; i++) printf(" ");
    printf(" ║\n");
    
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    printf("%s\n", TXT_RESET);
    
    if (!appYesNoPrompt("✅ Confirm this information?")) {
        printf("%s🚫 Employee creation cancelled.%s\n", UI_WARNING, TXT_RESET);
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
int getEmployeeNumberFromUser(char* buffer, const int bufferSize) {
    // Clear the buffer first
    memset(buffer, 0, bufferSize);
    
    // Use the proper validation system
    appFormField field = { 
        "Enter Employee Number: ", 
        buffer, 
        bufferSize, 
        IV_MAX_LEN, 
        {.rangeInt = {.max = bufferSize - 1}} 
    };
    
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
    
    Menu searchMenu = {1, "Search Employee", (MenuOption[]){
        {'1', "Search by Employee Number", "Find employee using their ID number", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Search by Name", "Find employee by first or last name", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Display All Employees", "Show complete list of all employees", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Back to Main Menu", "Return to the employee management menu", false, false, 9, 0, 7, 0, 8, 0, NULL}
    }, 4};
    
    char choice = runMenuWithInterface(&searchMenu);
    
    switch (choice) {
        case '1': {
            int shouldContinue = 1;
            
            do {
                winTermClearScreen();
                char empNumber[employeeNumberLen];
                printf("=== Search by Employee Number ===\n");
                if (getEmployeeNumberFromUser(empNumber, employeeNumberLen) == 0) {
                    Employee* emp = searchEmployeeByNumber(employeeList, empNumber);
                    if (emp) {
                        printf("\n=== Employee Found ===\n");
                        displayEmployeeDetails(emp);
                        shouldContinue = 0;
                    } else {
                        printf("\n❌ Employee with number '%s' not found.\n", empNumber);
                        printf("\nWhat would you like to do?\n");
                        printf("1. Try again with a different number\n");
                        printf("2. View all employees\n");
                        printf("3. Back to Search Menu\n");
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
                                continue; // Loop back to search prompt
                            case '3':
                            default:
                                shouldContinue = 0; // Exit to search menu
                                break;
                        }
                    }
                } else {
                    shouldContinue = 0;
                }
            } while (shouldContinue);
            break;
        }
        case '2': {
            int shouldContinue = 1;
            
            do {
                winTermClearScreen();
                char fullName[employeeNameLen];
                printf("=== Search by Name ===\n");
                appFormField field = { "Enter Full Name: ", fullName, employeeNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeNameLen - 1}} };
                appGetValidatedInput(&field, 1);
                
                Employee* emp = searchEmployeeByName(employeeList, fullName);
                if (emp) {
                    printf("\n=== Employee Found ===\n");
                    displayEmployeeDetails(emp);
                    shouldContinue = 0;
                } else {
                    printf("\n❌ Employee with name '%s' not found.\n", fullName);
                    printf("\nWhat would you like to do?\n");
                    printf("1. Try again with a different name\n");
                    printf("2. View all employees\n");
                    printf("3. Back to Search Menu\n");
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
                            continue; // Loop back to search prompt
                        case '3':
                        default:
                            shouldContinue = 0; // Exit to search menu
                            break;
                    }
                }
            } while (shouldContinue);
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
    if (!employeeList || employeeList->size == 0) {
        winTermClearScreen();
        printf("=== Edit Employee ===\n\n");
        printf("No employees available to edit.\n");
        printf("Please add some employees first.\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    int shouldContinue = 1;
    Employee* existingEmp = NULL;
    
    do {
        winTermClearScreen();
        printf("=== Edit Employee ===\n\n");
        printf("Current employees in the list:\n");
        printf("═══════════════════════════════════════════════════════════════════\n");
        displayAllEmployees(employeeList);
        printf("═══════════════════════════════════════════════════════════════════\n\n");
        
        char empNumber[employeeNumberLen];
        printf("Which employee would you like to edit?\n");
        if (getEmployeeNumberFromUser(empNumber, employeeNumberLen) != 0) {
            return -1;
        }
        
        existingEmp = searchEmployeeByNumber(employeeList, empNumber);
        if (!existingEmp) {
            printf("\n❌ Employee with number '%s' not found.\n", empNumber);
            printf("\nWhat would you like to do?\n");
            printf("1. Try again with a different employee number\n");
            printf("2. View all employees again\n");
            printf("3. Back to Employee Menu\n");
            printf("\nSelect an option (1-3): ");
            
            char choice = _getch();
            printf("%c\n", choice);
            
            switch (choice) {
                case '1':
                    continue; // Loop to try again
                case '2':
                    continue; // Loop back to show employees and try again
                case '3':
                default:
                    return -1; // Exit to employee menu
            }
        } else {
            shouldContinue = 0; // Found employee, exit loop
        }
    } while (shouldContinue);
    
    winTermClearScreen();
    printf("=== Edit Employee: %s ===\n\n", existingEmp->personal.employeeNumber);
    printf("Current Employee Information:\n");
    printf("═══════════════════════════════════════════════════════════════════\n");
    displayEmployeeDetails(existingEmp);
    printf("═══════════════════════════════════════════════════════════════════\n\n");
    
    Employee newData;
    memcpy(&newData, existingEmp, sizeof(Employee)); // Start with current data
    
    if (editEmployeeDataFromUser(&newData) == 0) {
        // Recalculate payroll with new data
        calculatePayroll(&newData);
        
        if (updateEmployeeData(existingEmp, &newData) == 0) {
            winTermClearScreen();
            printf("=== Employee Update Successful ===\n\n");
            printf("✅ Employee '%s' has been updated successfully!\n\n", existingEmp->personal.employeeNumber);
            printf("Updated Employee Information:\n");
            printf("═══════════════════════════════════════════════════════════════════\n");
            displayEmployeeDetails(existingEmp);
            printf("═══════════════════════════════════════════════════════════════════\n");
        } else {
            printf("❌ Failed to update employee data.\n");
            printf("Please try again or contact system administrator.\n");
        }
    } else {
        printf("🚫 Edit operation cancelled.\n");
        printf("No changes were made to the employee record.\n");
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

    // Loop variables
    int shouldContinue = 1;
    int operationSuccess = 0;
    
    do {
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
        
        const Employee* emp = searchEmployeeByNumber(employeeList, empNumber);
        if (emp) {
            printf("\n=== Employee to Delete ===\n");
            displayEmployeeDetails(emp);
            
            printf("\n⚠️  WARNING: This action cannot be undone!\n");
            if (appYesNoPrompt("Are you sure you want to delete this employee?")) {
                printf("\nDeleting employee...\n");
                if (removeEmployeeFromList(employeeList, empNumber) == 0) {
                    printf("✅ Employee '%s' deleted successfully!\n", empNumber);
                    printf("Employee count is now: %d\n", employeeList->size);
                    operationSuccess = 1;
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
            shouldContinue = 0; // Exit loop
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
                    shouldContinue = 1; // Continue the loop
                    break;
                case '2':
                    winTermClearScreen();
                    printf("=== All Employees ===\n\n");
                    displayAllEmployees(employeeList);
                    printf("\nPress any key to continue...");
                    _getch();
                    shouldContinue = 1; // Continue the loop
                    break;
                case '3':
                default:
                    shouldContinue = 0; // Exit the loop
                    break;
            }
        }
    } while (shouldContinue);
    
    return operationSuccess ? 0 : -1;
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
    
    // Display current employee information first
    winTermClearScreen();
    printf("═══════════════════════════════════════════════════════════════════\n");
    printf("                    CURRENT EMPLOYEE INFORMATION                   \n");
    printf("═══════════════════════════════════════════════════════════════════\n");
    printf("Name: %s\n", employee->personal.name.fullName);
    printf("  • First Name: %s\n", employee->personal.name.firstName);
    printf("  • Middle Name: %s\n", employee->personal.name.middleName);
    printf("  • Last Name: %s\n", employee->personal.name.lastName);
    printf("Employee Number: %s\n", employee->personal.employeeNumber);
    printf("Status: %s\n", (employee->employment.status == statusRegular) ? "Regular" : "Casual");
    printf("Hours Worked: %d hours\n", employee->employment.hoursWorked);
    printf("Basic Rate: ₱%.2f per hour\n", employee->employment.basicRate);
    printf("═══════════════════════════════════════════════════════════════════\n\n");
    printf("Select what you would like to edit:\n\n");
    
    // Create the menu with proper interface
    Menu editMenu = {1, menuTitle, (MenuOption[]){
        {'1', "Edit Name", "Modify employee's first, middle, or last name", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Edit Employee Number", "Change the employee identification number", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Edit Employment Status", "Switch between Regular and Casual status", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Edit Hours Worked", "Update the number of hours worked", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'5', "Edit Basic Rate", "Modify the hourly pay rate", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'6', "Edit All Fields", "Update all employee information at once", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'7', "Cancel", "Return without making any changes", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 7
    };
    
    char choice = runMenuWithInterface(&editMenu);
    
    switch (choice) {
        case '1': {
            // Edit name - with centered display
            winTermClearScreen();
            
            // Get console size for centering
            int consoleWidth, consoleHeight;
            getConsoleSize(&consoleWidth, &consoleHeight);
            
            // Display centered header
            const char* header = "=== Edit Employee Name ===";
            int headerLen = strlen(header);
            int startX = (consoleWidth - headerLen) / 2;
            int startY = 3;
            
            setCursor(startX, startY);
            printf("%s\n\n", header);
            
            // Center the form within a box
            int boxWidth = 60;
            int boxStartX = (consoleWidth - boxWidth) / 2;
            
            setCursor(boxStartX, startY + 2);
            printf("╔");
            for (int i = 0; i < boxWidth - 2; i++) printf("═");
            printf("╗\n");
            
            setCursor(boxStartX, startY + 3);
            printf("║%*s║\n", boxWidth - 2, "");
            
            char firstNamePrompt[128], middleNamePrompt[128], lastNamePrompt[128];
            sprintf(firstNamePrompt, "Enter First Name [%s]: ", employee->personal.name.firstName);
            sprintf(middleNamePrompt, "Enter Middle Name (optional) [%s]: ", employee->personal.name.middleName);
            sprintf(lastNamePrompt, "Enter Last Name [%s]: ", employee->personal.name.lastName);
            
            appFormField nameFields[] = {
                { firstNamePrompt, employee->personal.name.firstName, employeeFirstNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeFirstNameLen - 1}} },
                { middleNamePrompt, employee->personal.name.middleName, employeeMiddleNameLen, IV_OPTIONAL_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeMiddleNameLen - 1}} },
                { lastNamePrompt, employee->personal.name.lastName, employeeLastNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = employeeLastNameLen - 1}} }
            };
            
            // Position cursor for input
            setCursor(boxStartX + 2, startY + 4);
            appGetValidatedInput(nameFields, 3);
            
            setCursor(boxStartX, startY + 8);
            printf("║%*s║\n", boxWidth - 2, "");
            
            setCursor(boxStartX, startY + 9);
            printf("╚");
            for (int i = 0; i < boxWidth - 2; i++) printf("═");
            printf("╝\n");
            
            if (!composeEmployeeName(&employee->personal.name)) {
                printf("\n[Error] Name is too long to format properly. First and last names must each be\n");
                printf("        less than %d characters to fit in the %d character full name format. Changes reverted.\n", employeeNameLen - 5, employeeNameLen);
                memcpy(&employee->personal.name, &backup.personal.name, sizeof(EmployeeName));
                return -1;
            }
            break;
        }
        case '2': {
            // Edit employee number - with centered display
            winTermClearScreen();
            
            // Get console size for centering
            int consoleWidth, consoleHeight;
            getConsoleSize(&consoleWidth, &consoleHeight);
            
            // Display centered header
            const char* header = "=== Edit Employee Number ===";
            int headerLen = strlen(header);
            int startX = (consoleWidth - headerLen) / 2;
            int startY = 5;
            
            setCursor(startX, startY);
            printf("%s\n\n", header);
            
            char numberPrompt[64];
            sprintf(numberPrompt, "Enter Employee Number [%s]: ", employee->personal.employeeNumber);
            appFormField field = { numberPrompt, employee->personal.employeeNumber, employeeNumberLen, IV_EXACT_LEN, {.rangeInt = {.max = 10}} };
            appGetValidatedInput(&field, 1);
            break;
        }
        case '3': {
            // Edit status - with centered display
            winTermClearScreen();
            
            // Get console size for centering
            int consoleWidth, consoleHeight;
            getConsoleSize(&consoleWidth, &consoleHeight);
            
            // Display centered header
            const char* header = "=== Edit Employment Status ===";
            int headerLen = strlen(header);
            int startX = (consoleWidth - headerLen) / 2;
            int startY = 5;
            
            setCursor(startX, startY);
            printf("%s\n\n", header);
            
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
            // Edit hours worked - with centered display
            winTermClearScreen();
            
            // Get console size for centering
            int consoleWidth, consoleHeight;
            getConsoleSize(&consoleWidth, &consoleHeight);
            
            // Display centered header
            const char* header = "=== Edit Hours Worked ===";
            int headerLen = strlen(header);
            int startX = (consoleWidth - headerLen) / 2;
            int startY = 5;
            
            setCursor(startX, startY);
            printf("%s\n\n", header);
            
            char hoursBuffer[10];
            char hoursPrompt[64];
            
            sprintf(hoursPrompt, "Enter Hours Worked (0-744) [%d]: ", 
                   employee->employment.hoursWorked);
            sprintf(hoursBuffer, "%d", employee->employment.hoursWorked);
            
            appFormField field = { hoursPrompt, hoursBuffer, 10, IV_RANGE_INT, {.rangeInt = {.min = 0, .max = 744}} };
            appGetValidatedInput(&field, 1);
            employee->employment.hoursWorked = atoi(hoursBuffer);
            break;
        }
        case '5': {
            // Edit basic rate - with centered display
            winTermClearScreen();
            
            // Get console size for centering
            int consoleWidth, consoleHeight;
            getConsoleSize(&consoleWidth, &consoleHeight);
            
            // Display centered header
            const char* header = "=== Edit Basic Rate ===";
            int headerLen = strlen(header);
            int startX = (consoleWidth - headerLen) / 2;
            int startY = 5;
            
            setCursor(startX, startY);
            printf("%s\n\n", header);
            
            char rateBuffer[20];
            char ratePrompt[64];
            
            sprintf(ratePrompt, "Enter Basic Rate (minimum 0.00) [%.2f]: ", 
                   employee->employment.basicRate);
            sprintf(rateBuffer, "%.2f", employee->employment.basicRate);
            
            appFormField field = { ratePrompt, rateBuffer, 20, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 999.99}} };
            appGetValidatedInput(&field, 1);
            employee->employment.basicRate = (float)atof(rateBuffer);
            break;
        }
        case '6': {
            // Edit all fields - with centered display
            winTermClearScreen();
            
            // Get console size for centering
            int consoleWidth, consoleHeight;
            getConsoleSize(&consoleWidth, &consoleHeight);
            
            // Display centered header
            const char* header = "=== Edit All Employee Fields ===";
            int headerLen = strlen(header);
            int startX = (consoleWidth - headerLen) / 2;
            int startY = 3;
            
            setCursor(startX, startY);
            printf("%s\n\n", header);
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
            sprintf(hoursPrompt, "Enter Hours Worked (0-744) [%d]: ", 
                   employee->employment.hoursWorked);
            sprintf(ratePrompt, "Enter Basic Rate (minimum 0.00) [%.2f]: ", 
                   employee->employment.basicRate);
            
            // Initialize buffers with current values
            sprintf(hoursBuffer, "%d", employee->employment.hoursWorked);
            sprintf(rateBuffer, "%.2f", employee->employment.basicRate);
            statusInput[0] = (employee->employment.status == statusRegular) ? 'R' : 'C';
            statusInput[1] = '\0';
            
            appFormField employmentFields[] = {
                { numberPrompt, employee->personal.employeeNumber, employeeNumberLen, IV_EXACT_LEN, {.rangeInt = {.max = 10}} },
                { statusPrompt, statusInput, 3, IV_CHOICES, {.choices = {.choices = (const char*[]){"R", "C", "r", "c"}, .count = 4}} },
                { hoursPrompt, hoursBuffer, 10, IV_RANGE_INT, {.rangeInt = {.min = 0, .max = 744}} },
                { ratePrompt, rateBuffer, 20, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 999.99}} }
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

// Employee Table View Function

/**
 * @brief Handle the display of employee list in table format
 * Shows employees in a responsive tabular display with pagination
 */
void handleDisplayEmployeeTable() {
    // Check if we have an active employee list and data
    int hasActiveList = (empManager.activeEmployeeList >= 0 && empManager.employeeLists[empManager.activeEmployeeList]);
    if (!checkActiveList(hasActiveList, 0, "No employees available for table view")) {
        return;
    }
    
    list* employeeList = empManager.employeeLists[empManager.activeEmployeeList];
    
    printf("Loading Employee Table View...\n");
    printf("Press any key to continue...");
    _getch();
    
    // Run the interactive table view
    runEmployeeTableView(employeeList);
    
    printf("\nTable view closed.\n");
    printf("Press any key to continue...");
    _getch();
} 