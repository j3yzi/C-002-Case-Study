#include <stdio.h>
#include "../models/student.h"
#include <string.h>
#include <stdlib.h>

/**
 * @brief Composes the student's full name based on their name parts.
 * @param name Pointer to the StudentName struct.
 * @return true if the name was composed successfully, false otherwise.
 */
bool composeStudentName(StudentName* name) {
    if (!name) {
        return false;
    }

    size_t fnLen = strlen(name->firstName);
    size_t mnLen = strlen(name->middleName);
    size_t lnLen = strlen(name->lastName);
    size_t totalLen = fnLen + mnLen + lnLen;

    // If total length is exactly studentNameLen - 1, use LastName+FirstName+MiddleName.
    if (totalLen == (studentNameLen - 1)) {
        snprintf(name->fullName, studentNameLen, "%s%s%s", name->lastName, name->firstName, name->middleName);
        return true;
    }

    // For all other cases, try LastName+FirstName.
    if ((lnLen + fnLen) < studentNameLen) {
        snprintf(name->fullName, studentNameLen, "%s%s", name->lastName, name->firstName);
        return true;
    }

    // If LastName+FirstName is too long, or the name is invalid, reject.
    return false;
}

/**
 * @brief Calculates the final grade and assigns remarks for a student.
 * @param student Pointer to the Student struct.
 */
void calculateFinalGrade(Student* student) {
    if (!student) return;
    
    // Calculate final grade: (Prelim + Midterm + Final Exam) / 3
    student->academic.finalGrade = (student->academic.prelimGrade + 
                                   student->academic.midtermGrade + 
                                   student->academic.finalExamGrade) / 3.0f;
    
    // Assign remarks based on final grade
    if (student->academic.finalGrade >= 75.0f) {
        strcpy(student->academic.remarks, "Passed");
    } else {
        strcpy(student->academic.remarks, "Failed");
    }
}

/**
 * @brief Searches for a student by student number.
 * @param studentList Pointer to the student list.
 * @param studentNumber The student number to search for.
 * @return Pointer to the student if found, NULL otherwise.
 */
Student* searchStudentByNumber(const list* studentList, const char* studentNumber) {
    if (!studentList || !studentNumber || !studentList->head) {
        return NULL;
    }
    
    node* current = studentList->head;
    
    if (studentList->type == SINGLY || studentList->type == DOUBLY) {
        // Non-circular lists
        while (current != NULL) {
            Student* student = (Student*)current->data;
            if (student && strcmp(student->personal.studentNumber, studentNumber) == 0) {
                return student;
            }
            current = current->next;
        }
    } else {
        // Circular lists
        if (current != NULL) {
            do {
                Student* student = (Student*)current->data;
                if (student && strcmp(student->personal.studentNumber, studentNumber) == 0) {
                    return student;
                }
                current = current->next;
            } while (current != studentList->head);
        }
    }
    
    return NULL;
}

/**
 * @brief Searches for a student by full name.
 * @param studentList Pointer to the student list.
 * @param fullName The full name to search for.
 * @return Pointer to the student if found, NULL otherwise.
 */
Student* searchStudentByName(const list* studentList, const char* fullName) {
    if (!studentList || !fullName || !studentList->head) {
        return NULL;
    }
    
    node* current = studentList->head;
    
    if (studentList->type == SINGLY || studentList->type == DOUBLY) {
        // Non-circular lists
        while (current != NULL) {
            Student* student = (Student*)current->data;
            if (student && strcmp(student->personal.name.fullName, fullName) == 0) {
                return student;
            }
            current = current->next;
        }
    } else {
        // Circular lists
        if (current != NULL) {
            do {
                Student* student = (Student*)current->data;
                if (student && strcmp(student->personal.name.fullName, fullName) == 0) {
                    return student;
                }
                current = current->next;
            } while (current != studentList->head);
        }
    }
    
    return NULL;
}

/**
 * @brief Displays detailed information about a student.
 * @param student Pointer to the Student struct.
 */
void displayStudentDetails(const Student* student) {
    if (!student) {
        printf("Error: Invalid student data.\n");
        return;
    }
    
    printf("Student Number: %s\n", student->personal.studentNumber);
    printf("Full Name: %s\n", student->personal.name.fullName);
    printf("First Name: %s\n", student->personal.name.firstName);
    printf("Middle Name: %s\n", student->personal.name.middleName);
    printf("Last Name: %s\n", student->personal.name.lastName);
    printf("Gender: %s\n", (student->personal.gender == genderMale) ? "Male" : "Female");
    printf("Program: %s\n", (student->personal.programCode == PROG_IT) ? "Information Technology" : "Computer Science");
    printf("Year Level: %d\n", student->personal.yearLevel);
    printf("Units Enrolled: %d\n", student->academic.unitsEnrolled);
    printf("Prelim Grade: %.2f\n", student->academic.prelimGrade);
    printf("Midterm Grade: %.2f\n", student->academic.midtermGrade);
    printf("Final Exam Grade: %.2f\n", student->academic.finalExamGrade);
    printf("Final Grade: %.2f\n", student->academic.finalGrade);
    printf("Remarks: %s\n", student->academic.remarks);
    printf("Academic Standing: %s\n", (student->standing == acadRegular) ? "Regular" : "Probation");
}

/**
 * @brief Displays all students in tabular format.
 * @param studentList Pointer to the student list.
 */
void displayAllStudents(const list* studentList) {
    if (!studentList || !studentList->head || studentList->size == 0) {
        printf("No students to display.\n");
        return;
    }
    
    printf("%-12s | %-25s | %-8s | %-4s | %-11s | %-7s\n",
           "Student No.", "Full Name", "Course", "Year", "Final Grade", "Remarks");
    printf("------------------------------------------------------------------------------\n");
    
    node* current = studentList->head;
    int count = 0;
    
    if (studentList->type == SINGLY || studentList->type == DOUBLY) {
        // Non-circular lists
        while (current != NULL && count < studentList->size) {
            Student* student = (Student*)current->data;
            if (student) {
                printf("%-12s | %-25s | %-8s | %-4d | %11.2f | %-7s\n",
                       student->personal.studentNumber,
                       student->personal.name.fullName,
                       (student->personal.programCode == PROG_IT) ? "IT" : "CS",
                       student->personal.yearLevel,
                       student->academic.finalGrade,
                       student->academic.remarks);
                count++;
            }
            current = current->next;
        }
    } else {
        // Circular lists
        if (current != NULL) {
            do {
                Student* student = (Student*)current->data;
                if (student) {
                    printf("%-12s | %-25s | %-8s | %-4d | %11.2f | %-7s\n",
                           student->personal.studentNumber,
                           student->personal.name.fullName,
                           (student->personal.programCode == PROG_IT) ? "IT" : "CS",
                           student->personal.yearLevel,
                           student->academic.finalGrade,
                           student->academic.remarks);
                    count++;
                }
                current = current->next;
            } while (current != studentList->head && count < studentList->size);
        }
    }
    
    printf("------------------------------------------------------------------------------\n");
    printf("Total students: %d\n", count);
}

/**
 * @brief Creates a student and adds it to the list.
 * @param student Pointer to the Student struct.
 * @param studentList Pointer to the student list pointer.
 * @return 0 on success, -1 on failure.
 */
int createStudent(Student* student, list** studentList) {
    if (!student || !studentList) {
        return -1;
    }
    
    // If list doesn't exist, create it
    if (*studentList == NULL) {
        if (createList(studentList, SINGLY) != 0) {
            return -1;
        }
    }
    
    return addNode(studentList, student);
}

/**
 * @brief Creates a new student list.
 * @param studentList Pointer to the student list pointer.
 * @return 0 on success, -1 on failure.
 */
int createStudentList(list** studentList) {
    if (!studentList) {
        return -1;
    }
    
    return createList(studentList, SINGLY);
}

/**
 * @brief Updates student data.
 * @param existingStudent Pointer to the existing student.
 * @param newData Pointer to the new student data.
 * @return 0 on success, -1 on failure.
 */
int updateStudentData(Student* existingStudent, const Student* newData) {
    if (!existingStudent || !newData) {
        return -1;
    }
    
    // Copy new data to existing student
    memcpy(existingStudent, newData, sizeof(Student));
    
    // Recalculate final grade and remarks
    calculateFinalGrade(existingStudent);
    
    return 0;
}

/**
 * @brief Removes a student from the list by student number.
 * @param studentList Pointer to the student list.
 * @param studentNumber The student number to remove.
 * @return 0 on success, -1 on failure.
 */
int removeStudentFromList(list* studentList, const char* studentNumber) {
    if (!studentList || !studentNumber) {
        return -1;
    }
    
    Student* student = searchStudentByNumber(studentList, studentNumber);
    if (!student) {
        return -1; // Student not found
    }
    
    // Find and remove the node containing this student
    node* current = studentList->head;
    if (studentList->type == SINGLY || studentList->type == DOUBLY) {
        // Non-circular lists
        while (current != NULL) {
            if (current->data == student) {
                removeNode(studentList, student, freeStudent);
                return 0;
            }
            current = current->next;
        }
    } else {
        // Circular lists
        if (current != NULL) {
            do {
                if (current->data == student) {
                    removeNode(studentList, student, freeStudent);
                    return 0;
                }
                current = current->next;
            } while (current != studentList->head);
        }
    }
    
    return -1; // Student not found in list
}

/**
 * @brief Frees student memory.
 * @param student Pointer to the student to free.
 */
void freeStudent(void* student) {
    if (student) {
        free(student);
    }
} 