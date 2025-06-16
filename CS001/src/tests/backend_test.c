// ## HOW TO COMPILE ##
// gcc -Wall -Wextra -std=c11 -g backend_test.c ../backend/file_handler.c ../backend/list_manager.c ../backend/payroll_logic.c -I../../include -o backend_test

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "../../include/employee.h"

// Function prototypes for our test functions
EmployeeNode* createRandomEmployee(int id);
void printEmployeeData(EmployeeNode* node, const char* label);
void freeTestEmployeeList(EmployeeNode* head);

EmployeeNode *head = NULL;
EmployeeNode *mid = NULL;
EmployeeNode *tail = NULL;
const char* dataFile = "../../data/record_test.bin";

int main(void)
{    
    srand((unsigned int)time(NULL));
    
    // 1. Create the linked list with random employee data
    printf("--- Creating Employee Linked List ---\n");
    head = createRandomEmployee(1);
    mid = createRandomEmployee(2);
    tail = createRandomEmployee(3);
    
    head->next = mid;
    mid->next = tail;
    tail->next = NULL;
    
    // 2. Print the original employee data
    printf("\n--- Original Employee Data ---\n");
    EmployeeNode* current = head;
    int nodeCount = 0;
    while (current != NULL) {
        char label[20];
        snprintf(label, sizeof(label), "Node %d", ++nodeCount);
        printEmployeeData(current, label);
        current = current->next;
    }
    
    // 3. Save the employee data to a file
    printf("\n--- Saving Employee Data to File ---\n");
    if (!saveEmployeeDataToFile(head, dataFile)) {
        printf("Failed to save employee data to file.\n");
        freeTestEmployeeList(head);
        return 1;
    }
    printf("Employee data successfully saved to %s\n", dataFile);
    
    // 4. Free the list
    printf("\n--- Freeing Original Employee List ---\n");
    freeTestEmployeeList(head);
    head = mid = tail = NULL;
    printf("Original employee list freed from memory\n");
    
    // 5. Load the employee data from the file
    printf("\n--- Loading Employee Data from File ---\n");
    EmployeeNode* loadedHead = loadEmployeeDataFromFile(dataFile);
    if (loadedHead == NULL) {
        printf("Failed to load employee data from file.\n");
        return 1;
    }
    printf("Employee data successfully loaded from %s\n", dataFile);
    
    // 6. Print the loaded employee data
    printf("\n--- Loaded Employee Data ---\n");
    current = loadedHead;
    nodeCount = 0;
    while (current != NULL) {
        char label[20];
        snprintf(label, sizeof(label), "Node %d", ++nodeCount);
        printEmployeeData(current, label);
        current = current->next;
    }
    
    // Free the loaded list
    freeTestEmployeeList(loadedHead);
    
    printf("\nTest completed successfully!\n");
    
    return 0;
}

EmployeeNode* createRandomEmployee(int id) {
    EmployeeNode* employee = (EmployeeNode*)malloc(sizeof(EmployeeNode));
    if (employee == NULL) {
        perror("Failed to allocate memory for employee node");
        exit(1);
    }
    
    snprintf(employee->employeeNumber, EMPLOYEE_NUMBER_LEN, "EMP-%05d", id * 1000 + rand() % 1000);
    
    const char* firstNames[] = {"John", "Jane", "Bob", "Alice", "David", "Sara", "Mike", "Lisa"};
    const char* lastNames[] = {"Smith", "Jones", "Brown", "Davis", "Wilson", "Moore", "Taylor", "Lee"};
    
    int firstIndex = rand() % (sizeof(firstNames) / sizeof(firstNames[0]));
    int lastIndex = rand() % (sizeof(lastNames) / sizeof(lastNames[0]));
    
    snprintf(employee->employeeName, EMPLOYEE_NAME_LEN, "%s %s", 
             firstNames[firstIndex], lastNames[lastIndex]);
    
    employee->statusCode = (rand() % 2 == 0) ? STATUS_REGULAR : STATUS_CASUAL;
    employee->hoursWorked = 20 + (rand() % 41);
    employee->basicRate = 10.0f + ((float)(rand() % 4000) / 100.0f);
    employee->basicPay = employee->hoursWorked * employee->basicRate;
    employee->deductions = 50.0f + ((float)(rand() % 45000) / 100.0f);
    employee->overtimePay = (float)(rand() % 20000) / 100.0f;
    employee->netPay = employee->basicPay + employee->overtimePay - employee->deductions;
    employee->next = NULL;
    
    return employee;
}

void printEmployeeData(EmployeeNode* node, const char* label) {
    printf("\n%s Node:\n", label);
    printf("  Employee Number: %s\n", node->employeeNumber);
    printf("  Employee Name: %s\n", node->employeeName);
    printf("  Status: %s\n", node->statusCode == STATUS_REGULAR ? "Regular" : "Casual");
    printf("  Hours Worked: %d\n", node->hoursWorked);
    printf("  Basic Rate: %.2f\n", node->basicRate);
    printf("  Basic Pay: %.2f\n", node->basicPay);
    printf("  Deductions: %.2f\n", node->deductions);
    printf("  Overtime Pay: %.2f\n", node->overtimePay);
    printf("  Net Pay: %.2f\n", node->netPay);
}

void freeTestEmployeeList(EmployeeNode* head) {
    EmployeeNode* current = head;
    EmployeeNode* next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}