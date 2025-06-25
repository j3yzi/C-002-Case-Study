#ifndef COURSEIO_H
#define COURSEIO_H

#include "../../include/models/course.h"

// Course manager for the UI
typedef struct {
    CourseCatalog catalog;
    char currentCatalogFile[100];
    bool hasUnsavedChanges;
} CourseCatalogManager;

// Function declarations

/**
 * @brief Initializes the course catalog manager
 */
void initCourseCatalogManager(void);

/**
 * @brief Cleans up the course catalog manager
 */
void cleanupCourseCatalogManager(void);

/**
 * @brief Main menu for course management
 * @return Returns 0 on normal exit, other values on error
 */
int runCourseManagement(void);

/**
 * @brief Handles creating a new course catalog
 * @return Returns 0 on success, -1 on failure
 */
int handleCreateCourseCatalog(void);

/**
 * @brief Handles adding a new course to the catalog
 * @return Returns 0 on success, -1 on failure
 */
int handleAddCourse(void);

/**
 * @brief Handles editing an existing course
 * @return Returns 0 on success, -1 on failure
 */
int handleEditCourse(void);

/**
 * @brief Handles deleting a course from the catalog
 * @return Returns 0 on success, -1 on failure
 */
int handleDeleteCourse(void);

/**
 * @brief Handles searching for courses
 * @return Returns 0 on success, -1 on failure
 */
int handleSearchCourses(void);

/**
 * @brief Handles displaying all courses in the catalog
 * @return Returns 0 on success, -1 on failure
 */
int handleDisplayAllCourses(void);

/**
 * @brief Handles saving the course catalog
 * @return Returns 0 on success, -1 on failure
 */
int handleSaveCatalog(void);

/**
 * @brief Handles loading a course catalog
 * @return Returns 0 on success, -1 on failure
 */
int handleLoadCatalog(void);

/**
 * @brief Gets course data from user input
 * @param course Pointer to the course structure to fill
 * @param isNew Whether this is a new course (true) or editing an existing one (false)
 * @return Returns 0 on success, -1 on failure or cancellation
 */
int getCourseDataFromUser(Course* course, bool isNew);

/**
 * @brief Lists all course catalog files in the data directory
 * @return Returns 1 if files were found, 0 otherwise
 */
int listCourseCatalogFiles(void);

#endif // COURSEIO_H 