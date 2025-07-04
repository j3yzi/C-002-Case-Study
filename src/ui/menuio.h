#ifndef MENUIO_H
#define MENUIO_H

#include "../../include/headers/apctxt.h"
#include "../../include/headers/list.h"
#include "../../include/headers/interface.h"
#include "../../include/models/employee.h"
#include "../../include/models/student.h"

// Main coordination functions
void initMultiListManager(void);
void cleanupMultiListManager(void);
int menuLoop(void);
void checkStates(void);
void displaySystemInformation(void);

// Configuration management functions
int runConfigurationManagement(void);
int handleUpdatePayrollSettings(void);
int handleUpdateAcademicSettings(void);
int handleSaveConfiguration(const char* configPath);
int handleResetConfiguration(const char* configPath);

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
int handleSortStudentsByGrade(void);
int handleStudentReport(void);
int handleSaveStudentList(void);
int handleLoadStudentList(void);

// Course management function (implemented in courseio.c)
int runCourseManagement(void);

#endif 