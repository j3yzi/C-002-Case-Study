#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "stuio.h"
#include "../../../include/headers/apctxt.h"
#include "../../../include/models/student.h"

/**
 * @brief Prompts the user for all new student data and validates it.
 * @param newStudent A pointer to an allocated Student struct to be filled.
 * @return Returns 0 on success, -1 on failure (e.g., invalid name).
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
            { "Enter Middle Name (optional): ", newStudent->personal.name.middleName, studentMiddleNameLen, IV_OPTIONAL, {.maxLengthChars = {.maxLength = studentMiddleNameLen - 1}} },
            { "Enter Last Name: ", newStudent->personal.name.lastName, studentLastNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = studentLastNameLen - 1}} }
        };
        appGetValidatedInput(nameFields, 3);

        isNameValid = composeStudentName(&newStudent->personal.name);
        if (!isNameValid) {
            printf("\n[Error] Invalid name combination. It must be less than %d chars, or exactly %d with a middle name.\n", studentNameLen, studentNameLen - 1);
            printf("Press any key to try again, or ESC to cancel...");
            if (_getch() == 27) return -1; // ESC key
        }
    }

    winTermClearScreen();
    printf("=== Enter Student Details ===\n\n");
    
    char genderInput[3];
    char programInput[3];
    char yearBuffer[2];
    char unitsBuffer[3];
    char prelimBuffer[10], midtermBuffer[10], finalExamBuffer[10];
    
    appFormField detailFields[] = {
        { "Enter Student Number (10 digits): ", newStudent->personal.studentNumber, studentNumberLen, IV_MAX_LEN, {.rangeInt = {.max = studentNumberLen - 1}} },
        { "Enter Gender (M/F): ", genderInput, 3, IV_CHOICES, {.choices = {.choices = (const char*[]){"M", "F", "m", "f"}, .count = 4}} },
        { "Enter Program (IT/CS): ", programInput, 3, IV_CHOICES, {.choices = {.choices = (const char*[]){"IT", "CS", "it", "cs"}, .count = 4}} },
        { "Enter Year Level (1-4): ", yearBuffer, 2, IV_RANGE_INT, {.rangeInt = {.min = 1, .max = 4}} },
        { "Enter Units Enrolled (1-30): ", unitsBuffer, 3, IV_RANGE_INT, {.rangeInt = {.min = 1, .max = 30}} }
    };
    appGetValidatedInput(detailFields, 5);

    // Convert string inputs to appropriate data types
    newStudent->personal.gender = (genderInput[0] == 'M' || genderInput[0] == 'm') ? genderMale : genderFemale;
    newStudent->personal.programCode = (programInput[0] == 'I' || programInput[0] == 'i') ? PROG_IT : PROG_CS;
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

    // Set default academic standing
    newStudent->standing = acadRegular;

    printf("\n=== Student Information Summary ===\n");
    printf("Name: %s\n", newStudent->personal.name.fullName);
    printf("Student Number: %s\n", newStudent->personal.studentNumber);
    printf("Gender: %s\n", (newStudent->personal.gender == genderMale) ? "Male" : "Female");
    printf("Program: %s\n", (newStudent->personal.programCode == PROG_IT) ? "Information Technology" : "Computer Science");
    printf("Year Level: %d\n", newStudent->personal.yearLevel);
    printf("Units Enrolled: %d\n", newStudent->academic.unitsEnrolled);
    printf("Prelim Grade: %.2f\n", newStudent->academic.prelimGrade);
    printf("Midterm Grade: %.2f\n", newStudent->academic.midtermGrade);
    printf("Final Exam Grade: %.2f\n", newStudent->academic.finalExamGrade);
    
    printf("\nConfirm this information? (Y/N): ");
    char confirm = _getch();
    printf("%c\n", confirm);
    
    if (confirm != 'Y' && confirm != 'y') {
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
    appFormField field = { "Enter Student Number: ", buffer, bufferSize, IV_MAX_LEN, {.rangeInt = {.max = bufferSize - 1}} };
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
    printf("=== Search Student ===\n\n");
    printf("1. Search by Student Number\n");
    printf("2. Search by Name\n");
    printf("3. Display All Students\n");
    printf("4. Back to Main Menu\n");
    printf("\nEnter choice (1-4): ");
    
    char choice = _getch();
    printf("%c\n\n", choice);
    
    switch (choice) {
        case '1': {
            char studentNumber[studentNumberLen];
            printf("=== Search by Student Number ===\n");
            if (getStudentNumberFromUser(studentNumber, studentNumberLen) == 0) {
                Student* student = searchStudentByNumber(studentList, studentNumber);
                if (student) {
                    printf("\n=== Student Found ===\n");
                    displayStudentDetails(student);
                } else {
                    printf("\nStudent with number '%s' not found.\n", studentNumber);
                }
            }
            break;
        }
        case '2': {
            char fullName[studentNameLen];
            printf("=== Search by Name ===\n");
            appFormField field = { "Enter Full Name: ", fullName, studentNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = studentNameLen - 1}} };
            appGetValidatedInput(&field, 1);
            
            Student* student = searchStudentByName(studentList, fullName);
            if (student) {
                printf("\n=== Student Found ===\n");
                displayStudentDetails(student);
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