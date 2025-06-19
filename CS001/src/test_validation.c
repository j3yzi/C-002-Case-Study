#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/apctxt.h"

int main() {
    printf("Employee Status Validation Test\n");
    printf("===============================\n\n");
    
    const char* statusChoices[] = {"R", "C"};
    
    char employeeStatus[5]; 
    
    appFormField fields[1] = {
        {
            .prompt = "Enter Employee Status (R for Regular, C for Casual): ",
            .buffer = employeeStatus,
            .bufferSize = sizeof(employeeStatus),
            .validationType = IV_CHOICES,
            .validationParams = {
                .choices = {
                    .choices = statusChoices,
                    .count = 2
                }
            }
        }
    };
    
    appGetValidatedInput(fields, 1);
    
    printf("\nValidated Employee Status: %s\n", employeeStatus);
    
    printf("\nValidation test completed successfully!\n");
    
    return 0;
}