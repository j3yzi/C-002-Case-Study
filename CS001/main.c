#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/employee.h"

const char* dataFile = "data/record.bin";

int main(void)
{
    // Try to load existing data
    empHead = loadEmployeeDataFromFile(dataFile);
    
    if (empHead == NULL) {
        printf("No previous records found or file error. Starting fresh.\n\n");
    } else {
        printf("Employee records loaded successfully.\n\n");
        
        // Set tail pointer to the last node in the list
        empTail = empHead;
        while (empTail->next != NULL) {
            empTail = empTail->next;
        }
    }

    // Run the main menu loop
    runMenuLoop();
    
    // Clean up before exiting
    freeEmployeeList();
    
    return 0;
}