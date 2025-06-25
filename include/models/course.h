#ifndef COURSE_H
#define COURSE_H

#include <stdbool.h>
#include "../headers/list.h"

// Maximum lengths for string fields
#define courseCodeLen 10
#define courseNameLen 50
#define courseDescLen 200

// Course types
typedef enum {
    CORE,
    MAJOR,
    ELECTIVE,
    GENERAL_ED
} CourseType;

// Course structure
typedef struct {
    char code[courseCodeLen];        // Course code (e.g., "IT101")
    char name[courseNameLen];        // Course name (e.g., "Introduction to Programming")
    char description[courseDescLen]; // Course description
    int units;                       // Number of units/credits
    CourseType type;                 // Type of course
    bool isActive;                   // Whether the course is currently active
} Course;

// Course catalog manager
typedef struct {
    list* courseList;                // List of courses
    char catalogName[50];            // Name of the catalog
    bool isModified;                 // Whether the catalog has been modified since last save
} CourseCatalog;

// Function declarations

/**
 * @brief Creates a new course catalog
 * @param catalog Pointer to the catalog to initialize
 * @param name Name for the catalog
 * @return 0 on success, -1 on failure
 */
int createCourseCatalog(CourseCatalog* catalog, const char* name);

/**
 * @brief Frees resources used by a course catalog
 * @param catalog Pointer to the catalog to clean up
 */
void destroyCourseCatalog(CourseCatalog* catalog);

/**
 * @brief Adds a course to the catalog
 * @param catalog Pointer to the catalog
 * @param course Pointer to the course to add
 * @return 0 on success, -1 on failure
 */
int addCourse(CourseCatalog* catalog, Course* course);

/**
 * @brief Removes a course from the catalog by course code
 * @param catalog Pointer to the catalog
 * @param courseCode Code of the course to remove
 * @return 0 on success, -1 on failure
 */
int removeCourse(CourseCatalog* catalog, const char* courseCode);

/**
 * @brief Updates an existing course in the catalog
 * @param catalog Pointer to the catalog
 * @param courseCode Code of the course to update
 * @param updatedCourse Pointer to the updated course data
 * @return 0 on success, -1 on failure
 */
int updateCourse(CourseCatalog* catalog, const char* courseCode, const Course* updatedCourse);

/**
 * @brief Searches for a course by code
 * @param catalog Pointer to the catalog
 * @param courseCode Code of the course to find
 * @return Pointer to the course if found, NULL otherwise
 */
Course* findCourseByCode(const CourseCatalog* catalog, const char* courseCode);

/**
 * @brief Searches for courses by name (partial match)
 * @param catalog Pointer to the catalog
 * @param courseName Name to search for
 * @param results Array to store matching courses
 * @param maxResults Maximum number of results to return
 * @return Number of matching courses found
 */
int findCoursesByName(const CourseCatalog* catalog, const char* courseName, Course** results, int maxResults);

/**
 * @brief Saves the course catalog to a file
 * @param catalog Pointer to the catalog
 * @param filename Name of the file to save to
 * @return 0 on success, -1 on failure
 */
int saveCatalog(const CourseCatalog* catalog, const char* filename);

/**
 * @brief Loads a course catalog from a file
 * @param catalog Pointer to the catalog to load into
 * @param filename Name of the file to load from
 * @return 0 on success, -1 on failure
 */
int loadCatalog(CourseCatalog* catalog, const char* filename);

/**
 * @brief Displays all courses in the catalog
 * @param catalog Pointer to the catalog
 */
void displayAllCourses(const CourseCatalog* catalog);

/**
 * @brief Displays detailed information about a course
 * @param course Pointer to the course
 */
void displayCourseDetails(const Course* course);

/**
 * @brief Memory deallocation function for course objects
 * @param courseData Pointer to the course data to free
 */
void freeCourse(void* courseData);

/**
 * @brief Gets a string representation of a course type
 * @param type The course type
 * @return String representation of the course type
 */
const char* getCourseTypeString(CourseType type);

#endif // COURSE_H 