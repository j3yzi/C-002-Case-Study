#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "stuio.h"
#include "../../include/headers/apctxt.h"
#include "../../include/models/student.h"

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
    
    char genderInput[3], programInput[3];
    char yearBuffer[3], unitsBuffer[3];
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

    // Calculate final grade and set remarks
    calculateFinalGrade(newStudent);

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
    printf("Final Grade: %.2f (%s)\n", newStudent->academic.finalGrade, newStudent->academic.remarks);
    
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
    winTermClearScreen();
    printf("=== Edit Student ===\n\n");
    
    char stuNumber[studentNumberLen];
    if (getStudentNumberFromUser(stuNumber, studentNumberLen) != 0) {
        return -1;
    }
    
    Student* existingStu = searchStudentByNumber(studentList, stuNumber);
    if (!existingStu) {
        printf("\nStudent with number '%s' not found.\n", stuNumber);
        printf("Press any key to continue...");
        _getch();
        return -1;
    }
    
    printf("\n=== Current Student Information ===\n");
    displayStudentDetails(existingStu);
    
    Student newData;
    memcpy(&newData, existingStu, sizeof(Student)); // Start with current data
    
    if (editStudentDataFromUser(&newData) == 0) {
        if (updateStudentData(existingStu, &newData) == 0) {
            printf("\nStudent updated successfully!\n");
            printf("\n=== Updated Student Information ===\n");
            displayStudentDetails(existingStu);
        } else {
            printf("Failed to update student.\n");
        }
    } else {
        printf("Edit cancelled.\n");
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
    winTermClearScreen();
    printf("=== Delete Student ===\n\n");
    
    // Validate student list first
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
    
    char stuNumber[studentNumberLen];
    printf("Enter the student number you want to delete:\n");
    if (getStudentNumberFromUser(stuNumber, studentNumberLen) != 0) {
        printf("Operation cancelled.\n");
        printf("Press any key to continue...");
        _getch();
        return 0;
    }
    
    Student* stu = searchStudentByNumber(studentList, stuNumber);
    if (!stu) {
        printf("\n❌ Student with number '%s' was not found.\n", stuNumber);
        printf("\nWould you like to:\n");
        printf("1. Try a different student number\n");
        printf("2. View all students\n");
        printf("3. Return to main menu\n");
        printf("\nEnter choice (1-3): ");
        
        char choice = _getch();
        printf("%c\n", choice);
        
        switch (choice) {
            case '1':
                return handleDeleteStudent(studentList);
            case '2':
                printf("\n=== Current Students ===\n");
                displayAllStudents(studentList);
                printf("\nPress any key to continue...");
                _getch();
                return 0;
            case '3':
            default:
                printf("Returning to main menu...\n");
                printf("Press any key to continue...");
                _getch();
                return 0;
        }
    }
    
    printf("\n=== Student to Delete ===\n");
    displayStudentDetails(stu);
    
    printf("\n⚠️  WARNING: This action cannot be undone!\n");
    printf("Are you sure you want to delete this student? (Y/N): ");
    char confirm = _getch();
    printf("%c\n", confirm);
    
    if (confirm == 'Y' || confirm == 'y') {
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
    return 0;
}

/**
 * @brief Prompts the user for student data updates using appFormField validation.
 * @param student A pointer to the student to edit (contains current values).
 * @return Returns 0 on success, -1 on failure.
 */
int editStudentDataFromUser(Student* student) {
    Student backup;
    memcpy(&backup, student, sizeof(Student)); // Backup original data
    
    winTermClearScreen();
    printf("=== Edit Student Data ===\n\n");
    printf("Current Student Information:\n");
    printf("Name: %s %s %s\n", student->personal.name.firstName, 
           student->personal.name.middleName, student->personal.name.lastName);
    printf("Student Number: %s\n", student->personal.studentNumber);
    printf("Program: %s\n", (student->personal.programCode == PROG_IT) ? "IT" : "CS");
    printf("Year Level: %d\n", student->personal.yearLevel);
    printf("Final Grade: %.2f\n\n", student->academic.finalGrade);
    
    printf("Choose what to edit:\n");
    printf("1. Name\n");
    printf("2. Student Number\n");
    printf("3. Program\n");
    printf("4. Year Level\n");
    printf("5. Grades\n");
    printf("6. Edit All Fields\n");
    printf("7. Cancel\n");
    printf("\nEnter choice (1-7): ");
    
    char choice = _getch();
    printf("%c\n\n", choice);
    
    switch (choice) {
        case '1': {
            printf("=== Edit Name ===\n");
            appFormField nameFields[] = {
                { "Enter First Name: ", student->personal.name.firstName, studentFirstNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = studentFirstNameLen - 1}} },
                { "Enter Middle Name (optional): ", student->personal.name.middleName, studentMiddleNameLen, IV_OPTIONAL, {.maxLengthChars = {.maxLength = studentMiddleNameLen - 1}} },
                { "Enter Last Name: ", student->personal.name.lastName, studentLastNameLen, IV_ALPHA_ONLY_MAX_LEN, {.maxLengthChars = {.maxLength = studentLastNameLen - 1}} }
            };
            appGetValidatedInput(nameFields, 3);
            
            if (!composeStudentName(&student->personal.name)) {
                printf("\n[Error] Invalid name combination. Changes reverted.\n");
                memcpy(&student->personal.name, &backup.personal.name, sizeof(StudentName));
                return -1;
            }
            break;
        }
        case '2': {
            printf("=== Edit Student Number ===\n");
            appFormField field = { "Enter Student Number: ", student->personal.studentNumber, studentNumberLen, IV_MAX_LEN, {.rangeInt = {.max = studentNumberLen - 1}} };
            appGetValidatedInput(&field, 1);
            break;
        }
        case '3': {
            printf("=== Edit Program ===\n");
            char programInput[3];
            appFormField field = { "Enter Program (IT/CS): ", programInput, 3, IV_CHOICES, {.choices = {.choices = (const char*[]){"IT", "CS", "it", "cs"}, .count = 4}} };
            appGetValidatedInput(&field, 1);
            student->personal.programCode = (programInput[0] == 'I' || programInput[0] == 'i') ? PROG_IT : PROG_CS;
            break;
        }
        case '4': {
            printf("=== Edit Year Level ===\n");
            char yearBuffer[3];
            appFormField field = { "Enter Year Level (1-4): ", yearBuffer, 3, IV_RANGE_INT, {.rangeInt = {.min = 1, .max = 4}} };
            appGetValidatedInput(&field, 1);
            student->personal.yearLevel = atoi(yearBuffer);
            break;
        }
        case '5': {
            printf("=== Edit Grades ===\n");
            char prelimBuffer[10], midtermBuffer[10], finalExamBuffer[10];
            appFormField gradeFields[] = {
                { "Enter Prelim Grade (0.0-100.0): ", prelimBuffer, 10, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 100.0}} },
                { "Enter Midterm Grade (0.0-100.0): ", midtermBuffer, 10, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 100.0}} },
                { "Enter Final Exam Grade (0.0-100.0): ", finalExamBuffer, 10, IV_RANGE_FLT, {.rangeFloat = {.min = 0.0, .max = 100.0}} }
            };
            appGetValidatedInput(gradeFields, 3);
            
            student->academic.prelimGrade = (float)atof(prelimBuffer);
            student->academic.midtermGrade = (float)atof(midtermBuffer);
            student->academic.finalExamGrade = (float)atof(finalExamBuffer);
            calculateFinalGrade(student);
            break;
        }
        case '6': {
            // Edit all fields - just call getStudentDataFromUser
            printf("=== Edit All Fields ===\n\n");
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
    printf("Program: %s\n", (student->personal.programCode == PROG_IT) ? "Information Technology" : "Computer Science");
    printf("Year Level: %d\n", student->personal.yearLevel);
    printf("Final Grade: %.2f (%s)\n", student->academic.finalGrade, student->academic.remarks);
    
    printf("\nConfirm these changes? (Y/N): ");
    char confirm = _getch();
    printf("%c\n", confirm);
    
    if (confirm != 'Y' && confirm != 'y') {
        printf("Changes cancelled. Reverting to original values.\n");
        memcpy(student, &backup, sizeof(Student));
        return -1;
    }

    return 0; // Success
} 