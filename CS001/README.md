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

### 2. Input Validation Library
**File**: `src/lib/validation.c`
**Purpose**: Robust input validation system for all user inputs

**Key Functions**:
- `appGetValidatedInput(appFormField* fields, int fieldCount)`: Primary function for collecting and validating user input
- `is_valid(char* input, IValidationType type, IValidationParams params)`: Validates input according to specified validation type
- `read_line(char* buffer, int size)`: Safely reads user input with buffer overflow protection

**Validation Types**:
- `IV_NONE`: No validation required
- `IV_CHOICES`: Validates input against a list of valid choices (e.g., employee status: R, C)
- `IV_RANGE_INT`: Validates input as an integer within specified range 
- `IV_RANGE_FLT`: Validates input as a float within specified range
- `IV_MAX_LEN`: Validates that input doesn't exceed specified length

### 3. Frontend: Menu Interface
**File**: `src/frontend/menu_io.c`
**Purpose**: User interface for menu navigation

**Key Functions**:
- `printMenu()`: Displays the main menu options
- `runMenuLoop()`: Handles user input and menu navigation

### 4. Frontend: Employee Input/Output
**File**: `src/frontend/employee_io.c`
**Purpose**: User interface for employee data entry and display

**Key Functions**:
- `getEmployeeDataFromUser(EmployeeNode** newEmployee)`: Collects employee data via user input
- `displayReportHeader()`: Formats and displays the payroll report header
- `displayEmployeeRecord(EmployeeNode* employee)`: Formats and displays a single employee record

### 5. Backend: List Management
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

### 6. Backend: File Handling
**File**: `src/backend/file_handler.c`
**Purpose**: Manages data persistence through binary file operations

**Key Functions**:
- `exportEmployeeDataToFile(EmployeeNode* head, const char *filename)`: Saves employee data to a binary file
- `loadEmployeeDataFromFile(const char* filename)`: Loads employee data from a binary file

### 7. Backend: Payroll Logic
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

## Library Usage Tutorials

This section provides practical guidance for developers on how to use the core libraries in this project.

### Using list.h Library

The `list.h` library provides a generic linked list implementation that serves as the backbone for data structures in the system.

#### Basic List Operations

```c
#include "include/list.h"

// Create a new list
list* myList = createList(LIST_TYPE_EMPLOYEE);

// Check if list is empty
bool isEmpty = isListEmpty(myList);

// Add a new node (typically an EmployeeNode in this project)
void* newData = createEmployeeNode(); // Create your data first
addNode(myList, newData);

// Traverse a list
node* current = myList->head;
while (current != NULL) {
    // Process node data
    EmployeeNode* employee = (EmployeeNode*)current->data;
    printf("Employee: %s\n", employee->employeeName);
    
    current = current->next;
}

// Clean up when done
destroyList(myList);
```

### Using apctxt.h Library

The `apctxt.h` library provides application context and input validation utilities that ensure data integrity.

#### Input Validation Example

```c
#include "include/apctxt.h"

// Example: Validating employee status input
const char* statusChoices[] = {"R", "C"};
char employeeStatus[5];

// Set up validation fields
appFormField fields[1] = {
    {
        .prompt = "Enter Employee Status (R for Regular, C for Casual): ",
        .buffer = employeeStatus,
        .bufferSize = sizeof(employeeStatus),
        .validationType = IV_CHOICES,
        .validationParams = {
            .choices = {
                .choices = statusChoices,
                .count = 2
            }
        }
    }
};

// Collect and validate input
appGetValidatedInput(fields, 1);

// Now employeeStatus contains a validated status code
```

#### Menu System Example

```c
#include "include/apctxt.h"

// Define menu options
MenuOption mainMenuOptions[] = {
    {'1', "Add New Employee"},
    {'2', "Display Payroll Report"},
    {'3', "Save Data to File"},
    {'4', "Load Data from File"},
    {'0', "Exit"}
};

// Display menu and get user choice
char choice = appDisplayMenu(mainMenuOptions, 5, "");

// Process choice
switch (choice) {
    case '1':
        // Handle Add New Employee
        break;
    case '2':
        // Handle Display Payroll Report
        break;
    // ... etc.
}
```

### Using employee.h Library

The `employee.h` library defines the core data structures for employee management.

#### Creating and Manipulating Employee Records

```c
#include "include/employee.h"

// Create a new employee
Employee newEmployee;

// Set personal info
strcpy(newEmployee.personal.employeeNumber, "EMP-001");
strcpy(newEmployee.personal.employeeName, "John Doe");

// Set employment info
newEmployee.employment.status = STATUS_REGULAR;
newEmployee.employment.hoursWorked = 160;
newEmployee.employment.basicRate = 175.50;

// Calculate payroll
calculateBasicPay(&newEmployee);
calculateOvertimePay(&newEmployee);
calculateDeductions(&newEmployee);
calculateNetPay(&newEmployee);

// Now newEmployee.payroll contains calculated values
```

### Synergy Between Libraries

These three libraries work together to form a comprehensive system:

#### Data Flow Between Libraries

1. **list.h + employee.h**: 
   ```c
   // Create an employee list
   list* employeeList = createList(LIST_TYPE_EMPLOYEE);
   
   // Create and populate an employee node
   EmployeeNode* newEmp = createEmployeeNode();
   strcpy(newEmp->employeeNumber, "EMP-001");
   strcpy(newEmp->employeeName, "John Doe");
   newEmp->statusCode = STATUS_REGULAR;
   newEmp->hoursWorked = 160;
   newEmp->basicRate = 175.50;
   
   // Add to list
   addNode(employeeList, newEmp);
   ```

2. **apctxt.h + employee.h**:
   ```c
   // Use apctxt.h to validate employee data
   const char* statusChoices[] = {"R", "C"};
   char statusInput[5];
   
   // Set up validation field
   appFormField statusField = {
       .prompt = "Enter Status (R/C): ",
       .buffer = statusInput,
       .bufferSize = sizeof(statusInput),
       .validationType = IV_CHOICES,
       .validationParams = {.choices = {.choices = statusChoices, .count = 2}}
   };
   
   // Get validated input
   appGetValidatedInput(&statusField, 1);
   
   // Update employee record with validated data
   EmployeeNode* employee = createEmployeeNode();
   if (strcmp(statusInput, "R") == 0) {
       employee->statusCode = STATUS_REGULAR;
   } else {
       employee->statusCode = STATUS_CASUAL;
   }
   ```

3. **All Three Together**:
   ```c
   // Create application context
   AppContext appContext;
   appContext.head = NULL;
   appContext.tail = NULL;
   appContext.count = 0;
   appContext.dataFile = "employee_data.bin";
   
   // Get validated employee data using apctxt.h
   EmployeeNode* newEmployee = createEmployeeNode();
   // ... validate and collect data ...
   
   // Add to list using list.h functions
   if (appContext.head == NULL) {
       appContext.head = newEmployee;
       appContext.tail = newEmployee;
   } else {
       appContext.tail->next = newEmployee;
       appContext.tail = newEmployee;
   }
   appContext.count++;
   
   // Process payroll
   calculatePayroll(newEmployee);
   
   // Save to file
   exportEmployeeDataToFile(appContext.head, appContext.dataFile);
   ```

## Testing
The project includes a comprehensive testing framework located in the `src/tests/` directory. Each module has corresponding test files and compilation scripts to ensure code quality and functionality.

### Test Files
- `frontend_test.c`: Tests for UI components (menu_io.c and employee_io.c)
- `backend_test.c`: Tests for business logic components (list_manager.c, file_handler.c, and payroll_logic.c)
- `test_validation.c`: Tests for the input validation system (validation.c)

### Running Tests

#### Windows Users
Three batch files are provided for testing different components:

1. **Frontend Tests**:
   ```
   cd src/tests
   compile_frontend_test.bat
   ```
   
   This tests the menu interface and employee I/O functionality.

2. **Backend Tests**:
   ```
   cd src/tests
   compile_backend_test.bat
   ```
   
   This tests the linked list operations, file handling, and payroll calculations.

3. **System Tests**:
   ```
   cd src/tests
   compile_system_test.bat
   ```
   
   This tests the entire system with all modules integrated.

4. **Validation Tests**:
   ```
   cd src
   compile_test_validation.bat
   ```
   
   This tests the input validation system with various validation types.

#### Unix/Linux/macOS Users
Shell scripts with equivalent functionality are provided:

```bash
cd src/tests
chmod +x compile_frontend_test.sh
./compile_frontend_test.sh
```

### Understanding Test Results
After running a test script, you should see output indicating test status:
- A successful test will display "Test COMPLETE" with specific test results
- Failed tests will show error messages indicating what went wrong

### Writing New Tests
When adding new functionality, consider adding corresponding tests:

1. Add test functions to the appropriate test file (frontend_test.c or backend_test.c)
2. Follow the existing pattern of test functions
3. Add your test to the main() function in the test file

Example test function structure:
```c
void test_newFeature() {
    printf("Testing new feature...\n");
    // Test code here
    if (expected == actual) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

---

*Document last updated: June 19, 2025*
