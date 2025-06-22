#ifndef STUDENT_H
#define STUDENT_H

#define STUDENT_NUMBER_LEN 11 // 10 chars + null terminator
#define STUDENT_NAME_LEN 31   // 30 chars + null terminator

typedef enum {
    GENDER_MALE,
    GENDER_FEMALE
} Gender;

typedef enum {
    PROG_IT, // Information Technology
    PROG_CS, // Computer Science
    // Add other programs as needed
} ProgramCode;

typedef enum {
    YEAR_FIRST = 1,
    YEAR_SECOND,
    YEAR_THIRD,
    YEAR_FOURTH
} YearLevel;

typedef enum {
    ACAD_DEANS_LISTER,
    ACAD_REGULAR,
    ACAD_PROBATION
} AcademicStanding;


typedef struct {
    char studentNumber[STUDENT_NUMBER_LEN];
    char fullName[STUDENT_NAME_LEN];
    Gender gender;
    ProgramCode programCode;
    YearLevel yearLevel;
} StudentInfo;

typedef struct {
    int unitsEnrolled;
    float prelimGrade;
    float midtermGrade;
    float finalGrade;
    float averageGrade;
} AcademicInfo;

typedef struct {
    StudentInfo personal;
    AcademicInfo academic;
    AcademicStanding standing;
} Student;


#endif 