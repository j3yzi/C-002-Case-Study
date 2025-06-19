#include "../../include/employee.h"
#include "../../include/apctxt.h"

int getEmployeeDataFromUser(Employee** employee) {
    
    char statusStr[2];
    char hoursStr[4];
    char rateStr[8];

    appFormField employeeDataForm[] = {
        {"Employee Number: ", (*employee)->personal.employeeNumber, sizeof((*employee)->personal.employeeNumber), IV_NONE, {0}},
        
        {"Employee Name: ", (*employee)->personal.employeeName, sizeof((*employee)->personal.employeeName), IV_MAX_LEN, {
            .rangeInt = {
                .min = 1,
                .max = EMPLOYEE_NAME_LEN - 1
            }
        }},

        {"Employee Status (0 for Regular, 1 for Casual): ", statusStr, 0, IV_CHOICES, {
            .choices = {
                .choices = (const char*[]){"0", "1"},
                .count = 2
            }
        }},

        {"Hours Worked: ", hoursStr, 0, IV_RANGE_INT, {
            .rangeInt = {
                .min = 0,
                .max = 999
            }
        }},

        {"Basic Rate: ", rateStr, 0, IV_RANGE_FLT, {
            .rangeFloat = {
                .min = 0.0f,
                .max = 999.99f
            }
        }},
    };

    appGetValidatedInput(employeeDataForm, 5);

    (*employee)->employment.status = (EmployeeStatus)atoi(statusStr);
    (*employee)->employment.hoursWorked = atoi(hoursStr);
    (*employee)->employment.basicRate = atof(rateStr);

    return 0;
}