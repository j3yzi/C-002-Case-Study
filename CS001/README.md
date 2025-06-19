# CS001 Payroll Management System — Developer Documentation

## Overview

CS001 is a modular C payroll management system designed to demonstrate structured programming, modularity, and data management using custom data structures and validation utilities. The system is organized around three core components:

- **Application Context & Utilities (`apctxt.h`)**
- **Employee Data Management (`employee.h`)**
- **Generic Linked List Library (`list.h`)**

These modules are designed for extensibility and reusability, and are intended to be used together to provide a robust, maintainable payroll management solution.

---

## Project Structure

- `include/` — Header files for core modules
- `src/` — Source code for logic, modules, and UI
- `data/` — Data storage (e.g., payroll files)
- `output/` — Output files (e.g., reports)
- `docs/` — Documentation

---

## Core Modules and Their Roles

### 1. Application Context & Utilities (`apctxt.h`)

**Purpose:**
- Provides application-wide configuration, menu utilities, input validation, and form handling.
- Centralizes user interaction logic and validation, decoupling UI from business logic.

**Key Types:**
- `AppConfig`: Application metadata (name, version, license, creator)
- `Header`: For displaying section headers
- `MenuOption`: Menu item representation
- `IValidationType` & `IValidationParams`: Input validation types and parameters
- `appFormField`: Describes a form field for validated user input

**Key Functions:**
- `appClearScreen()`: Clears the console screen
- `appPrintHeader(const Header*)`: Prints a formatted header
- `appDisplayMenu(MenuOption[], int, const char*)`: Displays a menu and returns user choice
- `appGetValidatedInput(appFormField*, int)`: Prompts for and validates user input

**Validation:**
- Supports choices, integer/float ranges, and max length validation for robust user input.
- All user input flows through this module, ensuring data integrity before it reaches business logic.

---

### 2. Employee Data Management (`employee.h`)

**Purpose:**
- Defines the employee data model and provides functions for file I/O and record management.
- Encapsulates all logic related to employee records, including serialization and deserialization.

**Key Types:**
- `PersonalInfo`: Employee number and name
- `EmployeeStatus`: Regular or casual
- `EmploymentInfo`: Status, hours worked, basic rate
- `PayrollInfo`: Basic pay, overtime, deductions, net pay
- `Employee`: Aggregates all above info

**Key Functions:**
- `exportEmployeeDataToFile(list*, const char*)`: Exports employee list to a file
- `loadEmployeeDataFromFile(const char*, ListType)`: Loads employee data from a file into a list

**Constants:**
- `EMPLOYEE_NUMBER_LEN`, `EMPLOYEE_NAME_LEN`: Field sizes
- `MAX_EMPLOYEE_RECORDS`: Max records supported
- `PAYROLL_FILE_NAME`: Default data file

---

### 3. Generic Linked List Library (`list.h`)

**Purpose:**
- Implements a flexible linked list for storing arbitrary data, including employee records.
- Abstracts away the details of list management, allowing other modules to focus on business logic.

**Key Types:**
- `ListType`: Singly, doubly, circular, etc.
- `node`: List node (with `prev`, `next`, and `data`)
- `list`: Linked list structure (head, tail, size, type)

**Key Functions:**
- `addNode(list**, void*)`: Add a node with data
- `removeNode(list*, void*)`: Remove a node by data
- `getNodeData(list*, int)`: Retrieve data by index
- `clearList(list*)`: Remove all nodes
- `printList(list*, void (*printFunc)(void*))`: Print list using a custom function
- `createList(ListType)`: Create a new list
- `destroyList(list*)`: Free all memory

**Internal Linking Functions:**
- For different list types (singly, doubly, circular, etc.)

---

## Architectural Flow and Synergy

The system is designed for clear separation of concerns and modularity:

1. **Initialization:**
   - Application metadata and configuration are set up using `AppConfig` and related types from `apctxt.h`.
   - The main function initializes the required lists (typically via `createList`).

2. **User Interaction:**
   - All user input and menu navigation are handled by `apctxt.h` utilities.
   - Menus are defined using `MenuOption` arrays and displayed with `appDisplayMenu`.
   - Input for forms (e.g., adding an employee) is described using `appFormField` arrays and validated with `appGetValidatedInput`.

3. **Business Logic:**
   - When a user action requires data manipulation (e.g., add, edit, delete employee), the program operates on the in-memory list of employees (`list*`), with each node containing an `Employee` struct.
   - The list module (`list.h`) abstracts all list operations, so business logic does not need to manage node pointers directly.

4. **Persistence:**
   - Employee data is loaded from disk at startup using `loadEmployeeDataFromFile`.
   - Changes are saved back to disk using `exportEmployeeDataToFile`.
   - The file format and serialization logic are encapsulated in the employee module.

5. **Validation and Error Handling:**
   - All user input is validated before being processed, using the validation types and parameters in `apctxt.h`.
   - This ensures that only valid, well-formed data enters the business logic layer.

6. **Extensibility:**
   - New features (e.g., new employee fields, new menu options) can be added by extending the relevant modules without breaking the overall architecture.
   - The list module can be reused for other data types by changing the data pointer type.

---

## Example Flow: Adding an Employee (Developer Perspective)

1. **Menu Selection:**
   - The main menu is displayed using `appDisplayMenu`.
   - User selects "Add Employee".

2. **Form Input:**
   - An array of `appFormField` is defined, describing each required field (number, name, status, etc.), with validation parameters.
   - `appGetValidatedInput` is called to prompt and validate each field.

3. **Record Creation:**
   - A new `Employee` struct is populated from the validated input.
   - `addNode` is called to insert the new employee into the in-memory list.

4. **Persistence (Optional):**
   - When the user chooses to save, `exportEmployeeDataToFile` serializes the list to disk.

---

## Extending the System

- **Add new fields:**
  - Update the `Employee` struct and related logic in `employee.h`/`employee.c`.
  - Update form definitions and validation in the UI logic.
- **Change validation:**
  - Modify or extend validation logic in `apctxt.h`/`apctxt.c`.
- **Support new list types:**
  - Implement additional logic in `list.h`/`lismgr.c`.
- **Add new modules:**
  - Follow the same separation of concerns: define data structures, provide list integration, and expose a clear API.

---

## Code Snippet: Adding an Employee (Simplified)

```c
// Define form fields
appFormField fields[] = {
    {"Employee Number", empNumBuffer, EMPLOYEE_NUMBER_LEN, IV_MAX_LEN, {.choices = NULL}},
    {"Employee Name", empNameBuffer, EMPLOYEE_NAME_LEN, IV_MAX_LEN, {.choices = NULL}},
    // ... other fields ...
};
appGetValidatedInput(fields, sizeof(fields)/sizeof(fields[0]));

// Populate struct
Employee* emp = malloc(sizeof(Employee));
strcpy(emp->personal.employeeNumber, empNumBuffer);
strcpy(emp->personal.employeeName, empNameBuffer);
// ... set other fields ...

// Add to list
addNode(&employeeList, emp);
```

---

## License

See the root `LICENSE` file for details.

---

**For more details, see the source code and comments in each header file.**
