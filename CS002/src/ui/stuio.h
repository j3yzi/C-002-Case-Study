#ifndef STUIO_H
#define STUIO_H

#include "../../../include/models/student.h"
#include "../../../include/headers/list.h"

// Function prototypes for student I/O operations
int getStudentDataFromUser(Student* student);
int handleEditStudent(list* studentList);
int handleDeleteStudent(list* studentList);
int handleSearchStudent(list* studentList);

// Validation functions
int validateStudentNumber(const char* studentNumber, list* studentList);
int validateStudentName(const char* name);
int validateCourse(const char* course);
int validateYearLevel(int yearLevel);
int validateGrade(double grade);

// Display functions
void displayStudentSummary(const Student* student);

#endif 