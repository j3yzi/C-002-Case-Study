#include "../../include/apctxt.h"

// Forward declarations of functions
void displayMenu(Menu menu);
void initMenu(Menu m);
void addEmployeeMenu(void);
void backToMainMenu(void);
void exitProgram(void);

#define HIGHLIGHT_TEXT_COLOR 9 // Green
#define HIGHLIGHT_BG_COLOR 0 // Black
#define TEXT_COLOR 7 // White
#define BG_COLOR 0 // Black
#define DISABLED_TEXT_COLOR 8 // Gray
#define DISABLED_BG_COLOR 0 // Black

Menu mainMenu = {
    1,
    "Main Menu",
    (MenuOption[]){
        {'1', "Add Employees", FALSE, TRUE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &addEmployeeMenu},
        {'2', "Edit Employee Data", TRUE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'3', "Search Employee", FALSE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'4', "Delete Employee", FALSE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'5', "Load Employee From File", FALSE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'6', "Export Employee Data From To CSV", FALSE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'7', "Display Payroll Report", FALSE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'8', "Exit", FALSE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &exitProgram}
    },
    8
};

Menu addEmployeeSubMenu = {
    2,
    "Add Employee Sub Menu",
    (MenuOption[]){
        {'1', "Create Employee List", FALSE, TRUE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'2', "Add Employee", FALSE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'3', "Back", FALSE, FALSE, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &backToMainMenu}
    },
    3
};

void addEmployeeMenu(void) {
    // Switch to the add employee submenu
    appInitMenu(addEmployeeSubMenu);
}

void backToMainMenu(void) {
    // Return to the main menu
    appInitMenu(mainMenu);
}

void exitProgram(void) {
    system("cls");
    printf("Thank you for using the program!\n");
    exit(0);
}

int main(){
    // Main program loop
    appInitMenu(mainMenu);
    
    return 0;
}
