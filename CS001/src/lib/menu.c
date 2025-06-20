#include "../../include/apctxt.h"

static void appMenuSetColor(int textColor, int bgColor);
static void appDisplayMenu(Menu menu);
void appInitMenu(Menu m);

static void appMenuSetColor(int textColor, int bgColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textColor + (bgColor * 16));
}

static void appDisplayMenu(Menu menu) {
    system("cls");  // Clear screen
    printf("%s\n", menu.name);
    printf("====================================\n");
    
    for (int i = 0; i < menu.optionCount; i++) {
        if (menu.options[i].isDisabled) {
            appMenuSetColor(menu.options[i].disabledTextColor, menu.options[i].disabledBgColor); // Gray for disabled options
            printf(" %s (Disabled)\n", menu.options[i].text);
            continue;
        } else if (menu.options[i].isSelected) {
            appMenuSetColor(menu.options[i].highlightTextColor, menu.options[i].highlightBgColor);
            printf(">> %s\n", menu.options[i].text);
        } else {
            appMenuSetColor(menu.options[i].textColor, menu.options[i].bgColor); // Default color
            printf(" %s\n", menu.options[i].text);
        }
    }
    appMenuSetColor(7, 0); // Reset color
}

void appInitMenu(Menu m) {
    int selected = 0;
    int key;

    while (true) {
        // Reset color and mark selected option
        appMenuSetColor(7, 0);
        
        // Display the menu
        appDisplayMenu(m);
        
        // Get user input
        key = _getch();

        // Handle arrow keys
        if (key == 0 || key == -32 || key == 224) {
            key = _getch(); // Get the actual key code
            
            if (key == 72 || key == 75) { // Up/Left arrow
                selected--;
                if (selected < 0) selected = m.optionCount - 1;
                if (m.options[selected].isDisabled) {
                    do {
                        selected--;
                        if (selected < 0) selected = m.optionCount - 1;
                    } while (m.options[selected].isDisabled);
                }
            }
            
            if (key == 80 || key == 77) { // Down/Right arrow
                selected++;
                if (selected >= m.optionCount) selected = 0;
                if (m.options[selected].isDisabled) {
                    do {
                        selected++;
                        if (selected >= m.optionCount) selected = 0;
                    } while (m.options[selected].isDisabled);
                }
            }
        }

        // Reset all selections
        for (int i = 0; i < m.optionCount; i++) {
            m.options[i].isSelected = false;
        }
        
        // Mark the current selection
        m.options[selected].isSelected = true;

        // Process Enter key
        if (key == 13) {
            if (m.options[selected].isDisabled) {
                printf("Option is disabled.\n");
                continue;
            }
            
            // Execute the menu option's callback function if it exists
            if (m.options[selected].onSelect != NULL) {
                m.options[selected].onSelect();
            } else {
                printf("\nSelected: %s\n", m.options[selected].text);
                printf("Press any key to continue...\n");
                _getch();
            }
        }
    }
}