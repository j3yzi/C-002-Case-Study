#include "../../include/apctxt.h"

int main (){
    while (1)
        runMenuLoop();
    
    return 0;
}

void setColor(int text, int bg) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text + (bg * 16));
}
void addMenu (){
    int selected = 0;
    int key;

    while (1)
    {
        system("cls");  // Clear screen
        char add[3][50] = {
            "Create Employee List",
            "Add Employee",
            "Back"
        };

        for (int i = 0; i < 3; i++) {
            if (i == selected) {
                setColor(6, 0);
                printf(">> %s\n", add[i]);
            } else {
                setColor(7, 0);
                printf(" %s\n", add[i]);
            }
        }
        setColor(7, 0); // Reset color
        key = _getch();

        if (key == 0 || key == -32 || key == 224) {
            key = _getch();  // Get actual key

            if (key == 72 || key == 75) { // Up left arrow
                selected--;
                if (selected < 0) selected = 2;}
            if (key == 80 || key == 77) { // Down right arrow
                selected++;
                if (selected > 2) selected = 0;}
        }

        if (key == 13) // Enter key
            break;
    }

    switch (selected){
        case 0:
            printf("\nCreating Employee List...\n\n");
            //function();
            printf("\nEmployee List Created!!\n\n");
            break;
        case 1:
            printf("\nAdding Employees...\n\n");
            //function();
            printf("\nEmployees Added!!\n\n");
            break;
        case 2:
            printf("\nBack To Menu\n\n");
            runMenuLoop();
            break;
    }
}

void printMenu(int selected) {

    char menu[8][50] = {
        "Add Employees",
        "Edit Employee Data",
        "Search Employee",
        "Delete Employee",
        "Load Employee From File",
        "Export Employee Data From To CSV",
        "Display Payroll Report",
        "Exit"
    };

    for (int i = 0; i < 8; i++) {
        if (i == selected) {
            setColor(4, 0);
            printf(">> %s\n", menu[i]);
        } else {
            setColor(7, 0);
            printf(" %s\n", menu[i]);
        }
    }
    setColor(7, 0); // Reset color
}

int runMenuLoop() { //menu starting function
    int selected = 0;
    int key;

    while (1)
    {
        printMenu(selected);
        key = _getch();

        // Handle arrow keys (0 or 224 prefix code)
        if (key == 0 || key == -32 || key == 224) {
            key = _getch();  // Get actual key
            system("cls");  // Clear screen

            if (key == 72 || key == 75) { // Up left arrow
                selected--;
                if (selected < 0) selected = 7;}
            if (key == 80 || key == 77) { // Down right arrow
                selected++;
                if (selected > 7) selected = 0;}
        }

        if (key == 13) // Enter key
            break;
    }

    switch(selected){
        case 0:
            addMenu();
            //addEmployeeNode();
            break;
        case 1:
            printf("\nEditing Employee Info...\n\n");
            //editEmployeeNode();
            break;
        case 2:
            printf("\nSearching Employee Info...\n\n");
            //searchEmployeeNode();
            break;
        case 3:
            printf("\nDeleting Employee Info...\n\n");
            //deleteEmployeeNode();
            break;
        case 4:
            printf("\nLoading Employee From File...\n\n");
            //-EmployeeNode();
            break;
        case 5:
            printf("\nExporting Employee Data From To CSV...\n\n");
            //-EmployeeNode();
            break;
        case 6:
            printf("\nDisplaying Payroll Report...\n\n");
            //displayReportHeader();
            //displayEmployeeRecord(employee);
            break;
        case 7:
            printf("\nExiting...\n\n");
            exit(0);
            break;
    }
}
