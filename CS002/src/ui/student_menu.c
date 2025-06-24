#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "student_menu.h"
#include "stuio.h"
#include "../modules/data.h"
#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/state.h"
#include "../../../include/models/student.h"
#include "../../../include/headers/list.h"

// External reference to the global student manager
extern StudentManager stuManager;

// Student menu definition
Menu studentMenu = {3, "Student Management", (MenuOption[]){
    {'1', "Create New Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'2', "Switch Active List", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'3', "Add Student to Active List", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'4', "Edit Student", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'5', "Delete Student", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'6', "Search Student", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'7', "Display All Students", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'8', "Compute Final Grades", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'9', "Sort by Grade", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'A', "Generate Academic Report", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'B', "Save Active List", true, false, 9, 0, 7, 0, 8, 0, NULL},
    {'C', "Load Student List", false, false, 9, 0, 7, 0, 8, 0, NULL},
    {'D', "Back to Main Menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 13};

/**
 * @brief Updates student menu option states based on current application state
 */
void checkStudentMenuStates(void) {
    // Student menu states
    if (stuManager.studentListCount > 0) {
        studentMenu.options[1].isDisabled = false; // Switch Active List
        
        if (stuManager.activeStudentList >= 0 && 
            stuManager.studentLists[stuManager.activeStudentList] &&
            stuManager.studentLists[stuManager.activeStudentList]->size > 0) {
            // Enable all student operations
            for (int i = 2; i < 12; i++) {
                studentMenu.options[i].isDisabled = false;
            }
        } else {
            // Enable only add student, disable others
            studentMenu.options[2].isDisabled = false; // Add Student
            for (int i = 3; i < 12; i++) {
                studentMenu.options[i].isDisabled = true;
            }
        }
    } else {
        // No student lists, disable all except create
        for (int i = 1; i < 12; i++) {
            studentMenu.options[i].isDisabled = true;
        }
    }
}

/**
 * @brief Runs the student management submenu
 * @return 0 on success, -1 on failure
 */
int runStudentManagement(void) {
    char choice;
    
    do {
        checkStudentMenuStates();
        winTermClearScreen();
        
        printf("=== Student Management ===\n");
        printf("Lists: %d | Active: %s | Records: %d\n\n", 
               stuManager.studentListCount,
               (stuManager.activeStudentList >= 0) ? 
               stuManager.studentListNames[stuManager.activeStudentList] : "None",
               (stuManager.activeStudentList >= 0 && stuManager.studentLists[stuManager.activeStudentList]) ?
               stuManager.studentLists[stuManager.activeStudentList]->size : 0);
        
        choice = initMenu(&studentMenu);
        
        switch(choice) {
            case '1':
                handleCreateStudentList();
                break;
            case '2':
                if (!studentMenu.options[1].isDisabled) {
                    handleSwitchStudentList();
                } else {
                    printf("\nNo student lists available to switch!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '3':
                if (!studentMenu.options[2].isDisabled) {
                    handleAddStudent();
                } else {
                    printf("\nPlease create a student list first!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '4':
                if (!studentMenu.options[3].isDisabled) {
                    handleEditStudent(stuManager.studentLists[stuManager.activeStudentList]);
                } else {
                    printf("\nNo students to edit!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '5':
                if (!studentMenu.options[4].isDisabled) {
                    handleDeleteStudent(stuManager.studentLists[stuManager.activeStudentList]);
                } else {
                    printf("\nNo students to delete!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '6':
                if (!studentMenu.options[5].isDisabled) {
                    handleSearchStudent(stuManager.studentLists[stuManager.activeStudentList]);
                } else {
                    printf("\nNo students to search!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '7':
                if (!studentMenu.options[6].isDisabled) {
                    handleDisplayAllStudents();
                } else {
                    printf("\nNo students to display!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '8':
                if (!studentMenu.options[7].isDisabled) {
                    handleComputeFinalGrades();
                } else {
                    printf("\nNo students for grade computation!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case '9':
                if (!studentMenu.options[8].isDisabled) {
                    handleSortStudentsByGrade();
                } else {
                    printf("\nNo students to sort!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case 'A':
            case 'a':
                if (!studentMenu.options[9].isDisabled) {
                    handleStudentReport();
                } else {
                    printf("\nNo student data for report!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case 'B':
            case 'b':
                if (!studentMenu.options[10].isDisabled) {
                    handleSaveStudentList();
                } else {
                    printf("\nNo student data to save!\n");
                    printf("Press any key to continue...");
                    _getch();
                }
                break;
            case 'C':
            case 'c':
                handleLoadStudentList();
                break;
            case 'D':
            case 'd':
                return 0; // Back to main menu
            default:
                printf("\nInvalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (true);
    
    return 0;
}

/**
 * @brief Creates a new student list
 */
int handleCreateStudentList(void) {
    winTermClearScreen();
    printf("=== Create New Student List ===\n\n");
    
    if (stuManager.studentListCount >= 10) {
        printf("Maximum number of student lists (10) reached!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    char listName[50];
    printf("Enter name for this student list: ");
    if (!fgets(listName, sizeof(listName), stdin)) {
        return -1;
    }
    listName[strcspn(listName, "\n")] = 0; // Remove newline
    
    // Create new list
    list* newList = NULL;
    if (createStudentList(&newList) != 0) {
        printf("Failed to create student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Add to manager
    stuManager.studentLists[stuManager.studentListCount] = newList;
    strncpy(stuManager.studentListNames[stuManager.studentListCount], listName, 49);
    stuManager.studentListNames[stuManager.studentListCount][49] = '\0';
    stuManager.activeStudentList = stuManager.studentListCount;
    stuManager.studentListCount++;
    
    printf("Student list '%s' created successfully!\n", listName);
    printf("This list is now active. Add students to get started.\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Switches the active student list
 */
int handleSwitchStudentList(void) {
    winTermClearScreen();
    printf("=== Switch Active Student List ===\n\n");
    
    if (stuManager.studentListCount == 0) {
        printf("No student lists available!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Available Student Lists:\n");
    for (int i = 0; i < stuManager.studentListCount; i++) {
        printf("%d. %s (%d students) %s\n", i + 1, 
               stuManager.studentListNames[i],
               stuManager.studentLists[i] ? stuManager.studentLists[i]->size : 0,
               (i == stuManager.activeStudentList) ? "[ACTIVE]" : "");
    }
    
    printf("\nSelect list to activate (1-%d): ", stuManager.studentListCount);
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    getchar(); // Clear newline
    
    if (choice < 1 || choice > stuManager.studentListCount) {
        printf("Invalid choice!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    stuManager.activeStudentList = choice - 1;
    printf("Active student list switched to: %s\n", stuManager.studentListNames[stuManager.activeStudentList]);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Adds a student to the active list
 */
int handleAddStudent(void) {
    winTermClearScreen();
    printf("=== Add Student to Active List ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (!newStudent) {
        printf("Memory allocation failed!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Initialize student data
    memset(newStudent, 0, sizeof(Student));
    
    if (getStudentDataFromUser(newStudent) != 0) {
        printf("Failed to get student data. Operation cancelled.\n");
        free(newStudent);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Calculate final grade and remarks
    calculateFinalGrade(newStudent);
    
    if (createStudent(newStudent, &stuManager.studentLists[stuManager.activeStudentList]) != 0) {
        printf("Failed to add student to list!\n");
        free(newStudent);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("\nStudent '%s %s' added successfully to list '%s'!\n", 
           newStudent->personal.name.firstName, 
           newStudent->personal.name.lastName,
           stuManager.studentListNames[stuManager.activeStudentList]);
    printf("Student Number: %s\n", newStudent->personal.studentNumber);
    printf("Final Grade: %.2f (%s)\n", newStudent->academic.finalGrade, newStudent->academic.remarks);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Displays all students in the active list
 */
int handleDisplayAllStudents(void) {
    winTermClearScreen();
    printf("=== Display All Students ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Active List: %s\n", stuManager.studentListNames[stuManager.activeStudentList]);
    displayAllStudents(stuManager.studentLists[stuManager.activeStudentList]);
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Computes final grades for all students in the active list
 */
int handleComputeFinalGrades(void) {
    winTermClearScreen();
    printf("=== Compute Final Grades ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    list* activeList = stuManager.studentLists[stuManager.activeStudentList];
    node* current = activeList->head;
    int count = 0;
    
    printf("Recalculating grades for list: %s\n\n", stuManager.studentListNames[stuManager.activeStudentList]);
    
    if (current != NULL) {
        do {
            Student* student = (Student*)current->data;
            if (student != NULL) {
                count++;
                calculateFinalGrade(student);
                printf("%d. %s - Final Grade: %.2f (%s)\n", count, 
                       student->personal.name.fullName, 
                       student->academic.finalGrade, 
                       student->academic.remarks);
            }
            current = current->next;
        } while (current != activeList->head && current != NULL);
    }
    
    printf("\nSuccessfully computed grades for %d students.\n", count);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Sorts students by grade in the active list
 */
int handleSortStudentsByGrade(void) {
    winTermClearScreen();
    printf("=== Sort Students by Grade ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Sorting students by final grade (highest to lowest)...\n\n");
    
    if (sortStudentsByGrade(stuManager.studentLists[stuManager.activeStudentList], 1) == 0) {
        printf("Students sorted successfully!\n\n");
        displayAllStudents(stuManager.studentLists[stuManager.activeStudentList]);
    } else {
        printf("Failed to sort students!\n");
    }
    
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Generates academic report for the active student list
 */
int handleStudentReport(void) {
    winTermClearScreen();
    printf("=== Student Academic Report ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Generating academic report for: %s\n\n", stuManager.studentListNames[stuManager.activeStudentList]);
    
    // Generate the student report file
    char reportFilePath[512];
    int reportResult = generateStudentReportFile(stuManager.studentLists[stuManager.activeStudentList], reportFilePath, sizeof(reportFilePath));
    
    if (reportResult > 0) {
        printf("Successfully generated academic report!\n");
        printf("Report saved to: %s\n", reportFilePath);
        printf("Processed %d students\n", reportResult);
    } else {
        printf("Failed to generate academic report.\n");
    }
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Saves the active student list to file
 */
int handleSaveStudentList(void) {
    winTermClearScreen();
    printf("=== Save Student List ===\n\n");
    
    if (stuManager.activeStudentList < 0 || !stuManager.studentLists[stuManager.activeStudentList]) {
        printf("No active student list to save!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Saving list: %s\n", stuManager.studentListNames[stuManager.activeStudentList]);
    
    // Show existing data files
    printf("\nExisting student data files:\n");
    listStudentDataFiles();
    printf("\n");
    
    char filename[100];
    printf("Enter filename (will be saved as 'student_LISTNAME_TIMESTAMP.dat'): ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        return -1;
    }
    filename[strcspn(filename, "\n")] = 0; // Remove newline
    
    // Use the custom save function
    int savedCount = saveListWithCustomName(stuManager.studentLists[stuManager.activeStudentList], 
                                           filename, "student");
    
    if (savedCount >= 0) {
        printf("Successfully saved %d student records!\n", savedCount);
        printf("Data saved to data directory with timestamp.\n");
    } else {
        printf("Failed to save student list.\n");
    }
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Loads a student list from file
 */
int handleLoadStudentList(void) {
    winTermClearScreen();
    printf("=== Load Student List ===\n\n");
    
    // List available student data files
    listStudentDataFiles();
    printf("\n");
    
    char filename[100];
    printf("Enter filename to load: ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        return -1;
    }
    filename[strcspn(filename, "\n")] = 0; // Remove newline
    
    char listName[50];
    printf("Enter name for this loaded list: ");
    if (!fgets(listName, sizeof(listName), stdin)) {
        return -1;
    }
    listName[strcspn(listName, "\n")] = 0; // Remove newline
    
    // Load the data
    list* newList = loadListWithName(filename, "student", SINGLY);
    if (!newList) {
        printf("Failed to load student data from file '%s'!\n", filename);
        printf("Make sure the file exists and is in the correct format.\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Add to manager
    if (stuManager.studentListCount >= 10) {
        printf("Maximum number of student lists reached!\n");
        destroyList(&newList, freeStudent);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    stuManager.studentLists[stuManager.studentListCount] = newList;
    strncpy(stuManager.studentListNames[stuManager.studentListCount], listName, 49);
    stuManager.studentListNames[stuManager.studentListCount][49] = '\0';
    stuManager.activeStudentList = stuManager.studentListCount;
    stuManager.studentListCount++;
    
    printf("Student list '%s' loaded successfully!\n", listName);
    printf("Loaded %d student records.\n", newList->size);
    printf("This list is now active.\n");
    printf("Press any key to continue...");
    _getch();
    return 0;
} 