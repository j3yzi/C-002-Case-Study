#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include "../../include/headers/list.h"
#include "../../include/models/employee.h"
#include "../../include/models/student.h"

// Common data functions
int listDataFiles(void);
void getCurrentTimestamp(char* buffer, const int bufferSize);

// Employee data functions
int generatePayrollReportFile(const list* employeeList, char* generatedFilePath, const int pathBufferSize);
int saveEmployeeDataFromFile(list* employeeList, const char* filename);
list* loadEmployeeDataFromFile(const char* filename, ListType listType);

// Student data functions
int generateStudentReportFile(const list* studentList, char* generatedFilePath, const int pathBufferSize);
int saveStudentDataFromFile(list* studentList, const char* filename);
list* loadStudentDataFromFile(const char* filename, ListType listType);
int sortStudentsByGrade(list* studentList, const int descending);

// Multi-list management functions
int listEmployeeDataFiles(void);
int listStudentDataFiles(void);
int saveListWithCustomName(list* dataList, const char* listName, const char* dataType);
list* loadListWithName(const char* filename, const char* dataType, ListType listType);

#endif 