#ifndef DATA_H
#define DATA_H

#include "../../../include/models/employee.h"
#include "../../../include/headers/list.h"

int saveEmployeeDataFromFile(list* employeeList, const char *filename);
list* loadEmployeeDataFromFile(const char* filename, ListType listType);
int listDataFiles(void);
void getCurrentTimestamp(char* buffer, int bufferSize);
int generatePayrollReportFile(const list* employeeList, char* generatedFilePath, int pathBufferSize);

#endif 