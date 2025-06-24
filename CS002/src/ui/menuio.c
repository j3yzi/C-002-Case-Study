#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "menuio.h"
#include "employee_menu.h"
#include "student_menu.h"
#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/state.h"
#include "../../../include/models/employee.h"
#include "../../../include/models/student.h"
#include "../../../include/headers/list.h"

#define HIGHLIGHT_TEXT_COLOR 9
#define HIGHLIGHT_BG_COLOR 0
#define TEXT_COLOR 7
#define BG_COLOR 0
#define DISABLED_TEXT_COLOR 8
#define DISABLED_BG_COLOR 0

// Global managers - now shared across modules
EmployeeManager empManager;
StudentManager stuManager;

// Main menu definition - simplified for coordination only
Menu mainMenu = {1, "CS002 - Dual Management System", (MenuOption[]){
    {'1', "Employee Management", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Student Management", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "System Statistics", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'4', "Exit", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 4};

/**
 * @brief Initializes the multi-list management system
 */
void initMultiListManager(void) {
    // Initialize employee manager
    memset(&empManager, 0, sizeof(EmployeeManager));
    empManager.employeeListCount = 0;
    empManager.activeEmployeeList = -1;
    
    // Initialize student manager
    memset(&stuManager, 0, sizeof(StudentManager));
    stuManager.studentListCount = 0;
    stuManager.activeStudentList = -1;
    
    printf("Multi-list management system initialized!\n");
}

/**
 * @brief Cleans up all allocated lists and resources
 */
void cleanupMultiListManager(void) {
    // Clean up employee lists
    for (int i = 0; i < empManager.employeeListCount; i++) {
        if (empManager.employeeLists[i]) {
            destroyList(&empManager.employeeLists[i], freeEmployee);
        }
    }
    
    // Clean up student lists
    for (int i = 0; i < stuManager.studentListCount; i++) {
        if (stuManager.studentLists[i]) {
            destroyList(&stuManager.studentLists[i], freeStudent);
        }
    }
    
    printf("Multi-list management system cleaned up!\n");
}

/**
 * @brief Updates main menu option states (main menu is always enabled)
 */
void checkStates(void) {
    // Main menu options are always available
    // Individual submenu state checking is delegated to respective modules
}

/**
 * @brief Main menu loop - coordinates between different management systems
 * @return Returns 0 on normal exit, other values on error
 */
int menuLoop(void) {
    char choice;
    
    do {
        checkStates();
        winTermClearScreen();
        
        // Display system status
        printf("=== CS002 Dual Management System ===\n");
        printf("Employee Lists: %d | Active: %s\n", 
               empManager.employeeListCount,
               (empManager.activeEmployeeList >= 0) ? 
               empManager.employeeListNames[empManager.activeEmployeeList] : "None");
        printf("Student Lists: %d | Active: %s\n\n", 
               stuManager.studentListCount,
               (stuManager.activeStudentList >= 0) ? 
               stuManager.studentListNames[stuManager.activeStudentList] : "None");
        
        choice = initMenu(&mainMenu);
        
        switch(choice) {
            case '1':
                runEmployeeManagement(); // Delegated to employee_menu.c
                break;
            case '2':
                runStudentManagement(); // Delegated to student_menu.c
                break;
            case '3':
                displaySystemStatistics();
                break;
            case '4':
                printf("\nExiting CS002 Dual Management System...\n");
                return 0;
            default:
                printf("\nInvalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (true);
    
    return 0;
}

/**
 * @brief Displays comprehensive system statistics
 */
void displaySystemStatistics(void) {
    winTermClearScreen();
    printf("=== CS002 System Statistics ===\n\n");
    
    printf("EMPLOYEE MANAGEMENT:\n");
    printf("Total Employee Lists: %d\n", empManager.employeeListCount);
    int totalEmployees = 0;
    for (int i = 0; i < empManager.employeeListCount; i++) {
        if (empManager.employeeLists[i]) {
            int count = empManager.employeeLists[i]->size;
            totalEmployees += count;
            printf("  - %s: %d employees\n", empManager.employeeListNames[i], count);
        }
    }
    printf("Total Employees: %d\n", totalEmployees);
    
    if (empManager.activeEmployeeList >= 0) {
        printf("Active Employee List: %s\n", empManager.employeeListNames[empManager.activeEmployeeList]);
    } else {
        printf("Active Employee List: None\n");
    }
    printf("\n");
    
    printf("STUDENT MANAGEMENT:\n");
    printf("Total Student Lists: %d\n", stuManager.studentListCount);
    int totalStudents = 0;
    for (int i = 0; i < stuManager.studentListCount; i++) {
        if (stuManager.studentLists[i]) {
            int count = stuManager.studentLists[i]->size;
            totalStudents += count;
            printf("  - %s: %d students\n", stuManager.studentListNames[i], count);
        }
    }
    printf("Total Students: %d\n", totalStudents);
    
    if (stuManager.activeStudentList >= 0) {
        printf("Active Student List: %s\n", stuManager.studentListNames[stuManager.activeStudentList]);
    } else {
        printf("Active Student List: None\n");
    }
    printf("\n");
    
    printf("OVERALL STATISTICS:\n");
    printf("Total Records: %d\n", totalEmployees + totalStudents);
    printf("Total Lists: %d\n", empManager.employeeListCount + stuManager.studentListCount);
    printf("System Memory Usage: Employee Lists=%zu bytes, Student Lists=%zu bytes\n", 
           sizeof(EmployeeManager), sizeof(StudentManager));
    
    printf("\nSYSTEM CAPABILITIES:\n");
    printf("- Multi-list management (up to 10 lists per type)\n");
    printf("- Individual list saving with custom names\n");
    printf("- Comprehensive reporting and statistics\n");
    printf("- Data validation and error handling\n");
    printf("- Cross-platform compatibility\n");
    
    printf("\nPress any key to continue...");
    _getch();
}