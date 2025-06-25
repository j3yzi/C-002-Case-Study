#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
int runMenuLoop();
void setColor(int text, int bg) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text + (bg * 16));
}
void addMenu (){
    int selected = 0;
    int key;
    
    char add[3][50] = {
        "Create Employee List",
        "Add Employee",
        "Back"
    };
    
    char title[] = "Add Employees Sub Menu";
    int box1w = 42; //menu box width
    int pad1 = (box1w - strlen(title)) / 2; //padding for menu box
    
    while (1)
    {
        system("cls");  // Clear screen
        printf("╔");
        for (int i = 0; i <= box1w; i++) 
            printf("═");
        printf("╗\n");

        printf("║%*s%s%*s ║\n", pad1, "", title, pad1, " "); //sub menu

        //title dividers
        printf("╠");
        for (int i = 0; i <= box1w; i++) 
            printf("═");
        printf("╣\n");

        for (int i = 0; i < 3; i++) {
            printf("║   ");
            if (i == selected) {
                setColor(6, 0); 
                printf("→  %-35s", add[i]);
            } else {
                setColor(7, 0); 
                printf("   %-35s", add[i]);
            }
            setColor(7, 0); // Reset color
            printf("  ║\n");
            
        }

        printf("╚");
        for (int i = 0; i <= box1w; i++) 
            printf("═");
        printf("╝");
        
        key = _getch();

            // Handle arrow keys (0 or 224 prefix code)
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
            return;
            break;
    }
    return;
}

void printMenu(int selected) {
    char title1[] = "MENU";
    char title2[] = "KEYS";

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

    char shortcuts[3][30] = {
        " ↑↓     Move selection",
        "Enter   Select",
        " ESC    Exit"
    };

    char description[8][50] = {
        "Adds new employee to the record.",
        "Edits an emploee's information.",
        "Searches for an employee to display.",
        "Deletes an employee from records.",
        "Displays a saved file of records.",
        "Saves an employee record file.",
        "Displays the payroll records.",
        "Ends the program."
    };

    int box1w = 42; //menu box width
    int box2w = box1w / 2 + 3; //side box width
    int pad1 = (box1w - strlen(title1)) / 2; //padding for menu box
    int pad2 = (box2w - strlen(title2)) / 2; //padding for side box

    char *full = description[selected]; //stores selected description to a variable 
    int maxlinelen = box2w - 2; //setting max characters of ddescri in first line

    char line1[100] = "";
    char line2[100] = "";

    int split = 0;

    //finding where to cut the descri
    for (int i = 0; i < strlen(full) && i < maxlinelen; i++) {
        if (full[i] == ' ')
        split = i;
    }

    //first line of descri
    strncpy(line1, full, split);
    line1[split] = '\0';

    strcpy(line2, full + split + 1); //rest of descri

    //top boxes borders
    printf("╔");
    for (int i = 0; i <= box1w; i++) 
        printf("═");
    printf("╗");

    printf("╔");
    for (int i = 0; i <= box2w; i++) 
        printf("═");
    printf("╗\n");

    //box titles
    printf("║%*s%s%*s ║", pad1, "", title1, pad1, " "); //menu
    printf("║ %*s%s%*s║\n", pad2, " ", title2, box2w - pad2 - strlen(title2), ""); //keys

    //title dividers
    printf("╠");
    for (int i = 0; i <= box1w; i++) 
        printf("═");
    printf("╣");

    printf("╠");
    for (int i = 0; i <= box2w; i++) 
        printf("═");
    printf("╣\n");

    for (int i = 0; i < 8; i++) {
        //menu box left border
        printf("║   ");
        if (i == selected) {
            setColor(4, 0); 
            printf("→  %-35s", menu[i]);
        } else {
            setColor(7, 0);
            printf("   %-35s", menu[i]);
        }
        setColor(7, 0);

        if (i == 3) //adds bottom border of keys box
        {
            printf("  ║╚");
            for (int j = 0; j <= box2w; j++)
                printf("═"); 
            printf("╝\n");
        }
        else if (i == 4){ //top border of side bottom box
            printf("  ║╔");
            for (int j = 0; j <= box2w; j++)
                printf("═"); 
            printf("╗\n");
        }
        else 
            printf("  ║║"); //adds dividers 

        if (i < 3)
            printf(" %-*s  ║\n", box2w - 2, shortcuts[i]); //right border for keys box
        else if (i == 5)
            printf(" %-*s  ║\n", box2w - 2, line1); //for first line of descri
        else if(i == 6)
            printf(" %-*s  ║\n", box2w - 2, line2); //for second line
        else if (i == 7)
            printf(" %-*s  ║\n", box2w - 2, "");  //third line is empty
    }

    // top boxes bottom borders
    printf("╚");
    for (int i = 0; i <= box1w; i++) 
        printf("═");
    printf("╝");

    printf("╚");
    for (int i = 0; i <= box2w; i++) 
        printf("═");
    printf("╝\n");

    //bottom box
    int fullboxw = box1w + box2w + 3;
    printf("╔");
    for (int i = 0; i <= fullboxw; i++) 
        printf("═");
    printf("╗\n");

    printf("║%*s ║\n", fullboxw, " ");

    printf("╚");
    for (int i = 0; i <= fullboxw; i++) 
        printf("═");
    printf("╝\n");
}

int runMenuLoop() {
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

int main() {
    // Set console to UTF-8 encoding to display box characters properly
    system("chcp 65001 > nul");
    
    while (1)
        runMenuLoop();

        return 0;
}
