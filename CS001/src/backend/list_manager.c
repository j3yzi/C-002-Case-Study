#include "../../include/employee.h"

// Creates and initializes a list of employee nodes (up to MAX_EMPLOYEES)
int createEmployeeList() {
    for (int i = 0; i < MAX_EMPLOYEES; i++) {
        EmployeeNode* newEmployee = (EmployeeNode*)malloc(sizeof(EmployeeNode));
        if (newEmployee == NULL) {
            perror("Failed to allocate memory for employee");
            return -1; // memory allocation failure
        }
        
        // memset(newEmployee->employeeNumber, 0, EMPLOYEE_NUMBER_LEN);
        // memset(newEmployee->employeeName, 0, EMPLOYEE_NAME_LEN);

        
        // ! DAPAT MAYROONG CODE HERE NA NAGI-INITIALIZE MUNA NG DATA NUNG STRUCT ! 

        // Get user input data for this employee
        getEmployeeDataFromUser(&newEmployee); // Ipapasa na sa frontend toh
        
        // Add to the linked list
        if (empHead == NULL) {
            empHead = newEmployee;
            empTail = newEmployee;
        } else {
            empTail->next = newEmployee;
            empTail = newEmployee;
        }
    }

    return 0; // success
    
    /* NOTE TO DEVELOPERS:
     * This function creates MAX_EMPLOYEES number of nodes.
     * Each node is initialized and then filled with user data.
     * The nodes are linked together in a singly-linked list.
     */
}

// Creates and initializes a single employee node
EmployeeNode* createEmployeeNode() {
    EmployeeNode* newEmployee = (EmployeeNode*)malloc(sizeof(EmployeeNode));
    if (newEmployee == NULL) {
        perror("Failed to allocate memory for employee");
        return NULL;
    }
    
    /* NOTE TO DEVELOPERS:
     * This function creates a single employee node with default values.
     * After creation, you should populate it with actual employee data.
     * Don't forget to link it to the list if needed.
     */
    
    return newEmployee;
}

// Ito yung single node lang ia-add pero dapat may laman nang lima yung list
void addEmployeeNode() {
    // Create a new employee node
    EmployeeNode* newEmployee = createEmployeeNode();
    if (newEmployee == NULL) {
        return;  // Failed to create node
    }
    
    // Get user input for the new employee
    getEmployeeDataFromUser(&newEmployee);
    
    // Add to the linked list
    if (empHead == NULL) {
        // Empty list case
        empHead = newEmployee;
        empTail = newEmployee;
    } else {
        // Add to the end of the list
        empTail->next = newEmployee;
        empTail = newEmployee;
    }
    
    /* NOTE TO DEVELOPERS:
     * This function adds a new employee to the end of the list.
     * Make sure to update the tail pointer correctly.
     * Consider adding a check for MAX_EMPLOYEES limit if needed.
     */
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

void freeEmployeeList() {
    
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
