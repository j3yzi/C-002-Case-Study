#ifndef MENUIO_H
#define MENUIO_H

#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/list.h"
#include "../../../include/models/employee.h"
#include "../../../include/models/student.h"

// Multi-list management structures (shared across modules)
typedef struct {
    list* employeeLists[10];  // Support up to 10 employee lists
    char employeeListNames[10][50];
    int employeeListCount;
    int activeEmployeeList;
} EmployeeManager;

typedef struct {
    list* studentLists[10];   // Support up to 10 student lists  
    char studentListNames[10][50];
    int studentListCount;
    int activeStudentList;
} StudentManager;

// Global manager instances (declared in menuio.c)
extern EmployeeManager empManager;
extern StudentManager stuManager;

// Main coordination functions
void initMultiListManager(void);
void cleanupMultiListManager(void);
int menuLoop(void);
void checkStates(void);
void displaySystemStatistics(void);

#endif 