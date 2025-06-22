#include "menuio.h"
#include <stdio.h>
#include <conio.h>
#include "../../../include/headers/apctxt.h"

// Define colors for the menu options for better readability
#define HIGHLIGHT_TEXT_COLOR 9 // Bright Blue
#define HIGHLIGHT_BG_COLOR 0   // Black
#define TEXT_COLOR 7           // White
#define BG_COLOR 0             // Black
#define DISABLED_TEXT_COLOR 8  // Gray
#define DISABLED_BG_COLOR 0    // Black

// Forward declarations for menu handlers
void handleAddProfessor();
void handleEditProfessor();
void handleDeleteProfessor();
void handleSearchProfessor();
void handleSortProfessors();
void handleAddStudent();
void handleEditStudent();
void handleDeleteStudent();
void handleSearchStudent();
void handleSortStudents();
void handleAddUser();
void handleEditUser();
void handleDeleteUser();
void handleSearchUser();
void handleSortUsers();

Menu mainMenu = {
    1,
    "Academic Management System",
    (MenuOption[]){
        {'1', "Manage Professors", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'2', "Manage Students", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'3', "Manage Users", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'4', "Exit", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL}
    },
    4
};

// Professors submenu
Menu professorMenu = {
    2,
    "Professor Management",
    (MenuOption[]){
        {'1', "Add Professor", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleAddProfessor},
        {'2', "Edit Professor", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleEditProfessor},
        {'3', "Delete Professor", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleDeleteProfessor},
        {'4', "Search Professor", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleSearchProfessor},
        {'5', "Sort Professors", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleSortProfessors},
        {'6', "Back to Main Menu", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL}
    },
    6
};

// Students submenu
Menu studentMenu = {
    3,
    "Student Management",
    (MenuOption[]){
        {'1', "Add Student", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleAddStudent},
        {'2', "Edit Student", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleEditStudent},
        {'3', "Delete Student", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleDeleteStudent},
        {'4', "Search Student", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleSearchStudent},
        {'5', "Sort Students", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleSortStudents},
        {'6', "Back to Main Menu", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL}
    },
    6
};

// Users submenu
Menu userMenu = {
    4,
    "User Management",
    (MenuOption[]){
        {'1', "Add User", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleAddUser},
        {'2', "Edit User", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleEditUser},
        {'3', "Delete User", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleDeleteUser},
        {'4', "Search User", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleSearchUser},
        {'5', "Sort Users", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleSortUsers},
        {'6', "Back to Main Menu", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL}
    },
    6
};

// Implementation of handlers
void handleAddProfessor() {
    winTermClearScreen();
    printf("--- Add Professor Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleEditProfessor() {
    winTermClearScreen();
    printf("--- Edit Professor Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleDeleteProfessor() {
    winTermClearScreen();
    printf("--- Delete Professor Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleSearchProfessor() {
    winTermClearScreen();
    printf("--- Search Professor Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleSortProfessors() {
    winTermClearScreen();
    printf("--- Sort Professors Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleAddStudent() {
    winTermClearScreen();
    printf("--- Add Student Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleEditStudent() {
    winTermClearScreen();
    printf("--- Edit Student Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleDeleteStudent() {
    winTermClearScreen();
    printf("--- Delete Student Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleSearchStudent() {
    winTermClearScreen();
    printf("--- Search Student Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleSortStudents() {
    winTermClearScreen();
    printf("--- Sort Students Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleAddUser() {
    winTermClearScreen();
    printf("--- Add User Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleEditUser() {
    winTermClearScreen();
    printf("--- Edit User Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleDeleteUser() {
    winTermClearScreen();
    printf("--- Delete User Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleSearchUser() {
    winTermClearScreen();
    printf("--- Search User Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

void handleSortUsers() {
    winTermClearScreen();
    printf("--- Sort Users Selected ---\n");
    printf("This functionality is not yet implemented.\n");
    printf("Press any key to continue...");
    _getch();
}

int runMenuLoop()
{
    char mainMenuChoice;
    
    do {
        mainMenuChoice = initMenu(&mainMenu);
        
        switch (mainMenuChoice) {
            case '1': { // Manage Professors
                char submenuChoice;
                do {
                    submenuChoice = initMenu(&professorMenu);
                    // No need to handle individual options as they have callbacks set
                } while (submenuChoice != '6'); // Back to main menu
                break;
            }
            case '2': { // Manage Students
                char submenuChoice;
                do {
                    submenuChoice = initMenu(&studentMenu);
                    // No need to handle individual options as they have callbacks set
                } while (submenuChoice != '6'); // Back to main menu
                break;
            }
            case '3': { // Manage Users
                char submenuChoice;
                do {
                    submenuChoice = initMenu(&userMenu);
                    // No need to handle individual options as they have callbacks set
                } while (submenuChoice != '6'); // Back to main menu
                break;
            }
            case '4': // Exit
                winTermClearScreen();
                printf("Exiting application...\n");
                break;
        }
    } while (mainMenuChoice != '4'); // Exit when '4' is selected

    return 0;
}