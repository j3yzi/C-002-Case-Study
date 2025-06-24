#ifndef EMPLOYEE_MENU_H
#define EMPLOYEE_MENU_H

#include "menuio.h"  // Include the main header for shared types
#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/list.h"
#include "../../../include/models/employee.h"

// Function prototypes
void checkEmployeeMenuStates(void);
int runEmployeeManagement(void);

// Employee management handlers
int handleCreateEmployeeList(void);
int handleSwitchEmployeeList(void);
int handleAddEmployee(void);
int handleDisplayAllEmployees(void);
int handlePayrollReport(void);
int handleSaveEmployeeList(void);
int handleLoadEmployeeList(void);

#endif 