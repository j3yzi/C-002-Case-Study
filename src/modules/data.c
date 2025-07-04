/**
 * @file data.c
 * @brief Data Management and File I/O Operations
 * 
 * This file implements comprehensive data management functionality for the
 * PUP Information Management System. It provides functions for:
 * - File I/O operations for employee and student data
 * - Report generation (payroll and academic reports)
 * - Data persistence and loading
 * - Directory management for data storage
 * - Sorting and data manipulation operations
 * 
 * The module handles both binary data files for efficient storage and
 * text reports for human-readable output. All file operations include
 * proper error handling and data validation.
 * 
 * @author C002 - Group 1
 * @version 1.0
 * @date 2024
 */

// Standard C library includes
#include <stdio.h>      // For file operations and I/O
#include <stdlib.h>     // For memory allocation and general utilities
#include <string.h>     // For string manipulation functions
#include <time.h>       // For timestamp generation
#ifdef _WIN32
#include <windows.h>    // For Windows-specific file operations
#endif

// Application-specific includes
#include "data.h"                               // Data module header
#include "../../include/models/employee.h"     // Employee data structures
#include "../../include/models/student.h"      // Student data structures
#include "../../include/headers/list.h"        // Generic linked list implementation
#include "../../include/headers/apctxt.h"      // Application context and utilities

/**
 * @name Directory Management Functions
 * @brief Internal functions for managing data and output directories
 * @{
 */

/**
 * @brief Creates the data directory if it doesn't exist
 * 
 * Ensures that the "data" directory exists for storing data files.
 * This directory is used for persistent storage of employee and student records.
 * 
 * @return 0 on success, non-zero on error
 */
static int createDataDirectory(void) {
    return appCreateDirectory("data");
}

/**
 * @brief Creates the output directory if it doesn't exist
 * 
 * Ensures that the "output" directory exists for storing generated reports.
 * This directory is used for payroll reports, academic reports, and other
 * generated documents.
 * 
 * @return 0 on success, non-zero on error
 */
static int createOutputDirectory(void) {
    return appCreateDirectory("output");
}

/** @} */ // End of Directory Management Functions

// Function to list all .dat files in the data directory
int listDataFiles(void) {
    createDataDirectory();
    
    printf("=== Available Data Files ===\n");
    
    int result = appListFiles("data", "*.dat");
    if (result != 1) {
        printf("No .dat files found in the data directory.\n");
        return 0;
    }
    
    printf("=============================\n");
    return 1;
}

// Function to get current timestamp for filenames
void getCurrentTimestamp(char* buffer, const int bufferSize) {
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer, bufferSize, "%Y-%m-%d_%H-%M-%S", timeinfo);
}

// Function to generate payroll report file
int generatePayrollReportFile(const list* employeeList, char* generatedFilePath, const int pathBufferSize) {
    if (!employeeList || !employeeList->head || employeeList->size == 0) {
        return -1;
    }
    
    createOutputDirectory();
    
    char timestamp[32];
    getCurrentTimestamp(timestamp, sizeof(timestamp));
    
    char filename[256];
    snprintf(filename, sizeof(filename), "output/payroll_report-%s.txt", timestamp);
    
    snprintf(generatedFilePath, pathBufferSize, "%s", filename);
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        return -1;
    }
    
    /* -------------------- Header -------------------- */
    int reportWidth = 98;

    const char* univName = "POLYTECHNIC UNIVERSITY OF THE PHILIPPINES";
    int margin = (reportWidth - (int)strlen(univName)) / 2;
    if (margin < 0) margin = 0;
    fprintf(file, "%*s%s\n", margin, "", univName);

    const char* cityName = "Quezon City";
    margin = (reportWidth - (int)strlen(cityName)) / 2;
    if (margin < 0) margin = 0;
    fprintf(file, "%*s%s\n", margin, "", cityName);

    fprintf(file, "\n");

    const char* payrollTitle = "Payroll";
    margin = (reportWidth - (int)strlen(payrollTitle)) / 2;
    if (margin < 0) margin = 0;
    fprintf(file, "%*s%s\n\n", margin, "", payrollTitle);

    fprintf(file, "Generated on: %s\n\n", timestamp);

    /* ------------------ Table Header ---------------- */
    fprintf(file, "%-12s  %-20s  %-8s  %-12s  %-12s  %-12s  %-12s\n",
            "Employee", "Employee", "Status", "Basic", "Overtime", "Deductions", "Net");
    fprintf(file, "%-12s  %-20s  %-8s  %-12s  %-12s  %-12s  %-12s\n",
            "Number", "Name", "", "Salary", "Pay", "", "Pay");

    for (int i = 0; i < reportWidth; ++i) fputc('-', file);
    fputc('\n', file);

    /* ------------------- Table Rows ----------------- */
    node* current = employeeList->head;
    int count = 0;
    double totalBasicPay = 0.0;
    double totalOvertimePay = 0.0;
    double totalDeductions = 0.0;
    double totalNetPay = 0.0;

    if (current) {
        do {
            Employee* emp = (Employee*)current->data;
            if (emp) {
                fprintf(file, "%-12s  %-20s  %-8s  %12.2f  %12.2f  %12.2f  %12.2f\n",
                        emp->personal.employeeNumber,
                        emp->personal.name.fullName,
                        (emp->employment.status == statusRegular) ? "Regular" : "Casual",
                        emp->payroll.basicPay,
                        emp->payroll.overtimePay,
                        emp->payroll.deductions,
                        emp->payroll.netPay);

                totalBasicPay += emp->payroll.basicPay;
                totalOvertimePay += emp->payroll.overtimePay;
                totalDeductions += emp->payroll.deductions;
                totalNetPay += emp->payroll.netPay;
                count++;
            }
            current = current->next;
        } while (current && current != employeeList->head);
    }

    /* -------------------- Footer -------------------- */
    for (int i = 0; i < reportWidth; ++i) fputc('-', file);
    fputc('\n', file);

    fprintf(file, "%-41s  %12.2f  %12.2f  %12.2f  %12.2f\n",
            "TOTALS:", totalBasicPay, totalOvertimePay, totalDeductions, totalNetPay);

    for (int i = 0; i < reportWidth; ++i) fputc('-', file);
    fputc('\n', file);

    fprintf(file, "Total employees displayed: %d\n\n", count);
    fprintf(file, "Report generated by PUP Information Management System\n");

    fclose(file);
    return count;
}

// Function to generate student report file
int generateStudentReportFile(const list* studentList, char* generatedFilePath, const int pathBufferSize) {
    if (!studentList || !studentList->head || studentList->size == 0) {
        return -1;
    }
    
    createOutputDirectory();
    
    char timestamp[32];
    getCurrentTimestamp(timestamp, sizeof(timestamp));
    
    char filename[256];
    snprintf(filename, sizeof(filename), "output/student_report-%s.txt", timestamp);
    
    snprintf(generatedFilePath, pathBufferSize, "%s", filename);
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        return -1;
    }
    
    /* -------------------- Header -------------------- */
    int reportWidth = 105;

    const char* univName = "POLYTECHNIC UNIVERSITY OF THE PHILIPPINES - QUEZON CITY";
    int margin = (reportWidth - (int)strlen(univName)) / 2;
    if (margin < 0) margin = 0;
    fprintf(file, "%*s%s\n", margin, "", univName);

    const char* academicYear = "Academic Records - AY 2024-2025";
    margin = (reportWidth - (int)strlen(academicYear)) / 2;
    if (margin < 0) margin = 0;
    fprintf(file, "%*s%s\n\n", margin, "", academicYear);

    fprintf(file, "Generated on: %s\n\n", timestamp);

    // Determine dynamic column widths similar to displayAllStudents
    int consoleWidth = reportWidth; // file width fixed
    int fixedWidth = 12 + 2 + 8 + 2 + 4 + 2 + 11 + 2 + 8 + 2 + 12; // 65
    int fullNameWidth = 25;
    if (consoleWidth - 4 > fixedWidth + 10) {
        fullNameWidth = consoleWidth - 4 - fixedWidth;
        if (fullNameWidth > 40) fullNameWidth = 40;
    }

    /* ------------------ Table Header ---------------- */
    fprintf(file, "%-12s  %-*s  %-8s  %-4s  %-11s  %-8s  %-12s\n",
            "Student No.", fullNameWidth, "Full Name", "Course", "Year", "Final Grade", "Remarks", "Standing");

    for (int i = 0; i < reportWidth; ++i) fputc('-', file);
    fputc('\n', file);

    /* ------------------- Table Rows ----------------- */
    node* current = studentList->head;
    int count = 0;
    double totalFinalGrade = 0.0;
    int passedCount = 0;
    int failedCount = 0;

    if (current) {
        do {
            Student* stu = (Student*)current->data;
            if (stu) {
                calculateFinalGrade(stu);
                const char* standingStr = (stu->standing == acadDeansLister) ? "Dean's Lister" : (stu->standing == acadRegular) ? "Regular" : "Probation";
                fprintf(file, "%-12s  %-*.*s  %-8s  %-4d  %11.2f  %-8s  %-12s\n",
                        stu->personal.studentNumber,
                        fullNameWidth, fullNameWidth, stu->personal.name.fullName,
                        stu->personal.programCode,
                        stu->personal.yearLevel,
                        stu->academic.finalGrade,
                        stu->academic.remarks,
                        standingStr);

                totalFinalGrade += stu->academic.finalGrade;
                if (strcmp(stu->academic.remarks, "Passed") == 0) {
                    passedCount++;
                } else {
                    failedCount++;
                }
                count++;
            }
            current = current->next;
        } while (current && current != studentList->head);
    }

    /* -------------------- Footer -------------------- */
    for (int i = 0; i < reportWidth; ++i) fputc('-', file);
    fputc('\n', file);

    if (count > 0) {
        double averageGrade = totalFinalGrade / count;
        fprintf(file, "%-50s  %11.2f\n", "AVERAGE GRADE:", averageGrade);

        for (int i = 0; i < reportWidth; ++i) fputc('-', file);
        fputc('\n', file);
    }

    fprintf(file, "Total students: %d | Passed: %d | Failed: %d\n\n", count, passedCount, failedCount);
    fprintf(file, "Report generated by PUP Information Management System\n");

    fclose(file);
    return count;
}

// Complete implementations for file I/O functions
int saveEmployeeDataFromFile(list* employeeList, const char *filename) {
    if (!employeeList || !filename) {
        return -1;
    }
    
    createDataDirectory();
    
    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "data/%s", filename);
    
    FILE *file = fopen(fullPath, "wb");
    if (!file) {
        return -1;
    }
    
    // Write number of employees
    int count = employeeList->size;
    fwrite(&count, sizeof(int), 1, file);
    
    // Write each employee record
    node* current = employeeList->head;
    int saved = 0;
    
    if (current != NULL) {
        do {
            Employee* emp = (Employee*)current->data;
            if (emp != NULL) {
                fwrite(emp, sizeof(Employee), 1, file);
                saved++;
            }
            current = current->next;
        } while (current != employeeList->head && current != NULL);
    }
    
    fclose(file);
    return saved;
}

list* loadEmployeeDataFromFile(const char* filename, ListType listType) {
    if (!filename) {
        return NULL;
    }
    
    // Note: listType parameter reserved for future list type handling
    (void)listType; // Suppress unused parameter warning
    
    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "data/%s", filename);
    
    FILE *file = fopen(fullPath, "rb");
    if (!file) {
        return NULL;
    }
    
    // Read number of employees
    int count;
    if (fread(&count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return NULL;
    }
    
    // Create new list
    list* employeeList = NULL;
    if (createEmployeeList(&employeeList) != 0) {
        fclose(file);
        return NULL;
    }
    
    // Read each employee record
    for (int i = 0; i < count; i++) {
        Employee* emp = (Employee*)malloc(sizeof(Employee));
        if (!emp) {
            fclose(file);
            destroyList(&employeeList, freeEmployee);
            return NULL;
        }
        
        if (fread(emp, sizeof(Employee), 1, file) != 1) {
            free(emp);
            fclose(file);
            destroyList(&employeeList, freeEmployee);
            return NULL;
        }
        
        if (createEmployee(emp, &employeeList) != 0) {
            free(emp);
            fclose(file);
            destroyList(&employeeList, freeEmployee);
            return NULL;
        }
    }
    
    fclose(file);
    return employeeList;
}

int saveStudentDataFromFile(list* studentList, const char *filename) {
    if (!studentList || !filename) {
        return -1;
    }
    
    createDataDirectory();
    
    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "data/%s", filename);
    
    FILE *file = fopen(fullPath, "wb");
    if (!file) {
        return -1;
    }
    
    // Write number of students
    int count = studentList->size;
    fwrite(&count, sizeof(int), 1, file);
    
    // Write each student record
    node* current = studentList->head;
    int saved = 0;
    
    if (current != NULL) {
        do {
            Student* stu = (Student*)current->data;
            if (stu != NULL) {
                fwrite(stu, sizeof(Student), 1, file);
                saved++;
            }
            current = current->next;
        } while (current != studentList->head && current != NULL);
    }
    
    fclose(file);
    return saved;
}

list* loadStudentDataFromFile(const char* filename, ListType listType) {
    if (!filename) {
        return NULL;
    }
    
    // Note: listType parameter reserved for future list type handling
    (void)listType; // Suppress unused parameter warning
    
    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "data/%s", filename);
    
    FILE *file = fopen(fullPath, "rb");
    if (!file) {
        return NULL;
    }
    
    // Read number of students
    int count;
    if (fread(&count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return NULL;
    }
    
    // Create new list
    list* studentList = NULL;
    if (createStudentList(&studentList) != 0) {
        fclose(file);
        return NULL;
    }
    
    // Read each student record
    for (int i = 0; i < count; i++) {
        Student* stu = (Student*)malloc(sizeof(Student));
        if (!stu) {
            fclose(file);
            destroyList(&studentList, freeStudent);
            return NULL;
        }
        
        if (fread(stu, sizeof(Student), 1, file) != 1) {
            free(stu);
            fclose(file);
            destroyList(&studentList, freeStudent);
            return NULL;
        }
        
        if (createStudent(stu, &studentList) != 0) {
            free(stu);
            fclose(file);
            destroyList(&studentList, freeStudent);
            return NULL;
        }
    }
    
    fclose(file);
    return studentList;
}

// Function to find the middle node of a linked list using fast/slow pointer approach
static node* findMiddleNode(node* head, node* tail) {
    if (!head || head == tail) {
        return head;
    }
    
    node* slow = head;
    node* fast = head->next;
    
    // Fast pointer moves twice as fast as slow pointer
    while (fast != tail && fast->next != tail) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}

// Function to merge two sorted linked lists
static node* mergeSortedLists(node* left, node* right, node* tail, int descending, 
                             int (*compareFunc)(void*, void*, int)) {
    node dummy;
    node* current = &dummy;
    
    // Merge the two lists
    while (left != NULL && right != NULL && left != tail && right != tail) {
        if (compareFunc(left->data, right->data, descending)) {
            current->next = left;
            left = left->next;
        } else {
            current->next = right;
            right = right->next;
        }
        current = current->next;
    }
    
    // Attach the remaining nodes
    if (left != NULL && left != tail) {
        current->next = left;
    } else if (right != NULL && right != tail) {
        current->next = right;
    }
    
    return dummy.next;
}

// Compare function for student grades
static int compareStudentGrades(void* data1, void* data2, int descending) {
    Student* stu1 = (Student*)data1;
    Student* stu2 = (Student*)data2;
    
    if (descending) {
        return stu1->academic.finalGrade >= stu2->academic.finalGrade;
    } else {
        return stu1->academic.finalGrade <= stu2->academic.finalGrade;
    }
}

// Recursive merge sort implementation for linked list
static node* mergeSortList(node* head, node* tail, int descending, 
                          int (*compareFunc)(void*, void*, int)) {
    // Base case: empty list or single node
    if (!head || head == tail || head->next == tail) {
        return head;
    }
    
    // Find the middle node
    node* middle = findMiddleNode(head, tail);
    node* right = middle->next;
    
    // Split the list into two halves
    middle->next = NULL;
    
    // Recursively sort both halves
    node* leftSorted = mergeSortList(head, NULL, descending, compareFunc);
    node* rightSorted = mergeSortList(right, tail, descending, compareFunc);
    
    // Merge the sorted halves
    return mergeSortedLists(leftSorted, rightSorted, tail, descending, compareFunc);
}

int sortStudentsByGrade(list* studentList, const int descending) {
    if (!studentList || studentList->size <= 1) {
        return 0;
    }
    
    // Handle circular lists by temporarily breaking the circle
    node* oldTail = NULL;
    
    if (studentList->type == SINGLY_CIRCULAR || studentList->type == DOUBLY_CIRCULAR) {
        oldTail = studentList->tail;
        studentList->tail->next = NULL;  // Break the circle
    }
    
    // Apply merge sort
    studentList->head = mergeSortList(studentList->head, NULL, descending, compareStudentGrades);
    
    // Update the tail pointer
    node* current = studentList->head;
    while (current && current->next) {
        current = current->next;
    }
    studentList->tail = current;
    
    // Restore circular structure if needed
    if (oldTail) {
        studentList->tail->next = studentList->head;  // Restore the circle
    }
    
    return 1;
}

int listEmployeeDataFiles(void) {
    printf("=== Available Employee Data Files ===\n");
    int result = appListFiles("data", "*employee*.dat");
    if (result != 1) {
        printf("No employee .dat files found.\n");
        return 0;
    }
    return 1;
}

int getEmployeeDataFileNames(char fileNames[][256], int maxFiles) {
    if (!fileNames || maxFiles <= 0) {
        return 0;
    }
    
    createDataDirectory();
    
    // Construct the search path
    char searchPath[512];
    snprintf(searchPath, sizeof(searchPath), "data\\*employee*.dat");
    
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath, &findFileData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        return 0; // No files found
    }
    
    int fileCount = 0;
    
    do {
        // Skip directories and ensure we don't exceed maxFiles
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && fileCount < maxFiles) {
            strncpy(fileNames[fileCount], findFileData.cFileName, 255);
            fileNames[fileCount][255] = '\0'; // Ensure null termination
            fileCount++;
        }
    } while (FindNextFile(hFind, &findFileData) != 0 && fileCount < maxFiles);
    
    FindClose(hFind);
    return fileCount;
}

int listStudentDataFiles(void) {
    printf("=== Available Student Data Files ===\n");
    int result = appListFiles("data", "*student*.dat");
    if (result != 1) {
        printf("No student .dat files found.\n");
        return 0;
    }
    return 1;
}

int getStudentDataFileNames(char fileNames[][256], int maxFiles) {
    if (!fileNames || maxFiles <= 0) {
        return 0;
    }
    
    createDataDirectory();
    
    // Construct the search path
    char searchPath[512];
    snprintf(searchPath, sizeof(searchPath), "data\\*student*.dat");
    
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath, &findFileData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        return 0; // No files found
    }
    
    int fileCount = 0;
    
    do {
        // Skip directories and ensure we don't exceed maxFiles
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && fileCount < maxFiles) {
            strncpy(fileNames[fileCount], findFileData.cFileName, 255);
            fileNames[fileCount][255] = '\0'; // Ensure null termination
            fileCount++;
        }
    } while (FindNextFile(hFind, &findFileData) != 0 && fileCount < maxFiles);
    
    FindClose(hFind);
    return fileCount;
}

int saveListWithCustomName(list* dataList, const char* listName, const char* dataType) {
    if (!dataList || !listName || !dataType) {
        return -1;
    }
    
    // Generate simple filename without timestamp
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_%s.dat", dataType, listName);
    
    if (strcmp(dataType, "employee") == 0) {
        return saveEmployeeDataFromFile(dataList, filename);
    } else if (strcmp(dataType, "student") == 0) {
        return saveStudentDataFromFile(dataList, filename);
    }
    
    return -1;
}

list* loadListWithName(const char* filename, const char* dataType, ListType listType) {
    if (!filename || !dataType) {
        return NULL;
    }
    
    if (strcmp(dataType, "employee") == 0) {
        return loadEmployeeDataFromFile(filename, listType);
    } else if (strcmp(dataType, "student") == 0) {
        return loadStudentDataFromFile(filename, listType);
    }
    
    return NULL;
} 