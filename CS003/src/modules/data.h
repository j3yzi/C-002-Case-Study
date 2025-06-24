#ifndef DATA_H
#define DATA_H

#include "../../../include/models/student.h"
#include "../../../include/headers/list.h"

int saveStudentDataFromFile(list* studentList, const char *filename);
list* loadStudentDataFromFile(const char* filename, ListType listType);
int listDataFiles(void);
void getCurrentTimestamp(char* buffer, int bufferSize);
int generateStudentReportFile(const list* studentList, char* generatedFilePath, int pathBufferSize);
int sortStudentsByGrade(list* studentList, bool descending);

#endif 