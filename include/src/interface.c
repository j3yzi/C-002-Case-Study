/**
 * @file interface.c
 * @brief User Interface Management and Display Functions
 * 
 * This file implements the user interface system for the PUP Information
 * Management System. It provides functions for displaying menus, tables,
 * pagination, console management, and user interaction. The interface
 * supports Windows console features including colors, cursor positioning,
 * and box-drawing characters for professional-looking displays.
 * 
 * Key Features:
 * - Menu system with navigation and selection
 * - Paginated table displays for employee and student data
 * - Console management (cursor, colors, screen clearing)
 * - State management for menu options
 * - Real-time date/time display
 * - User input handling and validation
 * 
 * @author C002 - Group 1
 * @version 1.0
 * @date 2024
 */

// Standard C library includes
#include <stdio.h>      // For input/output operations
#include <conio.h>      // For console I/O functions like _getch()
#ifdef _WIN32
#include <windows.h>    // For Windows console API functions
#endif
#include <string.h>     // For string manipulation functions
#include <time.h>       // For date/time operations
#include <stdlib.h>     // For memory allocation and general utilities
#include <ctype.h>      // For character type checking functions

// Application-specific includes
#include "../headers/interface.h"  // Interface function declarations
#include "../headers/apctxt.h"     // Application context and configuration
#include "../headers/state.h"      // Application state management

/**
 * @name Display Configuration Constants
 * @brief Constants for controlling display layout and formatting
 * @{
 */
#define MENU_HEADER_WIDTH 60  // Standard width for menu headers across the application

/** @} */ // End of Display Configuration Constants

/**
 * @name Console Management Variables
 * @brief Static variables for managing Windows console state
 * @{
 */
static HANDLE hConsole;                           // Handle to the console output
static CONSOLE_SCREEN_BUFFER_INFO originalConsoleInfo; // Original console state for restoration

/** @} */ // End of Console Management Variables

// Initialize console settings
void initConsole() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE) {
        GetConsoleScreenBufferInfo(hConsole, &originalConsoleInfo);
        
        // Set console code page to UTF-8 for proper box-drawing characters
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    }
}

void getConsoleSize(int* width, int* height) {
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
    *width = consoleInfo.dwSize.X;
    *height = consoleInfo.dwSize.Y;
}

void setCursor(int x, int y) {
    // Make sure console is initialized
    if (hConsole == NULL) {
        initConsole();
    }
    
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

void displayUserInput(const char* prompt, int positionX, int positionY) {
    // get the console size
    int consoleWidth, consoleHeight;
    getConsoleSize(&consoleWidth, &consoleHeight);

    // display the prompt
    setCursor(consoleWidth / 2 - strlen(prompt) / 2, 5);
    // Calculate box dimensions
    int promptLength = strlen(prompt);
    int boxWidth = promptLength * 2 + 4; // 2 spaces padding on each side
    int boxHeight = 3; // top border, content line, bottom border
    
    // Calculate starting position for the box
    int startX = consoleWidth / 2 - boxWidth / 2;
    int startY = consoleHeight / 2 - boxHeight / 2;
    
    // Draw top border
    setCursor(startX, startY);
    printf("┌");
    for (int i = 0; i < boxWidth - 2; i++) {
        printf("─");
    }
    printf("┐");
    
    // Draw content line with side borders
    setCursor(startX, startY + 1);
    printf("│  %s", prompt);
    for (int i = 0; i < boxWidth - 2 - promptLength; i++) {
        printf(" ");
    }
    printf("│");

    // Draw bottom border
    setCursor(startX, startY + 2);
    printf("└");
    for (int i = 0; i < boxWidth - 2; i++) {
        printf("─");
    }
    printf("┘");
}

void displayMenu(const Menu* menu, int selected) {
    // Get the console size
    int consoleWidth, consoleHeight;
    getConsoleSize(&consoleWidth, &consoleHeight);

    // Use constant width for menu header
    int totalMenuNameWidth = MENU_HEADER_WIDTH;
    int paddingX = 5;
    int paddingY = 2;
    
    // Always make option box exactly 2/3 of the total width
    int totalOptionBoxWidth = (totalMenuNameWidth * 2) / 3;
    
    displayMenuHeader(menu, consoleWidth, totalMenuNameWidth, paddingX, paddingY);
    displayMenuOptions(menu, consoleWidth, totalMenuNameWidth, paddingX, paddingY, selected);
    displayInfoBox(menu, consoleWidth, totalMenuNameWidth, totalOptionBoxWidth, paddingX, paddingY, selected);
    displayMenuDescription(menu, consoleWidth, totalMenuNameWidth, paddingX, paddingY, selected);
}

void updateMenuSelection(const Menu* menu, int prevSelected, int newSelected, int consoleWidth, int totalMenuNameWidth, int paddingX, int paddingY) {
    // Use fixed width for menu
    totalMenuNameWidth = MENU_HEADER_WIDTH;
    
    // Find the starting position (left side of the menu header)
    int startX = consoleWidth / 2 - totalMenuNameWidth / 2;
    
    // Always make option box exactly 2/3 of the total width
    int totalOptionBoxWidth = (totalMenuNameWidth * 2) / 3;
    
    // Update previously selected option (unselect)
    if (prevSelected >= 0 && prevSelected < menu->optionCount) {
        int optionTextLength = strlen(menu->options[prevSelected].text);
        setCursor(startX, paddingY + 4 + prevSelected);
        printf("║");
        for (int j = 0; j < paddingX; j++) {
            printf(" ");
        }
        
        if (menu->options[prevSelected].isDisabled) {
            // Display disabled option in grey/dim color
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
            printf("%s", menu->options[prevSelected].text);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        } else {
            printf("%s", menu->options[prevSelected].text);
        }
        
        // Calculate remaining space in the box
        int remainingSpace = totalOptionBoxWidth - 2 - paddingX - optionTextLength;
        for (int j = 0; j < remainingSpace; j++) {
            printf(" ");
        }
        printf("║");
    }
    
    // Update newly selected option (select)
    if (newSelected >= 0 && newSelected < menu->optionCount) {
        int optionTextLength = strlen(menu->options[newSelected].text);
        setCursor(startX, paddingY + 4 + newSelected);
        printf("║");
        for (int j = 0; j < paddingX; j++) {
            printf(" ");
        }
        
        if (menu->options[newSelected].isDisabled) {
            // Display disabled selected option in grey/dim with arrow
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
            printf("→ %s", menu->options[newSelected].text);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            
            // Calculate remaining space in the box
            int remainingSpace = totalOptionBoxWidth - 2 - paddingX - optionTextLength - 2;
            for (int j = 0; j < remainingSpace; j++) {
                printf(" ");
            }
        } else {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("→ %s", menu->options[newSelected].text);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            
            // Calculate remaining space in the box
            int remainingSpace = totalOptionBoxWidth - 2 - paddingX - optionTextLength - 2;
            for (int j = 0; j < remainingSpace; j++) {
                printf(" ");
            }
        }
        printf("║");
        
        // Update description box with the new selection's description
        displayMenuDescription(menu, consoleWidth, totalMenuNameWidth, paddingX, paddingY, newSelected);
    }
}

void updateInfoBoxTimeDate(int consoleWidth, int totalMenuNameWidth, int totalOptionBoxWidth, int paddingX, int paddingY) {
    // Use fixed width for menu
    totalMenuNameWidth = MENU_HEADER_WIDTH;
    
    // Always make option box exactly 2/3 of the total width
    totalOptionBoxWidth = (totalMenuNameWidth * 2) / 3;
    
    // Find the starting position (left side of the menu header)
    int startX = consoleWidth / 2 - totalMenuNameWidth / 2;
    
    // Info box should start at the right of the options box
    int infoStartX = startX + totalOptionBoxWidth;
    
    // Calculate width for the info box
    int infoBoxWidth = totalMenuNameWidth - totalOptionBoxWidth;
    
    // Get current time
    time_t now = time(NULL);
    struct tm *timeInfo = localtime(&now);
    
    // Format time string
    char timeString[10];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", timeInfo);
    
    // Format date string
    char dateString[11];
    strftime(dateString, sizeof(dateString), "%d-%m-%Y", timeInfo);
    
    // Update time display
    setCursor(infoStartX, paddingY + 7);
    printf("║");
    printf("Time: %s", timeString);
    
    // Make sure we pad correctly to fit the box width
    int timeTextLen = strlen("Time: ") + strlen(timeString);
    for (int i = 0; i < infoBoxWidth - 2 - timeTextLen; i++) {
        printf(" ");
    }
    printf("║");
    
    // Update date display
    setCursor(infoStartX, paddingY + 8);
    printf("║");
    printf("Date: %s", dateString);
    
    // Make sure we pad correctly to fit the box width
    int dateTextLen = strlen("Date: ") + strlen(dateString);
    for (int i = 0; i < infoBoxWidth - 2 - dateTextLen; i++) {
        printf(" ");
    }
    printf("║");
}

void displayMenuHeader(const Menu* menu, int consoleWidth, int totalMenuNameWidth, int paddingX, int paddingY) 
{
    // Use fixed width for menu header
    totalMenuNameWidth = MENU_HEADER_WIDTH;
    
    // Top border
    setCursor(consoleWidth / 2 - totalMenuNameWidth / 2, paddingY);
    printf("╔");
    for (int i = 0; i < totalMenuNameWidth - 2; i++) {
        printf("═");
    }
    printf("╗\n");

    // Menu name - center within the fixed width
    setCursor(consoleWidth / 2 - totalMenuNameWidth / 2, paddingY + 1);
    printf("║");
    
    int menuNameLength = strlen(menu->name);
    int availableSpace = totalMenuNameWidth - 2; // Space between borders
    
    // Ensure menu name doesn't exceed available space
    if (menuNameLength > availableSpace) {
        menuNameLength = availableSpace;
    }
    
    // Calculate padding to center the menu name
    int leftPadding = (availableSpace - menuNameLength) / 2;
    int rightPadding = availableSpace - menuNameLength - leftPadding;
    
    // Add left padding
    for (int i = 0; i < leftPadding; i++) {
        printf(" ");
    }
    
    // Print menu name (truncated if necessary)
    for (int i = 0; i < menuNameLength; i++) {
        printf("%c", menu->name[i]);
    }
    
    // Add right padding to fill exactly to the border
    for (int i = 0; i < rightPadding; i++) {
        printf(" ");
    }
    
    printf("║\n");
    
    // Bottom border
    setCursor(consoleWidth / 2 - totalMenuNameWidth / 2, paddingY + 2);
    printf("╚");
    for (int i = 0; i < totalMenuNameWidth - 2; i++) {
        printf("═");
    }
    printf("╝\n");
}

void displayMenuOptions(const Menu* menu, int consoleWidth, int totalMenuNameWidth, int paddingX, int paddingY, int selected) {        
    // Use fixed width for menu
    totalMenuNameWidth = MENU_HEADER_WIDTH;
    
    // Always make option box exactly 2/3 of the total width
    int totalOptionBoxWidth = (totalMenuNameWidth * 2) / 3;
    
    // Find the starting position (left side of the menu header)
    int startX = consoleWidth / 2 - totalMenuNameWidth / 2;
    
    // Draw the options box
    setCursor(startX, paddingY + 3);
    printf("╔");
    for (int i = 0; i < totalOptionBoxWidth - 2; i++) {
        printf("═");
    }
    printf("╗\n");

    for (int i = 0; i < menu->optionCount; i++) {
        int optionTextLength = strlen(menu->options[i].text);
        setCursor(startX, paddingY + 4 + i);
        printf("║");
        for (int j = 0; j < paddingX; j++) {
            printf(" ");
        }
        
        if (menu->options[i].isDisabled) {
            // Display disabled option in grey/dim color
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
            if (i == selected) {
                printf("→ %s", menu->options[i].text);
                // Calculate remaining space in the box
                int remainingSpace = totalOptionBoxWidth - 2 - paddingX - optionTextLength - 2;
                for (int j = 0; j < remainingSpace; j++) {
                    printf(" ");
                }
            } else {
                printf("%s", menu->options[i].text);
                // Calculate remaining space in the box
                int remainingSpace = totalOptionBoxWidth - 2 - paddingX - optionTextLength;
                for (int j = 0; j < remainingSpace; j++) {
                    printf(" ");
                }
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        } else if (i == selected) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("→ %s", menu->options[i].text);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            
            // Calculate remaining space in the box
            int remainingSpace = totalOptionBoxWidth - 2 - paddingX - optionTextLength - 2;
            for (int j = 0; j < remainingSpace; j++) {
                printf(" ");
            }
        } else {
            // Display normal text without arrow
            printf("%s", menu->options[i].text);
            
            // Calculate remaining space in the box
            int remainingSpace = totalOptionBoxWidth - 2 - paddingX - optionTextLength;
            for (int j = 0; j < remainingSpace; j++) {
                printf(" ");
            }
        }
        
        printf("║\n");
    }

    setCursor(startX, paddingY + 3 + menu->optionCount + 1);
    printf("╚");
    for (int i = 0; i < totalOptionBoxWidth - 2; i++) {
        printf("═");
    }
    printf("╝");
}

void displayMenuDescription(const Menu* menu, int consoleWidth, int totalMenuNameWidth, int paddingX, int paddingY, int selected) {
    // Use fixed width for menu
    totalMenuNameWidth = MENU_HEADER_WIDTH;
    
    // Find the starting position (left side of the menu header)
    int startX = consoleWidth / 2 - totalMenuNameWidth / 2;
    
    // Calculate the description text length
    int optionTextLength = strlen(menu->options[selected].description);
    
    // Draw description box top border
    setCursor(startX, paddingY + 5 + menu->optionCount);
    printf("╔");
    for (int i = 0; i < totalMenuNameWidth - 2; i++) {
        printf("═");
    }
    printf("╗\n");
    
    // Draw description content
    setCursor(startX, paddingY + 6 + menu->optionCount);
    printf("║");
    
    // Calculate padding needed to center the description text
    int availableSpace = totalMenuNameWidth - 2;
    // Truncate description if too long
    if (optionTextLength > availableSpace) {
        optionTextLength = availableSpace;
    }
    
    int leftPadding = (availableSpace - optionTextLength) / 2;
    
    // Add left padding for centering
    for (int i = 0; i < leftPadding; i++) {
        printf(" ");
    }
    
    // Print description with highlight
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    // Ensure we don't print more than available space
    char desc[100]; // Buffer for truncated description
    strncpy(desc, menu->options[selected].description, availableSpace);
    desc[availableSpace] = '\0'; // Ensure null termination
    printf("%s", desc);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    
    // Add right padding to fill remaining space
    int rightPadding = availableSpace - optionTextLength - leftPadding;
    for (int i = 0; i < rightPadding; i++) {
        printf(" ");
    }
    
    printf("║\n");
    
    // Draw description box bottom border
    setCursor(startX, paddingY + 7 + menu->optionCount);
    printf("╚");
    for (int i = 0; i < totalMenuNameWidth - 2; i++) {
        printf("═");
    }
    printf("╝\n");
}

void displayInfoBox(const Menu* menu, int consoleWidth, int totalMenuNameWidth, int totalOptionBoxWidth, int paddingX, int paddingY, int selected) {
    // Use fixed width for menu
    totalMenuNameWidth = MENU_HEADER_WIDTH;
    
    // Always make option box exactly 2/3 of the total width
    totalOptionBoxWidth = (totalMenuNameWidth * 2) / 3;
    
    // Calculate width for the info box - make sure it fits in the header space
    int infoBoxWidth = totalMenuNameWidth - totalOptionBoxWidth;
    
    // Find the starting position (left side of the menu header)
    int startX = consoleWidth / 2 - totalMenuNameWidth / 2;
    
    // Info box should start at the right of the options box
    int infoStartX = startX + totalOptionBoxWidth;
    
    // Align the top border with the menu options top border
    setCursor(infoStartX, paddingY + 3);
    printf("╔");
    for (int i = 0; i < infoBoxWidth - 2; i++) {
        printf("═");
    }
    printf("╗\n");
    
    // system time
    time_t now = time(NULL);
    struct tm *timeInfo = localtime(&now);
    char timeString[10];
    strftime(timeString, sizeof(timeString), "%H:%M:%S", timeInfo);

    // system date
    time_t now2 = time(NULL);
    struct tm *timeInfo2 = localtime(&now2);
    char dateString[11];
    strftime(dateString, sizeof(dateString), "%d-%m-%Y", timeInfo2);    

    setCursor(infoStartX, paddingY + 4);
    printf("║");
    printf("[↑↓] Move");
    
    // Make sure we pad correctly to fit the box width
    int moveTextLen = strlen("[↑↓] Move");
    for (int i = 0; i < infoBoxWidth - 2 - moveTextLen + 4; i++) {
        printf(" ");
    }
    printf("║");

    setCursor(infoStartX, paddingY + 5);
    printf("║");
    printf("[↵] Select");
    
    // Make sure we pad correctly to fit the box width
    int selectTextLen = strlen("[↵] Select");
    for (int i = 0; i < infoBoxWidth - 2 - selectTextLen + 2; i++) {
        printf(" ");
    }
    printf("║");

    setCursor(infoStartX, paddingY + 6);
    printf("║");
    printf("[Esc] Exit");
    
    // Make sure we pad correctly to fit the box width
    int exitTextLen = strlen("[Esc] Exit");
    for (int i = 0; i < infoBoxWidth - 2 - exitTextLen; i++) {
        printf(" ");
    }
    printf("║");

    setCursor(infoStartX, paddingY + 7);
    
    printf("║");
    printf("Time: %s", timeString);
    
    // Make sure we pad correctly to fit the box width
    int timeTextLen = strlen("Time: ") + strlen(timeString);
    for (int i = 0; i < infoBoxWidth - 2 - timeTextLen; i++) {
        printf(" ");
    }
    printf("║");

    setCursor(infoStartX, paddingY + 8);
    printf("║");
    printf("Date: %s", dateString);
    
    // Make sure we pad correctly to fit the box width
    int dateTextLen = strlen("Date: ") + strlen(dateString);
    for (int i = 0; i < infoBoxWidth - 2 - dateTextLen; i++) {
        printf(" ");
    }
    printf("║");

    // print remaining rows to match height of menu option box
    for (int row = 9; row <= menu->optionCount + 3; row++) {
        setCursor(infoStartX, paddingY + row);
        printf("║");
        for (int i = 0; i < infoBoxWidth - 2; i++) {
            printf(" ");
        }
        printf("║");
    }

    // Draw bottom border, aligned with shortcut text
    setCursor(infoStartX, paddingY + menu->optionCount + 4);
    printf("╚");
    for (int i = 0; i < infoBoxWidth - 2; i++) {
        printf("═");
    }
    printf("╝");
}

// Simple Yes/No prompt function
bool appYesNoPrompt(const char* prompt) {
    if (!prompt) return false;
    
    bool isYesSelected = true;
    int key;
    
    // Display initial options
    while (1) {
        // Display options with current selection
        printf("\r%s (Y/N): ", prompt);
        
        // Display Yes option
        if (isYesSelected) {
            printf("[*] Yes ");
        } else {
            printf("[ ] Yes ");
        }
        
        // Display No option
        if (!isYesSelected) {
            printf("[*] No");
        } else {
            printf("[ ] No");
        }
        
        fflush(stdout);
        
        // Get user input
        key = _getch();
        
        // Handle arrow keys
        if (key == 0 || key == 224) {
            key = _getch(); // Get the actual key code
            
            if (key == 75 || key == 77) { // Left/Right arrow
                isYesSelected = !isYesSelected; // Toggle selection
            }
        }
        // Process Enter key
        else if (key == 13) {
            printf("\n"); // Move to next line after selection
            return isYesSelected;
        }
        // Handle Y/N keys as shortcuts
        else if (key == 'y' || key == 'Y') {
            printf("\n");
            return true;
        }
        else if (key == 'n' || key == 'N') {
            printf("\n");
            return false;
        }
    }
}

// State Management Functions

/**
 * @brief Generic function to check if an active list exists and has items
 * @param isActiveList Flag indicating if there's an active list
 * @param listSize Size of the active list (0 if no active list)
 * @param errorMessage Message to display if no active list
 * @return Returns true if list exists and operation can proceed, false otherwise
 */
bool checkActiveList(int isActiveList, int listSize, const char* errorMessage) {
    // Mark listSize as unused to avoid compiler warning
    (void)listSize; // Suppress unused parameter warning
    
    if (!isActiveList) {
        printf("╔═══════════════════════════════════════════════════════════════════╗\n");
        printf("║                            ERROR                                  ║\n");
        printf("╠═══════════════════════════════════════════════════════════════════╣\n");
        printf("║                                                                   ║\n");
        printf("║  %-63s  ║\n", errorMessage ? errorMessage : "No active list!");
        printf("║                                                                   ║\n");
        printf("║  Please create or load a list first before performing this       ║\n");
        printf("║  operation.                                                       ║\n");
        printf("║                                                                   ║\n");
        printf("╚═══════════════════════════════════════════════════════════════════╝\n");
        printf("\nPress any key to continue...");
        _getch();
        return false;
    }
    return true;
}

/**
 * @brief Generic function to update menu option states based on list availability
 * @param menu Pointer to the menu to update
 * @param hasActiveList Flag indicating if there's an active list
 * @param hasItems Flag indicating if the active list has items
 * @param hasMultipleLists Flag indicating if there are multiple lists
 */
void updateMenuOptionStates(Menu* menu, int hasActiveList, int hasItems, int hasMultipleLists) {
    if (!menu || !menu->options || menu->optionCount == 0) return;
    
    for (int i = 0; i < menu->optionCount; i++) {
        // Default behavior: enable all options
        menu->options[i].isDisabled = false;
        
        // You can add generic rules here based on option keys or text
        // For example, search/display operations typically need items
        if (hasItems == 0) {
            char key = menu->options[i].key;
            if (key == '6' || key == '7' || key == '8' || key == '9') {
                // Typically search, display, report, save operations
                menu->options[i].isDisabled = true;
            }
        }
        
        // Switch operations typically need multiple lists
        if (hasMultipleLists == 0) {
            char key = menu->options[i].key;
            if (key == '2') {
                // Typically switch list operation
                menu->options[i].isDisabled = true;
            }
        }
        
        // Add operations typically need an active list
        if (hasActiveList == 0) {
            char key = menu->options[i].key;
            if (key == '3' || key == '4' || key == '5') {
                // Typically add, edit, delete operations
                menu->options[i].isDisabled = true;
            }
        }
    }
}

/**
 * @brief Updates employee menu option states based on current manager state
 * @param menu Pointer to the employee menu to update
 */
void updateEmployeeMenuStates(Menu* menu) {
    if (!menu) return;
    
    // Get current employee manager state
    extern EmployeeManager empManager;
    
    int hasActiveList = (empManager.activeEmployeeList >= 0 && 
                        empManager.employeeLists[empManager.activeEmployeeList] != NULL);
    int hasEmployees = hasActiveList && 
                      (empManager.employeeLists[empManager.activeEmployeeList]->size > 0);
    int hasMultipleLists = (empManager.employeeListCount > 1);
    
    // Apply generic state rules
    updateMenuOptionStates(menu, hasActiveList, hasEmployees, hasMultipleLists);
    
    // Employee-specific overrides
    for (int i = 0; i < menu->optionCount; i++) {
        // Employee Table View (key 'T') - requires active list WITH employees
        if (menu->options[i].key == 'T' || menu->options[i].key == 't') {
            menu->options[i].isDisabled = !(hasActiveList && hasEmployees);
        }
        // Payroll reports might have special requirements
        else if (menu->options[i].key == '8') {
            // Payroll report needs active list with employees
            menu->options[i].isDisabled = !hasEmployees;
        }
    }
}

/**
 * @brief Updates student menu option states based on current manager state
 * @param menu Pointer to the student menu to update
 */
void updateStudentMenuStates(Menu* menu) {
    if (!menu) return;
    
    // Get current student manager state
    extern StudentManager stuManager;
    
    int hasActiveList = (stuManager.activeStudentList >= 0 && 
                        stuManager.studentLists[stuManager.activeStudentList] != NULL);
    int hasStudents = hasActiveList && 
                     (stuManager.studentLists[stuManager.activeStudentList]->size > 0);
    int hasMultipleLists = (stuManager.studentListCount > 1);
    int hasMultipleStudents = hasActiveList && 
                             (stuManager.studentLists[stuManager.activeStudentList]->size > 1);
    
    // Apply generic state rules
    updateMenuOptionStates(menu, hasActiveList, hasStudents, hasMultipleLists);
    
    // Student-specific overrides
    for (int i = 0; i < menu->optionCount; i++) {
        // Sort operation typically needs at least 2 students
        if (menu->options[i].key == '8') {
            menu->options[i].isDisabled = !hasMultipleStudents;
        }
        // Table view needs at least 1 student
        if (menu->options[i].key == 'T') {
            menu->options[i].isDisabled = !hasStudents;
        }
    }
}

/**
 * @brief Main function to check and update menu states based on global application state
 * @param menu Pointer to the menu to update states for
 */
void checkMenuStates(Menu* menu) {
    if (!menu) return;
    
    // Check if this is a main menu (usually has fewer options and different structure)
    if (menu->optionCount <= 6) {
        // Main menu options are usually always available
        for (int i = 0; i < menu->optionCount; i++) {
            menu->options[i].isDisabled = false;
        }
        return;
    }
    
    // For sub-menus, try to determine the type based on menu name or structure
    if (menu->name && strstr(menu->name, "Employee")) {
        updateEmployeeMenuStates(menu);
    } else if (menu->name && strstr(menu->name, "Student")) {
        updateStudentMenuStates(menu);
    } else {
        // Generic state checking for unknown menu types
        updateMenuOptionStates(menu, 1, 1, 1); // Enable all by default
    }
}

// Pagination Functions

/**
 * @brief Initialize pagination state
 * @param pagination Pointer to pagination state structure
 * @param totalItems Total number of items to paginate
 * @param itemsPerPage Number of items to display per page
 */
void initPagination(PaginationState* pagination, int totalItems, int itemsPerPage) {
    if (!pagination) return;
    
    pagination->totalItems = totalItems;
    pagination->itemsPerPage = itemsPerPage;
    pagination->totalPages = calculateTotalPages(totalItems, itemsPerPage);
    pagination->currentPage = 1;
    updatePagination(pagination, 1);
}

/**
 * @brief Update pagination state for a specific page
 * @param pagination Pointer to pagination state structure
 * @param newPage The page number to navigate to (1-indexed)
 */
void updatePagination(PaginationState* pagination, int newPage) {
    if (!pagination || newPage < 1 || newPage > pagination->totalPages) return;
    
    pagination->currentPage = newPage;
    pagination->startIndex = (newPage - 1) * pagination->itemsPerPage;
    pagination->endIndex = pagination->startIndex + pagination->itemsPerPage - 1;
    
    // Ensure end index doesn't exceed total items
    if (pagination->endIndex >= pagination->totalItems) {
        pagination->endIndex = pagination->totalItems - 1;
    }
}

/**
 * @brief Calculate total number of pages needed
 * @param totalItems Total number of items
 * @param itemsPerPage Items per page
 * @return Total number of pages
 */
int calculateTotalPages(int totalItems, int itemsPerPage) {
    if (itemsPerPage <= 0) return 1;
    return (totalItems + itemsPerPage - 1) / itemsPerPage;
}

// Student Table Display Functions

/**
 * @brief Display the header portion of the student table
 * @param consoleWidth Current console width
 * @param tableWidth Width of the main table
 * @param smlBoxWidth Width of the side box
 * @param tblMargin Left margin for centering
 * @param pagination Pagination state for page info
 * @param actualItemsOnPage Actual number of items displayed on current page
 */
void displayStudentTableHeader(int consoleWidth, int tableWidth, int smlBoxWidth, int tblMargin, PaginationState* pagination, int actualItemsOnPage) {
    char title[] = "STUDENT RECORDS";
    char keys[3][30] = {
        " ⇄      Navigate pages",
        "Enter  View details",
        "Esc    Return to menu"
    };
    
    int tablepad = (tableWidth - strlen(title)) / 2 - 1;
    int extra = (tableWidth - (int)strlen(title)) % 2;
    if (tablepad < 0) tablepad = 0;
    
    // Column positions
    int num = 5, id = 18, name = 36, prog = 46, yr = 53, grd = 67, rm = 77;
    
    // Build dynamic page info string so that width calculation remains accurate regardless of digit count
    char pageInfo[32];
    char countInfo[48];
    int spacing;
    snprintf(pageInfo, sizeof(pageInfo), "Page %d of %d", pagination->currentPage, pagination->totalPages);
    int firstRec = (pagination->startIndex) + 1;
    int lastRec = pagination->endIndex + 1;
    if (lastRec > pagination->totalItems) lastRec = pagination->totalItems;
    snprintf(countInfo, sizeof(countInfo), "%d-%d of %d Students", firstRec, lastRec, pagination->totalItems);
    spacing = tableWidth - (int)strlen(pageInfo) - (int)strlen(countInfo);
    if (spacing < 0) spacing = 0;
    printf("%*s%s", tblMargin, "", pageInfo);
    printf("%*s%s\n", spacing, "", countInfo);
    
    // Draw top border
    printf("%*s╔", tblMargin, "");
    for (int i = 0; i < tableWidth - 2; i++)
        printf("═");
    printf("╗╔");
    for (int i = 0; i < smlBoxWidth; i++)
        printf("═");
    printf("╗\n");
    
    // Draw title centered
    printf("%*s║%*s%s%*s║", tblMargin, "", tablepad, "", title, tablepad + extra, "");
    printf("║ %-22s ║\n", keys[0]);
    
    // Header separator
    printf("%*s╠", tblMargin, "");
    for (int i = 0; i < tableWidth - 2; i++){
        if (i == num || i == id || i == name || i == prog || i == yr || i == grd || i == rm)
            printf("╦");
        else
            printf("═");
    }
    printf("╣║ %-22s ║\n", keys[1]);
    
    // Column headers
    printf("%*s║ %-3s ║ %-10s ║ %-15s ║ %-7s ║ %-4s ║ %-11s ║ %-7s ║", 
            tblMargin, "",
            "NO.",
            "ID",
            "NAME",
            "PROGRAM",
            "YEAR",
            "FINAL GRADE",
            "REMARKS"
        );
    printf("║ %-22s ║\n", keys[2]);
    
    // Header bottom border
    printf("%*s╠", tblMargin, "");
    for (int i = 0; i < tableWidth - 2; i++){
        if (i == num || i == id || i == name || i == prog || i == yr || i == grd || i == rm)
            printf("╬");
        else
            printf("═");
    }
    printf("╣");
    printf("╚");
    for (int i = 0; i < smlBoxWidth; i++)
        printf("═");
    printf("╝\n");
}

/**
 * @brief Display a single student row in the table
 * @param student Pointer to student data
 * @param rowNumber Row number for display (1-indexed)
 * @param consoleWidth Current console width
 * @param tableWidth Width of the main table
 * @param tblMargin Left margin for centering
 */
void displayStudentTableRow(const Student* student, int rowNumber, int consoleWidth, int tableWidth, int tblMargin) {
    if (!student) return;
    
    // Handle long names by splitting
    char *fullName = (char*)student->personal.name.fullName;
    int nameLen = strlen(fullName);
    
    char name1[NAME_COL_WIDTH + 1] = "";
    char name2[NAME_COL_WIDTH + 1] = "";
    
    if (nameLen <= NAME_COL_WIDTH) {
        // Name fits in one line
        strncpy(name1, fullName, NAME_COL_WIDTH);
        name1[NAME_COL_WIDTH] = '\0';
        name2[0] = '\0';
    } else {
        // Find last space before NAME_COL_WIDTH to split nicely
        int split = NAME_COL_WIDTH;
        for (int j = NAME_COL_WIDTH; j > 0; j--) {
            if (fullName[j] == ' ') {
                split = j;
                break;
            }   
        }
        
        strncpy(name1, fullName, split);
        name1[split] = '\0';
        
        // Skip spaces after split for second line
        int startSecondLine = split;
        while (fullName[startSecondLine] == ' ') startSecondLine++;
        
        strncpy(name2, fullName + startSecondLine, NAME_COL_WIDTH);
        name2[NAME_COL_WIDTH] = '\0';
    }
    
    // Get final grade (use computed final grade or calculate if needed)
    float finalGrade = student->academic.finalGrade;
    if (finalGrade == 0.0f) {
        // Calculate if not already computed
        finalGrade = (student->academic.prelimGrade + student->academic.midtermGrade + student->academic.finalExamGrade) / 3.0f;
    }
    
    // Use existing remarks or determine based on passing grade
    char remarks[8];
    if (strlen(student->academic.remarks) > 0) {
        strncpy(remarks, student->academic.remarks, 7);
        remarks[7] = '\0';
    } else {
        extern float getPassingGrade(void);
        if (finalGrade >= getPassingGrade()) {
            strcpy(remarks, "Passed");
        } else {
            strcpy(remarks, "Failed");
        }
    }
    
    // Display first line of student data
    printf("%*s║  %-2d ║ %-10s ║ %-*s ║ %-7s ║  %-2d  ║    %-8.2f ║  %-6s ║\n",
        tblMargin, "",
        rowNumber,
        student->personal.studentNumber,
        NAME_COL_WIDTH, name1,
        student->personal.programCode,
        student->personal.yearLevel,
        finalGrade,
        remarks
    );
    
    // Display second line if name was split
    if (name2[0] != '\0'){
        printf("%*s║  %-2s ║ %-10s ║ %-*s ║ %-7s ║ %-4s ║    %-8s ║  %-6s ║\n",
            tblMargin, "",
            "",
            "", // empty ID
            NAME_COL_WIDTH, name2,
            "", "", "", ""); // empty other cells
    }
}

/**
 * @brief Display the footer of the student table
 * @param consoleWidth Current console width
 * @param tableWidth Width of the main table
 * @param tblMargin Left margin for centering
 */
void displayStudentTableFooter(int consoleWidth, int tableWidth, int tblMargin) {
    // Column positions
    int num = 5, id = 18, name = 36, prog = 46, yr = 53, grd = 67, rm = 77;
    
    printf("%*s╚", tblMargin, "");
    for (int i = 0; i < tableWidth - 2; i++){
        if (i == num || i == id || i == name || i == prog || i == yr || i == grd || i == rm)
            printf("╩");
        else
            printf("═");
    }
    printf("╝\n");
}

/**
 * @brief Display the complete student table with pagination
 * @param studentList Pointer to student list
 * @param pagination Pagination state
 */
void displayStudentTable(const list* studentList, PaginationState* pagination) {
    if (!studentList || !pagination) return;
    
    int consoleWidth, consoleHeight;
    getConsoleSize(&consoleWidth, &consoleHeight);
    
    int tableWidth = 79;
    int smlBoxWidth = 24;
    int tblMargin = (consoleWidth - (tableWidth + smlBoxWidth)) / 2;
    if (tblMargin < 0) tblMargin = 0;
    
    // Count actual items on current page first
    int actualItemsOnPage = 0;
    if (studentList->size > 0 && studentList->head) {
        node* current = studentList->head;
        int index = 0;
        
        // Count items in current page range
        do {
            if (index >= pagination->startIndex && index <= pagination->endIndex) {
                Student* student = (Student*)current->data;
                if (student) {
                    actualItemsOnPage++;
                }
            }
            current = current->next;
            index++;
        } while (current != studentList->head && current != NULL && index <= pagination->endIndex);
    }
    
    // Display header with actual count
    displayStudentTableHeader(consoleWidth, tableWidth, smlBoxWidth, tblMargin, pagination, actualItemsOnPage);
    
    // Display student rows
    if (studentList->size > 0 && studentList->head) {
        node* current = studentList->head;
        int index = 0;
        int displayedRows = 0;
        
        // Navigate to start position
        do {
            if (index >= pagination->startIndex && index <= pagination->endIndex) {
                Student* student = (Student*)current->data;
                if (student) {
                    displayStudentTableRow(student, index + 1, consoleWidth, tableWidth, tblMargin);
                    displayedRows++;
                }
            }
            current = current->next;
            index++;
        } while (current != studentList->head && current != NULL && index <= pagination->endIndex);
        
        // Fill remaining rows with empty space if needed
        for (int i = displayedRows; i < pagination->itemsPerPage; i++) {
            printf("%*s║  %-2s ║ %-10s ║ %-15s ║ %-7s ║ %-4s ║    %-8s ║  %-6s ║\n",
                tblMargin, "", "", "", "", "", "", "", "");
        }
    } else {
        // Display empty rows
        for (int i = 0; i < pagination->itemsPerPage; i++) {
            printf("%*s║  %-2s ║ %-10s ║ %-15s ║ %-7s ║ %-4s ║    %-8s ║  %-6s ║\n",
                tblMargin, "", "", "", "", "", "", "", "");
        }
    }
    
    // Display footer
    displayStudentTableFooter(consoleWidth, tableWidth, tblMargin);
}

/**
 * @brief Run the interactive student table view with pagination
 * @param studentList Pointer to student list to display
 * @return Returns 0 on normal exit
 */
int runStudentTableView(const list* studentList) {
    if (!studentList) {
        printf("Error: No student list provided!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Initialize pagination
    PaginationState pagination;
    initPagination(&pagination, studentList->size, STUDENTS_PER_PAGE);
    
    // Enable window input events for responsiveness
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hStdin, &prevMode);
    SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | prevMode);
    
    // Initial display
    winTermClearScreen();
    displayStudentTable(studentList, &pagination);
    
    INPUT_RECORD inputRecord;
    DWORD events;
    
    while (1) {
        // Wait for input event
        ReadConsoleInput(hStdin, &inputRecord, 1, &events);
        
        if (inputRecord.EventType == WINDOW_BUFFER_SIZE_EVENT) {
            // Console window resized - redraw
            winTermClearScreen();
            displayStudentTable(studentList, &pagination);
        } 
        else if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
            WORD key = inputRecord.Event.KeyEvent.wVirtualKeyCode;
            
            switch (key) {
                case VK_ESCAPE:
                    // Exit table view
                    goto exit_table;
                    
                case VK_LEFT:
                    // Previous page
                    if (pagination.currentPage > 1) {
                        updatePagination(&pagination, pagination.currentPage - 1);
                        winTermClearScreen();
                        displayStudentTable(studentList, &pagination);
                    }
                    break;
                    
                case VK_RIGHT:
                    // Next page
                    if (pagination.currentPage < pagination.totalPages) {
                        updatePagination(&pagination, pagination.currentPage + 1);
                        winTermClearScreen();
                        displayStudentTable(studentList, &pagination);
                    }
                    break;
                    
                case VK_HOME:
                    // First page
                    if (pagination.currentPage != 1) {
                        updatePagination(&pagination, 1);
                        winTermClearScreen();
                        displayStudentTable(studentList, &pagination);
                    }
                    break;
                    
                case VK_END:
                    // Last page
                    if (pagination.currentPage != pagination.totalPages) {
                        updatePagination(&pagination, pagination.totalPages);
                        winTermClearScreen();
                        displayStudentTable(studentList, &pagination);
                    }
                    break;
                    
                default:
                    // Ignore other keys
                    break;
            }
        }
    }
    
exit_table:
    // Restore original console mode
    SetConsoleMode(hStdin, prevMode);
    return 0;
}

// Employee Table Display Functions

/**
 * @brief Display the header portion of the employee table
 * @param consoleWidth Current console width
 * @param tableWidth Width of the main table
 * @param smlBoxWidth Width of the side box
 * @param tblMargin Left margin for centering
 * @param pagination Pagination state for page info
 * @param actualItemsOnPage Actual number of items displayed on current page
 */
void displayEmployeeTableHeader(int consoleWidth, int tableWidth, int smlBoxWidth, int tblMargin, PaginationState* pagination, int actualItemsOnPage) {
    char title[] = "EMPLOYEE RECORDS";
    char keys[3][30] = {
        " ⇄      Navigate pages",
        "Enter  View details", 
        "Esc    Return to menu"
    };
    
    int tablepad = (tableWidth - strlen(title)) / 2 - 1;
    int extra = (tableWidth - (int)strlen(title)) % 2;
    if (tablepad < 0) tablepad = 0;
    
    // Calculate responsive column widths
    int numWidth = 3;           // "NO." column width
    int empNumWidth = 10;       // "EMP NUMBER" column width
    int nameWidth = 15;         // "NAME" column width  
    int statusWidth = 8;        // "STATUS" column width
    
    // Calculate remaining width for salary column
    // Total format: "║ NO. ║ EMP NUMBER ║ NAME ║ STATUS ║ BASIC SALARY ║"
    // Width calculation: 2 + numWidth + 3 + empNumWidth + 3 + nameWidth + 3 + statusWidth + 3 + salaryWidth + 2
    int usedWidth = 2 + numWidth + 3 + empNumWidth + 3 + nameWidth + 3 + statusWidth + 3 + 2; // borders and separators
    int salaryWidth = tableWidth - usedWidth;
    if (salaryWidth < 12) salaryWidth = 12; // minimum width for salary column
    
    // Column separator positions (where ╦/╬ should be placed)
    int sep1 = 1 + numWidth + 1;                    // After NO. column
    int sep2 = sep1 + 1 + empNumWidth + 2;          // After EMP NUMBER column  
    int sep3 = sep2 + 1 + nameWidth + 2;            // After NAME column
    int sep4 = sep3 + 1 + statusWidth + 2;          // After STATUS column
    int sep5 = sep4 + 1 + salaryWidth + 4;          // After BASIC SALARY column
    
    // Build dynamic page info and count strings for employees
    char pageInfo[32];
    char countInfo[48];
    int spacing;

    snprintf(pageInfo, sizeof(pageInfo), "Page %d of %d", pagination->currentPage, pagination->totalPages);
    int firstRec = (pagination->startIndex) + 1;
    int lastRec = pagination->endIndex + 1;
    if (lastRec > pagination->totalItems) lastRec = pagination->totalItems;
    snprintf(countInfo, sizeof(countInfo), "%d-%d of %d Employees", firstRec, lastRec, pagination->totalItems);
    spacing = tableWidth - (int)strlen(pageInfo) - (int)strlen(countInfo);
    if (spacing < 0) spacing = 0;
    
    // Print page info followed by calculated spacing and count info, then new line
    printf("%*s%s", tblMargin, "", pageInfo);
    printf("%*s%s\n", spacing, "", countInfo);
    
    // Draw top border
    printf("%*s╔", tblMargin, "");
    for (int i = 0; i < tableWidth - 2; i++)
        printf("═");
    printf("╗╔");
    for (int i = 0; i < smlBoxWidth; i++)
        printf("═");
    printf("╗\n");
    
    // Draw title centered
    printf("%*s║%*s%s%*s║", tblMargin, "", tablepad, "", title, tablepad + extra, "");
    printf("║ %-22s ║\n", keys[0]);
    
    // Header separator
    printf("%*s╠", tblMargin, "");
    for (int i = 0; i < tableWidth - 2; i++){
        if (i == sep1 || i == sep2 || i == sep3 || i == sep4 || i == sep5)
            printf("╦");
        else
            printf("═");
    }
    printf("╣║ %-22s ║\n", keys[1]);
    
    // Column headers with dynamic widths
    printf("%*s║ %-*s ║ %-*s ║ %-*s ║ %-*s ║ %-*s ║", 
            tblMargin, "",
            numWidth, "NO.",
            empNumWidth, "EMP NUMBER", 
            nameWidth, "NAME",
            statusWidth, "STATUS",
            salaryWidth, "BASIC SALARY"
        );
    printf("║ %-22s ║\n", keys[2]);
    
    // Header bottom border
    printf("%*s╠", tblMargin, "");
    for (int i = 0; i < tableWidth - 2; i++){
        if (i == sep1 || i == sep2 || i == sep3 || i == sep4 || i == sep5)
            printf("╬");
        else
            printf("═");
    }
    printf("╣");
    printf("╚");
    for (int i = 0; i < smlBoxWidth; i++)
        printf("═");
    printf("╝\n");
}

/**
 * @brief Display a single employee row in the table
 * @param employee Pointer to employee data
 * @param rowNumber Row number for display (1-indexed)
 * @param consoleWidth Current console width
 * @param tableWidth Width of the main table
 * @param tblMargin Left margin for centering
 */
void displayEmployeeTableRow(const Employee* employee, int rowNumber, int consoleWidth, int tableWidth, int tblMargin) {
    if (!employee) return;
    
    // Calculate responsive column widths (same as header)
    int numWidth = 3;           
    int empNumWidth = 10;       
    int nameWidth = 15;         
    int statusWidth = 8;        
    
    // Calculate remaining width for salary column
    int usedWidth = 2 + numWidth + 3 + empNumWidth + 3 + nameWidth + 3 + statusWidth + 3 + 2; // borders and separators
    int salaryWidth = tableWidth - usedWidth + 2;
    if (salaryWidth < 12) salaryWidth = 12;
    
    // Handle long names by splitting
    char *fullName = (char*)employee->personal.name.fullName;
    int nameLen = strlen(fullName);
    
    char name1[nameWidth + 1];
    char name2[nameWidth + 1];
    memset(name1, 0, nameWidth + 1);
    memset(name2, 0, nameWidth + 1);
    
    if (nameLen <= nameWidth) {
        // Name fits in one line
        strncpy(name1, fullName, nameWidth);
        name1[nameWidth] = '\0';
        name2[0] = '\0';
    } else {
        // Find last space before nameWidth to split nicely
        int split = nameWidth;
        for (int j = nameWidth; j > 0; j--) {
            if (fullName[j] == ' ') {
                split = j;
                break;
            }   
        }
        
        strncpy(name1, fullName, split);
        name1[split] = '\0';
        
        // Skip spaces after split for second line
        int startSecondLine = split;
        while (fullName[startSecondLine] == ' ') startSecondLine++;
        
        strncpy(name2, fullName + startSecondLine, nameWidth);
        name2[nameWidth] = '\0';
    }
    
    // Format salary with proper width
    char salaryStr[salaryWidth + 1];
    memset(salaryStr, 0, salaryWidth + 1);
    snprintf(salaryStr, salaryWidth + 1, "₱%.2f", employee->payroll.basicPay);
    
    // Display first line of employee data
    printf("%*s║ %-*d ║ %-*s ║ %-*s ║ %-*s ║ %-*s ║\n",
        tblMargin, "",
        numWidth, rowNumber,
        empNumWidth, employee->personal.employeeNumber,
        nameWidth, name1,
        statusWidth, (employee->employment.status == statusRegular) ? "Regular" : "Casual",
        salaryWidth, salaryStr
    );
    
    // Display second line if name was split
    if (name2[0] != '\0'){
        printf("%*s║ %-*s ║ %-*s ║ %-*s ║ %-*s ║ %-*s ║\n",
            tblMargin, "",
            numWidth, "",
            empNumWidth, "", // empty employee number
            nameWidth, name2,
            statusWidth, "", // empty status
            salaryWidth, ""); // empty salary
    }
}

/**
 * @brief Display the footer of the employee table
 * @param consoleWidth Current console width
 * @param tableWidth Width of the main table
 * @param tblMargin Left margin for centering
 */
void displayEmployeeTableFooter(int consoleWidth, int tableWidth, int tblMargin) {
    // Calculate responsive column widths (same as header and row)
    int numWidth = 3;           
    int empNumWidth = 10;       
    int nameWidth = 15;         
    int statusWidth = 8;        
    
    // Calculate remaining width for salary column
    int usedWidth = 2 + numWidth + 3 + empNumWidth + 3 + nameWidth + 3 + statusWidth + 3 + 2; // borders and separators
    int salaryWidth = tableWidth - usedWidth;
    if (salaryWidth < 12) salaryWidth = 12;
    
    // Column separator positions (same as header)
    int sep1 = 1 + numWidth + 1;                    // After NO. column
    int sep2 = sep1 + 1 + empNumWidth + 2;          // After EMP NUMBER column  
    int sep3 = sep2 + 1 + nameWidth + 2;            // After NAME column
    int sep4 = sep3 + 1 + statusWidth + 2;          // After STATUS column
    int sep5 = sep4 + 1 + salaryWidth + 6;          // After BASIC SALARY column
    
    printf("%*s╚", tblMargin, "");
    for (int i = 0; i < tableWidth - 2; i++){
        if (i == sep1 || i == sep2 || i == sep3 || i == sep4 || i == sep5)
            printf("╩");
        else
            printf("═");
    }
    printf("╝\n");
}

/**
 * @brief Display the complete employee table with pagination
 * @param employeeList Pointer to employee list
 * @param pagination Pagination state
 */
void displayEmployeeTable(const list* employeeList, PaginationState* pagination) {
    if (!employeeList || !pagination) return;
    
    int consoleWidth, consoleHeight;
    getConsoleSize(&consoleWidth, &consoleHeight);
    
    int tableWidth = 79;
    int smlBoxWidth = 24;
    int tblMargin = (consoleWidth - (tableWidth + smlBoxWidth)) / 2;
    if (tblMargin < 0) tblMargin = 0;
    
    // Count actual items on current page first
    int actualItemsOnPage = 0;
    if (employeeList->size > 0 && employeeList->head) {
        node* current = employeeList->head;
        int index = 0;
        
        // Count items in current page range
        do {
            if (index >= pagination->startIndex && index <= pagination->endIndex) {
                Employee* employee = (Employee*)current->data;
                if (employee) {
                    actualItemsOnPage++;
                }
            }
            current = current->next;
            index++;
        } while (current != employeeList->head && current != NULL && index <= pagination->endIndex);
    }
    
    // Display header with actual count
    displayEmployeeTableHeader(consoleWidth, tableWidth, smlBoxWidth, tblMargin, pagination, actualItemsOnPage);
    
    // Display employee rows
    if (employeeList->size > 0 && employeeList->head) {
        node* current = employeeList->head;
        int index = 0;
        int displayedRows = 0;
        
        // Navigate to start position
        do {
            if (index >= pagination->startIndex && index <= pagination->endIndex) {
                Employee* employee = (Employee*)current->data;
                if (employee) {
                    displayEmployeeTableRow(employee, index + 1, consoleWidth, tableWidth, tblMargin);
                    displayedRows++;
                }
            }
            current = current->next;
            index++;
        } while (current != employeeList->head && current != NULL && index <= pagination->endIndex);
        
        // Fill remaining rows with empty space if needed
        for (int i = displayedRows; i < pagination->itemsPerPage; i++) {
            // Calculate responsive column widths for empty rows
            int numWidth = 3, empNumWidth = 10, nameWidth = 15, statusWidth = 8;
            int usedWidth = 2 + numWidth + 3 + empNumWidth + 3 + nameWidth + 3 + statusWidth + 3 + 2;
            int salaryWidth = tableWidth - usedWidth;
            if (salaryWidth < 12) salaryWidth = 12;
            
            printf("%*s║ %-*s ║ %-*s ║ %-*s ║ %-*s ║ %-*s ║\n",
                tblMargin, "", 
                numWidth, "",
                empNumWidth, "", 
                nameWidth, "",
                statusWidth, "",
                salaryWidth, "");
        }
    } else {
        // Display empty rows
        for (int i = 0; i < pagination->itemsPerPage; i++) {
            // Calculate responsive column widths for empty rows
            int numWidth = 3, empNumWidth = 10, nameWidth = 15, statusWidth = 8;
            int usedWidth = 2 + numWidth + 3 + empNumWidth + 3 + nameWidth + 3 + statusWidth + 3 + 2;
            int salaryWidth = tableWidth - usedWidth;
            if (salaryWidth < 12) salaryWidth = 12;
            
            printf("%*s║ %-*s ║ %-*s ║ %-*s ║ %-*s ║ %-*s ║\n",
                tblMargin, "", 
                numWidth, "",
                empNumWidth, "", 
                nameWidth, "",
                statusWidth, "",
                salaryWidth, "");
        }
    }
    
    // Display footer
    displayEmployeeTableFooter(consoleWidth, tableWidth, tblMargin);
}

/**
 * @brief Run the interactive employee table view with pagination
 * @param employeeList Pointer to employee list to display
 * @return Returns 0 on normal exit
 */
int runEmployeeTableView(const list* employeeList) {
    if (!employeeList) {
        printf("Error: No employee list provided!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Initialize pagination
    PaginationState pagination;
    initPagination(&pagination, employeeList->size, EMPLOYEES_PER_PAGE);
    
    // Enable window input events for responsiveness
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hStdin, &prevMode);
    SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | prevMode);
    
    // Initial display
    winTermClearScreen();
    displayEmployeeTable(employeeList, &pagination);
    
    INPUT_RECORD inputRecord;
    DWORD events;
    
    while (1) {
        // Wait for input event
        ReadConsoleInput(hStdin, &inputRecord, 1, &events);
        
        if (inputRecord.EventType == WINDOW_BUFFER_SIZE_EVENT) {
            // Console window resized - redraw
            winTermClearScreen();
            displayEmployeeTable(employeeList, &pagination);
        } 
        else if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
            WORD key = inputRecord.Event.KeyEvent.wVirtualKeyCode;
            
            switch (key) {
                case VK_ESCAPE:
                    // Exit table view
                    goto exit_emp_table;
                    
                case VK_LEFT:
                    // Previous page
                    if (pagination.currentPage > 1) {
                        updatePagination(&pagination, pagination.currentPage - 1);
                        winTermClearScreen();
                        displayEmployeeTable(employeeList, &pagination);
                    }
                    break;
                    
                case VK_RIGHT:
                    // Next page
                    if (pagination.currentPage < pagination.totalPages) {
                        updatePagination(&pagination, pagination.currentPage + 1);
                        winTermClearScreen();
                        displayEmployeeTable(employeeList, &pagination);
                    }
                    break;
                    
                case VK_HOME:
                    // First page
                    if (pagination.currentPage != 1) {
                        updatePagination(&pagination, 1);
                        winTermClearScreen();
                        displayEmployeeTable(employeeList, &pagination);
                    }
                    break;
                    
                case VK_END:
                    // Last page
                    if (pagination.currentPage != pagination.totalPages) {
                        updatePagination(&pagination, pagination.totalPages);
                        winTermClearScreen();
                        displayEmployeeTable(employeeList, &pagination);
                    }
                    break;
                    
                default:
                    // Ignore other keys
                    break;
            }
        }
    }
    
exit_emp_table:
    // Restore original console mode
    SetConsoleMode(hStdin, prevMode);
    return 0;
} 