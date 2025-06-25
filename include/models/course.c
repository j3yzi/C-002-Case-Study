#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "course.h"
#include "../headers/list.h"
#include "../headers/apctxt.h"

/**
 * @brief Creates a new course catalog
 * @param catalog Pointer to the catalog to initialize
 * @param name Name for the catalog
 * @return 0 on success, -1 on failure
 */
int createCourseCatalog(CourseCatalog* catalog, const char* name) {
    if (!catalog || !name) {
        return -1;
    }
    
    // Initialize catalog fields
    catalog->courseList = NULL;
    strncpy(catalog->catalogName, name, sizeof(catalog->catalogName) - 1);
    catalog->catalogName[sizeof(catalog->catalogName) - 1] = '\0';
    catalog->isModified = false;
    
    // Create the list to store courses
    if (createList(&catalog->courseList, SINGLY) != 0) {
        return -1;
    }
    
    return 0;
}

/**
 * @brief Frees resources used by a course catalog
 * @param catalog Pointer to the catalog to clean up
 */
void destroyCourseCatalog(CourseCatalog* catalog) {
    if (!catalog) {
        return;
    }
    
    // Free the course list and all courses in it
    if (catalog->courseList) {
        destroyList(&catalog->courseList, freeCourse);
    }
    
    // Reset catalog fields
    catalog->courseList = NULL;
    catalog->isModified = false;
}

/**
 * @brief Adds a course to the catalog
 * @param catalog Pointer to the catalog
 * @param course Pointer to the course to add
 * @return 0 on success, -1 on failure
 */
int addCourse(CourseCatalog* catalog, Course* course) {
    if (!catalog || !course || !catalog->courseList) {
        return -1;
    }
    
    // Check if a course with the same code already exists
    if (findCourseByCode(catalog, course->code) != NULL) {
        return -1; // Course code must be unique
    }
    
    // Add the course to the list
    if (addNode(&catalog->courseList, course) != 0) {
        return -1;
    }
    
    catalog->isModified = true;
    return 0;
}

/**
 * @brief Removes a course from the catalog by course code
 * @param catalog Pointer to the catalog
 * @param courseCode Code of the course to remove
 * @return 0 on success, -1 on failure
 */
int removeCourse(CourseCatalog* catalog, const char* courseCode) {
    if (!catalog || !courseCode || !catalog->courseList || !catalog->courseList->head) {
        return -1;
    }
    
    // Find the course to remove
    Course* course = findCourseByCode(catalog, courseCode);
    if (!course) {
        return -1; // Course not found
    }
    
    // Remove the course from the list
    removeNode(catalog->courseList, course, freeCourse);
    catalog->isModified = true;
    
    return 0;
}

/**
 * @brief Updates an existing course in the catalog
 * @param catalog Pointer to the catalog
 * @param courseCode Code of the course to update
 * @param updatedCourse Pointer to the updated course data
 * @return 0 on success, -1 on failure
 */
int updateCourse(CourseCatalog* catalog, const char* courseCode, const Course* updatedCourse) {
    if (!catalog || !courseCode || !updatedCourse || !catalog->courseList) {
        return -1;
    }
    
    // Find the course to update
    Course* course = findCourseByCode(catalog, courseCode);
    if (!course) {
        return -1; // Course not found
    }
    
    // Check if the updated course code is different and already exists
    if (strcmp(courseCode, updatedCourse->code) != 0) {
        if (findCourseByCode(catalog, updatedCourse->code) != NULL) {
            return -1; // New course code already exists
        }
    }
    
    // Update the course data
    strncpy(course->code, updatedCourse->code, courseCodeLen - 1);
    course->code[courseCodeLen - 1] = '\0';
    
    strncpy(course->name, updatedCourse->name, courseNameLen - 1);
    course->name[courseNameLen - 1] = '\0';
    
    strncpy(course->description, updatedCourse->description, courseDescLen - 1);
    course->description[courseDescLen - 1] = '\0';
    
    course->units = updatedCourse->units;
    course->type = updatedCourse->type;
    course->isActive = updatedCourse->isActive;
    
    catalog->isModified = true;
    return 0;
}

/**
 * @brief Searches for a course by code
 * @param catalog Pointer to the catalog
 * @param courseCode Code of the course to find
 * @return Pointer to the course if found, NULL otherwise
 */
Course* findCourseByCode(const CourseCatalog* catalog, const char* courseCode) {
    if (!catalog || !courseCode || !catalog->courseList || !catalog->courseList->head) {
        return NULL;
    }
    
    node* current = catalog->courseList->head;
    
    // Handle different list types
    if (catalog->courseList->type == SINGLY || catalog->courseList->type == DOUBLY) {
        // Non-circular list
        while (current) {
            Course* course = (Course*)current->data;
            if (course && strcmp(course->code, courseCode) == 0) {
                return course;
            }
            current = current->next;
        }
    } else {
        // Circular list
        do {
            Course* course = (Course*)current->data;
            if (course && strcmp(course->code, courseCode) == 0) {
                return course;
            }
            current = current->next;
        } while (current != catalog->courseList->head);
    }
    
    return NULL;
}

/**
 * @brief Searches for courses by name (partial match)
 * @param catalog Pointer to the catalog
 * @param courseName Name to search for
 * @param results Array to store matching courses
 * @param maxResults Maximum number of results to return
 * @return Number of matching courses found
 */
int findCoursesByName(const CourseCatalog* catalog, const char* courseName, Course** results, int maxResults) {
    if (!catalog || !courseName || !results || maxResults <= 0 || !catalog->courseList || !catalog->courseList->head) {
        return 0;
    }
    
    int foundCount = 0;
    node* current = catalog->courseList->head;
    
    // Convert search term to lowercase for case-insensitive search
    char searchTerm[courseNameLen];
    strncpy(searchTerm, courseName, courseNameLen - 1);
    searchTerm[courseNameLen - 1] = '\0';
    
    for (int i = 0; searchTerm[i]; i++) {
        searchTerm[i] = tolower(searchTerm[i]);
    }
    
    // Handle different list types
    if (catalog->courseList->type == SINGLY || catalog->courseList->type == DOUBLY) {
        // Non-circular list
        while (current && foundCount < maxResults) {
            Course* course = (Course*)current->data;
            if (course) {
                // Convert course name to lowercase for comparison
                char courseLower[courseNameLen];
                strncpy(courseLower, course->name, courseNameLen - 1);
                courseLower[courseNameLen - 1] = '\0';
                
                for (int i = 0; courseLower[i]; i++) {
                    courseLower[i] = tolower(courseLower[i]);
                }
                
                // Check if course name contains the search term
                if (strstr(courseLower, searchTerm) != NULL) {
                    results[foundCount++] = course;
                }
            }
            current = current->next;
        }
    } else {
        // Circular list
        do {
            Course* course = (Course*)current->data;
            if (course) {
                // Convert course name to lowercase for comparison
                char courseLower[courseNameLen];
                strncpy(courseLower, course->name, courseNameLen - 1);
                courseLower[courseNameLen - 1] = '\0';
                
                for (int i = 0; courseLower[i]; i++) {
                    courseLower[i] = tolower(courseLower[i]);
                }
                
                // Check if course name contains the search term
                if (strstr(courseLower, searchTerm) != NULL) {
                    results[foundCount++] = course;
                    if (foundCount >= maxResults) {
                        break;
                    }
                }
            }
            current = current->next;
        } while (current != catalog->courseList->head);
    }
    
    return foundCount;
}

/**
 * @brief Saves the course catalog to a file
 * @param catalog Pointer to the catalog
 * @param filename Name of the file to save to
 * @return 0 on success, -1 on failure
 */
int saveCatalog(const CourseCatalog* catalog, const char* filename) {
    if (!catalog || !filename || !catalog->courseList) {
        return -1;
    }
    
    // Create data directory if it doesn't exist
    appCreateDirectory("data");
    
    // Construct the full path
    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "data/%s", filename);
    
    // Open the file for writing
    FILE* file = fopen(fullPath, "wb");
    if (!file) {
        return -1;
    }
    
    // Write catalog name
    fwrite(catalog->catalogName, sizeof(catalog->catalogName), 1, file);
    
    // Write number of courses
    int count = catalog->courseList->size;
    fwrite(&count, sizeof(int), 1, file);
    
    // Write each course
    node* current = catalog->courseList->head;
    int saved = 0;
    
    if (catalog->courseList->type == SINGLY || catalog->courseList->type == DOUBLY) {
        // Non-circular list
        while (current) {
            Course* course = (Course*)current->data;
            if (course) {
                fwrite(course, sizeof(Course), 1, file);
                saved++;
            }
            current = current->next;
        }
    } else {
        // Circular list
        if (current) {
            do {
                Course* course = (Course*)current->data;
                if (course) {
                    fwrite(course, sizeof(Course), 1, file);
                    saved++;
                }
                current = current->next;
            } while (current != catalog->courseList->head);
        }
    }
    
    fclose(file);
    return (saved == count) ? 0 : -1;
}

/**
 * @brief Loads a course catalog from a file
 * @param catalog Pointer to the catalog to load into
 * @param filename Name of the file to load from
 * @return 0 on success, -1 on failure
 */
int loadCatalog(CourseCatalog* catalog, const char* filename) {
    if (!catalog || !filename) {
        return -1;
    }
    
    // Construct the full path
    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "data/%s", filename);
    
    // Open the file for reading
    FILE* file = fopen(fullPath, "rb");
    if (!file) {
        return -1;
    }
    
    // Clean up existing catalog if any
    destroyCourseCatalog(catalog);
    
    // Create a new course list
    if (createList(&catalog->courseList, SINGLY) != 0) {
        fclose(file);
        return -1;
    }
    
    // Read catalog name
    if (fread(catalog->catalogName, sizeof(catalog->catalogName), 1, file) != 1) {
        fclose(file);
        destroyCourseCatalog(catalog);
        return -1;
    }
    
    // Read number of courses
    int count;
    if (fread(&count, sizeof(int), 1, file) != 1) {
        fclose(file);
        destroyCourseCatalog(catalog);
        return -1;
    }
    
    // Read each course
    for (int i = 0; i < count; i++) {
        Course* course = (Course*)malloc(sizeof(Course));
        if (!course) {
            fclose(file);
            destroyCourseCatalog(catalog);
            return -1;
        }
        
        if (fread(course, sizeof(Course), 1, file) != 1) {
            free(course);
            fclose(file);
            destroyCourseCatalog(catalog);
            return -1;
        }
        
        if (addNode(&catalog->courseList, course) != 0) {
            free(course);
            fclose(file);
            destroyCourseCatalog(catalog);
            return -1;
        }
    }
    
    catalog->isModified = false;
    fclose(file);
    return 0;
}

/**
 * @brief Displays all courses in the catalog
 * @param catalog Pointer to the catalog
 */
void displayAllCourses(const CourseCatalog* catalog) {
    if (!catalog || !catalog->courseList || !catalog->courseList->head) {
        printf("No courses to display.\n");
        return;
    }
    
    printf("\n=== %s Course Catalog ===\n", catalog->catalogName);
    printf("%-10s | %-30s | %-10s | %-5s | %-10s\n", 
           "Code", "Name", "Type", "Units", "Status");
    printf("----------------------------------------------------------------------\n");
    
    node* current = catalog->courseList->head;
    int count = 0;
    
    if (catalog->courseList->type == SINGLY || catalog->courseList->type == DOUBLY) {
        // Non-circular list
        while (current) {
            Course* course = (Course*)current->data;
            if (course) {
                printf("%-10s | %-30s | %-10s | %-5d | %-10s\n", 
                       course->code,
                       course->name,
                       getCourseTypeString(course->type),
                       course->units,
                       course->isActive ? "Active" : "Inactive");
                count++;
            }
            current = current->next;
        }
    } else {
        // Circular list
        if (current) {
            do {
                Course* course = (Course*)current->data;
                if (course) {
                    printf("%-10s | %-30s | %-10s | %-5d | %-10s\n", 
                           course->code,
                           course->name,
                           getCourseTypeString(course->type),
                           course->units,
                           course->isActive ? "Active" : "Inactive");
                    count++;
                }
                current = current->next;
            } while (current != catalog->courseList->head);
        }
    }
    
    printf("----------------------------------------------------------------------\n");
    printf("Total courses: %d\n", count);
}

/**
 * @brief Displays detailed information about a course
 * @param course Pointer to the course
 */
void displayCourseDetails(const Course* course) {
    if (!course) {
        printf("No course data to display.\n");
        return;
    }
    
    printf("\n=== Course Details ===\n");
    printf("Code: %s\n", course->code);
    printf("Name: %s\n", course->name);
    printf("Type: %s\n", getCourseTypeString(course->type));
    printf("Units: %d\n", course->units);
    printf("Status: %s\n", course->isActive ? "Active" : "Inactive");
    printf("\nDescription:\n%s\n", course->description);
    printf("=====================\n");
}

/**
 * @brief Memory deallocation function for course objects
 * @param courseData Pointer to the course data to free
 */
void freeCourse(void* courseData) {
    if (courseData) {
        free(courseData);
    }
}

/**
 * @brief Gets a string representation of a course type
 * @param type The course type
 * @return String representation of the course type
 */
const char* getCourseTypeString(CourseType type) {
    switch (type) {
        case CORE:
            return "Core";
        case MAJOR:
            return "Major";
        case ELECTIVE:
            return "Elective";
        case GENERAL_ED:
            return "General Ed";
        default:
            return "Unknown";
    }
} 