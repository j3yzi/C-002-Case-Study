#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "stuio.h"
#include "../../include/headers/apctxt.h"
#include "../../include/headers/interface.h"
#include "../../include/models/student.h"

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
 * @brief Prompts the user for all new student data and validates it.
 * @param newStudent A pointer to an allocated Student struct to be filled.
 * @return Returns 0 on success, -1 on failure.
 */
int getStudentDataFromUser(Student* newStudent) {
    // Clear the student structure
    memset(newStudent, 0, sizeof(Student));

    int isNameValid = 0;
    while (!isNameValid) {
        winTermClearScreen();
        printf("=== Enter Student Name ===\n\n");
        
        appFormField nameFields[] = {
            { "Enter First Name: ", newStudent->personal.name.firstName, studentFirstNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = studentFirstNameLen - 1}} },
            { "Enter Middle Name (optional): ", newStudent->personal.name.middleName, studentMiddleNameLen, IV_OPTIONAL_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = studentMiddleNameLen - 1}} },
            { "Enter Last Name: ", newStudent->personal.name.lastName, studentLastNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = studentLastNameLen - 1}} }
        };        appGetValidatedInput(nameFields, 3);        isNameValid = composeStudentName(&newStudent->personal.name);
        if (!isNameValid) {
            printf("\n[Error] Name is too long to format properly. First and last names must each be\n");
            printf("        less than %d characters to fit in the %d character full name format.\n", studentNameLen - 5, studentNameLen);
            waitForKeypress(NULL);
        }
    }

    winTermClearScreen();
    printf("=== Enter Student Details ===\n\n");
    
    char genderInput[3], programInput[programCodeLen];
    char yearBuffer[3], unitsBuffer[3];
    char prelimBuffer[10], midtermBuffer[10], finalExamBuffer[10];
    
    // Load available programs if not already loaded
    if (g_programCount == 0) {
        loadProgramsFromConfig();
    }
    
    // Display available programs
    printf("Available Programs:\n");
    for (int i = 0; i < g_programCount; i++) {
        printf("%s = %s\n", g_programs[i].code, g_programs[i].name);
    }
    printf("\n");
    
    // Create program choices array for validation
    const char* programChoices[maxProgramCount];
    for (int i = 0; i < g_programCount; i++) {
        programChoices[i] = g_programs[i].code;
    }
    
    appFormField detailFields[] = {
        { "Enter Student Number (10 digits): ", newStudent->personal.studentNumber, studentNumberLen, IV_MAX_LEN, {.rangeInt = {.max = studentNumberLen - 1}} },
        { "Enter Gender (M/F): ", genderInput, 3, IV_CHOICES, {.choices = {.choices = (const char*[]){"M", "F", "m", "f"}, .count = 4}} },
        { "Enter Program Code: ", programInput, programCodeLen, IV_CHOICES, {.choices = {.choices = programChoices, .count = g_programCount}} },
        { "Enter Year Level (1-4): ", yearBuffer, 2, IV_RANGE_INT, {.rangeInt = {.min = 1, .max = 4}} },
        { "Enter Units Enrolled (1-30): ", unitsBuffer, 3, IV_RANGE_INT, {.rangeInt = {.min = 1, .max = 30}} }
    };
    appGetValidatedInput(detailFields, 5);

    // Convert string inputs to appropriate data types
    newStudent->personal.gender = (genderInput[0] == 'M' || genderInput[0] == 'm') ? genderMale : genderFemale;
    strncpy(newStudent->personal.programCode, programInput, programCodeLen - 1);
    newStudent->personal.programCode[programCodeLen - 1] = '\0';
    newStudent->personal.yearLevel = atoi(yearBuffer);
    newStudent->academic.unitsEnrolled = atoi(unitsBuffer);

    winTermClearScreen();
    printf("=== Enter Academic Grades ===\n\n");
    
    appFormField gradeFields[] = {
        { "Enter Prelim Grade (0.0-100.0): ", prelimBuffer, 10, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 100.0}} },
        { "Enter Midterm Grade (0.0-100.0): ", midtermBuffer, 10, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 100.0}} },
        { "Enter Final Exam Grade (0.0-100.0): ", finalExamBuffer, 10, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 100.0}} }
    };
    appGetValidatedInput(gradeFields, 3);

    // Convert grade inputs
    newStudent->academic.prelimGrade = (float)atof(prelimBuffer);
    newStudent->academic.midtermGrade = (float)atof(midtermBuffer);
    newStudent->academic.finalExamGrade = (float)atof(finalExamBuffer);

    // Calculate final grade and set remarks
    calculateFinalGrade(newStudent);

    // Set default academic standing
    newStudent->standing = acadRegular;

    printf("\n=== Student Information Summary ===\n");
    printf("Name: %s\n", newStudent->personal.name.fullName);
    printf("Student Number: %s\n", newStudent->personal.studentNumber);
    printf("Gender: %s\n", (newStudent->personal.gender == genderMale) ? "Male" : "Female");
    printf("Program: %s (%s)\n", newStudent->personal.programCode, getProgramName(newStudent->personal.programCode));
    printf("Year Level: %d\n", newStudent->personal.yearLevel);
    printf("Units Enrolled: %d\n", newStudent->academic.unitsEnrolled);
    printf("Prelim Grade: %.2f\n", newStudent->academic.prelimGrade);
    printf("Midterm Grade: %.2f\n", newStudent->academic.midtermGrade);
    printf("Final Exam Grade: %.2f\n", newStudent->academic.finalExamGrade);
    printf("Final Grade: %.2f (%s)\n", newStudent->academic.finalGrade, newStudent->academic.remarks);
    
    if (!appYesNoPrompt("Confirm this information?")) {
        printf("Student creation cancelled.\n");
        return -1;
    }

    return 0; // Success
}

/**
 * @brief Prompts the user for a student number to search.
 * @param buffer Buffer to store the student number.
 * @param bufferSize Size of the buffer.
 * @return Returns 0 on success, -1 on cancel.
 */
int getStudentNumberFromUser(char* buffer, int bufferSize) {
    // Clear the buffer first
    memset(buffer, 0, bufferSize);
    
    // Use the proper validation system
    appFormField field = { 
        "Enter Student Number: ", 
        buffer, 
        bufferSize, 
        IV_MAX_LEN, 
        {.rangeInt = {.max = bufferSize - 1}} 
    };
    
    appGetValidatedInput(&field, 1);
    return 0;
}

/**
 * @brief Prompts the user to search for a student and displays results.
 * @param studentList Pointer to the student list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleSearchStudent(const list* studentList) {
    winTermClearScreen();
    
    Menu searchMenu = {1, "Search Student", (MenuOption[]){
        {'1', "Search by Student Number", "Find student using their ID number", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Search by Name", "Find student by first or last name", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Display All Students", "Show complete list of all students", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Back to Main Menu", "Return to the student management menu", false, false, 9, 0, 7, 0, 8, 0, NULL}
    }, 4};
    
    char choice = runMenuWithInterface(&searchMenu);
    
    switch (choice) {
        case '1': {
            char stuNumber[studentNumberLen];
            printf("=== Search by Student Number ===\n");
            if (getStudentNumberFromUser(stuNumber, studentNumberLen) == 0) {
                Student* stu = searchStudentByNumber(studentList, stuNumber);
                if (stu) {
                    printf("\n=== Student Found ===\n");
                    displayStudentDetails(stu);
                } else {
                    printf("\nStudent with number '%s' not found.\n", stuNumber);
                }
            }
            break;
        }
        case '2': {
            char fullName[studentNameLen];
            printf("=== Search by Name ===\n");
            appFormField field = { "Enter Full Name: ", fullName, studentNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = studentNameLen - 1}} };
            appGetValidatedInput(&field, 1);
            
            Student* stu = searchStudentByName(studentList, fullName);
            if (stu) {
                printf("\n=== Student Found ===\n");
                displayStudentDetails(stu);
            } else {
                printf("\nStudent with name '%s' not found.\n", fullName);
            }
            break;
        }
        case '3':
            winTermClearScreen();
            printf("=== Display All Students ===\n");
            displayAllStudents(studentList);
            break;
        case '4':
            return 0; // Back to main menu
        default:
            printf("Invalid choice.\n");
            break;
    }
    
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Prompts the user to edit an existing student.
 * @param studentList Pointer to the student list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleEditStudent(list* studentList) {
    if (!studentList || studentList->size == 0) {
        winTermClearScreen();
        printf("=== Edit Student ===\n\n");
        printf("No students available to edit.\n");
        printf("Please add some students first.\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    winTermClearScreen();
    printf("=== Edit Student ===\n\n");
    printf("Current students in the list:\n");
    printf("═══════════════════════════════════════════════════════════════════\n");
    displayAllStudents(studentList);
    printf("═══════════════════════════════════════════════════════════════════\n\n");
    
    char stuNumber[studentNumberLen];
    printf("Which student would you like to edit?\n");
    if (getStudentNumberFromUser(stuNumber, studentNumberLen) != 0) {
        return -1;
    }
    
    Student* existingStu = searchStudentByNumber(studentList, stuNumber);
    if (!existingStu) {
        printf("\n❌ Student with number '%s' not found.\n", stuNumber);
        printf("Please check the student number and try again.\n");
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    winTermClearScreen();
    printf("=== Edit Student: %s ===\n\n", stuNumber);
    printf("Current Student Information:\n");
    printf("═══════════════════════════════════════════════════════════════════\n");
    displayStudentDetails(existingStu);
    printf("═══════════════════════════════════════════════════════════════════\n\n");
    
    Student newData;
    memcpy(&newData, existingStu, sizeof(Student)); // Start with current data
    
    if (editStudentDataFromUser(&newData) == 0) {
        // Recalculate final grade with new data
        calculateFinalGrade(&newData);
        
        if (updateStudentData(existingStu, &newData) == 0) {
            winTermClearScreen();
            printf("=== Student Update Successful ===\n\n");
            printf("✅ Student '%s' has been updated successfully!\n\n", stuNumber);
            printf("Updated Student Information:\n");
            printf("═══════════════════════════════════════════════════════════════════\n");
            displayStudentDetails(existingStu);
            printf("═══════════════════════════════════════════════════════════════════\n");
        } else {
            printf("❌ Failed to update student data.\n");
            printf("Please try again or contact system administrator.\n");
        }
    } else {
        printf("🚫 Edit operation cancelled.\n");
        printf("No changes were made to the student record.\n");
    }
    
    printf("\nPress any key to continue...");
    _getch();
    return 0;
}

/**
 * @brief Prompts the user to delete a student.
 * @param studentList Pointer to the student list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleDeleteStudent(list* studentList) {
    if (!studentList) {
        printf("Error: Student list is not available.\n");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }
    
    if (!studentList->head || studentList->size == 0) {
        printf("No students available to delete.\n");
        printf("Please add some students first.\n");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }

    while (1) {
        winTermClearScreen();
        printf("=== Delete Student ===\n\n");
        printf("Enter the student number to delete (or type 'back' to cancel):\n");
        
        char stuNumber[studentNumberLen];
        getStudentNumberFromUser(stuNumber, studentNumberLen);

        if (strcmp(stuNumber, "back") == 0) {
            printf("\nOperation Cancelled.\n");
            printf("Press any key to continue...");
            _getch();
            return 0;
        }
        
          Student* stu = searchStudentByNumber(studentList, stuNumber);
        if (stu) {
            printf("\n=== Student to Delete ===\n");
            displayStudentDetails(stu);
            
            printf("\n⚠️  WARNING: This action cannot be undone!\n");
            if (appYesNoPrompt("Are you sure you want to delete this student?")) {
                printf("\nDeleting student...\n");
                if (removeStudentFromList(studentList, stuNumber) == 0) {
                    printf("✅ Student '%s' deleted successfully!\n", stuNumber);
                    printf("Student count is now: %d\n", studentList->size);
                } else {
                    printf("❌ Failed to delete student from the system.\n");
                    printf("The student may have already been removed.\n");
                }
            } else {
                printf("Delete operation cancelled.\n");
                printf("Student '%s' was not deleted.\n", stuNumber);
            }
            
            printf("\nPress any key to continue...");
            _getch();
            return 0; // Exit loop
        } else {
            printf("\n❌ Student with number '%s' was not found.\n", stuNumber);
            printf("\nWhat would you like to do?\n");
            printf("1. Try again\n");
            printf("2. View all students\n");
            printf("3. Back to Student Menu\n");
            printf("\nSelect an option (1-3): ");
            
            char choice = _getch();
            printf("%c\n", choice);
            
            switch (choice) {
                case '1':
                    continue; // Loop to try again
                case '2':
                    winTermClearScreen();
                    printf("=== All Students ===\n\n");
                    displayAllStudents(studentList);
                    printf("\nPress any key to continue...");
                    _getch();
                    continue; // Loop back to delete prompt
                case '3':
                default:
                    return 0; // Exit to student menu
            }
        }
    }
}

/**
 * @brief Prompts the user for student data updates using appFormField validation.
 * @param student A pointer to the student to edit (contains current values).
 * @return Returns 0 on success, -1 on failure.
 */
int editStudentDataFromUser(Student* student) {
    Student backup;
    memcpy(&backup, student, sizeof(Student)); // Backup original data

    // Create a menu for editing options
    char menuTitle[100];
    sprintf(menuTitle, "Edit Student: %s", student->personal.studentNumber);
    
    // Display current student information first
    winTermClearScreen();
    printf("═══════════════════════════════════════════════════════════════════\n");
    printf("                     CURRENT STUDENT INFORMATION                   \n");
    printf("═══════════════════════════════════════════════════════════════════\n");
    printf("Name: %s\n", student->personal.name.fullName);
    printf("  • First Name: %s\n", student->personal.name.firstName);
    printf("  • Middle Name: %s\n", student->personal.name.middleName);
    printf("  • Last Name: %s\n", student->personal.name.lastName);
    printf("Student Number: %s\n", student->personal.studentNumber);
    printf("Gender: %s\n", (student->personal.gender == genderMale) ? "Male" : "Female");
    printf("Program: %s (%s)\n", student->personal.programCode, getProgramName(student->personal.programCode));
    printf("Year Level: %d\n", student->personal.yearLevel);
    printf("\nAcademic Information:\n");
    printf("  • Units Enrolled: %d units\n", student->academic.unitsEnrolled);
    printf("  • Prelim Grade: %.2f\n", student->academic.prelimGrade);
    printf("  • Midterm Grade: %.2f\n", student->academic.midtermGrade);
    printf("  • Final Exam Grade: %.2f\n", student->academic.finalExamGrade);
    printf("  • Final Grade: %.2f (%s)\n", student->academic.finalGrade, student->academic.remarks);
    printf("═══════════════════════════════════════════════════════════════════\n\n");
    printf("Select what you would like to edit:\n\n");
    
    // Create the menu with proper interface
    Menu editMenu = {1, menuTitle, (MenuOption[]){
        {'1', "Edit Name", "Modify student's first, middle, or last name", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'2', "Edit Student Number", "Change the student identification number", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'3', "Edit Program", "Change the academic program and course code", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'4', "Edit Year Level", "Update the current year level (1-4)", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'5', "Edit Grades", "Modify prelim, midterm, and final exam grades", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'6', "Edit All Fields", "Update all student information at once", false, false, 9, 0, 7, 0, 8, 0, NULL},
        {'7', "Cancel", "Return without making any changes", false, false, 9, 0, 7, 0, 8, 0, NULL}}, 7
    };
    
    char choice = runMenuWithInterface(&editMenu);
    
    switch (choice) {
        case '1': {
            // Edit name - with centered display
            winTermClearScreen();
            
            // Get console size for centering
            int consoleWidth, consoleHeight;
            getConsoleSize(&consoleWidth, &consoleHeight);
            
            // Display centered header
            const char* header = "=== Edit Student Name ===";
            int headerLen = strlen(header);
            int startX = (consoleWidth - headerLen) / 2;
            int startY = 5;
            
            setCursor(startX, startY);
            printf("%s\n\n", header);
            
            char firstNamePrompt[128], middleNamePrompt[128], lastNamePrompt[128];
            sprintf(firstNamePrompt, "Enter First Name [%s]: ", student->personal.name.firstName);
            sprintf(middleNamePrompt, "Enter Middle Name (optional) [%s]: ", student->personal.name.middleName);
            sprintf(lastNamePrompt, "Enter Last Name [%s]: ", student->personal.name.lastName);
            
            appFormField nameFields[] = {
                { firstNamePrompt, student->personal.name.firstName, studentFirstNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = studentFirstNameLen - 1}} },
                { middleNamePrompt, student->personal.name.middleName, studentMiddleNameLen, IV_OPTIONAL_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = studentMiddleNameLen - 1}} },
                { lastNamePrompt, student->personal.name.lastName, studentLastNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = studentLastNameLen - 1}} }
            };
            appGetValidatedInput(nameFields, 3);
            
            if (!composeStudentName(&student->personal.name)) {
                printf("\n[Error] Name is too long to format properly. First and last names must each be\n");
                printf("        less than %d characters to fit in the %d character full name format. Changes reverted.\n", studentNameLen - 5, studentNameLen);
                memcpy(&student->personal.name, &backup.personal.name, sizeof(StudentName));
                return -1;
            }
            break;
        }
        case '2': {
            // Edit student number - with centered display
            winTermClearScreen();
            
            // Get console size for centering
            int consoleWidth, consoleHeight;
            getConsoleSize(&consoleWidth, &consoleHeight);
            
            // Display centered header
            const char* header = "=== Edit Student Number ===";
            int headerLen = strlen(header);
            int startX = (consoleWidth - headerLen) / 2;
            int startY = 5;
            
            setCursor(startX, startY);
            printf("%s\n\n", header);
            
            char numberPrompt[64];
            sprintf(numberPrompt, "Enter Student Number [%s]: ", student->personal.studentNumber);
            appFormField field = { numberPrompt, student->personal.studentNumber, studentNumberLen, IV_MAX_LEN, {.rangeInt = {.max = studentNumberLen - 1}} };
            appGetValidatedInput(&field, 1);
            break;
        }
        case '3': {
            // Edit program - with centered display
            winTermClearScreen();
            
            // Get console size for centering
            int consoleWidth, consoleHeight;
            getConsoleSize(&consoleWidth, &consoleHeight);
            
            // Display centered header
            const char* header = "=== Edit Student Program ===";
            int headerLen = strlen(header);
            int startX = (consoleWidth - headerLen) / 2;
            int startY = 3;
            
            setCursor(startX, startY);
            printf("%s\n\n", header);
            char programInput[programCodeLen];
            char programPrompt[64];
            
            // Load available programs if not already loaded
            if (g_programCount == 0) {
                loadProgramsFromConfig();
            }
            
            // Display available programs
            printf("Available Programs:\n");
            for (int i = 0; i < g_programCount; i++) {
                printf("%s = %s\n", g_programs[i].code, g_programs[i].name);
            }
            printf("\n");
            
            // Create program choices array for validation
            const char* programChoices[maxProgramCount];
            for (int i = 0; i < g_programCount; i++) {
                programChoices[i] = g_programs[i].code;
            }
            
            sprintf(programPrompt, "Enter Program Code [%s]: ", student->personal.programCode);
            
            // Initialize with current value
            strncpy(programInput, student->personal.programCode, programCodeLen - 1);
            programInput[programCodeLen - 1] = '\0';
            
            appFormField field = { programPrompt, programInput, programCodeLen, IV_CHOICES, {.choices = {.choices = programChoices, .count = g_programCount}} };
            appGetValidatedInput(&field, 1);
            
            // Update program code
            strncpy(student->personal.programCode, programInput, programCodeLen - 1);
            student->personal.programCode[programCodeLen - 1] = '\0';
            break;
        }
        case '4': {
            // Edit year level - with centered display
            winTermClearScreen();
            
            // Get console size for centering
            int consoleWidth, consoleHeight;
            getConsoleSize(&consoleWidth, &consoleHeight);
            
            // Display centered header
            const char* header = "=== Edit Year Level ===";
            int headerLen = strlen(header);
            int startX = (consoleWidth - headerLen) / 2;
            int startY = 5;
            
            setCursor(startX, startY);
            printf("%s\n\n", header);
            
            char yearBuffer[3];
            char yearPrompt[64];
            
            sprintf(yearPrompt, "Enter Year Level (1-4) [%d]: ", student->personal.yearLevel);
            sprintf(yearBuffer, "%d", student->personal.yearLevel);
            
            appFormField field = { yearPrompt, yearBuffer, 3, IV_RANGE_INT, {.rangeInt = {.min = 1, .max = 4}} };
            appGetValidatedInput(&field, 1);
            student->personal.yearLevel = atoi(yearBuffer);
            break;
        }
        case '5': {
            // Edit grades - with centered display
            winTermClearScreen();
            
            // Get console size for centering
            int consoleWidth, consoleHeight;
            getConsoleSize(&consoleWidth, &consoleHeight);
            
            // Display centered header
            const char* header = "=== Edit Student Grades ===";
            int headerLen = strlen(header);
            int startX = (consoleWidth - headerLen) / 2;
            int startY = 3;
            
            setCursor(startX, startY);
            printf("%s\n\n", header);
            char prelimBuffer[10], midtermBuffer[10], finalExamBuffer[10];
            char prelimPrompt[64], midtermPrompt[64], finalPrompt[64];
            
            sprintf(prelimPrompt, "Enter Prelim Grade (0.0-100.0) [%.2f]: ", 
                   student->academic.prelimGrade);
            sprintf(midtermPrompt, "Enter Midterm Grade (0.0-100.0) [%.2f]: ", 
                   student->academic.midtermGrade);
            sprintf(finalPrompt, "Enter Final Exam Grade (0.0-100.0) [%.2f]: ", 
                   student->academic.finalExamGrade);
                   
            // Initialize with current values
            sprintf(prelimBuffer, "%.2f", student->academic.prelimGrade);
            sprintf(midtermBuffer, "%.2f", student->academic.midtermGrade);
            sprintf(finalExamBuffer, "%.2f", student->academic.finalExamGrade);
            
            appFormField gradeFields[] = {
                { prelimPrompt, prelimBuffer, 10, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 100.0}} },
                { midtermPrompt, midtermBuffer, 10, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 100.0}} },
                { finalPrompt, finalExamBuffer, 10, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 100.0}} }
            };
            appGetValidatedInput(gradeFields, 3);
            
            student->academic.prelimGrade = (float)atof(prelimBuffer);
            student->academic.midtermGrade = (float)atof(midtermBuffer);
            student->academic.finalExamGrade = (float)atof(finalExamBuffer);
            calculateFinalGrade(student);
            break;
        }
        case '6': {
            // Edit all fields - with centered display
            winTermClearScreen();
            
            // Get console size for centering
            int consoleWidth, consoleHeight;
            getConsoleSize(&consoleWidth, &consoleHeight);
            
            // Display centered header
            const char* header = "=== Edit All Student Fields ===";
            int headerLen = strlen(header);
            int startX = (consoleWidth - headerLen) / 2;
            int startY = 3;
            
            setCursor(startX, startY);
            printf("%s\n\n", header);
            
            if (getStudentDataFromUser(student) != 0) {
                memcpy(student, &backup, sizeof(Student));
                return -1;
            }
            return 0;
        }
        case '7':
            printf("Edit cancelled.\n");
            return -1;
        default:
            printf("Invalid choice. Edit cancelled.\n");
            return -1;
    }
    
    printf("\n=== Updated Information ===\n");
    printf("Name: %s\n", student->personal.name.fullName);
    printf("Student Number: %s\n", student->personal.studentNumber);
    printf("Program: %s (%s)\n", student->personal.programCode, getProgramName(student->personal.programCode));
    printf("Year Level: %d\n", student->personal.yearLevel);
    printf("Final Grade: %.2f (%s)\n", student->academic.finalGrade, student->academic.remarks);
    
    if (!appYesNoPrompt("Confirm these changes?")) {
        printf("Changes cancelled. Reverting to original values.\n");
        memcpy(student, &backup, sizeof(Student));
        return -1;
    }

    return 0; // Success
} 