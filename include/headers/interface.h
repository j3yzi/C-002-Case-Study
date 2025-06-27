#ifndef INTERFACE_H
#define INTERFACE_H

#include "apctxt.h"
#include "state.h"
#include "list.h"
#include "../models/employee.h"
#include "../models/student.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Table display constants
#define NAME_COL_WIDTH 15
#define STUDENTS_PER_PAGE 10
#define EMPLOYEES_PER_PAGE 10
#define MAX_STUDENTS 50
#define MAX_EMPLOYEES 50

// Pagination state structure
typedef struct {
    int currentPage;
    int totalPages;
    int itemsPerPage;
    int totalItems;
    int startIndex;
    int endIndex;
} PaginationState;

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

extern EmployeeManager empManager;
extern StudentManager stuManager;

// State management functions
void checkMenuStates(Menu* menu);
void updateMenuOptionStates(Menu* menu, int hasActiveList, int hasItems, int hasMultipleLists);
void updateEmployeeMenuStates(Menu* menu);
void updateStudentMenuStates(Menu* menu);
bool checkActiveList(int isActiveList, int listSize, const char* errorMessage);

// Pagination functions
void initPagination(PaginationState* pagination, int totalItems, int itemsPerPage);
void updatePagination(PaginationState* pagination, int newPage);
int calculateTotalPages(int totalItems, int itemsPerPage);

// Student table display functions
void displayStudentTableHeader(int consoleWidth, int tableWidth, int smlBoxWidth, int tblMargin, PaginationState* pagination, int actualItemsOnPage);
void displayStudentTableRow(const Student* student, int rowNumber, int consoleWidth, int tableWidth, int tblMargin);
void displayStudentTableFooter(int consoleWidth, int tableWidth, int tblMargin);
void displayStudentTable(const list* studentList, PaginationState* pagination);
int runStudentTableView(const list* studentList);

// Employee table display functions
void displayEmployeeTableHeader(int consoleWidth, int tableWidth, int smlBoxWidth, int tblMargin, PaginationState* pagination, int actualItemsOnPage);
void displayEmployeeTableRow(const Employee* employee, int rowNumber, int consoleWidth, int tableWidth, int tblMargin);
void displayEmployeeTableFooter(int consoleWidth, int tableWidth, int tblMargin);
void displayEmployeeTable(const list* employeeList, PaginationState* pagination);
int runEmployeeTableView(const list* employeeList);

// Menu display functions
void displayMenuHeader(const Menu* menu, int consoleWidth, int totalMenuNameWidth, int paddingX, int paddingY);
void displayMenuOptions(const Menu* menu, int consoleWidth, int totalMenuNameWidth, int paddingX, int paddingY, int selected);
void displayMenuDescription(const Menu* menu, int consoleWidth, int totalMenuNameWidth, int paddingX, int paddingY, int selected);
void displayInfoBox(const Menu* menu, int consoleWidth, int totalMenuNameWidth, int totalOptionBoxWidth, int paddingX, int paddingY, int selected);
void updateMenuSelection(const Menu* menu, int prevSelected, int newSelected, int consoleWidth, int totalMenuNameWidth, int paddingX, int paddingY);
void displayMenu(const Menu* menu, int selected);

// Console utility functions
void initConsole();
void getConsoleSize(int* width, int* height);
void setCursor(int x, int y);
void updateInfoBoxTimeDate(int consoleWidth, int totalMenuNameWidth, int totalOptionBoxWidth, int paddingX, int paddingY);
void displayUserInput(const char* prompt, int positionX, int positionY);

// User interaction functions
bool appYesNoPrompt(const char* prompt);

#endif // INTERFACE_H 