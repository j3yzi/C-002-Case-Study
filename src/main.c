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

/**
 * @brief Gets the path to the configuration file
 * @return A pointer to a static buffer containing the path
 */
static const char* getConfigPath(void) {
    static char configPath[600];
    char executablePath[512];
    
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
    
    return configPath;
}

int main(int argc, char* argv[])
{   
    // Prevent unused parameter warnings
    (void)argc;
    (void)argv;
    
    // Get configuration path and load configuration
    const char* configPath = getConfigPath();
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