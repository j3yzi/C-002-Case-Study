/**
 * @file data.h
 * @brief Data Management Module Header
 * 
 * This header file declares functions for comprehensive data management
 * in the PUP Information Management System. It provides interfaces for:
 * - File I/O operations for employee and student records
 * - Report generation for payroll and academic data
 * - Data persistence and loading from binary files
 * - Multi-list management capabilities
 * - Data sorting and manipulation functions
 * 
 * The module supports both binary data storage for efficient persistence
 * and text report generation for human-readable output.
 * 
 * @author C002 - Group 1
 * @version 1.0
 * @date 2024
 */

#ifndef DATA_H
#define DATA_H

// Standard C library includes
#include <stdio.h>      // For file operations and I/O
#include <stdlib.h>     // For memory allocation and general utilities

// Application-specific includes
#include "../../include/headers/list.h"        // Generic linked list implementation
#include "../../include/models/employee.h"    // Employee data structures
#include "../../include/models/student.h"     // Student data structures

/**
 * @name Common Data Management Functions
 * @brief General-purpose data management utilities
 * @{
 */

/**
 * @brief Lists all .dat files in the data directory
 * 
 * Scans the data directory and displays all available data files
 * with .dat extension. Useful for showing users what data files
 * are available for loading.
 * 
 * @return 1 if files found and listed, 0 if no files found, negative on error
 */
int listDataFiles(void);

/**
 * @brief Gets current timestamp formatted for filenames
 * 
 * Generates a timestamp string in the format "YYYY-MM-DD_HH-MM-SS"
 * suitable for use in automatically generated filenames.
 * 
 * @param buffer Buffer to store the timestamp string
 * @param bufferSize Size of the buffer (should be at least 32 characters)
 */
void getCurrentTimestamp(char* buffer, const int bufferSize);

/** @} */ // End of Common Data Management Functions

/**
 * @name Employee Data Management Functions
 * @brief Functions specific to employee data operations
 * @{
 */

/**
 * @brief Generates a formatted payroll report file
 * 
 * Creates a comprehensive payroll report in text format showing
 * employee details, pay calculations, and totals. The report is
 * saved with a timestamp-based filename.
 * 
 * @param employeeList Pointer to the employee list to generate report from
 * @param generatedFilePath Buffer to store the generated file path
 * @param pathBufferSize Size of the file path buffer
 * @return Number of employees included in report, or negative on error
 */
int generatePayrollReportFile(const list* employeeList, char* generatedFilePath, const int pathBufferSize);

/**
 * @brief Saves employee list data to a binary file
 * 
 * Serializes the entire employee list to a binary file for efficient
 * storage and later retrieval. The file format includes metadata
 * and individual employee records.
 * 
 * @param employeeList Pointer to the employee list to save
 * @param filename Name of the file to save to (will be placed in data directory)
 * @return Number of records saved, or negative on error
 */
int saveEmployeeDataFromFile(list* employeeList, const char* filename);

/**
 * @brief Loads employee data from a binary file
 * 
 * Reads employee records from a binary file and reconstructs the
 * employee list. Creates a new list with the specified type.
 * 
 * @param filename Name of the file to load from (in data directory)
 * @param listType Type of linked list to create for the loaded data
 * @return Pointer to newly created employee list, or NULL on error
 */
list* loadEmployeeDataFromFile(const char* filename, ListType listType);

/** @} */ // End of Employee Data Management Functions

/**
 * @name Student Data Management Functions
 * @brief Functions specific to student data operations
 * @{
 */

/**
 * @brief Generates a formatted academic report file
 * 
 * Creates a comprehensive academic report in text format showing
 * student details, grades, academic standing, and statistics.
 * The report is saved with a timestamp-based filename.
 * 
 * @param studentList Pointer to the student list to generate report from
 * @param generatedFilePath Buffer to store the generated file path
 * @param pathBufferSize Size of the file path buffer
 * @return Number of students included in report, or negative on error
 */
int generateStudentReportFile(const list* studentList, char* generatedFilePath, const int pathBufferSize);

/**
 * @brief Saves student list data to a binary file
 * 
 * Serializes the entire student list to a binary file for efficient
 * storage and later retrieval. The file format includes metadata
 * and individual student records.
 * 
 * @param studentList Pointer to the student list to save
 * @param filename Name of the file to save to (will be placed in data directory)
 * @return Number of records saved, or negative on error
 */
int saveStudentDataFromFile(list* studentList, const char* filename);

/**
 * @brief Loads student data from a binary file
 * 
 * Reads student records from a binary file and reconstructs the
 * student list. Creates a new list with the specified type.
 * 
 * @param filename Name of the file to load from (in data directory)
 * @param listType Type of linked list to create for the loaded data
 * @return Pointer to newly created student list, or NULL on error
 */
list* loadStudentDataFromFile(const char* filename, ListType listType);

/**
 * @brief Sorts students by final grade
 * 
 * Sorts the student list based on final grades using an efficient
 * sorting algorithm. Can sort in ascending or descending order.
 * 
 * @param studentList Pointer to the student list to sort
 * @param descending 1 for descending order (highest grades first), 0 for ascending
 * @return 0 on success, non-zero on error
 */
int sortStudentsByGrade(list* studentList, const int descending);

/** @} */ // End of Student Data Management Functions

/**
 * @name Multi-List Management Functions
 * @brief Functions for managing multiple data lists and files
 * @{
 */

/**
 * @brief Lists available employee data files
 * 
 * Scans the data directory for employee-specific data files
 * and displays them to the user for selection.
 * 
 * @return Number of files found, or negative on error
 */
int listEmployeeDataFiles(void);

/**
 * @brief Lists available student data files
 * 
 * Scans the data directory for student-specific data files
 * and displays them to the user for selection.
 * 
 * @return Number of files found, or negative on error
 */
int listStudentDataFiles(void);

/**
 * @brief Gets employee data filenames into an array
 * 
 * Retrieves the names of available employee data files and
 * stores them in the provided array for programmatic access.
 * 
 * @param fileNames 2D array to store filenames
 * @param maxFiles Maximum number of files to retrieve
 * @return Number of filenames retrieved, or negative on error
 */
int getEmployeeDataFileNames(char fileNames[][256], int maxFiles);

/**
 * @brief Gets student data filenames into an array
 * 
 * Retrieves the names of available student data files and
 * stores them in the provided array for programmatic access.
 * 
 * @param fileNames 2D array to store filenames
 * @param maxFiles Maximum number of files to retrieve
 * @return Number of filenames retrieved, or negative on error
 */
int getStudentDataFileNames(char fileNames[][256], int maxFiles);

/**
 * @brief Saves a list with a custom name and data type
 * 
 * Generic function to save any type of data list with a custom
 * filename and data type identifier. Provides flexibility for
 * different data storage scenarios.
 * 
 * @param dataList Pointer to the data list to save
 * @param listName Custom name for the saved list
 * @param dataType Type identifier ("employee" or "student")
 * @return 0 on success, non-zero on error
 */
int saveListWithCustomName(list* dataList, const char* listName, const char* dataType);

/**
 * @brief Loads a list with a specific name and data type
 * 
 * Generic function to load any type of data list by name and
 * data type identifier. Provides flexibility for different
 * data loading scenarios.
 * 
 * @param filename Name of the file to load
 * @param dataType Type identifier ("employee" or "student")
 * @param listType Type of linked list to create
 * @return Pointer to loaded list, or NULL on error
 */
list* loadListWithName(const char* filename, const char* dataType, ListType listType);

/** @} */ // End of Multi-List Management Functions

#endif // DATA_H 