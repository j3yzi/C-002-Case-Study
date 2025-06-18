#ifndef APPCTXT_H
#define APPCTXT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "employee.h"
#include <stdbool.h>

typedef struct AppContext {
    EmployeeNode* head;
    EmployeeNode* tail;
    EmployeeNode* current;
    int count;
    const char* dataFile;
    bool isInitialList;
} AppContext;

// AppContext* createAppContext(const char* dataFile);
// void appDestroyAppContext(AppContext* appContext);

// void addEmployee(AppContext* appContext, EmployeeNode* newEmployee);
// void createInitialEmployeeList(AppContext* appContext);

// EmployeeNode* findEmployeeByNumber(AppContext* appContext, const char* employeeNumber);



#endif