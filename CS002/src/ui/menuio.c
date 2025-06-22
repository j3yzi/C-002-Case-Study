#include "menuio.h"
#include "../../../include/headers/apctxt.h"

// Define colors for the menu options for better readability
#define HIGHLIGHT_TEXT_COLOR 9 // Bright Blue
#define HIGHLIGHT_BG_COLOR 0   // Black
#define TEXT_COLOR 7           // White
#define BG_COLOR 0             // Black
#define DISABLED_TEXT_COLOR 8  // Gray
#define DISABLED_BG_COLOR 0    // Black

Menu mainMenu = {
    1,
    "Main Menu",
    (MenuOption[]){
        {'1', "Add/Create Employee Options", false, true, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'2', "Edit Employee Data", true, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'3', "Search Employee", true, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'4', "Delete Employee", true, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'5', "Load Employee From File", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'6', "Export Employee Data To CSV", true, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'7', "Display Payroll Report", true, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'8', "Exit", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL}
    },
    8
};

int runMenuLoop ()
{
    char mainMenuChoice;
    do {
        mainMenuChoice = initMenu(&mainMenu);
        switch (mainMenuChoice) {
            case '1':
            case '2':
                break;
        }
    } while (mainMenuChoice != '8'); // Exit when '8' is selected

    return 0;
}