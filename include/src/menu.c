#include "../headers/apctxt.h"

void appMenuSetColor(int textColor, int bgColor);
void appDisplayMenu(const Menu* menu);
static void appDisplayMenuOption(const Menu* menu, int optionIndex, int x, int y);
static void appUpdateMenuSelection(Menu* menu, int oldSelection, int newSelection);
char initMenu(Menu* m);

void appMenuSetColor(int textColor, int bgColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textColor + (bgColor * 16));
}

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

char initMenu(Menu* m) {
    // Find the currently selected option, or default to 0
    int selected = 0;

    int key;

    while (true) {
        // Reset color and mark selected option
        appMenuSetColor(7, 0);
        
        // Display the menu
        appDisplayMenu(m);
        
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
                // If the key matches a disabled option, notify the user
                printf("Option '%c' is disabled.\n", m->options[i].key);
                Sleep(1000); // Pause for a moment to let the user read the message
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
            
            if (key == 80 || key == 77) { // Down/Right arrow
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
        }

        // Process Enter key
        if (key == 13) {
            if (m->options[selected].isDisabled) {
                printf("Option is disabled.\n");
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
    }
}