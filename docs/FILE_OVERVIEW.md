# 📂 Source File Reference

This document acts as a **yellow-pages** for the PUP Information Management System codebase. Each table row lists a file, the primary items it defines (structs 🏗️, global variables 🌍, public functions 🔧) and a short "what it's for". Use it in tandem with *PROGRAM_FLOW.md* to jump quickly into the right compilation unit.

> ⚠️  The list focuses on **public / widely-used** symbols. Helper `static` functions are omitted for brevity.

---

## 🛠 Infrastructure (`include/headers/`)

| **File** | **Key Definitions** | **Purpose** |
|----------|---------------------|-------------|
| **apctxt.h** | 🏗️ `Config`, `Menu`, `MenuOption`, `appFormField`<br>🔧 `loadConfig()`, `saveConfig()`, `getRegularHours()` | Global app context, validation enum (`IValidationType`), menu framework, INI config. |
| **apclrs.h** | 🌍 ANSI colour macros (`UI_HEADER`, `UI_ERROR` …) | Central colour palette for UI. |
| **interface.h** | 🏗️ `EmployeeManager`, `StudentManager`, `PaginationState`<br>🔧 Menu display helpers | High-level UI helpers used by all menus & tables. |
| **list.h** | 🏗️ `node`, `list`, `ListType`<br>🔧 `addNode()`, `removeNode()`, `destroyList()` | Generic singly/doubly linked-list implementation. |
| **state.h** | 🌍 Global appState flags | Track if lists have been created / loaded. |
| **auth.h** | 🔧 `hashPassword()`, `verifyPassword()` | Placeholder authentication utilities. |

---

## 📊 Domain Models (`include/models/`)

| **File** | **Structs / Enums** | **Highlight Functions** | **Notes** |
|----------|---------------------|-------------------------|-----------|
| **employee.h** | ▸ `EmployeeName`<br>▸ `PersonalInfo`<br>▸ `EmploymentInfo`<br>▸ `PayrollInfo`<br>▸ `Employee` | `composeEmployeeName()`<br>`searchEmployeeByNumber()`<br>`updateEmployeeData()` | Core employee entity + helpers. |
| **student.h** | ▸ `StudentName`<br>▸ `StudentInfo`<br>▸ `AcademicInfo`<br>▸ `Student` | `composeStudentName()`<br>`calculateFinalGrade()` | Handles grades & academic standing. |
| **course.h** | ▸ `Course`<br>Enum `CourseType` | CRUD helpers in `course.c` | Course catalog entry. |

---

## 🎛️ Business Logic (`src/modules/`)

| **File** | **Functions** | **Description** |
|----------|---------------|-----------------|
| **payroll.c / .h** | `calculatePayroll()` ➜ orchestrates<br>`calculateBasicPay()`<br>`calculateOvertimePay()`<br>`calculateDeductions()` | Implements payroll maths incl. caps + warnings. |
| **data.c / .h** | `saveListWithCustomName()`<br>`loadListWithName()`<br>`generatePayrollReportFile()` | Binary persistence + report generation. |

---

## 🎨 User-Interface Layer (`src/ui/`)

| **File** | **Role** | **Key Public Routines** |
|----------|----------|-------------------------|
| **menuio.c** | Master menu & multi-list manager (employee, student, course) | `menuLoop()`, `initMultiListManager()`, `runEmployeeManagement()`, `runStudentManagement()`, `runCourseManagement()` |
| **interface.c** | Console + UI toolkit (menus, tables, pagination) | `displayMenu()`, `displayMenuOptions()`, `displayEmployeeTable()`, `displayStudentTable()` |
| **empio.c** | Employee CRUD prompts | `handleAddEmployee()`, `handleEditEmployee()` |
| **stuio.c** | Student CRUD prompts | `handleAddStudent()`, `handleEditStudent()` |
| **courseio.c** | Course catalog UI | `handleAddCourse()`, `handleEditCourse()` |

---

## 🔑 Validation & Helpers (`include/src/`)

| **File** | **Symbols** | **Purpose** |
|----------|-------------|-------------|
| **validation.c** | `enableAnsiSupport()`, `isValid()`, `appGetValidatedInput()`<br>Enum cases include **`IV_EXACT_LEN`** | Centralised input validation dispatcher. |
| **apctxt.c** | `g_config` global, `loadConfig()`, `saveConfig()` | INI read/write + overtime / regular hour getters. |

---

## 🚀 Program Entry

| **File** | **What it contains** |
|----------|---------------------|
| **src/main.c** | `main()` → initialises context, shows splash, enters `menuLoop()` loop. |

---

### Legend

- 🏗️ **Struct / Enum**  — data type definition  
- 🌍 **Global Variable**  — shared across modules  
- 🔧 **Function**  — publicly callable routine

---

*Auto-generated overview (June 2025, refreshed 27 June 2025 after full repo scan). For a walk-through of runtime control flow see **docs/PROGRAM_FLOW.md**.* 