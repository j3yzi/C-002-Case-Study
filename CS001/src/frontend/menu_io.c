#include "../../include/employee.h"
#include "../../include/appctxt.h"

void printMenu() {
    
    /* NOTE TO DEVELOPERS: This function displays the main menu options. */

    printf("1. Create employee records.\n");
    printf("2. Add an employee.\n");
    printf("3. Display employee records.\n");
    printf("4. Edit employee record..\n");
    printf("5. Search employee.\n");
    printf("6. Export employee records.\n");
    printf("7. Load employee records.\n");
    printf("8. Save employee records.\n");
    printf("9. Exit.\n");
    printf("Enter your choice: ");
}

int runMenuLoop(AppContext* appContext) {
    int choice;
    int running = 1;
    
    while (running) {
        printMenu();

        // (SAMPLE LOGIC)
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                if (appContext->isInitialList != true) {
                    printf("Employee list already exists.\n");
                } else {
                    printf("\nCreating employee records...\n"); 
                    createEmployeeList(&appContext);
                }
                break;
            case 2:
                if (appContext->isInitialList != true) {
                    createEmployeeNode(&appContext);
                } else {
                    printf("Cannot add an employee, create a list first.\n");
                }
                break;
            case 3:
                printf("\nDisplaying employee records...\n");
                displayEmployeeRecord(appContext);
                break;
            case 7:
                printf("\nLoading employee records...\n");
                // Load the data and assign it to the head pointer
                appContext->head = loadEmployeeDataFromFile(appContext->dataFile);
                if (appContext->head == NULL) {
                    printf("No previous records found or file error.\n");
                } else {
                    printf("Employee records loaded successfully.\n");
                    // Set tail pointer to the last node in the list
                    appContext->tail = appContext->head;
                    while (appContext->tail->next != NULL) {
                        appContext->tail = appContext->tail->next;
                    }
                    // Count the employees
                    appContext->count = 0;
                    EmployeeNode* current = appContext->head;
                    while (current != NULL) {
                        appContext->count++;
                        current = current->next;
                    }
                }
                break;
            case 8:
                printf("\nSaving employee records...\n");
                exportEmployeeDataToFile(appContext->head, appContext->dataFile);
                break;
            case 9:
                printf("\nExiting program...\n");
                running = 0;
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                break;
        }
    }
    
    /* NOTE TO DEVELOPERS:
     * This function is the main control loop for the program.
     * Consider adding input validation.
     */
    
    return 0;
}