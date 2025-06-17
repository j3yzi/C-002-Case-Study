# Payroll System Architecture Documentation

## Project Overview
This document provides a centralized reference for the modular architecture of the ABC Company Payroll System. The system handles employee data management, payroll calculations, and reporting.

## Global Structure

### Core Header File
**File**: `include/employee.h`

This header file contains all central declarations needed across modules, including:

1. **Constants**:
   - `EMPLOYEE_NUMBER_LEN` (11): Maximum length of employee number strings
   - `EMPLOYEE_NAME_LEN` (16): Maximum length of employee name strings
   - `MAX_EMPLOYEES` (5): Maximum number of employees in the system

2. **Data Types**:
   - `EmployeeStatusCode`: Enum for employee status (Regular, Casual)
   - `EmployeeNode`: Struct defining employee record with linked list capabilities

3. **Global Variables** (defined in main.c):
   - `empHead`: Pointer to the first employee in the linked list
   - `empCurr`: Pointer for current employee during operations
   - `empTail`: Pointer to the last employee in the linked list

4. **Function Prototypes**: Declarations for all functions across modules

## Module Organization

### 1. Main Application Entry
**File**: `main.c`
**Purpose**: Program initialization and main loop

**Key Functions**:
- `main()`: Entry point that initializes data and starts the menu loop

### 2. Frontend: Menu Interface
**File**: `src/frontend/menu_io.c`
**Purpose**: User interface for menu navigation

**Key Functions**:
- `printMenu()`: Displays the main menu options
- `runMenuLoop()`: Handles user input and menu navigation

### 3. Frontend: Employee Input/Output
**File**: `src/frontend/employee_io.c`
**Purpose**: User interface for employee data entry and display

**Key Functions**:
- `getEmployeeDataFromUser(EmployeeNode** newEmployee)`: Collects employee data via user input
- `displayReportHeader()`: Formats and displays the payroll report header
- `displayEmployeeRecord(EmployeeNode* employee)`: Formats and displays a single employee record

### 4. Backend: List Management
**File**: `src/backend/list_manager.c`
**Purpose**: Handles employee linked list operations

**Key Functions**:
- `createEmployeeNode()`: Creates a new employee node with default values
- `createEmployeeList()`: Creates and initializes a list with MAX_EMPLOYEES nodes
- `addEmployeeNode()`: Adds a single employee node to the list
- `editEmployeeNode()`: Updates an existing employee record (optional)
- `deleteEmployeeNode()`: Removes an employee record (optional)
- `freeEmployeeList()`: Frees memory for all employee nodes
- `getEmployeeCount()`: Counts employees in the list

### 5. Backend: File Handling
**File**: `src/backend/file_handler.c`
**Purpose**: Manages data persistence through binary file operations

**Key Functions**:
- `exportEmployeeDataToFile(EmployeeNode* head, const char *filename)`: Saves employee data to a binary file
- `loadEmployeeDataFromFile(const char* filename)`: Loads employee data from a binary file

### 6. Backend: Payroll Logic
**File**: `src/backend/payroll_logic.c`
**Purpose**: Handles all payroll calculations

**Key Functions**:
- `calculatePayroll(EmployeeNode* employee)`: Main orchestration function for all calculations
- `calculateBasicPay(EmployeeNode* employee)`: Calculates basic salary based on hours and rate
- `calculateOvertimePay(EmployeeNode* employee)`: Calculates overtime pay when applicable
- `calculateDeductions(EmployeeNode* employee)`: Calculates various deductions from pay
- `calculateNetPay(EmployeeNode* employee)`: Calculates final take-home pay

## Data Flow

1. **Data Input Flow**:
   - User navigates through `printMenu()` and selects an option
   - For new employees, `getEmployeeDataFromUser()` collects information
   - `createEmployeeNode()` or `addEmployeeNode()` creates and links the record

2. **Calculation Flow**:
   - `calculatePayroll()` orchestrates a series of calculations:
     - `calculateBasicPay()` → `calculateOvertimePay()` → `calculateDeductions()` → `calculateNetPay()`

3. **Reporting Flow**:
   - `displayReportHeader()` prints the header information
   - A loop iterates through employees calling `displayEmployeeRecord()` for each

4. **Persistence Flow**:
   - `exportEmployeeDataToFile()` serializes employee list to disk
   - `loadEmployeeDataFromFile()` deserializes from disk to memory

## Developer Assignments

When working on a specific module, refer to this guide for understanding how your component interacts with others:

### For Frontend Developers
Focus on user experience in `employee_io.c` and `menu_io.c`:
- Format output to be user-friendly
- Ensure proper input validation
- Consider error messages and prompts

### For Backend Developers
Focus on the core business logic in `list_manager.c` and `payroll_logic.c`:
- Ensure memory management is proper (allocation/deallocation)
- Implement correct payroll calculation rules
- Handle edge cases (e.g., empty lists, invalid data)

### For Data Management Developers
Focus on file operations in `file_handler.c`:
- Ensure file reading/writing is efficient
- Handle file errors gracefully
- Consider data integrity and validation

## EmployeeNode Structure Reference

```c
typedef struct EmployeeNode {
    char employeeNumber[EMPLOYEE_NUMBER_LEN]; // 10 characters 
    char employeeName[EMPLOYEE_NAME_LEN];     // 15 characters 
    
    EmployeeStatusCode statusCode;            // R or C 
    
    int hoursWorked;                          // max 99 
    
    float basicPay;                           // max 999999.99 
    float basicRate;                          // max 999.99 
    float deductions;                         // max 99999.99 
    float overtimePay;                        // Computed
    float netPay;                             // Computed
    
    struct EmployeeNode *next;
} EmployeeNode;
```

## Compilation
The project is compiled using the provided `compile.bat` script which handles all dependencies and produces the final executable.

## Testing
Each module has corresponding test files in the `src/tests/` directory:
- `frontend_test.c`: Tests for UI components
- `backend_test.c`: Tests for business logic components

---

*Document last updated: June 17, 2025*
