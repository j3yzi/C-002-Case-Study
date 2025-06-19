#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/employee.h"
#include "../include/apctxt.h"


// Initialize configuration
AppConfig config;

int main(void)
{   
    Employee employee;

    appFormField employeeForm[] = {
        {"Employee Number: ", employee.personal.employeeNumber, EMPLOYEE_NUMBER_LEN, IV_NONE, {0}},
        {"Employee Name: ", employee.personal.employeeName, EMPLOYEE_NAME_LEN, IV_NONE, {0}},
        {"Employee Status (0 for Regular, 1 for Casual): ", NULL, 0, IV_CHOICES, {.choices = {"Regular", "Casual"}, .count = 2}},
        {"Hours Worked: ", NULL, 0, IV_RANGE_INT, {.rangeInt = {0, 99}}},
        {"Basic Rate: ", NULL, 0, IV_RANGE_FLT, {.rangeFloat = {0.0f, 999.99f}}}
    };

    appGetValidatedInput(employeeForm, 5);
    
    return 0;
}