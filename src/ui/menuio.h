#ifndef MENUIO_H
#define MENUIO_H

#include "../../include/headers/apctxt.h"
#include "../../include/headers/list.h"
#include "../../include/models/employee.h"
#include "../../include/models/student.h"

// Multi-list management structures
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

// Global manager instances
extern EmployeeManager empManager;
extern StudentManager stuManager;

// Main coordination functions
void initMultiListManager(void);
void cleanupMultiListManager(void);
int menuLoop(void);
void checkStates(void);
void displaySystemStatistics(void);

// Employee management functions
int runEmployeeManagement(void);
int handleCreateEmployeeList(void);
int handleSwitchEmployeeList(void);
int handleAddEmployee(void);
int handleDisplayAllEmployees(void);
int handlePayrollReport(void);
int handleSaveEmployeeList(void);
int handleLoadEmployeeList(void);

// Student management functions  
int runStudentManagement(void);
int handleCreateStudentList(void);
int handleSwitchStudentList(void);
int handleAddStudent(void);
int handleDisplayAllStudents(void);
int handleStudentReport(void);
int handleSaveStudentList(void);
int handleLoadStudentList(void);

#endif 