#ifndef STUDENT_H
#define STUDENT_H

#include "../headers/list.h"
#include "../headers/apctxt.h"

// Fixed-size definitions for struct compatibility  
#define studentNumberLen 11 // 10 chars + null terminator
#define studentNameLen 31   // 30 chars + null terminator
#define studentFirstNameLen 32
#define studentMiddleNameLen 32
#define studentLastNameLen 32
#define studentRemarksLen 8 // "Passed" or "Failed" + null terminator
#define maxStudentCreationCount 10
#define programCodeLen 8    // Program code length (e.g., "IT", "CS") + null terminator
#define programNameLen 64   // Program name length + null terminator
#define maxProgramCount 20  // Maximum number of programs that can be defined

// Configurable business values (loaded from config.ini)
// Use getPassingGrade() function instead of hard-coded threshold

typedef enum {
    genderMale,
    genderFemale
} Gender;

// Program structure for configurable programs
typedef struct {
    char code[programCodeLen];
    char name[programNameLen];
} Program;

// Global program list
extern Program g_programs[maxProgramCount];
extern int g_programCount;

typedef enum {
    YEAR_FIRST = 1,
    YEAR_SECOND,
    YEAR_THIRD,
    YEAR_FOURTH
} YearLevel;

typedef enum {
    acadDeansLister, // New academic standing for Dean's List (≥ 90 final grade)
    acadRegular,
    acadProbation
} AcademicStanding;


typedef struct {
    char firstName[studentFirstNameLen];
    char middleName[studentMiddleNameLen];
    char lastName[studentLastNameLen];
    char fullName[studentNameLen];
} StudentName;

typedef struct {
    char studentNumber[studentNumberLen];
    StudentName name;
    Gender gender;
    char programCode[programCodeLen]; // Changed from enum to string
    YearLevel yearLevel;
} StudentInfo;

typedef struct {
    int unitsEnrolled;
    float prelimGrade;
    float midtermGrade;
    float finalExamGrade;
    float finalGrade;  // Computed from (prelim + midterm + finalExam) / 3
    char remarks[studentRemarksLen]; // "Passed" or "Failed"
} AcademicInfo;

typedef struct {
    StudentInfo personal;
    AcademicInfo academic;
    AcademicStanding standing;
} Student;

// Program management functions
int loadProgramsFromConfig(void);
const char* getProgramName(const char* programCode);
int getProgramCount(void);
const Program* getPrograms(void);
int addProgram(const char* code, const char* name);
int removeProgram(const char* code);

// Function declarations  
int composeStudentName(StudentName* name);
void calculateFinalGrade(Student* student);
Student* searchStudentByNumber(const list* studentList, const char* studentNumber);
Student* searchStudentByName(const list* studentList, const char* fullName);
void displayStudentDetails(const Student* student);
void displayAllStudents(const list* studentList);
int createStudent(Student* student, list** studentList);
int createStudentList(list** studentList);
int updateStudentData(Student* existingStudent, const Student* newData);
int removeStudentFromList(list* studentList, const char* studentNumber);
void freeStudent(void* student);

#endif 