#ifndef STUDENT_MENU_H
#define STUDENT_MENU_H

#include "menuio.h"  // Include the main header for shared types
#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/list.h"
#include "../../../include/models/student.h"

// Function prototypes
void checkStudentMenuStates(void);
int runStudentManagement(void);

// Student management handlers
int handleCreateStudentList(void);
int handleSwitchStudentList(void);
int handleAddStudent(void);
int handleDisplayAllStudents(void);
int handleComputeFinalGrades(void);
int handleSortStudentsByGrade(void);
int handleStudentReport(void);
int handleSaveStudentList(void);
int handleLoadStudentList(void);

#endif 