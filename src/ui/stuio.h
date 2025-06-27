#ifndef STUIO_H
#define STUIO_H

#include "../../include/headers/list.h"
#include "../../include/models/student.h"

// Student data input functions
int getStudentDataFromUser(Student* newStudent);
int getStudentNumberFromUser(char* buffer, const int bufferSize);
int editStudentDataFromUser(Student* student);

// Student operations
int handleSearchStudent(const list* studentList);
int handleEditStudent(list* studentList);
int handleDeleteStudent(list* studentList);

// Student table view
int handleDisplayStudentTable(const list* studentList);

#endif 