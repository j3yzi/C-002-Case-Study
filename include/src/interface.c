#include <stdio.h>
#include <conio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include "../headers/interface.h"
#include "../headers/apctxt.h"
#include "../headers/state.h"

// Constants
#define MENU_HEADER_WIDTH 60  // Constant width for menu headers

static HANDLE hConsole;
static CONSOLE_SCREEN_BUFFER_INFO originalConsoleInfo;

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
    char dateString[10];
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
    // Calculate padding to center the menu name
    int namePadding = (totalMenuNameWidth - 2 - menuNameLength) / 2;
    
    for (int i = 0; i < namePadding; i++) {
        printf(" ");
    }
    
    printf("%s", menu->name);
    
    // Add extra padding if needed for odd-length names
    int rightPadding = totalMenuNameWidth - 2 - menuNameLength - namePadding;
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
    char dateString[10];
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
    
    // Display the prompt with simple formatting
    printf("%s (Y/N): ", prompt);
    fflush(stdout); // Ensure the prompt is displayed
    
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
    
    // This is a generic template - specific implementations should override this
    // based on their menu structure
    
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
    
    // Employee-specific overrides can be added here
    // For example, payroll reports might have special requirements
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