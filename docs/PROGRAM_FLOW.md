# 🚦 Program Execution Flow – PUP Information Management System

This document provides a **step-by-step narrative** of what happens when you launch the program, following the control flow from `main.c` outward into the infrastructure, business-logic, and UI layers.

---

## 🚀 Application Startup Sequence

### 1️⃣ **`main.c` – Application Entry Point**

1. **`main()` initialization**  
   Located in `src/main.c`, the function performs minimal setup and delegates control to the main menu system.

2. **Context initialization**  
   - **`initConsole()`** – Enables ANSI colours and sets the terminal title  
   - **`loadConfig()`** – Reads `config.ini` values into the global `g_config` structure (regular hours, overtime rate, passing grade)  
   - **`initMultiListManager()`** – Initializes employee and student managers  
   - **`initCourseCatalogManager()`** – Sets up course catalog system

3. **Program configuration loading**  
   - Loads academic programs from configuration  
   - Initializes `AppConfig` with system metadata  
   - Updates main menu with course management integration

4. **Main menu activation**  
   `menuLoop()` enters the primary navigation loop where users can access Employee, Student, Course, System Information, and Configuration sub-menus.

> **Control Transfer**: From this point on, control is entirely driven by the **Menu System** in `src/ui/menuio.c`.

---

## 🎛️ Menu Infrastructure & Navigation

### 2️⃣ **Menu System Architecture (`menuio.c`)**

#### **Menu Definition & Structure**
- **Menu Components**: Defined with `Menu` + `MenuOption` structs (see `apctxt.h`)
- **Option Properties**: Each option contains key, caption, description, state flags, and callback functionality
- **Dynamic States**: Options are enabled/disabled based on application state (active lists, data availability)

#### **Rendering Pipeline**
1. **`displayMenuHeader()`** – Draws the fancy double-box header with centered title
2. **`displayMenuOptions()`** – Renders selectable options with highlight colours and state indicators
3. **`displayInfoBox()`** – Shows contextual help, keyboard shortcuts, and real-time clock
4. **`displayMenuDescription()`** – Displays detailed description of currently selected option

#### **User Interaction Loop**
```
┌─────────────────────────────────────────┐
│  User Input (Arrow Keys, Enter, Esc)   │
└─────────────────┬───────────────────────┘
                  │
┌─────────────────▼───────────────────────┐
│     updateMenuSelection()               │
│  • Highlight new option                 │
│  • Un-highlight previous option         │
│  • Update description box               │
└─────────────────┬───────────────────────┘
                  │
┌─────────────────▼───────────────────────┐
│      Option Dispatch (Switch)          │
│  • Employee Management                  │
│  • Student Management                   │
│  • Course Management                    │
│  • System Information                   │
│  • Configuration Settings               │
└─────────────────────────────────────────┘
```

---

## 👥 Employee Management Workflow

### 3️⃣ **Employee Operations (`empio.c`)**

Below is the complete workflow for **adding a new employee** (edit, delete, and search operations follow similar patterns).

#### **Input Collection Pipeline**
1. **Header display**  
   `handleAddEmployee()` clears screen and shows coloured "Enter Employee Details" header

2. **Personal information collection**  
   Multi-stage name input using `appFormField` structs:
   ```
   First Name → Middle Name → Last Name → composeEmployeeName()
                                              ↓
                                        [Validation & Formatting]
                                              ↓
                                     Success/Retry Loop
   ```

3. **Employment information collection**  
   Structured data entry with enhanced validation:
   ```
   Employee Number (10 chars) ──┐
   Status (R/C)                 ├─► IV_EXACT_LEN validation
   Hours Worked (0-744)         ├─► IV_RANGE_INT validation  
   Basic Rate (≤ 999.99)       ─┘   IV_RANGE_FLT validation
   ```

#### **Validation Engine Integration**
- **`appGetValidatedInput()`** orchestrates the validation process
- **`isValid()`** applies type-specific validation rules
- **Error handling** with user-friendly messages and retry options
- **New validation types**: `IV_EXACT_LEN` for precise length requirements

#### **Business Logic Processing**
```
User Input → Validation → calculatePayroll() → Confirmation → List Storage
                              ↓
                    ┌─────────────────────────┐
                    │   Payroll Calculations  │
                    │  • calculateBasicPay()  │
                    │  • calculateOvertimePay()│
                    │  • calculateDeductions() │
                    │  • Apply safety caps     │
                    └─────────────────────────┘
```

#### **Confirmation & Persistence**
- **Summary display** with formatted employee details
- **User confirmation** via `appYesNoPrompt()`
- **List integration** using generic linked-list operations
- **Multi-list support** through `EmployeeManager` structure

---

## 🎓 Student Management Workflow

### 4️⃣ **Student Operations (`stuio.c`)**

The student workflow mirrors the employee system with academic-specific enhancements.

#### **Academic Data Collection**
```
Personal Info → Academic Details → Grade Processing → Standing Calculation
     ↓               ↓                    ↓                   ↓
Name Components   Program/Year      Prelim/Midterm/Final   Pass/Fail Status
Student Number    Units Enrolled   Grade Validation       Academic Standing
```

#### **Grade Calculation Engine**
- **`calculateFinalGrade()`** – Computes average of three grade components
- **Pass/Fail logic** – Compares against configurable threshold (default: 75.0)
- **Academic standing** – Determines regular/probation status
- **Remarks generation** – Automatic "PASSED"/"FAILED" assignment

#### **Enhanced Features**
- **Program validation** against configured academic programs
- **Grade sorting** capabilities for class rankings
- **Academic reports** with statistical summaries
- **Transcript generation** with professional formatting

---

## 📚 Course Management Workflow

### 5️⃣ **Course Operations (`courseio.c`)**

Course management follows the established CRUD pattern with catalog-specific features.

#### **Course Entity Management**
- **Course types** – Core, Major, Elective, General Education classification
- **Status management** – Active/inactive toggle for course availability
- **Unit validation** – Range checking (1-6 units per course)
- **Search capabilities** – By code (exact) or name (partial match)

#### **Catalog Operations**
- **Catalog creation** with custom naming
- **Save/load operations** using `.cat` file extension
- **Course discovery** through integrated search functionality
- **Academic integration** for program requirement mapping

---

## 💼 Business Logic Layer

### 6️⃣ **Core Business Operations**

#### **Payroll Calculation Engine (`payroll.c`)**
```
Employee Data → calculatePayroll() → Financial Components
                        ↓
    ┌─────────────────────────────────────────────────┐
    │                Payroll Pipeline                 │
    │                                                 │
    │  calculateBasicPay()     ← Hours × Rate logic   │
    │          ↓               ← Regular hours cap     │
    │  calculateOvertimePay()  ← Overtime @ +50%      │
    │          ↓               ← Beyond regular hours  │
    │  calculateDeductions()   ← Auto-deduction calc  │
    │          ↓               ← Safety caps applied   │
    │  Final Net Pay          ← Sum with validations  │
    └─────────────────────────────────────────────────┘
```

#### **Data Persistence Engine (`data.c`)**
- **Binary file I/O** for efficient storage and retrieval
- **Multi-list management** with custom naming schemes
- **Report generation** with timestamped filenames
- **Data integrity** validation during load operations

---

## 🔧 Configuration & Settings

### 7️⃣ **Configuration Management Workflow**

#### **Settings Categories**
```
Configuration Menu
├── Payroll Settings
│   ├── Regular Hours (40-744)
│   └── Overtime Rate (0.1-2.0)
├── Academic Settings
│   └── Passing Grade (configurable)
└── System Operations
    ├── Save Configuration
    └── Reset to Defaults
```

#### **Configuration Persistence**
1. **Runtime modification** through interactive prompts
2. **Validation** against business rule constraints  
3. **`saveConfig()`** persistence to `config.ini`
4. **Immediate effect** across all system operations

---

## 🔄 Data Flow & State Management

### 8️⃣ **System State Coordination**

#### **Multi-List Manager State**
```
EmployeeManager                    StudentManager
├── employeeLists[10]             ├── studentLists[10]
├── employeeListNames[10][50]     ├── studentListNames[10][50]  
├── employeeListCount             ├── studentListCount
└── activeEmployeeList            └── activeStudentList
```

#### **State-Driven UI Updates**
- **Menu option states** updated based on active lists
- **Dynamic feature availability** based on data presence
- **Context-sensitive help** and error messages
- **Cross-module state synchronization**

---

## 🛡️ System Reliability & Safety

### 9️⃣ **Error Handling & Recovery**

#### **Input Validation Safety**
- **Buffer overflow prevention** through controlled input methods
- **Type safety** with comprehensive validation rules
- **Range checking** for all numeric inputs
- **Graceful degradation** when errors occur

#### **Memory Management**
- **Automatic cleanup** on program exit
- **Resource tracking** through manager structures
- **Custom free functions** for proper deallocation
- **Leak prevention** with consistent allocation patterns

---

## 🚪 Program Shutdown Sequence

### 🔟 **Clean Exit Process**

1. **User selects Exit** from main menu
2. **Resource cleanup**:
   - `cleanupMultiListManager()` – Frees all employee and student lists
   - `cleanupCourseCatalogManager()` – Releases course catalog resources
   - `destroyList()` calls with type-specific free functions
3. **Configuration persistence** – `saveConfig()` writes any changed settings
4. **Console restoration** – Reset colours and cursor position
5. **Graceful termination** with success code

---

## 🔍 Developer Navigation Guide

### 📌 **Code Exploration Tips**

#### **Entry Points for New Contributors**
1. **Start with `main.c`** – Understand initialization sequence
2. **Follow `menuLoop()`** – See how user interaction drives the system  
3. **Explore a single workflow** – Pick Employee or Student management
4. **Trace data flow** – Follow data from input to storage
5. **Study validation** – Understand the `appGetValidatedInput()` pattern

#### **Debugging Strategies**
- **Breakpoint at `menuLoop()`** – See control flow in action
- **Monitor manager state** – Watch `empManager` and `stuManager`
- **Trace validation calls** – Follow `isValid()` execution
- **Check memory usage** – Verify proper cleanup

#### **Architecture Understanding**
```
UI Layer (menuio.c, empio.c, stuio.c)
         ↓
Business Logic (payroll.c, data.c)
         ↓  
Data Models (employee.h, student.h)
         ↓
Infrastructure (list.h, apctxt.h)
```

---

## 📊 Performance Characteristics

### ⚡ **System Efficiency**

#### **Time Complexity**
- **List operations**: O(n) for search, O(1) for add/remove
- **Validation**: O(1) for most validation types
- **File I/O**: O(n) relative to data size
- **Menu rendering**: O(1) for updates, O(n) for full redraw

#### **Space Complexity**
- **Memory usage**: Linear with number of records
- **File storage**: Efficient binary format
- **Configuration**: Minimal overhead
- **UI buffers**: Fixed size allocations

---

*Document generated June 2025 and **enhanced 27 June 2025** with improved formatting, detailed workflows, and comprehensive navigation guidance.* 