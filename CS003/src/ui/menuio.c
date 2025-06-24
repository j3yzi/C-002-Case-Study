#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/state.h"
#include "../../../include/models/student.h"
#include "../../../include/headers/list.h"
#include "menuio.h"
#include "stuio.h"
#include "../modules/data.h"

#define HIGHLIGHT_TEXT_COLOR 9
#define HIGHLIGHT_BG_COLOR 0
#define TEXT_COLOR 7
#define BG_COLOR 0
#define DISABLED_TEXT_COLOR 8
#define DISABLED_BG_COLOR 0

// Global variables
static list* studentList = NULL;

// Function prototypes
static int handleEnterStudentRecords(void);
static int handleComputeFinalGrades(void);
static int handleDisplayAllStudents(void);
static int handleSortStudentsByGrade(void);
static int handleSearchStudentByNumber(void);
static int handleSaveRecords(void);
static int handleLoadRecords(void);
static void showDisabledMessage(const char* action);

Menu studentMenu = {
    1,
    "Student Records Management System",
    (MenuOption[]){
        {'1', "Enter new student records", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'2', "Compute final grades and remarks", true, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'3', "Display all student records", true, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'4', "Sort students by final grade", true, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'5', "Search student", true, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'6', "Save records", true, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'7', "Load records", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL},
        {'8', "Exit", false, false, HIGHLIGHT_TEXT_COLOR, HIGHLIGHT_BG_COLOR, TEXT_COLOR, BG_COLOR, DISABLED_TEXT_COLOR, DISABLED_BG_COLOR, NULL}
    },
    8
};

/**
 * @brief Updates menu option states based on current application state
 */
void checkStates(void) {
    // Check if student list has been created and has data
    if (studentList != NULL && studentList->head != NULL && studentList->size > 0) {
        // Enable options that require data
        studentMenu.options[1].isDisabled = false; // Compute final grades
        studentMenu.options[2].isDisabled = false; // Display all students
        studentMenu.options[3].isDisabled = false; // Sort students
        studentMenu.options[4].isDisabled = false; // Search student
        studentMenu.options[5].isDisabled = false; // Save records
    } else {
        // Disable options that require data
        studentMenu.options[1].isDisabled = true;
        studentMenu.options[2].isDisabled = true;
        studentMenu.options[3].isDisabled = true;
        studentMenu.options[4].isDisabled = true;
        studentMenu.options[5].isDisabled = true;
    }
}

/**
 * @brief Initializes the menu I/O module with the student list.
 * @param list Pointer to the student list.
 */
void initMenuIO(list* list) {
    studentList = list;
}

int runStudentMenuLoop() {
    char choice;
    do {
        checkStates();
        winTermClearScreen();
        choice = initMenu(&studentMenu);
        
        switch (choice) {
            case '1':
                handleEnterStudentRecords();
                break;
            case '2':
                if (!studentMenu.options[1].isDisabled) {
                    handleComputeFinalGrades();
                } else {
                    showDisabledMessage("compute final grades");
                }
                break;
            case '3':
                if (!studentMenu.options[2].isDisabled) {
                    handleDisplayAllStudents();
                } else {
                    showDisabledMessage("display student records");
                }
                break;
            case '4':
                if (!studentMenu.options[3].isDisabled) {
                    handleSortStudentsByGrade();
                } else {
                    showDisabledMessage("sort students");
                }
                break;
            case '5':
                if (!studentMenu.options[4].isDisabled) {
                    handleSearchStudentByNumber();
                } else {
                    showDisabledMessage("search student");
                }
                break;
            case '6':
                if (!studentMenu.options[5].isDisabled) {
                    handleSaveRecords();
                } else {
                    showDisabledMessage("save records");
                }
                break;
            case '7':
                handleLoadRecords();
                break;
            case '8':
                winTermClearScreen();
                printf("Exiting Student Records Management System...\n");
                break;
            default:
                printf("\nInvalid option. Press any key to continue...");
                _getch();
                break;
        }
    } while (choice != '8');
    return 0;
}

/**
 * @brief Handles entering new student records (initial 10 students).
 * @return Returns 0 on success, -1 on failure.
 */
static int handleEnterStudentRecords(void) {
    winTermClearScreen();
    printf("=== Enter New Student Records ===\n\n");
    
    // Create the list if it doesn't exist
    if (studentList == NULL) {
        if (createStudentList(&studentList) != 0) {
            printf("Failed to create student list!\n");
            printf("Press any key to continue...");
            _getch();
            return -1;
        }
    }
    
    printf("You will now enter data for %d students.\n\n", maxStudentCreationCount);
    
    int studentsCreated = 0;
    for (int i = 0; i < maxStudentCreationCount; i++) {
        printf("=== Adding Student %d of %d ===\n\n", i + 1, maxStudentCreationCount);
        
        Student* newStudent = (Student*)malloc(sizeof(Student));
        if (!newStudent) {
            printf("Memory allocation failed for student %d!\n", i + 1);
            printf("Press any key to continue...");
            _getch();
            continue;
        }
        
        // Initialize student data
        memset(newStudent, 0, sizeof(Student));
        
        if (getStudentDataFromUser(newStudent) != 0) {
            printf("Student %d creation cancelled or failed.\n", i + 1);
            free(newStudent);
            
            printf("Do you want to continue with remaining students? (Y/N): ");
            char choice = _getch();
            printf("%c\n", choice);
            
            if (choice == 'N' || choice == 'n') {
                break;
            }
            continue;
        }
        
        // Calculate final grade and remarks
        calculateFinalGrade(newStudent);
        
        if (createStudent(newStudent, &studentList) != 0) {
            printf("Failed to add student %d to the list!\n", i + 1);
            free(newStudent);
            printf("Press any key to continue...");
            _getch();
            continue;
        }
        
        studentsCreated++;
        printf("\nStudent %d '%s %s' added successfully!\n", i + 1, newStudent->personal.name.firstName, newStudent->personal.name.lastName);
        printf("Final Grade: %.2f (%s)\n", newStudent->academic.finalGrade, newStudent->academic.remarks);
        
        if (i < maxStudentCreationCount - 1) {
            printf("Press any key to add the next student...");
            _getch();
        }
    }
    
    printf("\n=== Student Records Entry Complete ===\n");
    printf("Successfully created %d out of %d students.\n", studentsCreated, maxStudentCreationCount);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles computing final grades and remarks for all students.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleComputeFinalGrades(void) {
    winTermClearScreen();
    printf("=== Compute Final Grades and Remarks ===\n\n");
    
    if (!studentList || !studentList->head || studentList->size == 0) {
        printf("No students in the system!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    node* current = studentList->head;
    int count = 0;
    
    printf("Recalculating grades for all students...\n\n");
    
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
        } while (current != studentList->head && current != NULL);
    }
    
    printf("\nSuccessfully computed grades for %d students.\n", count);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles displaying all student records in tabular format.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleDisplayAllStudents(void) {
    winTermClearScreen();
    printf("=== All Student Records ===\n\n");
    
    if (!studentList || !studentList->head || studentList->size == 0) {
        printf("No students to display!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    displayAllStudents(studentList);
    
    // Generate and save report file
    char reportFilePath[512];
    int reportResult = generateStudentReportFile(studentList, reportFilePath, sizeof(reportFilePath));
    
    if (reportResult > 0) {
        printf("\nReport saved to: %s\n", reportFilePath);
    }
    
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles sorting students by final grade (descending).
 * @return Returns 0 on success, -1 on failure.
 */
static int handleSortStudentsByGrade(void) {
    winTermClearScreen();
    printf("=== Sort Students by Final Grade (Descending) ===\n\n");
    
    if (!studentList || !studentList->head || studentList->size == 0) {
        printf("No students to sort!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("Sorting students by final grade (highest to lowest)...\n\n");
    
    if (sortStudentsByGrade(studentList, true) == 0) {  // true for descending
        printf("Students sorted successfully!\n\n");
        displayAllStudents(studentList);
    } else {
        printf("Failed to sort students!\n");
    }
    
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles searching for a student.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleSearchStudentByNumber(void) {
    winTermClearScreen();
    printf("=== Search Student by Student Number ===\n\n");
    
    if (!studentList || !studentList->head || studentList->size == 0) {
        printf("No students in the system!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    char studentNumber[studentNumberLen];
    printf("Enter student number to search: ");
    if (scanf("%10s", studentNumber) != 1) {
        printf("Invalid input!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    getchar(); // Clear newline
    
    Student* student = searchStudentByNumber(studentList, studentNumber);
    if (student) {
        printf("\n=== Student Found ===\n");
        displayStudentDetails(student);
    } else {
        printf("\nStudent with number '%s' not found.\n", studentNumber);
    }
    
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles saving student records to file.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleSaveRecords(void) {
    winTermClearScreen();
    printf("=== Save Student Records ===\n\n");
    
    if (!studentList || !studentList->head || studentList->size == 0) {
        printf("No student records to save!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Show existing data files
    printf("Existing data files:\n");
    listDataFiles();
    printf("\n");
    
    char filename[100];
    printf("Enter filename to save records (include .dat extension): ");
    if (scanf("%99s", filename) != 1) {
        printf("Invalid filename!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    getchar(); // Clear newline
    
    int savedCount = saveStudentDataFromFile(studentList, filename);
    
    if (savedCount >= 0) {
        printf("Successfully saved %d student records to 'data/%s'!\n", savedCount, filename);
        printf("Data folder created automatically in the program directory.\n");
    } else {
        printf("Failed to save student records to 'data/%s'!\n", filename);
        printf("Please check if you have write permissions in the program directory.\n");
    }
    
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Handles loading student records from file.
 * @return Returns 0 on success, -1 on failure.
 */
static int handleLoadRecords(void) {
    winTermClearScreen();
    printf("=== Load Student Records ===\n\n");
    
    // List available .dat files
    listDataFiles();
    printf("\n");
    
    char filename[100];
    printf("Enter filename to load records from (include .dat extension): ");
    if (scanf("%99s", filename) != 1) {
        printf("Invalid filename!\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    getchar(); // Clear newline
    
    // If list already exists and has data, confirm overwrite
    if (studentList && studentList->head && studentList->size > 0) {
        printf("Warning: This will replace existing student data. Continue? (Y/N): ");
        char confirm = _getch();
        printf("%c\n", confirm);
        
        if (confirm != 'Y' && confirm != 'y') {
            printf("Operation cancelled.\n");
            printf("Press any key to continue...");
            _getch();
            return -1;
        }
        
        // Clean up old list
        clearList(studentList, freeStudent);
    }
    
    // Load new student data
    list* newList = loadStudentDataFromFile(filename, SINGLY);
    if (!newList) {
        printf("Failed to load student data from file '%s'!\n", filename);
        printf("Make sure the file exists and is in the correct format.\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    // Replace current list
    if (studentList != NULL && studentList != newList) {
        destroyList(&studentList, freeStudent);
    }
    studentList = newList;
    
    printf("Student data loaded successfully from '%s'!\n", filename);
    printf("Loaded %d student records.\n", studentList->size);
    printf("Press any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Shows a message when a disabled option is selected
 * @param action The action that cannot be performed
 */
static void showDisabledMessage(const char* action) {
    printf("\nCannot %s: ", action);
    if (studentList == NULL || studentList->size == 0) {
        printf("No students in the system. Please enter student records first.\n");
    } else {
        printf("This option is currently unavailable.\n");
    }
    printf("Press any key to continue...");
    _getch();
} 