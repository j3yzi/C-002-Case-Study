#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../models/student.h"
#include "../headers/apctxt.h"

// Global program list
Program g_programs[maxProgramCount];
int g_programCount = 0;

/**
 * @brief Loads program definitions from config.ini
 * @return Returns the number of programs loaded, or -1 on error
 */
int loadProgramsFromConfig(void) {
    char configPath[512];
    
    // Get the full path of the executable
    char executablePath[512];
    GetModuleFileName(NULL, executablePath, sizeof(executablePath));
    
    // Find the last backslash to get the directory
    char* lastSlash = strrchr(executablePath, '\\');
    if (lastSlash != NULL) {
        *lastSlash = '\0'; // Remove the executable name, keep directory
        snprintf(configPath, sizeof(configPath), "%s\\config.ini", executablePath);
    } else {
        // Fallback to current directory
        strncpy(configPath, "config.ini", sizeof(configPath) - 1);
        configPath[sizeof(configPath) - 1] = '\0';
    }
    
    FILE* file = fopen(configPath, "r");
    if (!file) {
        return -1;
    }
    
    // Reset program count
    g_programCount = 0;
    
    char line[256];
    int inProgramSection = 0;
    
    while (fgets(line, sizeof(line), file) && g_programCount < maxProgramCount) {
        // Remove trailing newline
        char* newline = strchr(line, '\n');
        if (newline) *newline = '\0';
        
        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#') continue;
        
        // Check for section header
        if (line[0] == '[') {
            inProgramSection = (strncmp(line, "[Programs]", 10) == 0);
            continue;
        }
        
        // Process program definitions
        if (inProgramSection) {
            char* equals = strchr(line, '=');
            if (equals) {
                // Extract program code and name
                *equals = '\0';
                char* code = line;
                char* name = equals + 1;
                
                // Trim whitespace
                while (*code && (*code == ' ' || *code == '\t')) code++;
                while (*name && (*name == ' ' || *name == '\t')) name++;
                
                char* end = code + strlen(code) - 1;
                while (end > code && (*end == ' ' || *end == '\t')) {
                    *end = '\0';
                    end--;
                }
                
                end = name + strlen(name) - 1;
                while (end > name && (*end == ' ' || *end == '\t')) {
                    *end = '\0';
                    end--;
                }
                
                // Add to program list if valid
                if (*code && *name) {
                    strncpy(g_programs[g_programCount].code, code, programCodeLen - 1);
                    g_programs[g_programCount].code[programCodeLen - 1] = '\0';
                    
                    strncpy(g_programs[g_programCount].name, name, programNameLen - 1);
                    g_programs[g_programCount].name[programNameLen - 1] = '\0';
                    
                    g_programCount++;
                }
            }
        }
    }
    
    fclose(file);
    
    // If no programs were loaded, add defaults
    if (g_programCount == 0) {
        strcpy(g_programs[0].code, "IT");
        strcpy(g_programs[0].name, "Information Technology");
        strcpy(g_programs[1].code, "CS");
        strcpy(g_programs[1].name, "Computer Science");
        g_programCount = 2;
    }
    
    return g_programCount;
}

/**
 * @brief Gets the program name for a given program code
 * @param programCode The program code to look up
 * @return The program name or "Unknown Program" if not found
 */
const char* getProgramName(const char* programCode) {
    if (!programCode) return "Unknown Program";
    
    for (int i = 0; i < g_programCount; i++) {
        if (strcmp(g_programs[i].code, programCode) == 0) {
            return g_programs[i].name;
        }
    }
    
    return "Unknown Program";
}

/**
 * @brief Gets the number of defined programs
 * @return The number of programs
 */
int getProgramCount(void) {
    return g_programCount;
}

/**
 * @brief Gets the array of defined programs
 * @return Pointer to the program array
 */
const Program* getPrograms(void) {
    return g_programs;
}

/**
 * @brief Adds a new program to the program list
 * @param code The program code
 * @param name The program name
 * @return 0 on success, -1 on failure
 */
int addProgram(const char* code, const char* name) {
    if (!code || !name || g_programCount >= maxProgramCount) {
        return -1;
    }
    
    // Check if program code already exists
    for (int i = 0; i < g_programCount; i++) {
        if (strcmp(g_programs[i].code, code) == 0) {
            return -1; // Program code already exists
        }
    }
    
    // Add new program
    strncpy(g_programs[g_programCount].code, code, programCodeLen - 1);
    g_programs[g_programCount].code[programCodeLen - 1] = '\0';
    
    strncpy(g_programs[g_programCount].name, name, programNameLen - 1);
    g_programs[g_programCount].name[programNameLen - 1] = '\0';
    
    g_programCount++;
    return 0;
}

/**
 * @brief Removes a program from the program list
 * @param code The program code to remove
 * @return 0 on success, -1 on failure
 */
int removeProgram(const char* code) {
    if (!code || g_programCount <= 0) {
        return -1;
    }
    
    // Find the program to remove
    int index = -1;
    for (int i = 0; i < g_programCount; i++) {
        if (strcmp(g_programs[i].code, code) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return -1; // Program not found
    }
    
    // Remove program by shifting remaining programs
    for (int i = index; i < g_programCount - 1; i++) {
        strcpy(g_programs[i].code, g_programs[i + 1].code);
        strcpy(g_programs[i].name, g_programs[i + 1].name);
    }
    
    g_programCount--;
    return 0;
}

/**
 * @brief Composes the student's full name based on their name parts.
 * @param name Pointer to the StudentName struct.
 * @return true if the name was composed successfully, false otherwise.
 */
int composeStudentName(StudentName* name) {
    if (!name) {
        return 0;
    }

    size_t fnLen = strlen(name->firstName);
    size_t mnLen = strlen(name->middleName);
    size_t lnLen = strlen(name->lastName);
    
    // Add validation check: If the first name or last name is excessive in length, reject it
    // This prevents accepting extremely long names that would be unreadable when truncated
    if (fnLen > studentNameLen - 5 || lnLen > studentNameLen - 5) {
        return 0;  // Names too long, reject
    }

    // Format 1: "Last, First" format (most compact)
    if ((lnLen + fnLen + 3) < studentNameLen) { // +3 for ", " and null
        snprintf(name->fullName, studentNameLen, "%.*s, %.*s", 
                (int)lnLen, name->lastName,
                (int)fnLen, name->firstName);
        
        // If middle name exists and there's room, add the initial
        if (mnLen > 0 && strlen(name->fullName) + 3 < studentNameLen) { // +3 for space, initial, and dot
            size_t currLen = strlen(name->fullName);
            snprintf(name->fullName + currLen, studentNameLen - currLen, " %c.", name->middleName[0]);
        }
        
        return 1;
    }
    
    // Format 2: Use initials for first name if full name is too long
    if ((lnLen + 3) < studentNameLen) { // +3 for ", " and initial and null
        snprintf(name->fullName, studentNameLen, "%.*s, %c.", 
                (int)lnLen, name->lastName,
                name->firstName[0]);
        return 1;
    }
    
    // Format 3: Truncate last name if it's still too long
    if (studentNameLen > 5) { // Make sure we have at least a few characters
        snprintf(name->fullName, studentNameLen, "%.*s...", 
                (int)(studentNameLen - 4), name->lastName);
        return 1;
    }

    // If everything fails, which shouldn't happen given the constraints above
    return 0;
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
    
    // Assign remarks based on configurable passing grade
    if (student->academic.finalGrade >= getPassingGrade()) {
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
    printf("Program: %s (%s)\n", student->personal.programCode, getProgramName(student->personal.programCode));
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
           "Student No.", "Full Name", "Program", "Year", "Final Grade", "Remarks");
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
                       student->personal.programCode,
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
                           student->personal.programCode,
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