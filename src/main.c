#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#include "../include/headers/apctxt.h"
#include "../include/headers/state.h"
#include "../include/headers/list.h"
#include "../include/models/employee.h"
#include "../include/models/student.h"
#include "ui/menuio.h"

int main(int argc, char* argv[])
{   
    // Get the directory where the executable is located
    char executablePath[512];
    char configPath[600];
    
    // Get the full path of the executable
    GetModuleFileName(NULL, executablePath, sizeof(executablePath));
    
    // Find the last backslash to get the directory
    char* lastSlash = strrchr(executablePath, '\\');
    if (lastSlash != NULL) {
        *lastSlash = '\0'; // Remove the executable name, keep directory
        sprintf(configPath, "%s\\config.ini", executablePath);
    } else {
        // Fallback to current directory
        strcpy(configPath, "config.ini");
    }
    
    // Load configuration first
    printf("Loading configuration...\n");
    printf("Config path: %s\n", configPath);
    loadConfig(configPath);
    
    // Initialize the Windows terminal
    appInitWinTerm("PUP Information Management System - Employee & Student Records");
    
    // Initialize the multi-list management system
    initMultiListManager();
    
    // Start the main menu loop
    int result = menuLoop();
    
    // Clean up all resources before exit
    cleanupMultiListManager();
    
    printf("Press any key to exit...");
    _getch();
    
    return result;
} 