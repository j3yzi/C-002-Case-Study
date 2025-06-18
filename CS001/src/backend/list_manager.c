#include "../../include/employee.h"
#include "../../include/appctxt.h"

// Creates and initializes a list of employee nodes (up to MAX_EMPLOYEES)
int createEmployeeList(AppContext** appContext) {
    
    for (int i = 0; i < MAX_EMPLOYEE_CREATION_COUNT; i++) {
        createEmployeeNode(appContext);
    }
    
    (*appContext)->isInitialList = false;

    return 0; // success
}

// Creates and initializes a single employee node
EmployeeNode* createEmployeeNode(AppContext** appContext) {
    EmployeeNode* newEmployee = (EmployeeNode*)malloc(sizeof(EmployeeNode));
        
    if (newEmployee == NULL) {
        perror("Failed to allocate memory for employee");
        return NULL; // memory allocation failure
    }
        
    memset(newEmployee->employeeNumber, 0, EMPLOYEE_NUMBER_LEN);
    memset(newEmployee->employeeName, 0, EMPLOYEE_NAME_LEN);
    
    newEmployee->statusCode = STATUS_REGULAR;
    newEmployee->hoursWorked = 0;
    newEmployee->basicPay = 0;
    newEmployee->basicRate = 0;
    newEmployee->deductions = 0;
    newEmployee->overtimePay = 0;
    newEmployee->netPay = 0;
    newEmployee->next = NULL;

    // Get user input data for this employee
    getEmployeeDataFromUser(&newEmployee, *appContext); // Ipapasa na sa frontend toh
    
    // Add to the linked list
    if ((*appContext)->head == NULL) {
        (*appContext)->head = newEmployee;
        (*appContext)->tail = newEmployee;
    } else {
        (*appContext)->tail->next = newEmployee;
        (*appContext)->tail = newEmployee;
    }
    
    (*appContext)->count++;

    return newEmployee;
}

void editEmployeeNode() {
    /* NOTE TO DEVELOPERS:
     * This function should:
     * 1. Ask for employee ID to edit
     * 2. Find that employee in the list
     * 3. Allow modification of employee data
     * 4. Recalculate any dependent values (like payroll)
     */
}

void deleteEmployeeNode() {
    /* NOTE TO DEVELOPERS:
     * This function should:
     * 1. Ask for employee ID to delete
     * 2. Find that employee in the list
     * 3. Remove the node and reconnect the list properly
     * 4. Free the deleted node's memory
     * 5. Update head/tail pointers if necessary
     */
}

void freeEmployeeList(AppContext** appContext) {
    EmployeeNode* current = (*appContext)->head;
    EmployeeNode* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    // Reset the context to a clean state
    (*appContext)->head = NULL;
    (*appContext)->tail = NULL;
    (*appContext)->count = 0;
    (*appContext)->isInitialList = true;

    /* NOTE TO DEVELOPERS:
     * This function frees all dynamically allocated memory for employee nodes.
     * It should be called before program termination or when reloading data.
     * After this function is called, the list is completely empty.
     */
}

// to track from time to time if ilan na yung laman ng list
int getEmployeeCount() {
    int count = 0;
    
    return count;
    
    /* NOTE TO DEVELOPERS:
     * This function counts all nodes in the linked list.
     * Use it to check if the list has reached MAX_EMPLOYEES.
     */
}
