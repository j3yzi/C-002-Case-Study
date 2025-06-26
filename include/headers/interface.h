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