#include "employee.h"
#include "../headers/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Creates a new, empty list to store employees.
 * @param employeeList A double pointer to the list structure to be created.
 * @return Returns 0 on success, -1 on failure.
 */
int createEmployeeList(list** employeeList) {
    if (createList(employeeList, SINGLY) != 0) {
        return -1;
    }
    return 0;
}

/**
 * @brief Adds a new employee to the specified list.
 * @param newEmployeeData A pointer to the Employee struct to be added.
 * @param l A double pointer to the list where the employee will be added.
 * @return Returns 0 on success, -1 on failure.
 */
int createEmployee(Employee* newEmployeeData, list** l) {
    if (addNode(l, newEmployeeData) != 0) {
        return -1;
    }
    return 0;
}

/**
 * @brief Deletes an employee and frees the associated memory.
 * @brief This function frees the memory of the Employee struct and sets the original pointer to NULL
 * @brief to prevent dangling pointer issues.
 * @param employee A double pointer to the Employee struct to be deleted.
 */
void deleteEmployee(Employee** employee) {
    if (employee != NULL && *employee != NULL) {
        free(*employee);
        *employee = NULL;
    }
}

/**
 * @brief Frees the memory of an employee's data.
 * @brief This function is designed to be used as a callback (e.g., for destroyList)
 * @brief and takes a void pointer to the employee data.
 * @param employeeData A void pointer to the Employee data to be freed.
 */
void freeEmployee(void* employeeData) {
    if (employeeData != NULL) {
        free(employeeData);
    }
}

/**
 * @brief Composes the employee's display name based on their full name parts.
 * @param name Pointer to the EmployeeName struct.
 * @return true if the name was composed successfully, false otherwise.
 */
int composeEmployeeName(EmployeeName* name) {
    if (!name) {
        return 0;
    }
    
    size_t fnLen = strlen(name->firstName);
    size_t lnLen = strlen(name->lastName);
    
    // Add validation check: If the first name or last name is excessive in length, reject it
    // This prevents accepting extremely long names that would be unreadable when truncated
    if (fnLen > employeeNameLen - 5 || lnLen > employeeNameLen - 5) {
        return 0;  // Names too long, reject
    }

    // Format 1: "Last, First" format (most compact)
    if ((lnLen + fnLen + 3) < employeeNameLen) { // +3 for ", " and null
        snprintf(name->fullName, employeeNameLen, "%.*s, %.*s", 
                (int)lnLen, name->lastName,
                (int)fnLen, name->firstName);
        return 1;
    }
    
    // Format 2: Use initials for first name if full name is too long
    if ((lnLen + 3) < employeeNameLen) { // +3 for ", " and initial and null
        snprintf(name->fullName, employeeNameLen, "%.*s, %c.", 
                (int)lnLen, name->lastName,
                name->firstName[0]);
        return 1;
    }
    
    // Format 3: Truncate last name if it's still too long
    if (employeeNameLen > 5) { // Make sure we have at least a few characters
        snprintf(name->fullName, employeeNameLen, "%.*s...", 
                (int)(employeeNameLen - 4), name->lastName);
        return 1;
    }

    // If everything fails, which shouldn't happen given the constraints above
    return 0;
}

/**
 * @brief Searches for an employee by employee number.
 * @param employeeList Pointer to the employee list.
 * @param employeeNumber The employee number to search for.
 * @return Pointer to the Employee if found, NULL otherwise.
 */
Employee* searchEmployeeByNumber(const list* employeeList, const char* employeeNumber) {
    if (!employeeList || !employeeList->head || !employeeNumber) {
        return NULL;
    }

    node* current = employeeList->head;
    do {
        Employee* emp = (Employee*)current->data;
        if (emp && strcmp(emp->personal.employeeNumber, employeeNumber) == 0) {
            return emp;
        }
        current = current->next;
    } while (current != employeeList->head);

    return NULL;
}

/**
 * @brief Searches for an employee by full name.
 * @param employeeList Pointer to the employee list.
 * @param lastName The last name to search for.
 * @return Pointer to the Employee if found, NULL otherwise.
 */
Employee* searchEmployeeByName(const list* employeeList, const char* lastName) {
    if (!employeeList || !employeeList->head || !lastName) {
        return NULL;
    }

    node* current = employeeList->head;
    do {
        Employee* emp = (Employee*)current->data;
        if (emp && strcmp(emp->personal.name.lastName, lastName) == 0) {
            return emp;
        }
        current = current->next;
    } while (current != employeeList->head);

    return NULL;
}

/**
 * @brief Searches for an employee by number and returns the index.
 * @param employeeList Pointer to the employee list.
 * @param employeeNumber The employee number to search for.
 * @return Index of the employee if found, -1 otherwise.
 */
int searchEmployeeIndexByNumber(const list* employeeList, const char* employeeNumber) {
    if (!employeeList || !employeeList->head || !employeeNumber) {
        return -1;
    }

    node* current = employeeList->head;
    int index = 0;
    do {
        Employee* emp = (Employee*)current->data;
        if (emp && strcmp(emp->personal.employeeNumber, employeeNumber) == 0) {
            return index;
        }
        current = current->next;
        index++;
    } while (current != employeeList->head);

    return -1;
}

/**
 * @brief Updates an existing employee's data with new data.
 * @param employee Pointer to the employee to update.
 * @param newData Pointer to the new employee data.
 * @return 0 on success, -1 on failure.
 */
int updateEmployeeData(Employee* employee, const Employee* newData) {
    if (!employee || !newData) {
        return -1;
    }

    // Copy new data, preserving original payroll calculations
    memcpy(&employee->personal, &newData->personal, sizeof(PersonalInfo));
    memcpy(&employee->employment, &newData->employment, sizeof(EmploymentInfo));
    
    return 0;
}

/**
 * @brief Edits an employee in the list by employee number.
 * @param employeeList Pointer to the employee list.
 * @param employeeNumber The employee number to edit.
 * @param newData Pointer to the new employee data.
 * @return 0 on success, -1 on failure.
 */
int editEmployeeInList(list* employeeList, const char* employeeNumber, const Employee* newData) {
    Employee* employee = searchEmployeeByNumber(employeeList, employeeNumber);
    if (!employee) {
        return -1; // Employee not found
    }

    return updateEmployeeData(employee, newData);
}

/**
 * @brief Removes an employee from the list by employee number.
 * @param employeeList Pointer to the employee list.
 * @param employeeNumber The employee number to remove.
 * @return 0 on success, -1 on failure.
 */
int removeEmployeeFromList(list* employeeList, const char* employeeNumber) {
    if (!employeeList || !employeeList->head || !employeeNumber) {
        return -1;
    }

    Employee* employee = searchEmployeeByNumber(employeeList, employeeNumber);
    if (!employee) {
        return -1; // Employee not found
    }

    // Use the list library's removeNode function
    removeNode(employeeList, employee, freeEmployee);
    return 0;
}

/**
 * @brief Displays detailed information about an employee.
 * @param employee Pointer to the employee to display.
 */
void displayEmployeeDetails(const Employee* employee) {
    if (!employee) {
        printf("No employee data to display.\n");
        return;
    }

    printf("\n=== Employee Details ===\n");
    printf("Employee Number: %s\n", employee->personal.employeeNumber);
    printf("Full Name: %s\n", employee->personal.name.fullName);
    printf("First Name: %s\n", employee->personal.name.firstName);
    printf("Middle Name: %s\n", employee->personal.name.middleName[0] ? employee->personal.name.middleName : "(none)");
    printf("Last Name: %s\n", employee->personal.name.lastName);
    
    printf("\n--- Employment Information ---\n");
    printf("Status: %s\n", (employee->employment.status == statusRegular) ? "Regular" : "Casual");
    printf("Hours Worked: %d\n", employee->employment.hoursWorked);
    printf("Basic Rate: %.2f\n", employee->employment.basicRate);
    
    printf("\n--- Payroll Information ---\n");
    printf("Basic Pay: %.2f\n", employee->payroll.basicPay);
    printf("Overtime Pay: %.2f\n", employee->payroll.overtimePay);
    printf("Deductions: %.2f\n", employee->payroll.deductions);
    printf("Net Pay: %.2f\n", employee->payroll.netPay);
    printf("========================\n\n");
}

/**
 * @brief Displays all employees in a professional payroll report format.
 * @param employeeList Pointer to the employee list.
 */
void displayAllEmployees(const list* employeeList) {
    if (!employeeList) {
        printf("No employee list available.\n");
        return;
    }
    
    if (!employeeList->head || employeeList->size == 0) {
        printf("No employees to display. The list is empty.\n");
        return;
    }

    // Get console dimensions for centering
    extern void getConsoleSize(int* width, int* height);
    int consoleWidth, consoleHeight;
    getConsoleSize(&consoleWidth, &consoleHeight);
    
    // Calculate the width of our payroll report
    int reportWidth = 98; // Total width of our formatted table
    int margin = (consoleWidth - reportWidth) / 2;
    if (margin < 0) margin = 0;

    // Clear screen and display professional header (centered)
    printf("\n");
    
    // Center the university name
    const char* univName = "POLYTECHNIC UNIVERSITY OF THE PHILIPPINES";
    int univNameLen = strlen(univName);
    int univMargin = (consoleWidth - univNameLen) / 2;
    if (univMargin < 0) univMargin = 0;
    printf("%*s%s\n", univMargin, "", univName);
    
    // Center the city name
    const char* cityName = "Quezon City";
    int cityNameLen = strlen(cityName);
    int cityMargin = (consoleWidth - cityNameLen) / 2;
    if (cityMargin < 0) cityMargin = 0;
    printf("%*s%s\n", cityMargin, "", cityName);
    
    printf("\n");
    
    // Center the "Payroll" title
    const char* payrollTitle = "Payroll";
    int payrollTitleLen = strlen(payrollTitle);
    int payrollMargin = (consoleWidth - payrollTitleLen) / 2;
    if (payrollMargin < 0) payrollMargin = 0;
    printf("%*s%s\n", payrollMargin, "", payrollTitle);
    
    printf("\n");

    // Table header with proper formatting (centered)
    printf("%*s%-12s  %-20s  %-8s  %-12s  %-12s  %-12s  %-12s\n", 
           margin, "",
           "Employee", "Employee", "Status", "Basic", "Overtime", "Deductions", "Net");
    printf("%*s%-12s  %-20s  %-8s  %-12s  %-12s  %-12s  %-12s\n", 
           margin, "",
           "Number", "Name", "", "Salary", "Pay", "", "Pay");
    
    // Separator line (centered)
    printf("%*s", margin, "");
    printf("─────────────────────────────────────────────────────────────────────────────────────────────────────\n");

    node* current = employeeList->head;
    int count = 0;
    float totalBasicPay = 0.0f;
    float totalOvertimePay = 0.0f;
    float totalDeductions = 0.0f;
    float totalNetPay = 0.0f;
    
    // For non-circular lists
    if (employeeList->type == SINGLY || employeeList->type == DOUBLY) {
        while (current != NULL) {
            Employee* emp = (Employee*)current->data;
            if (emp) {
                printf("%*s%-12s  %-20s  %-8s  %12.2f  %12.2f  %12.2f  %12.2f\n", 
                       margin, "",
                       emp->personal.employeeNumber,
                       emp->personal.name.fullName,
                       (emp->employment.status == statusRegular) ? "Regular" : "Casual",
                       emp->payroll.basicPay,
                       emp->payroll.overtimePay,
                       emp->payroll.deductions,
                       emp->payroll.netPay);
                
                // Add to totals
                totalBasicPay += emp->payroll.basicPay;
                totalOvertimePay += emp->payroll.overtimePay;
                totalDeductions += emp->payroll.deductions;
                totalNetPay += emp->payroll.netPay;
                count++;
            }
            current = current->next;
        }
    } 
    // For circular lists
    else {
        if (current != NULL) {
            do {
                Employee* emp = (Employee*)current->data;
                if (emp) {
                    printf("%*s%-12s  %-20s  %-8s  %12.2f  %12.2f  %12.2f  %12.2f\n", 
                           margin, "",
                           emp->personal.employeeNumber,
                           emp->personal.name.fullName,
                           (emp->employment.status == statusRegular) ? "Regular" : "Casual",
                           emp->payroll.basicPay,
                           emp->payroll.overtimePay,
                           emp->payroll.deductions,
                           emp->payroll.netPay);
                    
                    // Add to totals
                    totalBasicPay += emp->payroll.basicPay;
                    totalOvertimePay += emp->payroll.overtimePay;
                    totalDeductions += emp->payroll.deductions;
                    totalNetPay += emp->payroll.netPay;
                    count++;
                }
                current = current->next;
                
                // Safety check to prevent infinite loops
                if (count >= employeeList->size) {
                    break;
                }
            } while (current != NULL && current != employeeList->head);
        }
    }

    // Footer separator and totals (centered)
    printf("%*s", margin, "");
    printf("─────────────────────────────────────────────────────────────────────────────────────────────────────\n");
    
    // Display totals if we have employees (centered)
    if (count > 0) {
        printf("%*s%-41s  %12.2f  %12.2f  %12.2f  %12.2f\n", 
               margin, "",
               "TOTALS:", 
               totalBasicPay,
               totalOvertimePay,
               totalDeductions,
               totalNetPay);
        printf("%*s", margin, "");
        printf("─────────────────────────────────────────────────────────────────────────────────────────────────────\n");
    }
    
    // Center the summary
    char summaryText[50];
    snprintf(summaryText, sizeof(summaryText), "Total employees displayed: %d", count);
    int summaryLen = strlen(summaryText);
    int summaryMargin = (consoleWidth - summaryLen) / 2;
    if (summaryMargin < 0) summaryMargin = 0;
    printf("%*s%s\n", summaryMargin, "", summaryText);
    
    if (count == 0) {
        char noteText[100];
        snprintf(noteText, sizeof(noteText), "Note: List has %d entries but no valid employee data found.", employeeList->size);
        int noteLen = strlen(noteText);
        int noteMargin = (consoleWidth - noteLen) / 2;
        if (noteMargin < 0) noteMargin = 0;
        printf("%*s%s\n", noteMargin, "", noteText);
    }
}

/**
 * @brief Compares two employees by employee number.
 * @param emp1 Pointer to the first employee.
 * @param emp2 Pointer to the second employee.
 * @return Negative if emp1 < emp2, 0 if equal, positive if emp1 > emp2.
 */
int compareEmployeeByNumber(const void* emp1, const void* emp2) {
    if (!emp1 || !emp2) {
        return 0;
    }

    const Employee* e1 = (const Employee*)emp1;
    const Employee* e2 = (const Employee*)emp2;
    
    return strcmp(e1->personal.employeeNumber, e2->personal.employeeNumber);
}

/**
 * @brief Compares two employees by full name.
 * @param emp1 Pointer to the first employee.
 * @param emp2 Pointer to the second employee.
 * @return Negative if emp1 < emp2, 0 if equal, positive if emp1 > emp2.
 */
int compareEmployeeByName(const void* emp1, const void* emp2) {
    if (!emp1 || !emp2) {
        return 0;
    }

    const Employee* e1 = (const Employee*)emp1;
    const Employee* e2 = (const Employee*)emp2;
    
    return strcmp(e1->personal.name.fullName, e2->personal.name.fullName);
}