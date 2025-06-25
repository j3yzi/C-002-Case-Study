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
    
    // Force the screen buffer to be flushed completely before redrawing
    fflush(stdout);
    
    // Set console to UTF-8 encoding (in a more robust way)
    SetConsoleOutputCP(65001); // Set output to UTF-8 (CP_UTF8)
    
    // Box dimensions matching menuui.c style
    int box1w = 42; // menu box width
    int box2w = 28; // side box width (increased to accommodate longer descriptions)
    
    // Title text
    char title1[] = "MENU";
    char title2[] = "KEYS";
    
    // Calculate padding for centered titles
    int pad1 = (box1w - strlen(title1)) / 2;
    int pad2 = (box2w - strlen(title2)) / 2;
    
    // Get selected menu option for description
    int selectedIndex = 0;
    for (int i = 0; i < menu->optionCount; i++) {
        if (menu->options[i].isSelected) {
            selectedIndex = i;
            break;
        }
    }
    
    // Navigation shortcuts
    char shortcuts[3][30] = {
        " ↑↓     Move selection ",
        "Enter   Select",
        " ESC    Exit"
    };
    
    // Get description from selected option, with fallback
    const char* description = menu->options[selectedIndex].description;
    if (!description || strlen(description) == 0) {
        description = "Select this option to proceed.";
    }
    
    // Split description into two lines with simple, robust wrapping
    char line1[100] = "";
    char line2[100] = "";
    int maxlinelen = 26; // Increased width to match wider side box
    
    // Initialize both lines to empty
    line1[0] = '\0';
    line2[0] = '\0';
    
    if (strlen(description) <= maxlinelen) {
        // Description fits in one line
        strcpy(line1, description);
    } else {
        // Find a good split point
        int splitPos = -1;
        
        // Look for the last space before or at maxlinelen
        for (int i = maxlinelen; i >= 8; i--) {
            if (i < strlen(description) && description[i] == ' ') {
                splitPos = i;
                break;
            }
        }
        
        if (splitPos > 0) {
            // Split at the space
            strncpy(line1, description, splitPos);
            line1[splitPos] = '\0';
            
            // Rest goes to second line
            strcpy(line2, description + splitPos + 1);
            
            // If second line is too long, truncate it
            if (strlen(line2) > maxlinelen) {
                line2[maxlinelen - 3] = '\0';
                strcat(line2, "...");
            }
        } else {
            // No good split point, just truncate
            strncpy(line1, description, maxlinelen - 3);
            line1[maxlinelen - 3] = '\0';
            strcat(line1, "...");
        }
    }
    
    // Top boxes borders
    printf("╔");
    for (int i = 0; i <= box1w; i++) 
        printf("═");
    printf("╗");

    printf("╔");
    for (int i = 0; i <= box2w; i++) 
        printf("═");
    printf("╗\n");

    // Box titles
    printf("║%*s%s%*s ║", pad1, "", title1, pad1, " "); // menu
    printf("║ %*s%s%*s║\n", pad2, " ", title2, box2w - pad2 - strlen(title2), ""); // keys

    // Title dividers
    printf("╠");
    for (int i = 0; i <= box1w; i++) 
        printf("═");
    printf("╣");

    printf("╠");
    for (int i = 0; i <= box2w; i++) 
        printf("═");
    printf("╣\n");

    // Display menu options - ensure we display exactly the number of options in the menu
    int maxRows = menu->optionCount > 8 ? menu->optionCount : 8; // Minimum 8 rows for side boxes
    
    for (int i = 0; i < maxRows; i++) {
        // Left menu box border and content
        printf("║");
        
        if (i < menu->optionCount) {
            // Display actual menu option with proper spacing
            if (menu->options[i].isDisabled) {
                appMenuSetColor(menu->options[i].disabledTextColor, menu->options[i].disabledBgColor);
                printf("   %-38s", menu->options[i].text);
            } else if (menu->options[i].isSelected) {
                appMenuSetColor(menu->options[i].highlightTextColor, menu->options[i].highlightBgColor);
                printf(" → %-37s", menu->options[i].text);
            } else {
                appMenuSetColor(menu->options[i].textColor, menu->options[i].bgColor);
                printf("   %-38s", menu->options[i].text);
            }
            winTermResetColors();
        } else {
            // Empty row for side box alignment
            printf("%-41s", "");
        }

        // Right side box content based on row position
        if (i == 3) { // Bottom border of keys box
            printf(" ║╚");
            for (int j = 0; j <= box2w; j++)
                printf("═"); 
            printf("╝\n");
        }
        else if (i == 4) { // Top border of description box
            printf(" ║╔");
            for (int j = 0; j <= box2w; j++)
                printf("═"); 
            printf("╗\n");
        }
        else if (i < 3) {
            // Keys box content - ensure perfect box alignment
            printf(" ║║ %-*s║\n", box2w - 1, shortcuts[i]);
        }
        else if (i == 5) {
            // First line of description
            printf(" ║║ %-*s║\n", box2w - 1, line1);
        }
        else if (i == 6) {
            // Second line of description
            printf(" ║║ %-*s║\n", box2w - 1, line2);
        }
        else if (i == 7) {
            // Empty description line
            printf(" ║║ %-*s║\n", box2w - 1, "");
        }
        else {
            // Empty right side for additional menu rows
            printf(" ║║ %-*s║\n", box2w - 1, "");
        }
    }

    // Bottom borders for menu box
    printf("╚");
    for (int i = 0; i <= box1w; i++) 
        printf("═");
    printf("╝");

    printf("╚");
    for (int i = 0; i <= box2w; i++) 
        printf("═");
    printf("╝\n");

    // Bottom information box - sized to fit error messages
    int fullboxw = box1w + box2w + 4; // Adjusted for proper spacing
    printf("╔");
    for (int i = 0; i < fullboxw; i++) 
        printf("═");
    printf("╗\n");

    // Make space for longer messages with proper padding
    printf("║%-*s║\n", fullboxw, " ");

    printf("╚");
    for (int i = 0; i < fullboxw; i++) 
        printf("═");
    printf("╝\n");
    
    // Force flush of all content to ensure complete rendering
    fflush(stdout);
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
    
    // Position cursor at the start of the menu option area (after "║")
    winTermSetCursor(1, y);
    
    // Start with the box border
    printf("║");
    
    MenuOption* option = &menu->options[optionIndex];
    
    if (option->isDisabled) {
        appMenuSetColor(option->disabledTextColor, option->disabledBgColor);
        printf("   %-38s", option->text);
    } else if (option->isSelected) {
        appMenuSetColor(option->highlightTextColor, option->highlightBgColor);
        printf(" → %-37s", option->text);
    } else {
        appMenuSetColor(option->textColor, option->bgColor);
        printf("   %-38s", option->text);
    }
    winTermResetColors();
    
    // Add the right border to ensure the box stays intact
    printf(" ║");
}

/**
 * @brief Calculates the number of header lines in a menu based on the box-drawing format
 * @param menu A pointer to the Menu struct
 * @return The number of header lines
 */
static int calculateMenuHeaderLines(const Menu* menu) {
    // If menu is NULL, return a default value
    if (!menu) return 3;
    
    // Box-drawing menu format components:
    int headerLines = 0;
    headerLines += 1; // Top border "╔═══╗╔═══╗"
    headerLines += 1; // Title line "║MENU║║KEYS║"
    headerLines += 1; // Separator "╠═══╣╠═══╣"
    
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
    
    // Update the menu options selection state
    menu->options[oldSelection].isSelected = false;
    menu->options[newSelection].isSelected = true;
    
    // Calculate positioning for the menu options
    int headerLines = calculateMenuHeaderLines(menu);
    int menuStartY = headerLines;
    
    // Update only the options that changed by directly positioning the cursor
    // and redrawing just those specific lines
    appDisplayMenuOption(menu, oldSelection, 1, menuStartY + oldSelection);
    appDisplayMenuOption(menu, newSelection, 1, menuStartY + newSelection);
    
    // Update the description box when selection changes
    const char* description = menu->options[newSelection].description;
    if (!description || strlen(description) == 0) {
        description = "Select this option to proceed.";
    }
    
    // Split description into two lines with simple, robust wrapping
    char line1[100] = "";
    char line2[100] = "";
    int maxlinelen = 26;
    
    // Initialize both lines to empty
    line1[0] = '\0';
    line2[0] = '\0';
    
    if (strlen(description) <= maxlinelen) {
        // Description fits in one line
        strcpy(line1, description);
    } else {
        // Find a good split point
        int splitPos = -1;
        
        // Look for the last space before or at maxlinelen
        for (int i = maxlinelen; i >= 8; i--) {
            if (i < strlen(description) && description[i] == ' ') {
                splitPos = i;
                break;
            }
        }
        
        if (splitPos > 0) {
            // Split at the space
            strncpy(line1, description, splitPos);
            line1[splitPos] = '\0';
            
            // Rest goes to second line
            strcpy(line2, description + splitPos + 1);
            
            // If second line is too long, truncate it
            if (strlen(line2) > maxlinelen) {
                line2[maxlinelen - 3] = '\0';
                strcat(line2, "...");
            }
        } else {
            // No good split point, just truncate
            strncpy(line1, description, maxlinelen - 3);
            line1[maxlinelen - 3] = '\0';
            strcat(line1, "...");
        }
    }
    
    // Update description box content (rows 5 and 6)
    winTermSetCursor(43, menuStartY + 5);
    printf("║ %-*s║", 26, line1);
    
    winTermSetCursor(43, menuStartY + 6);
    printf("║ %-*s║", 26, line2);
}

/**
 * @brief Displays an error message at the specified position without clearing the entire screen.
 * @param message The error message to display.
 * @param errorY The Y position where the error should be displayed.
 */
static void appDisplayErrorMessage(const char* message, int errorY) {
    if (!message) return;
    
    // Position cursor at the bottom information box
    winTermSetCursor(1, errorY);
    
    // Clear just this line to prepare for the error message
    winTermClearLine();
    
    // Get maximum display width for the message
    int box1w = 42; // menu box width
    int box2w = 28; // side box width
    int fullboxw = box1w + box2w + 4; // Full width with proper spacing
    
    // Format message to fit within the box with proper padding
    // Format: ║ ⚠ Error message... ║
    printf("║ ⚠ %-*s ║", fullboxw - 5, message);
}

/**
 * @brief Clears the error message at the specified position.
 * @param errorY The Y position where the error message is displayed.
 */
static void appClearErrorMessage(int errorY) {
    // Position cursor at the error message line
    winTermSetCursor(1, errorY);
    
    // Clear the entire line using winTermClearLine function
    winTermClearLine();
    
    // Get maximum display width for the cleared space
    int box1w = 42; // menu box width
    int box2w = 28; // side box width
    int fullboxw = box1w + box2w + 3; // Full width minus borders and spacing
    
    // Redraw the box border characters
    printf("║%-*s║", fullboxw, " ");
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
    
    // Ensure all options are marked as not selected initially
    for (int i = 0; i < m->optionCount; i++) {
        m->options[i].isSelected = false;
    }
    
    // Find the first non-disabled option for initial selection
    int selected = 0;
    while (selected < m->optionCount && m->options[selected].isDisabled) {
        selected++;
    }
    
    // If all options are disabled, default to first option
    if (selected >= m->optionCount) {
        selected = 0;
    }
    
    // Mark only the selected option as selected for highlighting
    m->options[selected].isSelected = true;

    // Initial menu display (only once!)
    appMenuSetColor(7, 0);
    winTermClearScreen(); // Clear once at the beginning
    appDisplayMenu(m);
    
    int key;
    char errorMessage[100] = "";
    int showError = 0;
    int needsFullRedraw = 0; // Only used when absolutely necessary
    
    // Calculate error message position to match menu positioning
    int headerLines = calculateMenuHeaderLines(m);
    int menuStartY = headerLines;
    int maxRows = m->optionCount > 8 ? m->optionCount : 8;
    int errorY = menuStartY + maxRows + 3; // Position in the bottom information box

    while (true) {
        // Only redraw completely if needed (rarely should be needed)
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

        // Handle arrow keys - use targeted updates for efficient navigation
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
                
                // Clear any previous error message
                appClearErrorMessage(errorY);
                
                // Update only the changed menu items (targeted update!)
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
                
                // Clear any previous error message
                appClearErrorMessage(errorY);
                
                // Update only the changed menu items (targeted update!)
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
        
        // Handle Escape key to exit menu if needed
        if (key == 27) { // ESC key
            // If there's a default exit option (usually the last one), return that
            if (m->optionCount > 0) {
                char exitKey = m->options[m->optionCount-1].key;
                return exitKey;
            }
            return 0; // No options, just return 0
        }
        
        // Handle invalid key press
        if (key != 0 && key != -32 && key != 224 && key != 13 && key != 27) {
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
    
    // Display the prompt - make sure it's visible with appropriate formatting
    winTermClearLine();
    printf("%s%s%s\n", UI_PROMPT, prompt, TXT_RESET);
    fflush(stdout); // Ensure the prompt is displayed
    
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