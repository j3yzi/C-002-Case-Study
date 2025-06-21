#include "../../../include/headers/apctxt.h"

Menu mainMenu = {
    1,
    "Main Menu",
    (MenuOption[]){
        {'1', "Add/Create Employee Options", FALSE, TRUE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'2', "Edit Employee Data", TRUE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'3', "Search Employee", TRUE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'4', "Delete Employee", TRUE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'5', "Load Employee From File", FALSE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'6', "Export Employee Data To CSV", TRUE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'7', "Display Payroll Report", TRUE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'8', "Exit", FALSE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL}
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