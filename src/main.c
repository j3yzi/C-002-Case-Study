/**
 * @file main.c
 * @brief Entry point for the PUP Information Management System
 * 
 * This file contains the main function that initializes and starts the
 * PUP Information Management System. The system manages employee records,
 * student records, and course catalogs for the Polytechnic University
 * of the Philippines.
 * 
 * @author C002 - Group 1
 * @version 1.0
 * @date 2024
 */

// Standard C library headers
#include <stdio.h>      // For input/output operations (printf, etc.)
#include <stdlib.h>     // For memory allocation and general utilities
#include <string.h>     // For string manipulation functions
#include <conio.h>      // For console I/O functions like _getch()
#include <windows.h>    // For Windows-specific functions

// Application-specific headers
#include "../include/headers/apctxt.h"    // Application context and configuration
#include "../include/headers/state.h"     // Application state management
#include "../include/headers/list.h"      // Generic linked list implementation
#include "../include/headers/interface.h" // User interface functions
#include "../include/models/employee.h"   // Employee data structures and functions
#include "../include/models/student.h"    // Student data structures and functions
#include "../include/models/course.h"     // Course data structures and functions
#include "ui/menuio.h"                    // Menu I/O operations
#include "ui/courseio.h"                  // Course-specific I/O operations

// External reference to the main menu defined in menuio.c
extern Menu mainMenu;

/**
 * @brief Gets the path to the configuration file
 * 
 * This function determines where the configuration file should be located
 * relative to the executable. It uses the Windows API to get the executable's
 * path and then constructs the config file path in the same directory.
 * 
 * @return A pointer to a static buffer containing the configuration file path
 */
static const char* getConfigPath(void) {
    static char configPath[600];           // Static buffer to hold the path
    char executablePath[512];              // Buffer for executable path
    
    // Get the full path of the current executable
    GetModuleFileName(NULL, executablePath, sizeof(executablePath));
    
    // Find the last backslash to get the directory containing the executable
    char* lastSlash = strrchr(executablePath, '\\');
    if (lastSlash != NULL) {
        *lastSlash = '\0'; // Remove the executable name, keep directory only
        sprintf(configPath, "%s\\config.ini", executablePath);
    } else {
        // Fallback: use current directory if path parsing fails
        strcpy(configPath, "config.ini");
    }
    return configPath;
}

/**
 * @brief Main entry point of the PUP Information Management System
 * 
 * This function performs the following initialization steps:
 * 1. Loads application configuration from config.ini
 * 2. Initializes the Windows terminal with a custom title
 * 3. Sets up the console interface system
 * 4. Initializes the data management systems (lists, courses)
 * 5. Loads program definitions from configuration
 * 6. Sets up the main menu structure
 * 7. Starts the main application loop
 * 8. Performs cleanup when the application exits
 * 
 * @return 0 on successful execution, non-zero on error
 */
int main(void)
{   
    // Step 1: Load application configuration
    const char* configPath = getConfigPath();
    loadConfig(configPath); // Load settings like payroll rates, grading thresholds, etc.
    
    // Step 2: Initialize the Windows terminal with custom title
    appInitWinTerm("PUP Information Management System - Employee, Student & Course Records");
    
    // Step 3: Initialize core system components
    initConsole();              // Set up console for proper display (UTF-8, colors, etc.)
    initMultiListManager();     // Initialize the system that manages multiple data lists
    initCourseCatalogManager(); // Initialize the course catalog management system
    
    // Step 4: Load program definitions (IT, CS, etc.) from configuration
    int programCount = loadProgramsFromConfig();
    printf("Loaded %d program(s) from configuration.\n", programCount);
    
    // Step 5: Initialize application metadata
    AppConfig appConfig = {
        "PUP Information Management System",  // Application name
        "1.0",                               // Version number
        "MIT License",                       // License type
        "C002 - Group 1"                     // Development team
    };
    
    // Step 6: Configure the main menu structure
    // This menu provides access to all major system functions
    mainMenu = (Menu){1, "PUP Information Management System", (MenuOption[]){
        {'1', "Employee Management", "Manage employee records and payroll information", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Student Management", "Handle student enrollment and academic records", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Course Management", "Create and manage course catalogs and schedules", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "System Information", "View system information and application details", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'5', "Configuration Settings", "Modify system configuration and settings", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'6', "Exit", "Close the application and return to system", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 6
    };
    
    // Step 7: Start the main application loop
    // This function handles user interaction and menu navigation
    int result = menuLoop();
    
    // Step 8: Cleanup all resources before program termination
    cleanupMultiListManager();      // Free all data lists and associated memory
    cleanupCourseCatalogManager();  // Clean up course catalog resources
    
    return result;
} 