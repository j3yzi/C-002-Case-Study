#include "../headers/apctxt.h"

void appMenuSetColor(int textColor, int bgColor);
void appDisplayMenu(const Menu* menu);
static void appDisplayMenuOption(const Menu* menu, int optionIndex, int x, int y);
static void appUpdateMenuSelection(Menu* menu, int oldSelection, int newSelection);
char initMenu(Menu* m);

/**
 * @brief Sets the text and background colors for the console.
 * @param textColor The desired text color (0-15).
 * @param bgColor The desired background color (0-15).
 */
void appMenuSetColor(int textColor, int bgColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textColor + (bgColor * 16));
}

/**
 * @brief Displays a complete menu on the screen.
 * @brief This function clears the screen and then iterates through all menu options,
 * @brief printing them with appropriate highlighting or disabled status.
 * @param menu A const pointer to the Menu struct to be displayed.
 */
void appDisplayMenu(const Menu* menu) {
    winTermClearScreen();

    printf("====================================\n");
    printf("%s\n", menu->name);
    printf("====================================\n\n");
    
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    for (int i = 0; i < menu->optionCount; i++) {
        if (menu->options[i].isDisabled) {
            appMenuSetColor(menu->options[i].disabledTextColor, menu->options[i].disabledBgColor); // Gray for disabled options
            printf(" %s (Disabled)\n", menu->options[i].text);
            continue;
        } else if (menu->options[i].isSelected) {
            appMenuSetColor(menu->options[i].highlightTextColor, menu->options[i].highlightBgColor);
            printf(">> %s\n", menu->options[i].text);
        } else {
            appMenuSetColor(menu->options[i].textColor, menu->options[i].bgColor); // Default color
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
    if (optionIndex < 0 || optionIndex >= menu->optionCount) return;
    
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
 * @brief Updates the visual selection of the menu, highlighting the new option.
 * @brief This is used for interactive menus where the user can navigate with arrow keys.
 * @param menu A pointer to the Menu struct being updated.
 * @param oldSelection The index of the previously selected option.
 * @param newSelection The index of the newly selected option.
 */
static void appUpdateMenuSelection(Menu* menu, int oldSelection, int newSelection) {
    winTermCursorPos pos;
    winTermGetCursorPosition(&pos);
    
    // Calculate the Y positions of the old and new selections
    // Assuming menu header takes 2 lines (title + separator)
    int oldY = 2 + oldSelection;
    int newY = 2 + newSelection;
    
    // Update the old selection (removing the selection indicator)
    menu->options[oldSelection].isSelected = false;
    appDisplayMenuOption(menu, oldSelection, 0, oldY);
    
    // Update the new selection (adding the selection indicator)
    menu->options[newSelection].isSelected = true;
    appDisplayMenuOption(menu, newSelection, 0, newY);
    
    // Restore cursor position
    winTermSetCursor(pos.x, pos.y);
}

/**
 * @brief Initializes and runs an interactive menu.
 * @brief This function handles user input (arrow keys, enter, character keys) to navigate
 * @brief and select options from the given menu. It calls the `onSelect` callback for the chosen option.
 * @param m A pointer to the Menu struct to be initialized.
 * @return The character key of the selected menu option.
 */
char initMenu(Menu* m) {
    // Find the currently selected option, or default to 0
    int selected = 0;

    int key;
    char errorMessage[100] = "";
    int showError = 0;

    while (true) {
        // Reset color and mark selected option
        appMenuSetColor(7, 0);
        
        // Display the menu
        appDisplayMenu(m);
        
        // Display error message if needed
        if (showError) {
            printf("\n");
            appMenuSetColor(12, 0); // Red text for error
            printf("%s", errorMessage);
            appMenuSetColor(7, 0); // Reset to default
            showError = 0;
        }
        
        // Get user input
        key = _getch();

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

        // Handle arrow keys
        if (key == 0 || key == -32 || key == 224) {
            key = _getch(); // Get the actual key code
            
            if (key == 72 || key == 75) { // Up/Left arrow
                int oldSelected = selected; 
                selected--;
                if (selected < 0) selected = m->optionCount - 1;
                if (m->options[selected].isDisabled) {
                    do {
                        selected--;
                        if (selected < 0) selected = m->optionCount - 1;
                    } while (m->options[selected].isDisabled);
                }
                appUpdateMenuSelection(m, oldSelected, selected);
            }
            else if (key == 80 || key == 77) { // Down/Right arrow
                int oldSelected = selected;
                selected++;
                if (selected >= m->optionCount) selected = 0;
                if (m->options[selected].isDisabled) {
                    do {
                        selected++;
                        if (selected >= m->optionCount) selected = 0;
                    } while (m->options[selected].isDisabled);
                }
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
                m->options[i].isSelected = (i == 0 && !m->options[i].isDisabled);
            }
            
            // If first option is disabled, find the first enabled option
            if (m->options[0].isDisabled) {
                for (int j = 1; j < m->optionCount; j++) {
                    if (!m->options[j].isDisabled) {
                        m->options[j].isSelected = true;
                        break;
                    }
                }
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