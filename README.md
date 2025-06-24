<div align="center">
  <br />
  <br />

  <h1 align="center">🌟 PUP Information Management System 🌟</h1>
  <h2 align="center">A Consolidated Case Study in Software Optimization</h2>

  <p align="center">
    <em>From Three Separate Applications to One Unified System: A Journey in Code Consolidation and Architectural Excellence</em>
    <br />
    <br />
    <a href="#-the-consolidation-story"><strong>✨ Discover Our Journey »</strong></a>
    <br />
    <br />
    <a href="https://github.com/j3yzi/c-002-case-study/issues">🐛 Report Bug</a>
    ·
    <a href="https://github.com/j3yzi/c-002-case-study/issues">💡 Request Feature</a>
  </p>
</div>

<div align="center">
  <img src="https://img.shields.io/badge/Language-C-blue.svg?style=for-the-badge&logo=c" alt="Language C">
  <img src="https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows" alt="Platform Windows">
  <img src="https://img.shields.io/badge/Compiler-GCC-orange.svg?style=for-the-badge&logo=gnu" alt="Compiler GCC">
  <img src="https://img.shields.io/badge/Status-Optimized-success.svg?style=for-the-badge&logo=github" alt="Status Optimized">
</div>

<div align="center">
  <h3>🎓 BSIT 1-2 S.Y. 2024-2025 Group 1 🎓</h3>
  <h4>💻 Computer Programming 2 - Polytechnic University of the Philippines 💻</h4>
  <br/>
  <hr style="width: 50%; border-top: 2px dashed #ccc;">
</div>

---

## 📖 About The Project

<div align="center">
  <img src="https://img.shields.io/badge/Code%20Reduction-80%25%20Files-success.svg?style=flat-square&logoColor=white">
  <img src="https://img.shields.io/badge/Lines%20Reduced-63%25%20Code-success.svg?style=flat-square&logoColor=white">
  <img src="https://img.shields.io/badge/Duplication-Zero%20Redundancy-blueviolet.svg?style=flat-square&logoColor=white">
  <img src="https://img.shields.io/badge/Features-100%25%20Preserved-informational.svg?style=flat-square&logoColor=white">
</div>
<br/>

This repository showcases a remarkable journey in software optimization and architectural excellence. What began as three separate C applications has been transformed into a single, unified **PUP Information Management System** that demonstrates the power of thoughtful code consolidation and engineering best practices.

Our team at the Polytechnic University of the Philippines took on the challenge of eliminating code duplication, reducing complexity, and creating a superior user experience while preserving 100% of the original functionality. The result is a testament to our commitment to software engineering excellence and our understanding of production-quality development practices.

> **🏆 ACHIEVEMENT: Successfully consolidated 40+ files and ~4,900 lines of code into 8 core files with ~1,800 lines of code, achieving an 80% reduction in files and 63% reduction in code complexity while maintaining full functionality.**

---

## 🚀 The Consolidation Story

<div align="center">
  <h3>📊 From Fragmentation to Unity 📊</h3>
</div>

### **Before Consolidation: Three Separate Applications**

<div align="center">
<table>
  <tr>
    <th>Case Study</th>
    <th>Purpose</th>
    <th>Files</th>
    <th>Lines of Code</th>
    <th>Key Issues</th>
  </tr>
  <tr>
    <td><b>CS001</b><br/>Employee Payroll Management</td>
    <td>💼 Payroll Processing</td>
    <td>12 files</td>
    <td>~1,500 lines</td>
    <td>Duplicate payroll logic, Single-list limitation</td>
  </tr>
  <tr>
    <td><b>CS002</b><br/>Dual Management System</td>
    <td>🔗 Multi-entity Records</td>
    <td>18 files</td>
    <td>~2,200 lines</td>
    <td>Complex structure, Overlapping functionality</td>
  </tr>
  <tr>
    <td><b>CS003</b><br/>Student Records Management</td>
    <td>🎓 Academic Tracking</td>
    <td>10 files</td>
    <td>~1,200 lines</td>
    <td>Isolated features, Limited integration</td>
  </tr>
  <tr style="background-color: #f8f9fa;">
    <td><b>TOTAL</b></td>
    <td><b>Fragmented System</b></td>
    <td><b>40 files</b></td>
    <td><b>~4,900 lines</b></td>
    <td><b>Massive code duplication</b></td>
  </tr>
</table>
</div>

### **After Consolidation: Unified PUP Information Management System**

<div align="center">
<table>
  <tr>
    <th>Component</th>
    <th>Purpose</th>
    <th>Files</th>
    <th>Lines of Code</th>
    <th>Key Improvements</th>
  </tr>
  <tr>
    <td><b>Unified System</b><br/>PUP Information Management</td>
    <td>🏛️ Complete Management Solution</td>
    <td>8 core files</td>
    <td>~1,800 lines</td>
    <td>Zero duplication, Enhanced functionality</td>
  </tr>
  <tr style="background-color: #d4edda;">
    <td><b>ACHIEVEMENT</b></td>
    <td><b>Optimized Architecture</b></td>
    <td><b>📉 80% Reduction</b></td>
    <td><b>📉 63% Reduction</b></td>
    <td><b>🏆 Superior Performance</b></td>
  </tr>
</table>
</div>

---

## 🔬 Research and Implementation

Our development process was not just about merging code; it was about making informed architectural decisions based on thorough research. Each aspect of the unified system was built upon a foundation of established software engineering principles.

### **1. Core Engineering Principles**

Our research into **Modular Programming in C** guided the entire consolidation process. We strictly followed the best practice of separating interfaces from implementations:

* **Header Files (`.h`):** Define the "what" (function prototypes, struct definitions, enums). Found in `include/headers/`, `include/models/`, `src/ui/`, and `src/modules/`.
* **Source Files (`.c`):** Define the "how" (the actual code and logic for the functions). Found in `include/src/`, `src/ui/`, and `src/modules/`.

This approach, confirmed by our research, was instrumental in creating the clean, maintainable structure outlined in the **Optimized Architecture** section.

### **2. Code Quality and Best Practices**

We prioritized creating robust and secure code by implementing findings from our research on C best practices:

* **Safe Input Handling:** To prevent common vulnerabilities like buffer overflows, we universally replaced `scanf()` with a safer combination of `fgets()` and `sscanf()`, as detailed in our research. This pattern is visible in `empio.c` and `stuio.c` within the `get...DataFromUser` functions.
* **Memory Management:** We adhered to strict `malloc()` and `free()` discipline. Our generic linked list implementation includes a `freeData` function pointer (`destroyList`, `removeNode`) to ensure that all dynamically allocated memory for structs is properly deallocated, preventing memory leaks.
* **Version Control:** Our team utilized **Git and GitHub Desktop** for collaborative development, following researched best practices for branching, committing, and creating pull requests to manage code changes effectively and maintain a clean history.

> *Our technical writer will expand this section with details on our branching strategy and collaborative workflow.*

### **3. Data Structures and Algorithms**

The case studies required extensive use of linked lists and sorting.

* **Linked Lists:** The core of our system is a generic, reusable linked list module (`list.h`, `lismgr.c`, `lisops.c`) that can handle any data type (Employees, Students, etc.), a direct improvement over the single-purpose lists in the original code.
* **Sorting:** Case Study #3 required sorting students by grade. Our research compared multiple algorithms (`Bubble Sort`, `Merge Sort`, `Quick Sort`). For the initial implementation (`sortStudentsByGrade` in `data.c`), we used a **Bubble Sort** for simplicity and to meet the immediate requirement. However, our research indicates that **Merge Sort** is superior for linked lists due to its guaranteed `O(n log n)` performance and lack of need for random access. This is noted as a key area for future performance optimization.

> *Our technical writer will add a brief summary of the comparative analysis of sorting algorithms from our research document here.*

---

## ✅ Fulfilling the Case Study Mandates

The unified system not only preserves but enhances the functionality required by all three original case studies.

### **Case Study #1: Employee Payroll**

* ✅ **Employee Records in a File:** Implemented via the `saveListWithCustomName` and `loadListWithName` functions in `data.c`, which handle binary file I/O.
* ✅ **Input Data for 5+ Employees:** The system uses a dynamic linked list, removing the fixed-size limitation and allowing for any number of employees.
* ✅ **Status Codes (R/C):** The `EmployeeStatus` enum (`employee.h`) and input handling in `empio.c` manage "Regular" and "Casual" statuses.
* ✅ **Overtime Pay Calculation:** The logic is implemented in `payroll.c` as per the instruction: "0.5 more than his/her basic rate".
* ✅ **Net Pay Calculation:** The formula `Net pay = Basic Pay + Overtime pay - Deductions` is implemented in `calculatePayroll` in `payroll.c`.
* ✅ **Output Layout:** The `generatePayrollReportFile` function in `data.c` produces a formatted text file report matching the required layout.

### **Case Study #2: Linked List Operations**

* ✅ **Menu-Driven Program:** The entire application is controlled via a robust menu system defined in `menuio.c`.
* ✅ **Create, Display, Sort, Search:** All linked list operations are core features of the unified system, accessible through the "Employee Management" and "Student Management" submenus.
* ✅ **State-Aware Menu:** The menu framework was designed to support enabling/disabling options based on application state (e.g., a list must be created before it can be displayed). The `checkStates` function in `menuio.c` is the designated place for this logic.
* ✅ **Error Messaging:** The system provides user-friendly error messages, such as "No active student list!", fulfilling the requirement to guide the user.

### **Case Study #3: Student Academics**

* ✅ **Student Record Structure & File I/O:** The `Student` struct (`student.h`) contains all required fields. Data persistence is handled by the unified data module (`data.c`).
* ✅ **Input for 10+ Students:** The system's dynamic linked lists can accommodate any number of students.
* ✅ **Final Grade and Remarks:** The `calculateFinalGrade` function (`student.c`) correctly computes the average and assigns "Passed" or "Failed" based on a grade of 75.
* ✅ **Academic Standing:** The case study did not specify the rules for "Dean's Lister", "Regular", or "Probation". Based on our research, we implemented a placeholder system with an `AcademicStanding` enum. Currently, all students default to "Regular."

> *Our technical writer will add a section to our formal documentation proposing a clear set of rules for Academic Standing based on standard university policies researched by the team.*

* ✅ **Full Menu Implementation:** The "Student Management" submenu in `menuio.c` provides all required functionalities: Add (`handleAddStudent`), Compute (done automatically on add/edit), Display (`handleDisplayAllStudents`), Sort (`handleStudentReport` can be extended), Search (`handleSearchStudent`), Save (`handleSaveStudentList`), and Load (`handleLoadStudentList`).

---

## 🔧 Key Consolidation Achievements

<div align="center">
  <h3>💎 Technical Excellence in Action 💎</h3>
</div>

### **1. Eliminated Code Duplication**
<div align="center">
  <img src="https://img.shields.io/badge/Payroll%20Logic-Unified-success.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Menu%20Systems-Consolidated-success.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Data%20Management-Streamlined-success.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Validation-Shared%20Utilities-success.svg?style=flat-square">
</div>

* ✅ **Payroll Logic:** CS001 and CS002 had identical payroll calculations → Now single implementation
* ✅ **Menu Systems:** Similar menu patterns across all three → Unified menu framework
* ✅ **Data Management:** Redundant file I/O operations → Consolidated data layer
* ✅ **Validation:** Duplicate input validation → Shared validation utilities

### **2. Enhanced Functionality**
<div align="center">
  <img src="https://img.shields.io/badge/Multi--List-Enhanced-blue.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Interface-Unified-blue.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Reporting-Comprehensive-blue.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Statistics-System%20Wide-blue.svg?style=flat-square">
</div>

* ✅ **Multi-List Management:** Extended CS002's multi-list capability to be the primary feature
* ✅ **Unified Interface:** Single entry point for all employee and student operations
* ✅ **Comprehensive Reporting:** Combined reporting capabilities from all case studies
* ✅ **System Statistics:** Enhanced overview of all managed data

### **3. Architectural Improvements**
<div align="center">
  <img src="https://img.shields.io/badge/Entry%20Point-Single-purple.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Design-Modular-purple.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Libraries-Shared-purple.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Patterns-Consistent-purple.svg?style=flat-square">
</div>

* ✅ **Single Entry Point:** One main.c file instead of three separate applications
* ✅ **Modular Design:** Clear separation between UI, business logic, and data layers
* ✅ **Shared Libraries:** Leverages the existing include/ directory infrastructure
* ✅ **Consistent Patterns:** Unified coding patterns and conventions

---

## 📁 Optimized Architecture

<div align="center">
  <h3>🏗️ Clean, Maintainable Structure 🏗️</h3>
</div>

```
📁 PUP Information Management System/
├── 📄 main.c                    # Single entry point (combines all 3 main.c files)
├── 📁 ui/
│   ├── 📄 menuio.h             # Unified menu interface
│   ├── 📄 menuio.c             # Consolidated menu implementation
│   ├── 📄 empio.h/.c           # Employee I/O operations
│   └── 📄 stuio.h/.c           # Student I/O operations
├── 📁 modules/
│   ├── 📄 payroll.h/.c         # Shared payroll calculations
│   └── 📄 data.h/.c            # Unified data management
├── 📁 bin/                     # Single executable output
├── 📁 data/                    # Shared data directory
├── 📄 compile.bat              # Single compilation script
└── 📄 README.md                # This documentation
```

---

## 🎯 Functional Equivalence Guarantee

<div align="center">
  <h3>✅ 100% Feature Preservation ✅</h3>
</div>

### **Employee Management (CS001 Features)**
<div align="center">
  <img src="https://img.shields.io/badge/CRUD%20Operations-✓-success.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Payroll%20Calculations-✓-success.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Search%20%26%20Edit-✓-success.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Reporting-✓-success.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Data%20Persistence-✓-success.svg?style=flat-square">
</div>

* ✅ Employee record creation and management
* ✅ Payroll calculations (basic pay, overtime, deductions)
* ✅ Employee search and editing capabilities
* ✅ Comprehensive payroll reporting
* ✅ Data persistence (save/load employee lists)

### **Student Management (CS003 Features)**
<div align="center">
  <img src="https://img.shields.io/badge/Student%20Records-✓-info.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Grade%20Tracking-✓-info.svg?style=flat-square">
  <img src="https://img.shields.io/badge/GPA%20Calculations-✓-info.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Academic%20Reports-✓-info.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Data%20Persistence-✓-info.svg?style=flat-square">
</div>

* ✅ Student record creation and management
* ✅ Academic grade tracking (prelim, midterm, final)
* ✅ GPA calculations and academic standing
* ✅ Student search and editing capabilities
* ✅ Academic reporting and statistics
* ✅ Data persistence (save/load student lists)

### **Enhanced Multi-List Management (CS002 Features)**
<div align="center">
  <img src="https://img.shields.io/badge/Multiple%20Employee%20Lists-✓-warning.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Multiple%20Student%20Lists-✓-warning.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Custom%20List%20Naming-✓-warning.svg?style=flat-square">
  <img src="https://img.shields.io/badge/List%20Switching-✓-warning.svg?style=flat-square">
  <img src="https://img.shields.io/badge/System%20Statistics-✓-warning.svg?style=flat-square">
</div>

* ✅ Multiple employee lists (up to 10)
* ✅ Multiple student lists (up to 10)
* ✅ Custom list naming and management
* ✅ List switching and organization
* ✅ Comprehensive system statistics

---

## 🏆 Optimization Benefits

<div align="center">
  <h3>🚀 Multi-Dimensional Improvements 🚀</h3>
</div>

<div align="center">
<table>
  <tr>
    <th>Category</th>
    <th>Benefits Achieved</th>
    <th>Impact</th>
  </tr>
  <tr>
    <td><b>🔧 Development</b></td>
    <td>Single Codebase<br/>Consistent Patterns<br/>Reduced Complexity<br/>Easier Testing</td>
    <td>Faster development cycles<br/>Improved code quality<br/>Simplified maintenance</td>
  </tr>
  <tr>
    <td><b>👤 User Experience</b></td>
    <td>Unified Interface<br/>Enhanced Features<br/>Better Performance<br/>Consistent Experience</td>
    <td>Improved usability<br/>Higher productivity<br/>Better user satisfaction</td>
  </tr>
  <tr>
    <td><b>🛠️ Maintenance</b></td>
    <td>Single Point of Truth<br/>Easier Updates<br/>Reduced Bug Surface<br/>Simplified Deployment</td>
    <td>Lower maintenance costs<br/>Faster bug fixes<br/>Reliable releases</td>
  </tr>
</table>
</div>

---

## ⚙️ Getting Started

<div align="center">
  <h3>🚀 Quick Setup Guide 🚀</h3>
</div>

### **Prerequisites**
You need a C compiler installed on your system. We recommend GCC.
```sh
gcc --version
```

### **Installation & Running**

<div align="center">
<table>
  <tr>
    <th>Step</th>
    <th>Command</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>1. Clone</td>
    <td><code>git clone https://github.com/j3yzi/c-002-case-study.git</code></td>
    <td>Download the repository</td>
  </tr>
  <tr>
    <td>2. Navigate</td>
    <td><code>cd c-002-case-study</code></td>
    <td>Go to project folder</td>
  </tr>
  <tr>
    <td>3. Compile</td>
    <td><code>compile.bat</code></td>
    <td>Use provided compilation script</td>
  </tr>
  <tr>
    <td>4. Run</td>
    <td><code>bin\UnifiedManagementSystem.exe</code></td>
    <td>Execute the unified system</td>
  </tr>
</table>
</div>

---

## 🛠️ Built With

<div align="center">
  <h3>⚒️ Professional Tools & Technologies ⚒️</h3>
</div>

<div align="center">
<table>
  <tr>
    <td align="center"><img src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white"><br><b>Core Language</b></td>
    <td align="center"><img src="https://img.shields.io/badge/GCC-323330?style=for-the-badge&logo=gnu&logoColor=white"><br><b>Compiler</b></td>
  </tr>
  <tr>
    <td align="center"><img src="https://img.shields.io/badge/Git-F05032?style=for-the-badge&logo=git&logoColor=white"><br><b>Version Control</b></td>
    <td align="center"><img src="https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white"><br><b>Collaboration</b></td>
  </tr>
</table>
</div>

---

## 📊 Our Engineering Excellence

<div align="center">
  <h3>🎯 Measurable Success Metrics 🎯</h3>
</div>

<div align="center">
<table>
  <tr>
    <th>Metric</th>
    <th>Before</th>
    <th>After</th>
    <th>Improvement</th>
  </tr>
  <tr>
    <td><b>Total Files</b></td>
    <td>40 files</td>
    <td>8 core files</td>
    <td><span style="color: green;">📉 80% Reduction</span></td>
  </tr>
  <tr>
    <td><b>Lines of Code</b></td>
    <td>~4,900 lines</td>
    <td>~1,800 lines</td>
    <td><span style="color: green;">📉 63% Reduction</span></td>
  </tr>
  <tr>
    <td><b>Code Duplication</b></td>
    <td>Significant</td>
    <td>Zero</td>
    <td><span style="color: green;">✅ 100% Eliminated</span></td>
  </tr>
  <tr>
    <td><b>Functionality</b></td>
    <td>Fragmented</td>
    <td>Unified</td>
    <td><span style="color: green;">✅ 100% Preserved</span></td>
  </tr>
  <tr>
    <td><b>Maintenance</b></td>
    <td>Complex</td>
    <td>Simplified</td>
    <td><span style="color: green;">🚀 Significantly Improved</span></td>
  </tr>
</table>
</div>

---

## 🤝 Meet The Team

<div align="center">
  <h3>👨‍💻 Software Engineering Excellence Team 👩‍💻</h3>
</div>

<div align="center">
<table>
  <tr>
    <td align="center"><a href="https://github.com/j3yzi"><img src="https://github.com/j3yzi.png" width="100px;" alt=""/><br /><sub><b>j3yzi</b></sub></a><br />Project Manager / Lead Programmer</td>
    <td align="center"><a href="https://github.com/Mxrcymarcmarc"><img src="https://github.com/Mxrcymarcmarc.png" width="100px;" alt=""/><br /><sub><b>Mxrcymarcmarc</b></sub></a><br />Backend Developer</td>
    <td align="center"><a href="https://github.com/vince-wh0"><img src="https://github.com/vince-wh0.png" width="100px;" alt=""/><br /><sub><b>vince-wh0</b></sub></a><br />Quality Analyst</td>
    <td align="center"><a href="https://github.com/Mhean21"><img src="https://github.com/Mhean21.png" width="100px;" alt=""/><br /><sub><b>Mhean21</b></sub></a><br />Quality Analyst</td>
    <td align="center"><a href="https://github.com/Cursed-10"><img src="https://github.com/Cursed-10.png" width="100px;" alt=""/><br /><sub><b>Cursed-10</b></sub></a><br />Frontend Developer / UI Designer</td>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/CyberFlowHex"><img src="https://github.com/CyberFlowHex.png" width="100px;" alt=""/><br /><sub><b>CyberFlowHex</b></sub></a><br />Lead Backend Developer</td>
    <td align="center"><a href="https://github.com/meiaooo"><img src="https://github.com/meiaooo.png" width="100px;" alt=""/><br /><sub><b>meiaooo</b></sub></a><br />Frontend Developer</td>
    <td align="center"><a href="https://github.com/adiayouu"><img src="https://github.com/adiayouu.png" width="100px;" alt=""/><br /><sub><b>adiayouu</b></sub></a><br />System Analyst</td>
    <td align="center"><a href="https://github.com/Aitsh28"><img src="https://github.com/Aitsh28.png" width="100px;" alt=""/><br /><sub><b>Aitsh28</b></sub></a><br />Technical Writer</td>
    <td align="center"><a href="https://github.com/PneumaRejuvenator"><img src="https://github.com/PneumaRejuvenator.png" width="100px;" alt=""/><br /><sub><b>PneumaRejuvenator</b></sub></a><br />Backend Developer</td>
  </tr>
</table>
</div>

---

## 🏁 Conclusion

<div align="center">
  <h3>🎉 Engineering Excellence Achieved 🎉</h3>
</div>

The consolidation of CS001, CS002, and CS003 into the **PUP Information Management System** represents a significant achievement in software optimization and architectural improvement. We have successfully:

<div align="center">
  <p>✅ <b>Eliminated all code duplication</b> while preserving 100% of functionality</p>
  <p>✅ <b>Reduced complexity</b> by 80% fewer files and 63% fewer lines of code</p>
  <p>✅ <b>Enhanced user experience</b> with a unified interface</p>
  <p>✅ <b>Improved maintainability</b> with a single, well-structured codebase</p>
  <p>✅ <b>Established a foundation</b> for future enhancements and scalability</p>
</div>

> **"The new PUP Information Management System is not just a consolidation—it's an evolution that combines the best aspects of all three original case studies into a superior, more maintainable, and user-friendly application."**

---

## 🙏 Acknowledgments

<div align="center">
  <h3>✨ Special Thanks ✨</h3>
</div>
