#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>
#include "../headers/apctxt.h"
#include "../headers/apclrs.h"

// Forward declarations for static internal functions
static void appDisplayMenuOption(const Menu* menu, int optionIndex, int x, int y);
static void appUpdateMenuSelection(Menu* menu, int oldSelection, int newSelection);
static void appDisplayErrorMessage(const char* message, int errorY);
static void appClearErrorMessage(int errorY);
static int calculateMenuHeaderLines(const Menu* menu);

/**
 * @brief Sets the text and background colors for the console.
 * @param textColor The desired text color (0-15).
 * @param bgColor The desired background color (0-15).
 */
void appMenuSetColor(int textColor, int bgColor) {
    // Use Windows API for direct color control
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textColor + (bgColor * 16));
}

/**
 * @brief Displays a complete menu on the screen.
 * @brief This function clears the screen and then iterates through all menu options,
 * @brief printing them with appropriate highlighting or disabled status.
 * @param menu A const pointer to the Menu struct to be displayed.
 */
void appDisplayMenu(const Menu* menu) {
    // Validate menu pointer
    if (!menu) {
        printf("Error: NULL menu pointer passed to appDisplayMenu\n");
        return;
    }
    
    winTermClearScreen();

    // Display header with dynamic width based on title length
    int titleWidth = 0;
    
    // Calculate the maximum width needed for the title
    if (menu->name) {
        const char* ptr = menu->name;
        int currentLineWidth = 0;
        
        while (*ptr) {
            if (*ptr == '\n') {
                // Check if this line is wider than previous ones
                if (currentLineWidth > titleWidth) {
                    titleWidth = currentLineWidth;
                }
                currentLineWidth = 0;
            } else {
                currentLineWidth++;
            }
            ptr++;
        }
        
        // Check the last line
        if (currentLineWidth > titleWidth) {
            titleWidth = currentLineWidth;
        }
    }
    
    // Ensure minimum width
    if (titleWidth < 20) {
        titleWidth = 20;
    }
    
    // Add some padding
    titleWidth += 4;
    
    // Print top separator
    printf("%s", UI_HEADER);
    for (int i = 0; i < titleWidth; i++) {
        printf("=");
    }
    printf("\n");
    
    // Print title (supporting multi-line titles)
    if (menu->name) {
        const char* ptr = menu->name;
        while (*ptr) {
            printf("%c", *ptr);
            ptr++;
        }
        printf("\n");
    } else {
        printf("Menu\n");
    }
    
    // Print bottom separator
    for (int i = 0; i < titleWidth; i++) {
        printf("=");
    }
    printf("\n\n%s", TXT_RESET);
    
    // Print menu separator
    for (int i = 0; i < titleWidth; i++) {
        printf("~");
    }
    printf("\n");

    for (int i = 0; i < menu->optionCount; i++) {
        if (menu->options[i].isDisabled) {
            appMenuSetColor(menu->options[i].disabledTextColor, menu->options[i].disabledBgColor);
            printf(" %s (Disabled)\n", menu->options[i].text);
            continue;
        } else if (menu->options[i].isSelected) {
            appMenuSetColor(menu->options[i].highlightTextColor, menu->options[i].highlightBgColor);
            printf(">> %s\n", menu->options[i].text);
        } else {
            appMenuSetColor(menu->options[i].textColor, menu->options[i].bgColor);
            printf(" %s\n", menu->options[i].text);
        }
    }
    
    winTermResetColors(); // Reset colors to default
}

/**
 * @brief Displays a single menu option at a specific screen location.
 * @param menu A const pointer to the Menu struct containing the option.
 * @param optionIndex The index of the option to display.
 * @param x The x-coordinate (column) where the option should be displayed.
 * @param y The y-coordinate (row) where the option should be displayed.
 */
static void appDisplayMenuOption(const Menu* menu, int optionIndex, int x, int y) {
    // Validate parameters
    if (!menu || optionIndex < 0 || optionIndex >= menu->optionCount) return;
    
    winTermSetCursor(x, y);
    winTermClearLine();
    
    MenuOption* option = &menu->options[optionIndex];
    
    if (option->isDisabled) {
        appMenuSetColor(option->disabledTextColor, option->disabledBgColor);
        printf(" %s (Disabled)", option->text);
    } else if (option->isSelected) {
        appMenuSetColor(option->highlightTextColor, option->highlightBgColor);
        printf(">> %s", option->text);
    } else {
        appMenuSetColor(option->textColor, option->bgColor);
        printf(" %s", option->text);
    }
    
    // Reset to default colors
    winTermResetColors();
}

/**
 * @brief Calculates the number of header lines in a menu based on the standard format
 * @param menu A pointer to the Menu struct
 * @return The number of header lines
 */
static int calculateMenuHeaderLines(const Menu* menu) {
    // If menu is NULL, return a default value
    if (!menu) return 5;
    
    // Standard menu format components:
    int headerLines = 0;
    headerLines += 1; // "====" separator line
    
    // Count lines in the menu title (if it contains newlines)
    if (menu->name) {
        int titleLines = 1; // At least one line
        const char* ptr = menu->name;
        
        // Count newlines in the title
        while (*ptr) {
            if (*ptr == '\n') {
                titleLines++;
            }
            ptr++;
        }
        
        headerLines += titleLines;
    } else {
        headerLines += 1; // Default title line if no title
    }
    
    headerLines += 1; // "====" separator line
    headerLines += 1; // Blank line
    headerLines += 1; // "~~~~~" separator line
    
    return headerLines;
}

/**
 * @brief Updates the visual selection of the menu, highlighting the new option.
 * @brief This is used for interactive menus where the user can navigate with arrow keys.
 * @param menu A pointer to the Menu struct being updated.
 * @param oldSelection The index of the previously selected option.
 * @param newSelection The index of the newly selected option.
 */
static void appUpdateMenuSelection(Menu* menu, int oldSelection, int newSelection) {
    // Validate parameters
    if (!menu) return;
    if (oldSelection < 0 || oldSelection >= menu->optionCount) return;
    if (newSelection < 0 || newSelection >= menu->optionCount) return;
    
    winTermCursorPos pos;
    winTermGetCursorPosition(&pos);
    
    // Calculate the Y positions dynamically based on the menu header format
    int headerLines = calculateMenuHeaderLines(menu);
    int menuStartY = headerLines;
    int oldY = menuStartY + oldSelection;
    int newY = menuStartY + newSelection;
    
    // Update the old selection (removing the selection indicator)
    menu->options[oldSelection].isSelected = false;
    appDisplayMenuOption(menu, oldSelection, 0, oldY);
    
    // Update the new selection (adding the selection indicator)
    menu->options[newSelection].isSelected = true;
    appDisplayMenuOption(menu, newSelection, 0, newY);
    
    // Restore cursor position (move it below the menu for cleaner appearance)
    winTermSetCursor(0, menuStartY + menu->optionCount + 2);
}

/**
 * @brief Displays an error message at the specified position without clearing the entire screen.
 * @param message The error message to display.
 * @param errorY The Y position where the error should be displayed.
 */
static void appDisplayErrorMessage(const char* message, int errorY) {
    if (!message) return;
    
    winTermSetCursor(0, errorY);
    winTermClearLine();
    printf("%s⚠ %s%s", UI_ERROR, message, TXT_RESET);
}

/**
 * @brief Clears the error message at the specified position.
 * @param errorY The Y position where the error message is displayed.
 */
static void appClearErrorMessage(int errorY) {
    winTermSetCursor(0, errorY);
    winTermClearLine();
}

/**
 * @brief Initializes and runs an interactive menu.
 * @brief This function handles user input (arrow keys, enter, character keys) to navigate
 * @brief and select options from the given menu. It calls the `onSelect` callback for the chosen option.
 * @param m A pointer to the Menu struct to be initialized.
 * @return The character key of the selected menu option, or 0 if error.
 */
char initMenu(Menu* m) {
    // Validate menu pointer
    if (!m) {
        printf("%sError: NULL menu pointer passed to initMenu%s\n", UI_ERROR, TXT_RESET);
        return 0;
    }
    
    // Validate menu has options
    if (m->optionCount <= 0 || !m->options) {
        printf("%sError: Menu has no options%s\n", UI_ERROR, TXT_RESET);
        return 0;
    }
    
    // Find the first non-disabled option for initial selection
    int selected = 0;
    
    // Reset all selections first
    for (int i = 0; i < m->optionCount; i++) {
        m->options[i].isSelected = false;
    }
    
    // Find first available (non-disabled) option
    while (selected < m->optionCount && m->options[selected].isDisabled) {
        selected++;
    }
    
    // If all options are disabled, default to first option
    if (selected >= m->optionCount) {
        selected = 0;
    }
    
    // Mark the selected option as selected for highlighting
    if (selected < m->optionCount) {
        m->options[selected].isSelected = true;
    }

    // Initial menu display (only once!)
    appMenuSetColor(7, 0);
    appDisplayMenu(m);
    
    int key;
    char errorMessage[100] = "";
    int showError = 0;
    int needsFullRedraw = 0;
    
    // Calculate error message position to match menu positioning
    int headerLines = calculateMenuHeaderLines(m);
    int menuStartY = headerLines;
    int errorY = menuStartY + m->optionCount + 2; // After menu items

    while (true) {
        // Only redraw completely if needed
        if (needsFullRedraw) {
            appMenuSetColor(7, 0);
            appDisplayMenu(m);
            needsFullRedraw = 0;
        }
        
        // Display error message at fixed position if needed
        if (showError) {
            appDisplayErrorMessage(errorMessage, errorY);
            showError = 0;
        }
        
        // Get user input
        key = _getch();

        // Check for direct key matches
        for (int i = 0; i < m->optionCount; i++) {
            if (key == m->options[i].key && !m->options[i].isDisabled) {
                // If the key matches this option, trigger its action
                if (m->options[i].onSelect != NULL) {
                    m->options[i].onSelect();
                }
                return m->options[i].key; 
            } else if (key == m->options[i].key && m->options[i].isDisabled) {
                // If the key matches a disabled option, show error
                snprintf(errorMessage, sizeof(errorMessage), 
                         "Option '%c - %s' is currently disabled.", 
                         m->options[i].key, m->options[i].text);
                showError = 1;
                continue;
            }
        }

        // Handle arrow keys - use selective update for smooth navigation
        if (key == 0 || key == -32 || key == 224) {
            key = _getch(); // Get the actual key code
            
            if (key == 72 || key == 75) { // Up/Left arrow
                int oldSelected = selected; 
                selected--;
                if (selected < 0) selected = m->optionCount - 1;
                
                // Skip disabled options
                int loopGuard = 0;
                while (m->options[selected].isDisabled && loopGuard < m->optionCount) {
                    selected--;
                    if (selected < 0) selected = m->optionCount - 1;
                    loopGuard++;
                }
                
                // If all options are disabled, keep the original selection
                if (loopGuard >= m->optionCount) {
                    selected = oldSelected;
                    snprintf(errorMessage, sizeof(errorMessage), 
                             "All options are disabled!");
                    showError = 1;
                    continue;
                }
                
                // Clear any previous error message and update selection
                appClearErrorMessage(errorY);
                appUpdateMenuSelection(m, oldSelected, selected);
            }
            else if (key == 80 || key == 77) { // Down/Right arrow
                int oldSelected = selected;
                selected++;
                if (selected >= m->optionCount) selected = 0;
                
                // Skip disabled options
                int loopGuard = 0;
                while (m->options[selected].isDisabled && loopGuard < m->optionCount) {
                    selected++;
                    if (selected >= m->optionCount) selected = 0;
                    loopGuard++;
                }
                
                // If all options are disabled, keep the original selection
                if (loopGuard >= m->optionCount) {
                    selected = oldSelected;
                    snprintf(errorMessage, sizeof(errorMessage), 
                             "All options are disabled!");
                    showError = 1;
                    continue;
                }
                
                // Clear any previous error message and update selection
                appClearErrorMessage(errorY);
                appUpdateMenuSelection(m, oldSelected, selected);
            }
            // Continue to skip the invalid key check for arrow keys
            continue;
        }

        // Process Enter key
        if (key == 13) {
            if (m->options[selected].isDisabled) {
                snprintf(errorMessage, sizeof(errorMessage), 
                         "Option '%c - %s' is currently disabled.", 
                         m->options[selected].key, m->options[selected].text);
                showError = 1;
                continue;
            }
            // Execute the menu option's callback function if it exists
            if (m->options[selected].onSelect != NULL) {
                m->options[selected].onSelect();
            } 
            
            // Reset selection state for next time menu is shown
            for (int i = 0; i < m->optionCount; i++) {
                m->options[i].isSelected = false;
            }
            
            return m->options[selected].key; // Return the key of the selected option
        }
        
        // Handle invalid key press
        if (key != 0 && key != -32 && key != 224 && key != 13) {
            // Check if it's not a valid menu option key
            int validKey = 0;
            for (int i = 0; i < m->optionCount; i++) {
                if (key == m->options[i].key) {
                    validKey = 1;
                    break;
                }
            }
            if (!validKey) {
                snprintf(errorMessage, sizeof(errorMessage), 
                         "Invalid option '%c'. Please select a valid menu option.", key);
                showError = 1;
            }
        }
    }
}

/**
 * @brief Displays a Yes/No prompt and allows selection with arrow keys
 * @param prompt The question to display to the user
 * @return true if Yes was selected, false if No was selected
 */
bool appYesNoPrompt(const char* prompt) {
    if (!prompt) return false;
    
    bool isYesSelected = true;
    int key;
    
    // Calculate position for the prompt
    winTermCursorPos pos;
    winTermGetCursorPosition(&pos);
    
    // Display the prompt
    printf("%s%s%s\n", UI_PROMPT, prompt, TXT_RESET);
    
    // Display initial options
    while (true) {
        // Display options with current selection
        winTermSetCursor(0, pos.y + 1);
        winTermClearLine();
        
        // Display Yes option
        if (isYesSelected) {
            printf("%s[*] Yes %s", UI_SUCCESS, TXT_RESET);
        } else {
            printf("[ ] Yes ");
        }
        
        // Display No option
        if (!isYesSelected) {
            printf("%s[*] No%s", UI_ERROR, TXT_RESET);
        } else {
            printf("[ ] No");
        }
        
        // Get user input
        key = _getch();
        
        // Handle arrow keys
        if (key == 0 || key == -32 || key == 224) {
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