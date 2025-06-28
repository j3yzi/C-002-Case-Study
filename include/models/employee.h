/**
 * @file employee.h
 * @brief Employee Data Model and Management Functions
 * 
 * This header file defines the data structures and functions for managing
 * employee records in the PUP Information Management System. It includes
 * structures for personal information, employment details, payroll calculations,
 * and various operations for creating, searching, updating, and managing
 * employee data.
 * 
 * @author C002 - Group 1
 * @version 1.0
 * @date 2024
 */

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "../headers/list.h"    // For linked list data structure
#include "../headers/apctxt.h"  // For application context and configuration

// Forward declaration to avoid circular dependency
typedef struct AppContext AppContext;

/**
 * @name Employee Data Structure Size Constants
 * @brief Fixed-size definitions for struct compatibility and memory management
 * 
 * These constants define the maximum sizes for various employee data fields.
 * Using fixed sizes ensures consistent memory layout and compatibility
 * across different system architectures.
 * @{
 */
#define employeeNumberLen 11        // Employee ID: 10 characters + null terminator
#define employeeNameLen 16          // Full name display: 15 characters + null terminator  
#define employeeFirstNameLen 32     // First name: 31 characters + null terminator
#define employeeMiddleNameLen 32    // Middle name: 31 characters + null terminator
#define employeeLastNameLen 32      // Last name: 31 characters + null terminator
#define maxEmployeeCreationCount 5  // Maximum employees that can be created in one batch
#define maxEmployeeRecords 50       // Maximum total employee records in system

/** @} */ // End of Employee Data Structure Size Constants

/**
 * @name File Configuration Constants
 * @brief Constants for file operations and data persistence
 * @{
 */
#define payrollFileName "payroll.dat"  // Default filename for payroll data storage

/** @} */ // End of File Configuration Constants

/**
 * @note Configurable Business Values
 * 
 * The following values are loaded from config.ini at runtime:
 * - Regular work hours (typically 40 hours/week)
 * - Overtime rate multiplier (typically 1.5 for time-and-a-half)
 * 
 * Use getRegularHours() and getOvertimeRate() functions instead of hard-coded values.
 */

/**
 * @struct EmployeeName
 * @brief Structure for storing employee name components
 * 
 * Separates the employee's name into individual components for better
 * data organization and search capabilities. The fullName field is
 * typically a concatenation or formatted version of the individual names.
 */
typedef struct {
    char firstName[employeeFirstNameLen];   // Employee's first/given name
    char middleName[employeeMiddleNameLen]; // Employee's middle name or initial
    char lastName[employeeLastNameLen];     // Employee's last/family name
    char fullName[employeeNameLen];         // Formatted full name for display
} EmployeeName;

/**
 * @struct PersonalInfo
 * @brief Structure for employee personal identification information
 * 
 * Contains the essential identifying information for an employee,
 * including their unique employee number and complete name details.
 */
typedef struct {
    char employeeNumber[employeeNumberLen]; // Unique employee identifier
    EmployeeName name;                      // Complete name information
} PersonalInfo;

/**
 * @enum EmployeeStatus
 * @brief Employment status classification
 * 
 * Defines the different employment statuses available in the system.
 * This affects payroll calculations and benefits eligibility.
 */
typedef enum {
    statusRegular,  // Regular/permanent employee (full benefits, job security)
    statusCasual    // Casual/temporary employee (limited benefits)
} EmployeeStatus;

/**
 * @struct EmploymentInfo
 * @brief Structure for employment-related information
 * 
 * Contains information about the employee's work status, hours worked,
 * and basic pay rate. This information is used for payroll calculations.
 */
typedef struct {
    EmployeeStatus status;  // Employment classification (regular/casual)
    int hoursWorked;        // Total hours worked in current pay period
    float basicRate;        // Basic hourly or daily pay rate
} EmploymentInfo;

/**
 * @struct PayrollInfo
 * @brief Structure for payroll calculation results
 * 
 * Contains the calculated payroll amounts including basic pay, overtime,
 * deductions, and final net pay. These values are typically computed
 * based on the employment information and configured rates.
 */
typedef struct {
    float basicPay;     // Base pay for regular hours
    float overtimePay;  // Additional pay for overtime hours
    float deductions;   // Total deductions (taxes, insurance, etc.)
    float netPay;       // Final take-home pay (basic + overtime - deductions)
} PayrollInfo;

/**
 * @struct Employee
 * @brief Complete employee record structure
 * 
 * The main employee data structure that combines all employee information
 * including personal details, employment status, and payroll data.
 * This is the primary data type used throughout the employee management system.
 */
typedef struct {
    PersonalInfo personal;      // Personal identification information
    EmploymentInfo employment;  // Employment status and work details
    PayrollInfo payroll;        // Calculated payroll information
} Employee;

/**
 * @name Employee Name Management Functions
 * @brief Functions for handling employee name operations
 * @{
 */

/**
 * @brief Composes and formats an employee's full name
 * 
 * Takes the individual name components (first, middle, last) and creates
 * a properly formatted full name string for display purposes.
 * 
 * @param name Pointer to EmployeeName structure to process
 * @return 0 on success, non-zero on error
 */
int composeEmployeeName(EmployeeName* name);

/** @} */ // End of Employee Name Management Functions

/**
 * @name File I/O Operations
 * @brief Functions for saving and loading employee data from files
 * @{
 */

/**
 * @brief Saves employee list data to a file
 * 
 * Serializes the entire employee list to a binary file for persistent storage.
 * The file format includes a header with the record count followed by
 * individual employee records.
 * 
 * @param employeeList Pointer to the employee list to save
 * @param filename Name of the file to save to
 * @return Number of records saved, or negative on error
 */
int saveEmployeeDataToFile(const list* employeeList, const char *filename);

/**
 * @brief Loads employee data from a file
 * 
 * Reads employee records from a binary file and reconstructs the employee list.
 * The function creates a new list and populates it with the loaded data.
 * 
 * @param filename Name of the file to load from
 * @param listType Type of linked list to create (singly, doubly, etc.)
 * @return Pointer to newly created employee list, or NULL on error
 */
list* loadEmployeeDataFromFile(const char* filename, ListType listType);

/** @} */ // End of File I/O Operations

/**
 * @name Employee Creation and Management
 * @brief Functions for creating and managing employee records
 * @{
 */

/**
 * @brief Creates a new employee record and adds it to the list
 * 
 * Creates a new employee with the provided data and adds it to the
 * specified employee list. Performs validation and ensures data integrity.
 * 
 * @param newEmployeeData Pointer to employee data to add
 * @param l Double pointer to the employee list
 * @return 0 on success, non-zero on error
 */
int createEmployee(Employee* newEmployeeData, list** l);

/**
 * @brief Safely deletes an employee record
 * 
 * Frees all memory associated with an employee record and sets the
 * pointer to NULL to prevent dangling pointer issues.
 * 
 * @param employee Double pointer to the employee to delete
 */
void deleteEmployee(Employee** employee);

/**
 * @brief Creates a new empty employee list
 * 
 * Initializes a new linked list specifically configured for storing
 * employee records. Sets up the appropriate list type and structure.
 * 
 * @param employeeList Double pointer to store the new list
 * @return 0 on success, non-zero on error
 */
int createEmployeeList(list** employeeList);

/**
 * @brief Memory cleanup function for employee data
 * 
 * Callback function used by the list management system to properly
 * free memory associated with employee records when removing them
 * from lists or destroying lists.
 * 
 * @param employeeData Void pointer to employee data to free
 */
void freeEmployee(void* employeeData);

/** @} */ // End of Employee Creation and Management

/**
 * @name Search Operations
 * @brief Functions for finding employees in the system
 * @{
 */

/**
 * @brief Searches for an employee by employee number
 * 
 * Performs a linear search through the employee list to find an employee
 * with the specified employee number (unique identifier).
 * 
 * @param employeeList Pointer to the employee list to search
 * @param employeeNumber Employee number to search for
 * @return Pointer to found employee, or NULL if not found
 */
Employee* searchEmployeeByNumber(const list* employeeList, const char* employeeNumber);

/**
 * @brief Searches for an employee by last name
 * 
 * Performs a linear search through the employee list to find an employee
 * with the specified last name. Returns the first match found.
 * 
 * @param employeeList Pointer to the employee list to search
 * @param lastName Last name to search for
 * @return Pointer to found employee, or NULL if not found
 */
Employee* searchEmployeeByName(const list* employeeList, const char* lastName);

/**
 * @brief Finds the index position of an employee by number
 * 
 * Searches for an employee and returns their position (index) in the list
 * rather than a pointer to the employee data.
 * 
 * @param employeeList Pointer to the employee list to search
 * @param employeeNumber Employee number to search for
 * @return Index position (0-based), or -1 if not found
 */
int searchEmployeeIndexByNumber(const list* employeeList, const char* employeeNumber);

/** @} */ // End of Search Operations

/**
 * @name Edit Operations
 * @brief Functions for modifying existing employee records
 * @{
 */

/**
 * @brief Updates an existing employee record with new data
 * 
 * Replaces the data in an existing employee record with new information.
 * Performs validation to ensure data integrity and consistency.
 * 
 * @param employee Pointer to the employee record to update
 * @param newData Pointer to the new employee data
 * @return 0 on success, non-zero on error
 */
int updateEmployeeData(Employee* employee, const Employee* newData);

/**
 * @brief Edits an employee record within a list
 * 
 * Finds an employee by their employee number and updates their record
 * with new data. Combines search and update operations in one function.
 * 
 * @param employeeList Pointer to the employee list
 * @param employeeNumber Employee number to find and update
 * @param newData Pointer to the new employee data
 * @return 0 on success, non-zero on error
 */
int editEmployeeInList(list* employeeList, const char* employeeNumber, const Employee* newData);

/** @} */ // End of Edit Operations

/**
 * @name Remove Operations
 * @brief Functions for removing employees from the system
 * @{
 */

/**
 * @brief Removes an employee from the list by employee number
 * 
 * Searches for an employee by their employee number and removes them
 * from the list. Properly frees associated memory.
 * 
 * @param employeeList Pointer to the employee list
 * @param employeeNumber Employee number of the employee to remove
 * @return 0 on success, non-zero on error
 */
int removeEmployeeFromList(list* employeeList, const char* employeeNumber);

/** @} */ // End of Remove Operations

/**
 * @name Display Operations
 * @brief Functions for displaying employee information
 * @{
 */

/**
 * @brief Displays detailed information for a single employee
 * 
 * Prints a formatted display of all employee information including
 * personal details, employment status, and payroll information.
 * 
 * @param employee Pointer to the employee to display
 */
void displayEmployeeDetails(const Employee* employee);

/**
 * @brief Displays a summary of all employees in the list
 * 
 * Prints a formatted table showing key information for all employees
 * in the list. Typically includes employee number, name, status, and pay.
 * 
 * @param employeeList Pointer to the employee list to display
 */
void displayAllEmployees(const list* employeeList);

/** @} */ // End of Display Operations

/**
 * @name Utility Functions
 * @brief Helper functions for employee data management
 * @{
 */

/**
 * @brief Comparison function for sorting employees by number
 * 
 * Callback function used by sorting algorithms to compare two employee
 * records based on their employee numbers. Used for maintaining sorted lists.
 * 
 * @param emp1 Pointer to first employee for comparison
 * @param emp2 Pointer to second employee for comparison
 * @return Negative if emp1 < emp2, 0 if equal, positive if emp1 > emp2
 */
int compareEmployeeByNumber(const void* emp1, const void* emp2);

/**
 * @brief Comparison function for sorting employees by name
 * 
 * Callback function used by sorting algorithms to compare two employee
 * records based on their last names. Used for alphabetical sorting.
 * 
 * @param emp1 Pointer to first employee for comparison
 * @param emp2 Pointer to second employee for comparison
 * @return Negative if emp1 < emp2, 0 if equal, positive if emp1 > emp2
 */
int compareEmployeeByName(const void* emp1, const void* emp2);

/** @} */ // End of Utility Functions

#endif // EMPLOYEE_H