#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#include "../include/headers/apctxt.h"
#include "../include/headers/state.h"
#include "../include/headers/list.h"
#include "../include/headers/interface.h"
#include "../include/models/employee.h"
#include "../include/models/student.h"
#include "../include/models/course.h"
#include "ui/menuio.h"
#include "ui/courseio.h"

// External reference to the main menu defined in menuio.c
extern Menu mainMenu;

/**
 * @brief Gets the path to the configuration file
 * @return A pointer to a static buffer containing the path
 */
static const char* getConfigPath(void) {
    static char configPath[600];
    char executablePath[512];
    GetModuleFileName(NULL, executablePath, sizeof(executablePath));   // Get the full path of the executable
    char* lastSlash = strrchr(executablePath, '\\'); // Find the last backslash to get the directory
    if (lastSlash != NULL) {
        *lastSlash = '\0'; // Remove the executable name, keep directory
        sprintf(configPath, "%s\\config.ini", executablePath);
    } else {
        strcpy(configPath, "config.ini"); // Fallback to current directory
    }
    return configPath;
}

int main(void)
{   
    const char* configPath = getConfigPath();
    loadConfig(configPath); // Get configuration path and load configuration
    appInitWinTerm("PUP Information Management System - Employee, Student & Course Records"); // Initialize the Windows terminal
    initConsole(); // Initialize the new console interface system
    initMultiListManager(); // Initialize the multi-list management system
    initCourseCatalogManager(); // Initialize the course catalog manager
    
    // Load program configuration
    int programCount = loadProgramsFromConfig();
    printf("Loaded %d program(s) from configuration.\n", programCount);
    
    // Initialize AppConfig with specified values
    AppConfig appConfig = {
        "PUP Information Management System",
        "1.0",
        "MIT License",
        "C002 - Group 1"
    };
    
    // Update the main menu to include course management
    mainMenu = (Menu){1, "PUP Information Management System", (MenuOption[]){
        {'1', "Employee Management", "Manage employee records and payroll information", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Student Management", "Handle student enrollment and academic records", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Course Management", "Create and manage course catalogs and schedules", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "System Information", "View system information and application details", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'5', "Configuration Settings", "Modify system configuration and settings", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'6', "Exit", "Close the application and return to system", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 6
    };
    
    int result = menuLoop(); // Start the main menu loop
    
    cleanupMultiListManager(); // Clean up all resources before exit
    cleanupCourseCatalogManager(); // Clean up course catalog resources
    waitForKeypress("Press any key to exit...");
    
    return result;
} 