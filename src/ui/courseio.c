#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "courseio.h"
#include "menuio.h"
#include "../../include/headers/apctxt.h"
#include "../../include/headers/apclrs.h"
#include "../../include/headers/interface.h"
#include "../../include/models/course.h"

// Global course catalog manager
static CourseCatalogManager courseMgr;

/**
 * @brief Helper function to run a menu with the new interface system
 * @param menu Pointer to the menu to display
 * @return Returns the selected menu option character
 */
static char runMenuWithInterface(Menu* menu) {
    int selected = 0;
    int key;
    
    // Find first non-disabled option
    while (selected < menu->optionCount && menu->options[selected].isDisabled) {
        selected++;
    }
    if (selected >= menu->optionCount) selected = 0;
    
    while (1) {
        // Clear screen and display the menu
        winTermClearScreen();
        displayMenu(menu, selected);
        
        // Get user input
        key = _getch();
        
        // Handle special keys (arrow keys)
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72) { // Up arrow
                int oldSelected = selected;
                do {
                    selected--;
                    if (selected < 0) selected = menu->optionCount - 1;
                } while (menu->options[selected].isDisabled && selected != oldSelected);
            }
            else if (key == 80) { // Down arrow
                int oldSelected = selected;
                do {
                    selected++;
                    if (selected >= menu->optionCount) selected = 0;
                } while (menu->options[selected].isDisabled && selected != oldSelected);
            }
        }
        else if (key == 13) { // Enter key
            if (!menu->options[selected].isDisabled) {
                return menu->options[selected].key;
            }
        }
        else if (key == 27) { // Escape key
            return menu->options[menu->optionCount - 1].key; // Return last option (usually Exit/Back)
        }
        else {
            // Check if key matches any menu option
            for (int i = 0; i < menu->optionCount; i++) {
                if (key == menu->options[i].key && !menu->options[i].isDisabled) {
                    return menu->options[i].key;
                }
            }
        }
    }
}

/**
 * @brief Initializes the course catalog manager
 */
void initCourseCatalogManager(void) {
    memset(&courseMgr, 0, sizeof(CourseCatalogManager));
    courseMgr.hasUnsavedChanges = false;
    courseMgr.currentCatalogFile[0] = '\0';
    
    // Create an empty catalog with a default name
    createCourseCatalog(&courseMgr.catalog, "Default Catalog");
    
    printf("Course Catalog Manager initialized!\n");
}

/**
 * @brief Cleans up the course catalog manager
 */
void cleanupCourseCatalogManager(void) {
    destroyCourseCatalog(&courseMgr.catalog);
    printf("Course Catalog Manager cleaned up!\n");
}

/**
 * @brief Main menu for course management
 * @return Returns 0 on normal exit, other values on error
 */
int runCourseManagement(void) {
    char choice;
    char menuTitle[100] = "Course Catalog Management";
    
    // Update the menu title with the current catalog name if available
    if (courseMgr.catalog.courseList) {
        sprintf(menuTitle, "Course Catalog Management - %s", 
                courseMgr.catalog.catalogName);
        
        if (courseMgr.hasUnsavedChanges) {
            strcat(menuTitle, " *");
        }
    }
    
    Menu courseMenu = {1, menuTitle, (MenuOption[]){
        {'1', "Create New Catalog", "Initialize a new course catalog", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Add Course", "Add a new course to the catalog", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Edit Course", "Modify existing course information", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Delete Course", "Remove a course from the catalog", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'5', "Search Courses", "Find courses by code or title", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'6', "Display All Courses", "Show complete course catalog", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'7', "Save Catalog", "Save current catalog to file", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'8', "Load Catalog", "Load course data from saved file", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'9', "Back to Main Menu", "Return to the main system menu", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 9
    };
    
    do {
        // Update menu title with current catalog info
        if (courseMgr.catalog.courseList) {
            sprintf(menuTitle, "Course Catalog Management - %s", 
                    courseMgr.catalog.catalogName);
            
            if (courseMgr.hasUnsavedChanges) {
                strcat(menuTitle, " *");
            }
        }
        courseMenu.name = menuTitle;
        
        // Update menu options based on catalog state
        bool hasCatalog = (courseMgr.catalog.courseList != NULL);
        bool hasCourses = (hasCatalog && courseMgr.catalog.courseList->size > 0);
        
        // Options that require a catalog
        courseMenu.options[1].isDisabled = !hasCatalog; // Add Course
        courseMenu.options[6].isDisabled = !hasCatalog || !courseMgr.hasUnsavedChanges; // Save Catalog
        
        // Options that require courses
        courseMenu.options[2].isDisabled = !hasCourses; // Edit Course
        courseMenu.options[3].isDisabled = !hasCourses; // Delete Course
        courseMenu.options[4].isDisabled = !hasCourses; // Search Courses
        courseMenu.options[5].isDisabled = !hasCourses; // Display All Courses
        
        choice = runMenuWithInterface(&courseMenu);
        
        switch(choice) {
            case '1':
                handleCreateCourseCatalog();
                break;
            case '2':
                handleAddCourse();
                break;
            case '3':
                handleEditCourse();
                break;
            case '4':
                handleDeleteCourse();
                break;
            case '5':
                handleSearchCourses();
                break;
            case '6':
                handleDisplayAllCourses();
                break;
            case '7':
                handleSaveCatalog();
                break;
            case '8':
                handleLoadCatalog();
                break;
            case '9':
                // Check for unsaved changes before exiting
                if (courseMgr.hasUnsavedChanges) {
                    if (appYesNoPrompt("\nYou have unsaved changes. Do you want to save before exiting?")) {
                        handleSaveCatalog();
                    }
                }
                return 0;
            default:
                break;
        }
    } while (1);
    
    return 0;
}

/**
 * @brief Handles creating a new course catalog
 * @return Returns 0 on success, -1 on failure
 */
int handleCreateCourseCatalog(void) {
    winTermClearScreen();
    printf("=== Create New Course Catalog ===\n\n");
    
    // Check for unsaved changes
    if (courseMgr.hasUnsavedChanges) {
        if (appYesNoPrompt("You have unsaved changes. Do you want to save before creating a new catalog?")) {
            handleSaveCatalog();
        }
    }
    
    char catalogName[50];
    appFormField field = { "Enter name for the new catalog: ", catalogName, 50, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = 49}} };
    appGetValidatedInput(&field, 1);
    
    // Clean up existing catalog
    destroyCourseCatalog(&courseMgr.catalog);
    
    // Create new catalog
    if (createCourseCatalog(&courseMgr.catalog, catalogName) != 0) {
        printf("Failed to create course catalog!\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    // Reset file name and changes flag
    courseMgr.currentCatalogFile[0] = '\0';
    courseMgr.hasUnsavedChanges = false;
    
    printf("Course catalog '%s' created successfully!\n", catalogName);
    waitForKeypress(NULL);
    return 0;
}

/**
 * @brief Lists all course catalog files in the data directory
 * @return Returns 1 if files were found, 0 otherwise
 */
int listCourseCatalogFiles(void) {
    // Create data directory if it doesn't exist
    appCreateDirectory("data");
    
    // List .cat files
    int result = appListFiles("data", "*.cat");
    if (result != 1) {
        return 0;
    }
    
    return 1;
}

/**
 * @brief Handles adding a new course to the catalog
 * @return Returns 0 on success, -1 on failure
 */
int handleAddCourse(void) {
    winTermClearScreen();
    printf("=== Add New Course ===\n\n");
    
    if (!courseMgr.catalog.courseList) {
        printf("No active catalog! Please create or load a catalog first.\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    Course* newCourse = (Course*)malloc(sizeof(Course));
    if (!newCourse) {
        printf("Memory allocation failed!\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    // Initialize course data
    memset(newCourse, 0, sizeof(Course));
    newCourse->isActive = true;
    
    // Get course data from user
    if (getCourseDataFromUser(newCourse, true) != 0) {
        printf("Course creation cancelled.\n");
        free(newCourse);
        waitForKeypress(NULL);
        return -1;
    }
    
    // Add course to catalog
    if (addCourse(&courseMgr.catalog, newCourse) != 0) {
        printf("Failed to add course to catalog! Course code may already exist.\n");
        free(newCourse);
        waitForKeypress(NULL);
        return -1;
    }
    
    courseMgr.hasUnsavedChanges = true;
    printf("\nCourse '%s - %s' added successfully!\n", newCourse->code, newCourse->name);
    waitForKeypress(NULL);
    return 0;
}

/**
 * @brief Gets course data from user input
 * @param course Pointer to the course structure to fill
 * @param isNew Whether this is a new course (true) or editing an existing one (false)
 * @return Returns 0 on success, -1 on failure or cancellation
 */
int getCourseDataFromUser(Course* course, bool isNew) {
    if (!course) {
        return -1;
    }
    
    // Prepare form fields
    char codeStr[courseCodeLen];
    char nameStr[courseNameLen];
    char descStr[courseDescLen];
    char unitsStr[10];
    char typeStr[2];
    char statusStr[2];
    
    // Initialize with existing values if editing
    if (!isNew) {
        strncpy(codeStr, course->code, sizeof(codeStr) - 1);
        codeStr[sizeof(codeStr) - 1] = '\0';
        
        strncpy(nameStr, course->name, sizeof(nameStr) - 1);
        nameStr[sizeof(nameStr) - 1] = '\0';
        
        strncpy(descStr, course->description, sizeof(descStr) - 1);
        descStr[sizeof(descStr) - 1] = '\0';
        
        sprintf(unitsStr, "%d", course->units);
        sprintf(typeStr, "%d", course->type);
        sprintf(statusStr, "%d", course->isActive ? 1 : 0);
    } else {
        codeStr[0] = '\0';
        nameStr[0] = '\0';
        descStr[0] = '\0';
        unitsStr[0] = '\0';
        typeStr[0] = '\0';
        statusStr[0] = '\0';
    }
    
    // Create form fields
    appFormField fields[] = {
        { "Course Code (e.g., IT101): ", codeStr, courseCodeLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = courseCodeLen - 2}} },
        { "Course Name: ", nameStr, courseNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = courseNameLen - 2}} },
        { "Course Units (1-6): ", unitsStr, 10, IV_RANGE_INT, {.rangeInt = {.min = 1, .max = 6}} },
        { "Course Type (0=Core, 1=Major, 2=Elective, 3=General Ed): ", typeStr, 2, IV_RANGE_INT, {.rangeInt = {.min = 0, .max = 3}} },
        { "Course Status (1=Active, 0=Inactive): ", statusStr, 2, IV_RANGE_INT, {.rangeInt = {.min = 0, .max = 1}} },
        { "Course Description: ", descStr, courseDescLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = courseDescLen - 2}} }
    };
    
    // Get input from user
    appGetValidatedInput(fields, 6);
    
    // Update course structure
    strncpy(course->code, codeStr, courseCodeLen - 1);
    course->code[courseCodeLen - 1] = '\0';
    
    strncpy(course->name, nameStr, courseNameLen - 1);
    course->name[courseNameLen - 1] = '\0';
    
    strncpy(course->description, descStr, courseDescLen - 1);
    course->description[courseDescLen - 1] = '\0';
    
    course->units = atoi(unitsStr);
    course->type = (CourseType)atoi(typeStr);
    course->isActive = (atoi(statusStr) == 1);
    
    return 0;
}

/**
 * @brief Handles editing an existing course
 * @return Returns 0 on success, -1 on failure
 */
int handleEditCourse(void) {
    winTermClearScreen();
    printf("=== Edit Course ===\n\n");
    
    if (!courseMgr.catalog.courseList || courseMgr.catalog.courseList->size == 0) {
        printf("No courses to edit! The catalog is empty.\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    int shouldContinue = 1;
    Course* course = NULL;
    
    do {
        // Display all courses for reference
        winTermClearScreen();
        printf("=== Edit Course ===\n\n");
        displayAllCourses(&courseMgr.catalog);
        
        // Get course code to edit
        char courseCode[courseCodeLen];
        appFormField field = { "\nEnter course code to edit: ", courseCode, courseCodeLen, IV_MAX_LEN, {.maxLengthChars = {.maxLength = courseCodeLen - 1}} };
        appGetValidatedInput(&field, 1);
        
        // Find the course
        course = findCourseByCode(&courseMgr.catalog, courseCode);
        if (!course) {
            printf("\n❌ Course with code '%s' not found!\n", courseCode);
            printf("\nWhat would you like to do?\n");
            printf("1. Try again with a different course code\n");
            printf("2. View all courses again\n");
            printf("3. Back to Course Menu\n");
            printf("\nSelect an option (1-3): ");
            
            char choice = _getch();
            printf("%c\n", choice);
            
            switch (choice) {
                case '1':
                    continue; // Loop to try again
                case '2':
                    continue; // Loop back to show courses and try again
                case '3':
                default:
                    return -1; // Exit to course menu
            }
        } else {
            shouldContinue = 0; // Found course, exit loop
        }
    } while (shouldContinue);
    
    // Display current course details
    printf("\nCurrent course details:\n");
    displayCourseDetails(course);
    
    // Create a copy for editing
    Course updatedCourse;
    memcpy(&updatedCourse, course, sizeof(Course));
    
    // Get updated course data
    if (getCourseDataFromUser(&updatedCourse, false) != 0) {
        printf("Course editing cancelled.\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    // Update the course
    if (updateCourse(&courseMgr.catalog, course->code, &updatedCourse) != 0) {
        printf("Failed to update course! Course code may already exist.\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    courseMgr.hasUnsavedChanges = true;
    printf("\nCourse '%s - %s' updated successfully!\n", updatedCourse.code, updatedCourse.name);
    waitForKeypress(NULL);
    return 0;
}

/**
 * @brief Handles deleting a course from the catalog
 * @return Returns 0 on success, -1 on failure
 */
int handleDeleteCourse(void) {
    winTermClearScreen();
    printf("=== Delete Course ===\n\n");
    
    if (!courseMgr.catalog.courseList || courseMgr.catalog.courseList->size == 0) {
        printf("No courses to delete! The catalog is empty.\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    // Display all courses for reference
    displayAllCourses(&courseMgr.catalog);
    
    // Get course code to delete
    char courseCode[courseCodeLen];
    appFormField field = { "\nEnter course code to delete: ", courseCode, courseCodeLen, IV_MAX_LEN, {.maxLengthChars = {.maxLength = courseCodeLen - 1}} };
    appGetValidatedInput(&field, 1);
    
    // Find the course
    Course* course = findCourseByCode(&courseMgr.catalog, courseCode);
    if (!course) {
        printf("Course with code '%s' not found!\n", courseCode);
        waitForKeypress(NULL);
        return -1;
    }
    
    // Display course details and confirm deletion
    printf("\nCourse to delete:\n");
    displayCourseDetails(course);
    
    if (!appYesNoPrompt("\nAre you sure you want to delete this course?")) {
        printf("Course deletion cancelled.\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    // Delete the course
    if (removeCourse(&courseMgr.catalog, courseCode) != 0) {
        printf("Failed to delete course!\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    courseMgr.hasUnsavedChanges = true;
    printf("\nCourse '%s' deleted successfully!\n", courseCode);
    waitForKeypress(NULL);
    return 0;
}

/**
 * @brief Handles searching for courses
 * @return Returns 0 on success, -1 on failure
 */
int handleSearchCourses(void) {
    winTermClearScreen();
    printf("=== Search Courses ===\n\n");
    
    if (!courseMgr.catalog.courseList || courseMgr.catalog.courseList->size == 0) {
        printf("No courses to search! The catalog is empty.\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    // Display search options
    printf("Search options:\n");
    printf("1. Search by Course Code\n");
    printf("2. Search by Course Name\n");
    
    char option[2];
    appFormField optionField = { "Enter your choice (1-2): ", option, 2, IV_RANGE_INT, {.rangeInt = {.min = 1, .max = 2}} };
    appGetValidatedInput(&optionField, 1);
    
    int choice = atoi(option);
    
    if (choice == 1) {
        // Search by course code
        int shouldContinue = 1;
        
        do {
            char courseCode[courseCodeLen];
            appFormField field = { "Enter course code to search: ", courseCode, courseCodeLen, IV_MAX_LEN, {.maxLengthChars = {.maxLength = courseCodeLen - 1}} };
            appGetValidatedInput(&field, 1);
            
            Course* course = findCourseByCode(&courseMgr.catalog, courseCode);
            if (!course) {
                printf("\n❌ Course with code '%s' not found!\n", courseCode);
                printf("\nWhat would you like to do?\n");
                printf("1. Try again with a different course code\n");
                printf("2. View all courses\n");
                printf("3. Back to Search Menu\n");
                printf("\nSelect an option (1-3): ");
                
                char choice = _getch();
                printf("%c\n", choice);
                
                switch (choice) {
                    case '1':
                        continue; // Loop to try again
                    case '2':
                        winTermClearScreen();
                        printf("=== All Courses ===\n\n");
                        displayAllCourses(&courseMgr.catalog);
                        printf("\nPress any key to continue...");
                        _getch();
                        continue; // Loop back to search prompt
                    case '3':
                    default:
                        shouldContinue = 0; // Exit to search menu
                        break;
                }
            } else {
                printf("\nSearch Results:\n");
                displayCourseDetails(course);
                shouldContinue = 0; // Found course, exit loop
            }
        } while (shouldContinue);
    }
    else if (choice == 2) {
        // Search by course name
        char courseName[courseNameLen];
        appFormField field = { "Enter course name to search (partial match): ", courseName, courseNameLen, IV_MAX_LEN, {.maxLengthChars = {.maxLength = courseNameLen - 1}} };
        appGetValidatedInput(&field, 1);
        
        // Allocate array for search results
        const int MAX_RESULTS = 20;
        Course* results[MAX_RESULTS];
        
        int foundCount = findCoursesByName(&courseMgr.catalog, courseName, results, MAX_RESULTS);
        
        if (foundCount == 0) {
            printf("No courses found matching '%s'!\n", courseName);
            waitForKeypress(NULL);
            return -1;
        }
        
        printf("\nSearch Results (%d courses found):\n", foundCount);
        printf("%-10s | %-30s | %-10s | %-5s | %-10s\n", 
               "Code", "Name", "Type", "Units", "Status");
        printf("----------------------------------------------------------------------\n");
        
        for (int i = 0; i < foundCount; i++) {
            Course* course = results[i];
            printf("%-10s | %-30s | %-10s | %-5d | %-10s\n", 
                   course->code,
                   course->name,
                   getCourseTypeString(course->type),
                   course->units,
                   course->isActive ? "Active" : "Inactive");
        }
        
        printf("----------------------------------------------------------------------\n");
        
        // Allow viewing details without a yes/no prompt
        printf("\nEnter course code to view details (or press Enter to return): ");
        char detailCode[courseCodeLen];
        fgets(detailCode, courseCodeLen, stdin);
        
        // Remove newline character
        char* newline = strchr(detailCode, '\n');
        if (newline) *newline = '\0';
        
        // If user entered a code, show details
        if (detailCode[0] != '\0') {
            Course* course = findCourseByCode(&courseMgr.catalog, detailCode);
            if (!course) {
                printf("Course with code '%s' not found!\n", detailCode);
            } else {
                displayCourseDetails(course);
            }
        }
    }
    
    waitForKeypress(NULL);
    return 0;
}

/**
 * @brief Handles displaying all courses in the catalog
 * @return Returns 0 on success, -1 on failure
 */
int handleDisplayAllCourses(void) {
    winTermClearScreen();
    printf("=== Display All Courses ===\n\n");
    
    if (!courseMgr.catalog.courseList || courseMgr.catalog.courseList->size == 0) {
        printf("No courses to display! The catalog is empty.\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    displayAllCourses(&courseMgr.catalog);
    
    // Allow viewing details of a specific course without a yes/no prompt
    printf("\nEnter course code to view details (or press Enter to return): ");
    char courseCode[courseCodeLen];
    fgets(courseCode, courseCodeLen, stdin);
    
    // Remove newline character
    char* newline = strchr(courseCode, '\n');
    if (newline) *newline = '\0';
    
    // If user entered a code, show details
    if (courseCode[0] != '\0') {
        Course* course = findCourseByCode(&courseMgr.catalog, courseCode);
        if (!course) {
            printf("Course with code '%s' not found!\n", courseCode);
        } else {
            displayCourseDetails(course);
        }
    }
    
    waitForKeypress(NULL);
    return 0;
}

/**
 * @brief Handles saving the course catalog
 * @return Returns 0 on success, -1 on failure
 */
int handleSaveCatalog(void) {
    winTermClearScreen();
    printf("=== Save Course Catalog ===\n\n");
    
    if (!courseMgr.catalog.courseList) {
        printf("No active catalog to save!\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    // Show existing catalog files
    printf("Existing catalog files:\n");
    listCourseCatalogFiles();
    printf("\n");
    
    char filename[100];
    
    // If we have a current filename, offer to use it
    if (courseMgr.currentCatalogFile[0] != '\0') {
        printf("Current catalog file: %s\n", courseMgr.currentCatalogFile);
        if (appYesNoPrompt("Do you want to save to the same file?")) {
            strncpy(filename, courseMgr.currentCatalogFile, sizeof(filename) - 1);
            filename[sizeof(filename) - 1] = '\0';
        } else {
            appFormField field = { "Enter filename (will be saved as 'FILENAME.cat'): ", filename, 100, IV_MAX_LEN, {.rangeInt = {.min = 0, .max = 99}} };
            appGetValidatedInput(&field, 1);
            
            // Add .cat extension if not present
            if (strstr(filename, ".cat") == NULL) {
                strcat(filename, ".cat");
            }
        }
    } else {
        appFormField field = { "Enter filename (will be saved as 'FILENAME.cat'): ", filename, 100, IV_MAX_LEN, {.rangeInt = {.min = 0, .max = 99}} };
        appGetValidatedInput(&field, 1);
        
        // Add .cat extension if not present
        if (strstr(filename, ".cat") == NULL) {
            strcat(filename, ".cat");
        }
    }
    
    // Save the catalog
    if (saveCatalog(&courseMgr.catalog, filename) != 0) {
        printf("Failed to save catalog!\n");
        waitForKeypress(NULL);
        return -1;
    }
    
    // Update current filename and changes flag
    strncpy(courseMgr.currentCatalogFile, filename, sizeof(courseMgr.currentCatalogFile) - 1);
    courseMgr.currentCatalogFile[sizeof(courseMgr.currentCatalogFile) - 1] = '\0';
    courseMgr.hasUnsavedChanges = false;
    
    printf("Catalog saved successfully to '%s'!\n", filename);
    waitForKeypress(NULL);
    return 0;
}

/**
 * @brief Handles loading a course catalog
 * @return Returns 0 on success, -1 on failure
 */
int handleLoadCatalog(void) {
    winTermClearScreen();
    printf("=== Load Course Catalog ===\n\n");
    
    // Check for unsaved changes
    if (courseMgr.hasUnsavedChanges) {
        if (appYesNoPrompt("You have unsaved changes. Do you want to save before loading a different catalog?")) {
            handleSaveCatalog();
        }
    }
    
    // Show existing catalog files
    printf("Available catalog files:\n");
    if (!listCourseCatalogFiles()) {
        printf("No catalog files found!\n");
        waitForKeypress(NULL);
        return -1;
    }
    printf("\n");
    
    char filename[100];
    appFormField field = { "Enter filename to load: ", filename, 100, IV_MAX_LEN, {.rangeInt = {.min = 0, .max = 99}} };
    appGetValidatedInput(&field, 1);
    
    // Add .cat extension if not present
    if (strstr(filename, ".cat") == NULL) {
        strcat(filename, ".cat");
    }
    
    // Load the catalog
    if (loadCatalog(&courseMgr.catalog, filename) != 0) {
        printf("Failed to load catalog '%s'!\n", filename);
        waitForKeypress(NULL);
        return -1;
    }
    
    // Update current filename and changes flag
    strncpy(courseMgr.currentCatalogFile, filename, sizeof(courseMgr.currentCatalogFile) - 1);
    courseMgr.currentCatalogFile[sizeof(courseMgr.currentCatalogFile) - 1] = '\0';
    courseMgr.hasUnsavedChanges = false;
    
    printf("Catalog '%s' loaded successfully!\n", courseMgr.catalog.catalogName);
    printf("Loaded %d courses.\n", courseMgr.catalog.courseList->size);
    waitForKeypress(NULL);
    return 0;
}
