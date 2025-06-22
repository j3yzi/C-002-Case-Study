#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "menuio.h"
#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/state.h"
#include "../../../include/models/employee.h"

// Define colors for the menu options for better readability
#define HIGHLIGHT_TEXT_COLOR 9 // Bright Blue
#define HIGHLIGHT_BG_COLOR 0   // Black
#define TEXT_COLOR 7           // White
#define BG_COLOR 0             // Black
#define DISABLED_TEXT_COLOR 8  // Gray
#define DISABLED_BG_COLOR 0    // Black

/**
 * @brief Handles the action for creating a new employee list.
 * @brief This function is intended to be used as a callback for a menu option. It sets a global state,
 * @brief clears the screen, and displays a confirmation message to the user.
 */
void handleCreateEmployeeList() {
    employeeListCreated.isEnabled = true; // Enable the state
    system("cls");
    printf("--- Action: Create Employee List selected ---\n");
    printf("Press any key to continue...");
    _getch();
}

/**
 * @brief Handles the action for adding a new employee.
 * @brief This function is a placeholder callback for a menu option. It clears the screen
 * @brief and displays a message indicating the selected action.
 */
void handleAddEmployee() {
    system("cls");
    printf("--- Action: Add Employee selected ---\n");
    printf("Press any key to continue...");
    _getch();
}

// Definition for the main menu
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

// Definition for the sub-menu
Menu addEmployeeSubMenu = {
    2,
    "Add Employee Sub Menu",
    (MenuOption[]){
        // Assign the actual functions to the onSelect callback
        {'1', "Create Employee List", false, true, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleCreateEmployeeList},
        {'2', "Add an Employee", true, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, &handleAddEmployee},
        // For the 'Back' option, the key is what matters. The onSelect callback should be NULL.
        {'3', "Back to Main Menu", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL}
    },
    3
};

/**
 * @brief Checks and updates the state of menu options based on application flags.
 * @brief This function dynamically enables or disables menu options. For example, it enables
 * @brief options like "Add an Employee" only after an employee list has been created.
 */
void checkStates() {
    if (employeeListCreated.isEnabled) {
        addEmployeeSubMenu.options[1].isDisabled = false; // Enable "Add an Employee" option
        mainMenu.options[1].isDisabled = false; // Enable "Edit Employee Data" option
        mainMenu.options[2].isDisabled = false; // Enable "Search Employee" option
        mainMenu.options[3].isDisabled = false; // Enable "Delete Employee" option
        mainMenu.options[5].isDisabled = false; // Enable "Export Employee Data To CSV" option
        mainMenu.options[6].isDisabled = false; // Enable "Display Payroll Report" option
    } 
}

/**
 * @brief The main loop for navigating the application's menu system.
 * @brief This function displays the main menu, processes user input, and navigates to sub-menus
 * @brief or executes actions as needed. It continues to loop until the user chooses to exit.
 * @return Returns 0 upon successful exit of the menu loop.
 */
int menuLoop() {
    char mainMenuChoice;    
    do {
        checkStates();
        mainMenuChoice = initMenu(&mainMenu);

        switch (mainMenuChoice) {
            case '1': { // "Add/Create Employee Options" was selected
                char subMenuChoice;
                do {
                    checkStates();
                    subMenuChoice = initMenu(&addEmployeeSubMenu);
                    switch (subMenuChoice) {
                        case '1': // "Create Employee List"
                            // Display a message and wait for user input before returning to menu
                            break;
                    }
                } while (subMenuChoice != '3');
                break; 
            }            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
                //exportEmployeeDataToFile(&employeeList, "../../data/record.bin");
            case '7':
                winTermClearScreen();
                printf("--- Action for option '%c' is not implemented yet. ---\n", mainMenuChoice);
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

    } while (mainMenuChoice != '8');
    
    return 0;
}
