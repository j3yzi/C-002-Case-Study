/**
 * @file student.h
 * @brief Student Data Model and Academic Management Functions
 * 
 * This header file defines the data structures and functions for managing
 * student records in the PUP Information Management System. It includes
 * structures for personal information, academic details, grade calculations,
 * program management, and various operations for creating, searching,
 * updating, and managing student data.
 * 
 * @author C002 - Group 1
 * @version 1.0
 * @date 2024
 */

#ifndef STUDENT_H
#define STUDENT_H

#include "../headers/list.h"    // For linked list data structure
#include "../headers/apctxt.h"  // For application context and configuration

/**
 * @name Student Data Structure Size Constants
 * @brief Fixed-size definitions for struct compatibility and memory management
 * 
 * These constants define the maximum sizes for various student data fields.
 * Using fixed sizes ensures consistent memory layout and compatibility
 * across different system architectures.
 * @{
 */
#define studentNumberLen 11         // Student ID: 10 characters + null terminator
#define studentNameLen 31           // Full name display: 30 characters + null terminator
#define studentFirstNameLen 32      // First name: 31 characters + null terminator
#define studentMiddleNameLen 32     // Middle name: 31 characters + null terminator
#define studentLastNameLen 32       // Last name: 31 characters + null terminator
#define studentRemarksLen 8         // Grade remarks: "Passed"/"Failed" + null terminator
#define maxStudentCreationCount 10  // Maximum students that can be created in one batch
#define programCodeLen 8            // Program code: "IT", "CS", etc. + null terminator
#define programNameLen 64           // Program full name + null terminator
#define maxProgramCount 20          // Maximum number of academic programs supported

/** @} */ // End of Student Data Structure Size Constants

/**
 * @note Configurable Academic Values
 * 
 * The following values are loaded from config.ini at runtime:
 * - Passing grade threshold (typically 75.0)
 * - Minimum grade (typically 0.0)
 * - Maximum grade (typically 100.0)
 * 
 * Use getPassingGrade() function instead of hard-coded threshold values.
 */

/**
 * @enum Gender
 * @brief Gender classification for student records
 * 
 * Defines the gender options available for student personal information.
 */
typedef enum {
    genderMale,     // Male gender
    genderFemale    // Female gender
} Gender;

/**
 * @struct Program
 * @brief Academic program definition structure
 * 
 * Represents an academic program offered by the university, including
 * both the short code (e.g., "IT", "CS") and the full program name.
 */
typedef struct {
    char code[programCodeLen];  // Short program code (e.g., "IT", "CS", "EE")
    char name[programNameLen];  // Full program name (e.g., "Information Technology")
} Program;

/**
 * @name Global Program Management
 * @brief Global variables for managing academic programs
 * @{
 */

/**
 * @brief Global array of available academic programs
 * 
 * Contains all the academic programs that students can be enrolled in.
 * This array is populated from the configuration file at startup.
 */
extern Program g_programs[maxProgramCount];

/**
 * @brief Current number of programs loaded in the system
 * 
 * Tracks how many programs are currently available in the g_programs array.
 */
extern int g_programCount;

/** @} */ // End of Global Program Management

/**
 * @enum YearLevel
 * @brief Academic year level classification
 * 
 * Defines the different year levels available for students in their
 * academic program progression.
 */
typedef enum {
    YEAR_FIRST = 1,     // First year (freshman)
    YEAR_SECOND,        // Second year (sophomore)
    YEAR_THIRD,         // Third year (junior)
    YEAR_FOURTH         // Fourth year (senior)
} YearLevel;

/**
 * @enum AcademicStanding
 * @brief Academic performance classification
 * 
 * Defines the different academic standings based on student performance.
 * This affects academic privileges and requirements.
 */
typedef enum {
    acadDeansLister,    // Dean's List status (≥ 90 final grade)
    acadRegular,        // Regular academic standing
    acadProbation       // Academic probation (poor performance)
} AcademicStanding;

/**
 * @struct StudentName
 * @brief Structure for storing student name components
 * 
 * Separates the student's name into individual components for better
 * data organization and search capabilities. The fullName field is
 * typically a concatenation or formatted version of the individual names.
 */
typedef struct {
    char firstName[studentFirstNameLen];    // Student's first/given name
    char middleName[studentMiddleNameLen];  // Student's middle name or initial
    char lastName[studentLastNameLen];      // Student's last/family name
    char fullName[studentNameLen];          // Formatted full name for display
} StudentName;

/**
 * @struct StudentInfo
 * @brief Structure for student personal and enrollment information
 * 
 * Contains the essential identifying and enrollment information for a student,
 * including their unique student number, name, demographics, and program details.
 */
typedef struct {
    char studentNumber[studentNumberLen];   // Unique student identifier
    StudentName name;                       // Complete name information
    Gender gender;                          // Student's gender
    char programCode[programCodeLen];       // Academic program code (e.g., "IT")
    YearLevel yearLevel;                    // Current year level (1-4)
} StudentInfo;

/**
 * @struct AcademicInfo
 * @brief Structure for academic performance and grading information
 * 
 * Contains all academic performance data including enrollment details,
 * individual grade components, calculated final grade, and pass/fail status.
 * Grade calculations follow the formula: (prelim + midterm + finalExam) / 3
 */
typedef struct {
    int unitsEnrolled;                      // Total academic units enrolled
    float prelimGrade;                      // Preliminary examination grade
    float midtermGrade;                     // Midterm examination grade
    float finalExamGrade;                   // Final examination grade
    float finalGrade;                       // Computed average grade
    char remarks[studentRemarksLen];        // "Passed" or "Failed" status
} AcademicInfo;

/**
 * @struct Student
 * @brief Complete student record structure
 * 
 * The main student data structure that combines all student information
 * including personal details, academic performance, and standing.
 * This is the primary data type used throughout the student management system.
 */
typedef struct {
    StudentInfo personal;           // Personal and enrollment information
    AcademicInfo academic;          // Academic performance data
    AcademicStanding standing;      // Current academic standing
} Student;

/**
 * @name Program Management Functions
 * @brief Functions for managing academic programs
 * @{
 */

/**
 * @brief Loads academic programs from configuration file
 * 
 * Reads the list of available academic programs from the configuration
 * file and populates the global program array. This is typically called
 * during system initialization.
 * 
 * @return Number of programs loaded, or negative on error
 */
int loadProgramsFromConfig(void);

/**
 * @brief Gets the full program name from a program code
 * 
 * Looks up a program code (e.g., "IT") and returns the corresponding
 * full program name (e.g., "Information Technology").
 * 
 * @param programCode Short program code to look up
 * @return Pointer to program name string, or NULL if not found
 */
const char* getProgramName(const char* programCode);

/**
 * @brief Gets the current number of loaded programs
 * 
 * Returns the count of academic programs currently available in the system.
 * 
 * @return Number of programs loaded
 */
int getProgramCount(void);

/**
 * @brief Gets a pointer to the programs array
 * 
 * Provides read-only access to the global programs array for iteration
 * or display purposes.
 * 
 * @return Pointer to the programs array
 */
const Program* getPrograms(void);

/**
 * @brief Adds a new academic program to the system
 * 
 * Adds a new program with the specified code and name to the global
 * programs list. Performs validation to prevent duplicates.
 * 
 * @param code Short program code (e.g., "CS")
 * @param name Full program name (e.g., "Computer Science")
 * @return 0 on success, non-zero on error
 */
int addProgram(const char* code, const char* name);

/**
 * @brief Removes an academic program from the system
 * 
 * Removes a program with the specified code from the global programs list.
 * This should be used carefully as it may affect existing student records.
 * 
 * @param code Program code to remove
 * @return 0 on success, non-zero on error
 */
int removeProgram(const char* code);

/** @} */ // End of Program Management Functions

/**
 * @name Student Name and Grade Management Functions
 * @brief Functions for handling student names and grade calculations
 * @{
 */

/**
 * @brief Composes and formats a student's full name
 * 
 * Takes the individual name components (first, middle, last) and creates
 * a properly formatted full name string for display purposes.
 * 
 * @param name Pointer to StudentName structure to process
 * @return 0 on success, non-zero on error
 */
int composeStudentName(StudentName* name);

/**
 * @brief Calculates the final grade and determines pass/fail status
 * 
 * Computes the final grade using the formula: (prelim + midterm + finalExam) / 3
 * Also determines academic standing and sets the remarks field based on
 * the passing grade threshold from configuration.
 * 
 * @param student Pointer to student record to update
 */
void calculateFinalGrade(Student* student);

/** @} */ // End of Student Name and Grade Management Functions

/**
 * @name Search Operations
 * @brief Functions for finding students in the system
 * @{
 */

/**
 * @brief Searches for a student by student number
 * 
 * Performs a linear search through the student list to find a student
 * with the specified student number (unique identifier).
 * 
 * @param studentList Pointer to the student list to search
 * @param studentNumber Student number to search for
 * @return Pointer to found student, or NULL if not found
 */
Student* searchStudentByNumber(const list* studentList, const char* studentNumber);

/**
 * @brief Searches for a student by last name
 * 
 * Performs a linear search through the student list to find a student
 * with the specified last name. Returns the first match found.
 * 
 * @param studentList Pointer to the student list to search
 * @param lastName Last name to search for
 * @return Pointer to found student, or NULL if not found
 */
Student* searchStudentByName(const list* studentList, const char* lastName);

/** @} */ // End of Search Operations

/**
 * @name Display Operations
 * @brief Functions for displaying student information
 * @{
 */

/**
 * @brief Displays detailed information for a single student
 * 
 * Prints a formatted display of all student information including
 * personal details, academic performance, and current standing.
 * 
 * @param student Pointer to the student to display
 */
void displayStudentDetails(const Student* student);

/**
 * @brief Displays a summary of all students in the list
 * 
 * Prints a formatted table showing key information for all students
 * in the list. Typically includes student number, name, program, grades, and standing.
 * 
 * @param studentList Pointer to the student list to display
 */
void displayAllStudents(const list* studentList);

/** @} */ // End of Display Operations

/**
 * @name Student Creation and Management
 * @brief Functions for creating and managing student records
 * @{
 */

/**
 * @brief Creates a new student record and adds it to the list
 * 
 * Creates a new student with the provided data and adds it to the
 * specified student list. Performs validation and ensures data integrity.
 * Automatically calculates final grades and academic standing.
 * 
 * @param student Pointer to student data to add
 * @param studentList Double pointer to the student list
 * @return 0 on success, non-zero on error
 */
int createStudent(Student* student, list** studentList);

/**
 * @brief Creates a new empty student list
 * 
 * Initializes a new linked list specifically configured for storing
 * student records. Sets up the appropriate list type and structure.
 * 
 * @param studentList Double pointer to store the new list
 * @return 0 on success, non-zero on error
 */
int createStudentList(list** studentList);

/**
 * @brief Updates an existing student record with new data
 * 
 * Replaces the data in an existing student record with new information.
 * Performs validation, recalculates grades, and updates academic standing.
 * 
 * @param existingStudent Pointer to the student record to update
 * @param newData Pointer to the new student data
 * @return 0 on success, non-zero on error
 */
int updateStudentData(Student* existingStudent, const Student* newData);

/**
 * @brief Removes a student from the list by student number
 * 
 * Searches for a student by their student number and removes them
 * from the list. Properly frees associated memory.
 * 
 * @param studentList Pointer to the student list
 * @param studentNumber Student number of the student to remove
 * @return 0 on success, non-zero on error
 */
int removeStudentFromList(list* studentList, const char* studentNumber);

/**
 * @brief Memory cleanup function for student data
 * 
 * Callback function used by the list management system to properly
 * free memory associated with student records when removing them
 * from lists or destroying lists.
 * 
 * @param student Void pointer to student data to free
 */
void freeStudent(void* student);

/** @} */ // End of Student Creation and Management

#endif // STUDENT_H 