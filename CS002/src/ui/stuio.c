#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "stuio.h"
#include "../../../include/headers/apctxt.h"
#include "../../../include/models/student.h"
#include "../../../include/headers/list.h"

/**
 * @brief Gets student data from user input with comprehensive validation
 * @param student Pointer to Student structure to populate
 * @return 0 on success, -1 on failure
 */
int getStudentDataFromUser(Student* student) {
    if (!student) {
        return -1;
    }
    
    printf("=== Student Data Entry ===\n\n");
    
    // Get student number
    printf("Enter Student Number (max %d chars): ", studentNumberLen - 1);
    if (!fgets(student->personal.studentNumber, studentNumberLen, stdin)) {
        return -1;
    }
    student->personal.studentNumber[strcspn(student->personal.studentNumber, "\n")] = 0; // Remove newline
    
    if (validateStudentNumber(student->personal.studentNumber, NULL) != 0) {
        printf("Invalid student number format!\n");
        return -1;
    }
    
    // Get first name
    printf("Enter First Name (max %d chars): ", studentFirstNameLen - 1);
    if (!fgets(student->personal.name.firstName, studentFirstNameLen, stdin)) {
        return -1;
    }
    student->personal.name.firstName[strcspn(student->personal.name.firstName, "\n")] = 0;
    
    if (validateStudentName(student->personal.name.firstName) != 0) {
        printf("Invalid first name!\n");
        return -1;
    }
    
    // Get middle name
    printf("Enter Middle Name (max %d chars, optional): ", studentMiddleNameLen - 1);
    if (!fgets(student->personal.name.middleName, studentMiddleNameLen, stdin)) {
        return -1;
    }
    student->personal.name.middleName[strcspn(student->personal.name.middleName, "\n")] = 0;
    
    // Get last name
    printf("Enter Last Name (max %d chars): ", studentLastNameLen - 1);
    if (!fgets(student->personal.name.lastName, studentLastNameLen, stdin)) {
        return -1;
    }
    student->personal.name.lastName[strcspn(student->personal.name.lastName, "\n")] = 0;
    
    if (validateStudentName(student->personal.name.lastName) != 0) {
        printf("Invalid last name!\n");
        return -1;
    }
    
    // Create full name
    composeStudentName(&student->personal.name);
    
    // Get program code (simplified to IT)
    student->personal.programCode = PROG_IT;
    printf("Program: Information Technology (default)\n");
    
    // Get year level
    printf("Enter Year Level (1-4): ");
    int yearInput;
    if (scanf("%d", &yearInput) != 1) {
        printf("Invalid year level input!\n");
        return -1;
    }
    getchar(); // Clear newline
    
    if (validateYearLevel(yearInput) != 0) {
        printf("Invalid year level! Must be 1-4.\n");
        return -1;
    }
    student->personal.yearLevel = (YearLevel)yearInput;
    
    // Get grades
    printf("\n=== Academic Grades ===\n");
    
    printf("Enter Prelim Grade (0-100): ");
    if (scanf("%f", &student->academic.prelimGrade) != 1) {
        printf("Invalid prelim grade input!\n");
        return -1;
    }
    
    if (validateGrade(student->academic.prelimGrade) != 0) {
        printf("Invalid prelim grade! Must be 0-100.\n");
        return -1;
    }
    
    printf("Enter Midterm Grade (0-100): ");
    if (scanf("%f", &student->academic.midtermGrade) != 1) {
        printf("Invalid midterm grade input!\n");
        return -1;
    }
    
    if (validateGrade(student->academic.midtermGrade) != 0) {
        printf("Invalid midterm grade! Must be 0-100.\n");
        return -1;
    }
    
    printf("Enter Final Exam Grade (0-100): ");
    if (scanf("%f", &student->academic.finalExamGrade) != 1) {
        printf("Invalid final exam grade input!\n");
        return -1;
    }
    getchar(); // Clear newline
    
    if (validateGrade(student->academic.finalExamGrade) != 0) {
        printf("Invalid final exam grade! Must be 0-100.\n");
        return -1;
    }
    
    // Calculate final grade and remarks
    calculateFinalGrade(student);
    
    printf("\n=== Student Data Summary ===\n");
    displayStudentSummary(student);
    
    printf("\nConfirm this data? (Y/N): ");
    char confirm = _getch();
    printf("%c\n", confirm);
    
    if (confirm != 'Y' && confirm != 'y') {
        printf("Student data entry cancelled.\n");
        return -1;
    }
    
    return 0;
}

/**
 * @brief Handles editing student data
 * @param studentList Pointer to the student list
 * @return 0 on success, -1 on failure
 */
int handleEditStudent(list* studentList) {
    if (!studentList || !studentList->head || studentList->size == 0) {
        printf("No students to edit!\n");
        return -1;
    }
    
    char studentNumber[studentNumberLen];
    printf("Enter student number to edit: ");
    if (!fgets(studentNumber, studentNumberLen, stdin)) {
        return -1;
    }
    studentNumber[strcspn(studentNumber, "\n")] = 0;
    
    Student* student = searchStudentByNumber(studentList, studentNumber);
    if (!student) {
        printf("Student with number '%s' not found.\n", studentNumber);
        return -1;
    }
    
    printf("Current student data:\n");
    displayStudentDetails(student);
    
    printf("\nEnter new data for this student:\n");
    Student tempStudent = *student; // Backup original data
    
    if (getStudentDataFromUser(student) != 0) {
        *student = tempStudent; // Restore original data
        printf("Edit cancelled.\n");
        return -1;
    }
    
    printf("Student data updated successfully!\n");
    return 0;
}

/**
 * @brief Handles deleting a student
 * @param studentList Pointer to the student list
 * @return 0 on success, -1 on failure
 */
int handleDeleteStudent(list* studentList) {
    if (!studentList || !studentList->head || studentList->size == 0) {
        printf("No students to delete!\n");
        return -1;
    }
    
    char studentNumber[studentNumberLen];
    printf("Enter student number to delete: ");
    if (!fgets(studentNumber, studentNumberLen, stdin)) {
        return -1;
    }
    studentNumber[strcspn(studentNumber, "\n")] = 0;
    
    Student* student = searchStudentByNumber(studentList, studentNumber);
    if (!student) {
        printf("Student with number '%s' not found.\n", studentNumber);
        return -1;
    }
    
    printf("Student to delete:\n");
    displayStudentDetails(student);
    
    printf("\nAre you sure you want to delete this student? (Y/N): ");
    char confirm = _getch();
    printf("%c\n", confirm);
    
    if (confirm != 'Y' && confirm != 'y') {
        printf("Delete cancelled.\n");
        return -1;
    }
    
    if (removeStudentFromList(studentList, studentNumber) == 0) {
        printf("Student deleted successfully!\n");
        return 0;
    } else {
        printf("Failed to delete student.\n");
        return -1;
    }
}

/**
 * @brief Handles searching for students
 * @param studentList Pointer to the student list
 * @return 0 on success, -1 on failure
 */
int handleSearchStudent(list* studentList) {
    if (!studentList || !studentList->head || studentList->size == 0) {
        printf("No students to search!\n");
        return -1;
    }
    
    printf("Search by:\n");
    printf("1. Student Number\n");
    printf("2. Name\n");
    printf("Choose search method: ");
    
    char choice = _getch();
    printf("%c\n\n", choice);
    
    if (choice == '1') {
        char studentNumber[studentNumberLen];
        printf("Enter student number: ");
        if (!fgets(studentNumber, studentNumberLen, stdin)) {
            return -1;
        }
        studentNumber[strcspn(studentNumber, "\n")] = 0;
        
        Student* student = searchStudentByNumber(studentList, studentNumber);
        if (student) {
            printf("Student found:\n");
            displayStudentDetails(student);
        } else {
            printf("Student with number '%s' not found.\n", studentNumber);
        }
    } else if (choice == '2') {
        char name[studentNameLen];
        printf("Enter student name (partial match): ");
        if (!fgets(name, studentNameLen, stdin)) {
            return -1;
        }
        name[strcspn(name, "\n")] = 0;
        
        Student* student = searchStudentByName(studentList, name);
        if (student) {
            printf("Student found:\n");
            displayStudentDetails(student);
        } else {
            printf("Student with name containing '%s' not found.\n", name);
        }
    } else {
        printf("Invalid choice.\n");
        return -1;
    }
    
    return 0;
}

// Validation functions

/**
 * @brief Validates student number format
 * @param studentNumber The student number to validate
 * @param studentList The student list (for duplicate checking, can be NULL)
 * @return 0 if valid, -1 if invalid
 */
int validateStudentNumber(const char* studentNumber, list* studentList) {
    if (!studentNumber || strlen(studentNumber) == 0) {
        return -1;
    }
    
    // Check if it's not too long
    if (strlen(studentNumber) >= studentNumberLen) {
        return -1;
    }
    
    // Check for basic format (alphanumeric)
    for (int i = 0; studentNumber[i]; i++) {
        if (!isalnum(studentNumber[i]) && studentNumber[i] != '-') {
            return -1;
        }
    }
    
    // Check for duplicates if list provided
    if (studentList && searchStudentByNumber(studentList, studentNumber)) {
        printf("Student number already exists!\n");
        return -1;
    }
    
    return 0;
}

/**
 * @brief Validates student name
 * @param name The name to validate
 * @return 0 if valid, -1 if invalid
 */
int validateStudentName(const char* name) {
    if (!name || strlen(name) == 0) {
        return -1;
    }
    
    // Check if name contains only letters, spaces, and basic punctuation
    for (int i = 0; name[i]; i++) {
        if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '.' && name[i] != '-') {
            return -1;
        }
    }
    
    return 0;
}

/**
 * @brief Validates course name
 * @param course The course to validate
 * @return 0 if valid, -1 if invalid
 */
int validateCourse(const char* course) {
    // Since we're using a fixed program (IT), this is always valid
    (void)course; // Suppress unused parameter warning
    return 0;
}

/**
 * @brief Validates year level
 * @param yearLevel The year level to validate
 * @return 0 if valid, -1 if invalid
 */
int validateYearLevel(int yearLevel) {
    return (yearLevel >= 1 && yearLevel <= 4) ? 0 : -1;
}

/**
 * @brief Validates grade value
 * @param grade The grade to validate
 * @return 0 if valid, -1 if invalid
 */
int validateGrade(double grade) {
    return (grade >= 0.0 && grade <= 100.0) ? 0 : -1;
}

/**
 * @brief Displays a summary of student data
 * @param student Pointer to the student
 */
void displayStudentSummary(const Student* student) {
    if (!student) {
        return;
    }
    
    printf("Student Number: %s\n", student->personal.studentNumber);
    printf("Name: %s\n", student->personal.name.fullName);
    printf("Program: Information Technology\n");
    printf("Year Level: %d\n", student->personal.yearLevel);
    printf("Prelim: %.2f\n", student->academic.prelimGrade);
    printf("Midterm: %.2f\n", student->academic.midtermGrade);
    printf("Final Exam: %.2f\n", student->academic.finalExamGrade);
    printf("Final Grade: %.2f (%s)\n", student->academic.finalGrade, student->academic.remarks);
} 