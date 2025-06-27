# 🚦 Program Execution Flow – PUP Information Management System

This document provides a **step-by-step narrative** of what happens when you launch the program, following the control flow from `main.c` outward into the infrastructure, business-logic, and UI layers.

---

## 1️⃣  `main.c` – Application Entry Point

1. **`main()` starts**  
   Located in `src/main.c`, the function does minimal work: it sets up the application context and hands control to the main menu loop.
2. **Context Initialisation**  
   `initConsole()` – enables ANSI colours and sets the terminal title.  
   `loadConfig()` – reads `config.ini` values into the global `g_config` structure (regular hours, overtime rate, etc.).
3. **Manager Setup**  
   • `empManager` and `stuManager` arrays are zero-initialised.  
   • List counters (`employeeListCount`, etc.) reset to `0`.
4. **Main Menu Loop**  
   `displayMainMenu()` enters a loop where the user can select Employee, Student, Course, Statistics or Config sub-menus.

> From this point on, control is entirely driven by the **Menu System** in `src/ui/menuio.c`.

---

## 2️⃣  Menu Infrastructure (`menuio.c`)

1. **Menu Definition**  
   Menus are defined with `Menu` + `MenuOption` structs (see `apctxt.h`).  Each option contains: key, caption, description, colour settings and an optional *callback pointer*.
2. **Rendering**  
   • `displayMenuHeader()` draws the fancy double-box header.  
   • `displayMenuOptions()` prints the selectable options, handling highlight colours.  
   • `displayInfoBox()` shows contextual help on the right.
3. **User Navigation**  
   The arrow-key loop uses `_getch()` to read key strokes, updating the highlighted option until **Enter** is pressed.
4. **Option Dispatch**  
   When the user confirms, control dispatches to a handler function **chosen via `switch`** – for instance `handleAddEmployee()` or `openConfigurationMenu()`.

---

## 3️⃣  Employee Workflow (`empio.c`)

Below is the happy-path for **adding a new employee** (other operations such as edit or delete are similar).

1. **Prompt Headers**  
   `handleAddEmployee()` clears the screen and shows the coloured "Enter Employee Details" header.
2. **Collect Personal Info**  
   A loop of `appFormField` structs prompts for First, Middle, Last names **until `composeEmployeeName()` succeeds**.
3. **Collect Employment Info**  
   Second group of fields (now using the new `IV_EXACT_LEN`, `IV_RANGE_INT`, `IV_RANGE_FLT`):
   - Employee Number (10 chars)  
   - Status (R/C)  
   - Hours Worked (0-744)  
   - Basic Rate (≤ 999.99)
4. **Validation Engine** (`include/src/validation.c`)  
   `appGetValidatedInput()` loops each field:  
   • Shows prompt  
   • Captures input via `readLine()`  
   • Calls `isValid()` which runs the appropriate case of `IValidationType`, including the new `IV_EXACT_LEN`.
5. **Payroll Calculation**  
   After successful input, `calculatePayroll()` is called:
   ```c
   calculateBasicPay()   // honours 160h regular limit + cap to 999,999
   calculateOvertimePay() // 50% premium if >160h
   calculateDeductions()  // deficiency * basicRate, capped at 99,999.99
   ```
6. **Confirmation Screen**  
   A summary box is rendered with perfectly aligned Unicode borders; user confirms or cancels.
7. **Persist to List**  
   On confirmation, a new `Employee` struct is allocated and pushed into the active `list`.

---

## 4️⃣  Student Workflow (`stuio.c`)

Very similar structure to employees, with student-specific fields such as Program, Year Level and three grade entries.  
`calculateFinalGrade()` averages the grades, stores remarks and updates standing.

---

## 5️⃣  Course Workflow (`courseio.c`)

Handles course catalog CRUD operations. Uses the same validation + menu patterns. Unique aspects include course type enum and active/inactive status toggle.

---

## 6️⃣  Business Logic Layer (`src/modules/`)

### `payroll.c`
• Houses the salary math functions described earlier, now with safety caps.  

### `data.c`
• Wraps binary file I/O for saving / loading lists.  
• Generates payroll & student reports via `generatePayrollReportFile()` etc.

---

## 7️⃣  Data Persistence Flow

```
 list → data.c → fopen (wb) → fwrite(each node data) → fclose
``` 
The inverse occurs on load.  Filenames are user-defined and stored in the *data* directory.

---

## 8️⃣  Configuration Workflow

`menuio.c` ➜ *Configuration Settings* option ➜ prompts for Regular Hours (40-744) & Overtime Rate (0.1-2.0).  Values are written back to `config.ini` through `saveConfig()` (in `apctxt.c`).

---

## 9️⃣  Program Shutdown

1. User selects **Exit** in the main menu.  
2. Each active list is destroyed via `destroyList()` with type-specific free functions.  
3. `saveConfig()` persists any changed settings.  
4. Console colours are reset and the application terminates.

---

### 📌 Tip for New Contributors

Open **`main.c`**, put a breakpoint on `displayMainMenu()`, then step-through to see the flow in real-time.  Each UI module follows the same template, so once you understand one, the others are intuitive.

---

*Document generated June 2025 to accompany sprint-15 changes.* 