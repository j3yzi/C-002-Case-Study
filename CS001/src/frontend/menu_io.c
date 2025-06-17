#include "../../include/employee.h"

void printMenu() {
    
    /* NOTE TO DEVELOPERS: This function displays the main menu options. */
}

int runMenuLoop() {
    int choice;
    int running = 1;
    const char* dataFile = "data/record.bin";
    
    while (running) {
        printMenu();

        // (SAMPLE LOGIC)
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("\nCreating employee records...\n");
                /* NOTE: Call createEmployeeList() here */
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