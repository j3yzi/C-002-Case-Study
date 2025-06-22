#include <stdio.h>
#include <conio.h>
#include "../../../include/headers/apctxt.h"
#include "../../../include/models/student.h"
#include "menuio.h"

#define HIGHLIGHT_TEXT_COLOR 9
#define HIGHLIGHT_BG_COLOR 0
#define TEXT_COLOR 7
#define BG_COLOR 0
#define DISABLED_TEXT_COLOR 8
#define DISABLED_BG_COLOR 0

Menu studentMenu = {
    1,
    "Student Records Menu",
    (MenuOption[]){
        {'1', "Enter new student records", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'2', "Compute final grades and remarks", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'3', "Display all student records", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'4', "Sort students by final grade", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'5', "Search student", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'6', "Save records", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'7', "Load records", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'8', "Exit", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL}
    },
    8
};

int runStudentMenuLoop() {
    char choice;
    do {
        choice = initMenu(&studentMenu);
        switch (choice) {            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                winTermClearScreen();
                printf("--- Action for option '%c' is not implemented yet. ---\n", choice);
                printf("Press any key to continue...");
                _getch();
                break;
            case '8':
                winTermClearScreen();
                printf("Exiting application...\n");
                break;
            default:
                break;
        }
    } while (choice != '8');
    return 0;
} 