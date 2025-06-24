#ifndef STUIO_H
#define STUIO_H

#include "../../../include/models/student.h"

/**
 * @brief Prompts the user for all new student data and validates it.
 * @param newStudent A pointer to an allocated Student struct to be filled.
 * @return Returns 0 on success, -1 on failure (e.g., invalid name).
 */
int getStudentDataFromUser(Student* newStudent);

/**
 * @brief Prompts the user to search for a student and displays results.
 * @param studentList Pointer to the student list.
 * @return Returns 0 on success, -1 on failure or cancel.
 */
int handleSearchStudent(const list* studentList);

/**
 * @brief Prompts the user for a student number to search.
 * @param buffer Buffer to store the student number.
 * @param bufferSize Size of the buffer.
 * @return Returns 0 on success, -1 on cancel.
 */
int getStudentNumberFromUser(char* buffer, int bufferSize);

#endif 