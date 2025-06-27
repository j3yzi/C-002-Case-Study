# 📚 Complete Codebase Reference

This document provides a **comprehensive inventory** of all functions, variables, and data structures in the PUP Information Management System, including detailed information about parameters, return values, and dependencies.

> 📌 **How to use this reference**: Find the file you're interested in, then review its functions, variables, and dependencies. Each function lists its parameters, return value, and any prerequisites.

---

## 📁 `include/headers/apctxt.h` - Application Context

### 🏗️ Data Structures

#### `Config` Struct
```c
typedef struct {
    float regularHours;    // Default: 160.0
    float overtimeRate;    // Default: 0.5 (50%)
    float passingGrade;    // Default: 75.0
} Config;
```

#### `IValidationType` Enum
```c
typedef enum {
    IV_NONE,                        // No validation
    IV_OPTIONAL,                    // Optional field
    IV_CHOICES,                     // Must match one of provided choices
    IV_RANGE_INT,                   // Integer within range
    IV_RANGE_FLT,                   // Float within range
    IV_MAX_LEN,                     // Maximum string length
    IV_MAX_LEN_CHARS,               // Maximum string length (alternative)
    IV_ALPHA_ONLY,                  // Alphabetic characters only
    IV_ALPHA_ONLY_MAX_LEN,          // Alphabetic with max length
    IV_OPTIONAL_ALPHA_ONLY_MAX_LEN, // Optional alphabetic with max length
    IV_EXACT_LEN                    // EXACTLY specified length
} IValidationType;
```

#### `IValidationParams` Union
```c
typedef union {
    // For IV_CHOICES
    struct {
        const char** choices;
        int count;
    } choices;
    
    // For IV_RANGE_INT
    struct {
        long min;
        long max;
    } rangeInt;
    
    // For IV_RANGE_FLT
    struct {
        double min;
        double max;
    } rangeFloat;
    
    // For IV_MAX_LEN_CHARS
    struct {
        int maxLength;
    } maxLengthChars;
} IValidationParams;
```

#### `appFormField` Struct
```c
typedef struct {
    const char* prompt;             // Text to display
    char* buffer;                   // Where to store input
    int bufferSize;                 // Size of buffer
    IValidationType validationType; // Type of validation
    IValidationParams validationParams; // Validation parameters
} appFormField;
```

### 🌍 Global Variables

| Variable | Type | Purpose | Default Value |
|----------|------|---------|---------------|
| `g_config` | `Config` | Global configuration | `{160.0, 0.5, 75.0}` |

### 🔧 Functions

#### `loadConfig()`
- **Purpose**: Loads configuration from INI file
- **Parameters**: None
- **Returns**: `int` - 0 on success, -1 on failure
- **Prerequisites**: File system access, `config.ini` in expected location

#### `saveConfig()`
- **Purpose**: Saves current configuration to INI file
- **Parameters**: None
- **Returns**: `int` - 0 on success, -1 on failure
- **Prerequisites**: File system write access

#### `getRegularHours()`
- **Purpose**: Retrieves configured regular hours
- **Parameters**: None
- **Returns**: `float` - Regular hours value
- **Prerequisites**: `g_config` initialized

#### `getOvertimeRate()`
- **Purpose**: Retrieves configured overtime rate
- **Parameters**: None
- **Returns**: `float` - Overtime rate value
- **Prerequisites**: `g_config` initialized

#### `appGetValidatedInput()`
- **Purpose**: Collects and validates user input for multiple fields
- **Parameters**: 
  - `appFormField* fields` - Array of form fields
  - `int fieldCount` - Number of fields
- **Returns**: `void`
- **Prerequisites**: Console initialized, ANSI support enabled

---

## 📁 `include/headers/list.h` - Generic Linked List

### 🏗️ Data Structures

#### `ListType` Enum
```c
typedef enum { 
    SINGLY, 
    DOUBLY, 
    SINGLY_CIRCULAR, 
    DOUBLY_CIRCULAR 
} ListType;
```

#### `node` Struct
```c
typedef struct Node {
    struct Node* prev;   // Previous node (NULL for SINGLY)
    void* data;          // Generic data pointer
    struct Node* next;   // Next node
} node;
```

#### `list` Struct
```c
typedef struct LinkedList {
    struct Node* head;   // First node
    ListType type;       // List type
    struct Node* tail;   // Last node
    int size;            // Current number of nodes
} list;
```

### 🔧 Functions

#### `createList()`
- **Purpose**: Creates a new linked list
- **Parameters**: 
  - `list** l` - Double pointer to list
  - `ListType type` - Type of list to create
- **Returns**: `int` - 0 on success, -1 on failure
- **Prerequisites**: None

#### `addNode()`
- **Purpose**: Adds a node to the list
- **Parameters**: 
  - `list** l` - Double pointer to list
  - `void* data` - Data to store
- **Returns**: `int` - 0 on success, -1 on failure
- **Prerequisites**: Valid data pointer

#### `removeNode()`
- **Purpose**: Removes a node from the list
- **Parameters**: 
  - `list* l` - List pointer
  - `const void* data` - Data to find and remove
  - `void (*freeData)(void* data)` - Function to free data
- **Returns**: `void`
- **Prerequisites**: Valid list, comparison function

#### `getNodeData()`
- **Purpose**: Gets data at specified index
- **Parameters**: 
  - `const list* l` - List pointer
  - `int index` - Index to retrieve
- **Returns**: `void*` - Data pointer or NULL if not found
- **Prerequisites**: Valid list, index in range

#### `destroyList()`
- **Purpose**: Destroys list and frees all memory
- **Parameters**: 
  - `list** l` - Double pointer to list
  - `void (*freeData)(void* data)` - Function to free data
- **Returns**: `void`
- **Prerequisites**: Valid list pointer

---

## 📁 `include/models/employee.h` - Employee Model

### 🏗️ Data Structures

#### `EmployeeName` Struct
```c
typedef struct {
    char firstName[employeeFirstNameLen];    // 32 chars
    char middleName[employeeMiddleNameLen];  // 32 chars
    char lastName[employeeLastNameLen];      // 32 chars
    char fullName[employeeNameLen];          // 16 chars (formatted)
} EmployeeName;
```

#### `PersonalInfo` Struct
```c
typedef struct {
    char employeeNumber[employeeNumberLen];  // EXACTLY 10 chars + null
    EmployeeName name;                      // Name components
} PersonalInfo;
```

#### `EmployeeStatus` Enum
```c
typedef enum {
    statusRegular,  // Regular employee (R)
    statusCasual    // Casual employee (C)
} EmployeeStatus;
```

#### `EmploymentInfo` Struct
```c
typedef struct {
    EmployeeStatus status;   // Regular or Casual
    int hoursWorked;         // 0-744 hours
    float basicRate;         // Hourly rate (≤ 999.99)
} EmploymentInfo;
```

#### `PayrollInfo` Struct
```c
typedef struct {
    float basicPay;          // Capped at 999,999.00
    float overtimePay;       // For hours > regularHours
    float deductions;        // Capped at 99,999.99
    float netPay;            // basicPay + overtimePay - deductions
} PayrollInfo;
```

#### `Employee` Struct
```c
typedef struct {
    PersonalInfo personal;       // ID and name
    EmploymentInfo employment;   // Status, hours, rate
    PayrollInfo payroll;         // Calculated pay values
} Employee;
```

### 🔧 Functions

#### `composeEmployeeName()`
- **Purpose**: Formats full name from components
- **Parameters**: 
  - `EmployeeName* name` - Name structure
- **Returns**: `int` - 1 on success, 0 on failure
- **Prerequisites**: First and last name populated

#### `searchEmployeeByNumber()`
- **Purpose**: Finds employee by ID
- **Parameters**: 
  - `const list* employeeList` - List to search
  - `const char* employeeNumber` - ID to find
- **Returns**: `Employee*` - Pointer to found employee or NULL
- **Prerequisites**: Valid list

#### `searchEmployeeByName()`
- **Purpose**: Finds employee by name
- **Parameters**: 
  - `const list* employeeList` - List to search
  - `const char* fullName` - Name to find
- **Returns**: `Employee*` - Pointer to found employee or NULL
- **Prerequisites**: Valid list

#### `updateEmployeeData()`
- **Purpose**: Updates employee data
- **Parameters**: 
  - `Employee* employee` - Employee to update
  - `const Employee* newData` - New data
- **Returns**: `int` - 0 on success, -1 on failure
- **Prerequisites**: Valid employee pointers

#### `displayEmployeeDetails()`
- **Purpose**: Prints employee information
- **Parameters**: 
  - `const Employee* employee` - Employee to display
- **Returns**: `void`
- **Prerequisites**: Valid employee pointer

---

## 📁 `include/models/student.h` - Student Model

### 🏗️ Data Structures

#### `StudentName` Struct
```c
typedef struct {
    char firstName[studentFirstNameLen];     // 32 chars
    char middleName[studentMiddleNameLen];   // 32 chars
    char lastName[studentLastNameLen];       // 32 chars
    char fullName[studentNameLen];           // 31 chars (formatted)
} StudentName;
```

#### `StudentInfo` Struct
```c
typedef struct {
    char studentNumber[studentNumberLen];    // EXACTLY 10 chars + null
    StudentName name;                        // Name components
    char programCode[programCodeLen];        // Academic program
    int yearLevel;                           // 1-4
} StudentInfo;
```

#### `AcademicInfo` Struct
```c
typedef struct {
    int unitsEnrolled;                       // 1-30
    float prelimGrade;                       // 0-100
    float midtermGrade;                      // 0-100
    float finalExamGrade;                    // 0-100
    float finalGrade;                        // Calculated average
    char remarks[studentRemarksLen];         // "PASSED" or "FAILED"
} AcademicInfo;
```

#### `AcademicStanding` Enum
```c
typedef enum {
    acadRegular,     // Good standing
    acadProbation    // Below threshold
} AcademicStanding;
```

#### `Student` Struct
```c
typedef struct {
    StudentInfo personal;        // ID, name, program
    AcademicInfo academic;       // Grades and units
    AcademicStanding standing;   // Academic status
} Student;
```

### 🔧 Functions

#### `composeStudentName()`
- **Purpose**: Formats full name from components
- **Parameters**: 
  - `StudentName* name` - Name structure
- **Returns**: `int` - 1 on success, 0 on failure
- **Prerequisites**: First and last name populated

#### `calculateFinalGrade()`
- **Purpose**: Calculates final grade and updates standing
- **Parameters**: 
  - `Student* student` - Student to update
- **Returns**: `void`
- **Prerequisites**: Prelim, midterm, final exam grades set

#### `searchStudentByNumber()`
- **Purpose**: Finds student by ID
- **Parameters**: 
  - `const list* studentList` - List to search
  - `const char* studentNumber` - ID to find
- **Returns**: `Student*` - Pointer to found student or NULL
- **Prerequisites**: Valid list

---

## 📁 `src/modules/payroll.c` - Payroll Calculations

### 🔧 Functions

#### `calculatePayroll()`
- **Purpose**: Master function for all payroll calculations
- **Parameters**: 
  - `Employee* employee` - Employee to calculate for
- **Returns**: `void`
- **Prerequisites**: Valid employee with hours and rate set

#### `calculateBasicPay()`
- **Purpose**: Calculates basic pay with cap
- **Parameters**: 
  - `Employee* employee` - Employee to calculate for
- **Returns**: `void`
- **Prerequisites**: Valid employee
- **Business Rules**:
  - If hours ≥ regularHours: basicPay = basicRate * regularHours
  - If hours < regularHours: basicPay = basicRate * hours
  - Cap at 999,999.00 with warning

#### `calculateOvertimePay()`
- **Purpose**: Calculates overtime pay
- **Parameters**: 
  - `Employee* employee` - Employee to calculate for
- **Returns**: `void`
- **Prerequisites**: Valid employee
- **Business Rules**:
  - If hours > regularHours: overtimePay = (hours - regularHours) * basicRate * (1 + overtimeRate)
  - Otherwise: overtimePay = 0

#### `calculateDeductions()`
- **Purpose**: Calculates deductions with cap
- **Parameters**: 
  - `Employee* employee` - Employee to calculate for
- **Returns**: `void`
- **Prerequisites**: Valid employee
- **Business Rules**:
  - If hours < regularHours: deductions = basicRate * (regularHours - hours)
  - Otherwise: deductions = 0
  - Cap at 99,999.99 with warning

---

## 📁 `src/ui/empio.c` - Employee UI

### 🔧 Functions

#### `handleAddEmployee()`
- **Purpose**: Prompts for new employee data
- **Parameters**: 
  - `list* employeeList` - List to add to
- **Returns**: `int` - 0 on success, -1 on cancel/failure
- **Prerequisites**: Valid list, active employee list
- **UI Flow**:
  1. Display header
  2. Collect name (first, middle, last)
  3. Collect employment details (number, status, hours, rate)
  4. Show confirmation summary
  5. Add to list if confirmed

#### `handleEditEmployee()`
- **Purpose**: Edits existing employee
- **Parameters**: 
  - `list* employeeList` - List containing employee
- **Returns**: `int` - 0 on success, -1 on cancel/failure
- **Prerequisites**: Valid list with employees
- **UI Flow**:
  1. Show employee list
  2. Prompt for employee number
  3. Display edit menu (name, number, status, hours, rate)
  4. Update selected field(s)
  5. Recalculate payroll
  6. Show confirmation

#### `getEmployeeDataFromUser()`
- **Purpose**: Collects all employee data fields
- **Parameters**: 
  - `Employee* newEmployee` - Employee struct to populate
- **Returns**: `int` - 0 on success, -1 on cancel
- **Prerequisites**: Valid employee pointer
- **Validation**:
  - Employee Number: EXACTLY 10 chars (IV_EXACT_LEN)
  - Hours Worked: 0-744 (IV_RANGE_INT)
  - Basic Rate: 0.01-999.99 (IV_RANGE_FLT)

---

## 📁 `src/ui/stuio.c` - Student UI

### 🔧 Functions

#### `handleAddStudent()`
- **Purpose**: Prompts for new student data
- **Parameters**: 
  - `list* studentList` - List to add to
- **Returns**: `int` - 0 on success, -1 on cancel/failure
- **Prerequisites**: Valid list, active student list
- **UI Flow**:
  1. Display header
  2. Collect name (first, middle, last)
  3. Collect student details (number, gender, program, year)
  4. Collect academic info (units, grades)
  5. Calculate final grade
  6. Show confirmation summary
  7. Add to list if confirmed

#### `handleEditStudent()`
- **Purpose**: Edits existing student
- **Parameters**: 
  - `list* studentList` - List containing student
- **Returns**: `int` - 0 on success, -1 on cancel/failure
- **Prerequisites**: Valid list with students
- **UI Flow**:
  1. Show student list
  2. Prompt for student number
  3. Display edit menu (name, number, program, grades)
  4. Update selected field(s)
  5. Recalculate final grade
  6. Show confirmation

#### `getStudentDataFromUser()`
- **Purpose**: Collects all student data fields
- **Parameters**: 
  - `Student* newStudent` - Student struct to populate
- **Returns**: `int` - 0 on success, -1 on cancel
- **Prerequisites**: Valid student pointer
- **Validation**:
  - Student Number: EXACTLY 10 chars (IV_EXACT_LEN)
  - Year Level: 1-4 (IV_RANGE_INT)
  - Units: 1-30 (IV_RANGE_INT)
  - Grades: 0.0-100.0 (IV_RANGE_FLT)

---

## 📁 `include/src/validation.c` - Input Validation

### 🔧 Functions

#### `isValid()`
- **Purpose**: Validates input against specified type
- **Parameters**: 
  - `const char* input` - Input string
  - `IValidationType type` - Validation type
  - `IValidationParams params` - Validation parameters
  - `const char* fieldName` - Field name for error messages
- **Returns**: `bool` - true if valid, false if invalid
- **Prerequisites**: None
- **Validation Types**:
  - `IV_NONE`: Always valid
  - `IV_OPTIONAL`: Valid if empty
  - `IV_CHOICES`: Must match one of choices
  - `IV_RANGE_INT`: Integer within range
  - `IV_RANGE_FLT`: Float within range
  - `IV_MAX_LEN`: Maximum string length
  - `IV_EXACT_LEN`: Exactly specified length
  - `IV_ALPHA_ONLY`: Alphabetic characters only
  - `IV_ALPHA_ONLY_MAX_LEN`: Alphabetic with max length
  - `IV_OPTIONAL_ALPHA_ONLY_MAX_LEN`: Optional alphabetic with max length

#### `appGetValidatedInput()`
- **Purpose**: Collects and validates user input
- **Parameters**: 
  - `appFormField* fields` - Array of form fields
  - `int fieldCount` - Number of fields
- **Returns**: `void`
- **Prerequisites**: Console initialized
- **Process**:
  1. For each field, show prompt
  2. Read input with `readLine()`
  3. Validate with `isValid()`
  4. If invalid, show error and retry
  5. If valid, copy to buffer and continue

---

## 📁 `src/modules/data.c` - Data Persistence

### 🔧 Functions

#### `saveListWithCustomName()`
- **Purpose**: Saves list to file with custom name
- **Parameters**: 
  - `list* dataList` - List to save
  - `const char* listName` - Custom name
  - `const char* dataType` - Type identifier
- **Returns**: `int` - 0 on success, -1 on failure
- **Prerequisites**: Valid list, file system access
- **Process**:
  1. Format filename with dataType and listName
  2. Open file in binary write mode
  3. Write list size
  4. For each node, write data
  5. Close file

#### `loadListWithName()`
- **Purpose**: Loads list from file
- **Parameters**: 
  - `const char* filename` - File to load
  - `const char* dataType` - Type identifier
  - `ListType listType` - Type of list to create
- **Returns**: `list*` - Loaded list or NULL on failure
- **Prerequisites**: File exists, file system access
- **Process**:
  1. Open file in binary read mode
  2. Read list size
  3. Create new list
  4. For each item, read data and add to list
  5. Close file and return list

#### `generatePayrollReportFile()`
- **Purpose**: Creates payroll report
- **Parameters**: 
  - `const list* employeeList` - List to report on
  - `char* generatedFilePath` - Output buffer for path
  - `int pathBufferSize` - Size of path buffer
- **Returns**: `int` - 0 on success, -1 on failure
- **Prerequisites**: Valid list
- **Process**:
  1. Generate timestamped filename
  2. Open file in text write mode
  3. Write header and summary
  4. For each employee, write details
  5. Write totals and close file

---

## 📁 `src/main.c` - Program Entry

### 🔧 Functions

#### `main()`
- **Purpose**: Program entry point
- **Parameters**: None
- **Returns**: `int` - 0 on success
- **Prerequisites**: None
- **Process**:
  1. Initialize console
  2. Load configuration
  3. Initialize managers
  4. Display main menu
  5. Clean up and exit

---

## 🔄 Dependencies & Prerequisites

### Core Dependencies

| Component | Depends On | Required For |
|-----------|------------|-------------|
| `main.c` | `apctxt.h`, `menuio.h` | Program startup |
| Employee module | `list.h`, `apctxt.h` | Employee management |
| Student module | `list.h`, `apctxt.h` | Student management |
| Course module | `list.h` | Course catalog |
| Payroll calculations | `employee.h`, `apctxt.h` | Salary computation |
| UI modules | `interface.h`, `apctxt.h`, domain models | User interaction |
| Validation | `apctxt.h`, `apclrs.h` | Input safety |

### External Dependencies

- Standard C library (`stdio.h`, `stdlib.h`, `string.h`, etc.)
- Windows-specific for console control (`windows.h`, `conio.h`)

---

## 🔍 Memory Management

### Allocation Points

| Function | Allocates | Freed By |
|----------|-----------|----------|
| `createList()` | `list` struct | `destroyList()` |
| `addNode()` | `node` struct | `removeNode()`, `destroyList()` |
| `createEmployee()` | `Employee` struct | `freeEmployee()` |
| `createStudent()` | `Student` struct | `freeStudent()` |
| `createCourse()` | `Course` struct | `freeCourse()` |

### Free Functions

| Function | Frees | Called By |
|----------|-------|-----------|
| `freeEmployee()` | `Employee` struct | `removeNode()`, `destroyList()` |
| `freeStudent()` | `Student` struct | `removeNode()`, `destroyList()` |
| `freeCourse()` | `Course` struct | `removeNode()`, `destroyList()` |
| `destroyList()` | `list` and all nodes | Program exit, list reload |

---

*This comprehensive reference was auto-generated June 2025. It includes all public functions, variables, and data structures in the PUP Information Management System.* 