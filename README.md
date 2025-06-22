<div align="center">
  <br />
  <br />

  <h1 align="center">🌟 Foundations in Programming: A Case Study Collection in C 🌟</h1>

  <p align="center">
    <em>A comprehensive university project showcasing three distinct C applications, built with a focus on modularity, best practices, and in-depth software development principles.</em>
    <br />
    <br />
    <a href="#-the-case-studies"><strong>✨ Explore the Projects »</strong></a>
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
  <img src="https://img.shields.io/badge/Status-In%20Development-yellow.svg?style=for-the-badge&logo=github" alt="Status In Development">
</div>

<div align="center">
  <h3>🎓 BSIT 1-2 S.Y. 2024-2025 Group 1 🎓</h3>
  <h4>💻 Computer Programming 2 💻</h4>
  <br/>
  <hr style="width: 50%; border-top: 2px dashed #ccc;">
</div>

---

## 📖 About The Project

<div align="center">
  <img src="https://img.shields.io/badge/Code%20Quality-Professional-success.svg?style=flat-square&logoColor=white">
  <img src="https://img.shields.io/badge/Documentation-Comprehensive-informational.svg?style=flat-square&logoColor=white">
  <img src="https://img.shields.io/badge/Best%20Practices-Implemented-blueviolet.svg?style=flat-square&logoColor=white">
</div>
<br/>

This repository is the culmination of our case study project for the BSIT program at Polytechnic University of the Philippines. Our goal was not just to fulfill the academic requirements but to create a comprehensive portfolio piece that demonstrates a professional software development workflow.

We went beyond the code to research and implement key industry practices, including:
* 🔄 **A structured Software Development Life Cycle (SDLC)** 
* 🧩 **Modular programming** for maintainable and reusable code 
* 🌿 **Version control with Git & GitHub** for effective team collaboration 
* 🔒 **A strong emphasis on code safety and best practices** 

This project is a testament to our team's dedication to quality engineering and our passion for building robust software from the ground up.

> **⚠️ NOTE: This project is currently in development.** Features and documentation are being actively worked on and expanded.

---

## 🚀 The Case Studies

<div align="center">
  <h3>📊 Three Powerful C Applications 📊</h3>
</div>

This repository contains three distinct command-line applications, each designed to solve a different problem.

### **CS-001: Employee-Payroll Management System** 💼
<div align="center">
  <img src="https://img.shields.io/badge/Type-File%20Handling-green.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Focus-Data%20Processing-blue.svg?style=flat-square">
</div>

A file-handling application to manage employee records and calculate payroll.
* ✅ **Features:** Create, read, and process employee data from files.
* 🧮 **Core Logic:** Calculates basic pay, overtime pay, and deductions to determine net pay.
* 📋 **Data Structure:** `Employee` struct with fields for number, name, status, pay rates, and more.
* 🔍 **Status:** Most complete case study with detailed documentation.

<details>
  <summary><b>🔎 Click to see more details</b></summary>
  <p>This system allows HR personnel to efficiently manage employee information and automate payroll calculations, saving time and reducing errors in the payment process.</p>
  <p>Key capabilities include employee data management, time tracking, payment calculations, and generating reports.</p>
</details>

### **CS-002: Record Management System** 🔗
<div align="center">
  <img src="https://img.shields.io/badge/Type-Data%20Management-green.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Focus-Multi--entity%20Records-blue.svg?style=flat-square">
</div>

A comprehensive system for managing records of employees, professors, students, and system users.
* ✅ **Features:** Create, display, update, and search for different types of records.
* 🖥️ **User Experience:** Interactive menu that guides the user through various record management operations.
* 🗃️ **Data Handling:** Implements robust data structures for storing and managing different record types.
* 🔍 **Status:** Under active development.

<details>
  <summary><b>🔎 Click to see more details</b></summary>
  <p>This versatile record management system provides a unified interface for handling various entity types, allowing for consistent data handling across different record categories.</p>
  <p>The system implements CRUD operations (Create, Read, Update, Delete) for all record types with specialized fields for each entity category.</p>
</details>

### **CS-003: PUP - Student Information System (Console Edition)** 🎓
<div align="center">
  <img src="https://img.shields.io/badge/Type-Academic%20Management-green.svg?style=flat-square">
  <img src="https://img.shields.io/badge/Focus-Student%20Records-blue.svg?style=flat-square">
</div>

A console-based student information management system tailored for PUP.
* ✅ **Features:** Manage student profiles, academic records, and determine academic standing.
* 💾 **File I/O:** Save and load student records to and from a file.
* 🔍 **Data Operations:** Includes menu options for sorting records by various criteria and searching by student number.
* 🚧 **Status:** Initial implementation complete, enhancements in progress.

<details>
  <summary><b>🔎 Click to see more details</b></summary>
  <p>This system helps university administrators track student enrollment, academic performance, and generate reports for academic planning and evaluation.</p>
  <p>Features include student profile management, grade recording and calculation, academic standing determination, and customized reporting options.</p>
</details>

---

## 🧱 Project Architecture

<div align="center">
  <h3>📐 Thoughtfully Structured for Maintainability 📐</h3>
</div>

This project uses a carefully designed architecture with shared components:

```
📁 project/
├── 📁 CS001/      # Employee-Payroll Management System
├── 📁 CS002/      # Record Management System
├── 📁 CS003/      # PUP - Student Information System
├── 📁 include/    # Shared libraries and utilities
│   ├── 📁 headers/    # Common header files
│   ├── 📁 models/     # Data structures and models
│   └── 📁 src/        # Implementation of shared functionality
├── 📁 data/       # Data storage for all applications
└── 📁 output/     # Output files generated by the applications
```

Each case study follows a similar internal structure:

```
📁 CS00x/
├── 📄 compile.bat     # Compilation script
├── 📁 src/
│   ├── 📄 main.c      # Entry point
│   ├── 📁 include/    # Project-specific headers
│   ├── 📁 modules/    # Business logic modules
│   └── 📁 ui/         # User interface components
└── 📁 bin/            # Compiled executables
```

---

## 🛠️ Built With

<div align="center">
  <h3>⚒️ Professional Tools & Technologies ⚒️</h3>
</div>

This project was built using C and a disciplined development workflow.

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

* **Project Management:** Agile Methodology with Scrum practices 

---

## 🔬 Our Development Philosophy

<div align="center">
  <h3>🧠 A Commitment to Excellence 🧠</h3>
</div>

The quality of our project is built on a foundation of research and a commitment to best practices.

* **💫 Software Development Life Cycle (SDLC):** We adopted an Agile model to facilitate flexibility, iterative development, and continuous feedback, which is ideal for projects where requirements can evolve.

* **🧩 Modular Programming:** We structured our code into reusable modules with clear separation of concerns, using `.h` header files for interfaces and `.c` source files for implementation. This makes the code easier to maintain, debug, and collaborate on.

* **🔒 Safety & Robustness:** We prioritized writing safe and robust code by:
    * **🛡️ Avoiding Dangerous Functions:** Using `fgets()` combined with `sscanf()` to prevent buffer overflows common with `scanf()`.
    * **🧹 Memory Management:** Following a strict discipline of pairing `malloc()` with `free()` to prevent memory leaks.
    * **🔍 Input Validation:** Ensuring all user input is validated before being processed to maintain program stability.

---

## ⚙️ Getting Started

<div align="center">
  <h3>🚀 Quick Setup Guide 🚀</h3>
</div>

To get a local copy up and running, follow these simple steps.

### **Prerequisites**
You need a C compiler installed on your system. We recommend GCC.
* **How to check if you have GCC:**
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
    <td><code>cd c-002-case-study/CS001</code></td>
    <td>Go to desired project folder</td>
  </tr>
  <tr>
    <td>3. Compile</td>
    <td><code>compile.bat</code></td>
    <td>Use provided compilation script</td>
  </tr>
  <tr>
    <td>4. Run</td>
    <td><code>bin\project_payroll.exe</code></td>
    <td>Execute the compiled program</td>
  </tr>
</table>
</div>

Alternatively, you can compile manually:

```sh
gcc src/main.c src/modules/data.c src/modules/payroll.c src/ui/empio.c src/ui/menuio.c -o bin/project_payroll.exe
```

---

## 📸 Screenshots

<div align="center">
  <h3>🖼️ Visual Showcase 🖼️</h3>
  <p><i>(Coming soon - Screenshots will be added as the projects are completed)</i></p>
  
  <table>
    <tr>
      <td><img src="https://via.placeholder.com/320x180?text=CS-001+Screenshot+Coming+Soon" alt="CS-001 Preview"></td>
      <td><img src="https://via.placeholder.com/320x180?text=CS-002+Screenshot+Coming+Soon" alt="CS-002 Preview"></td>
      <td><img src="https://via.placeholder.com/320x180?text=CS-003+Screenshot+Coming+Soon" alt="CS-003 Preview"></td>
    </tr>
    <tr>
      <td align="center"><b>CS-001: Payroll System</b></td>
      <td align="center"><b>CS-002: Record Management</b></td>
      <td align="center"><b>CS-003: Student Information</b></td>
    </tr>
  </table>
</div>

---

## 🔄 Development Status

<div align="center">
  <h3>📊 Project Progress Tracker 📊</h3>
</div>

| Case Study | Status | Progress | Next Steps |
|------------|:------:|:--------:|------------|
| CS-001: Employee-Payroll Management System | ⚙️ In Progress | ![70%](https://progress-bar.dev/70) | Complete validation, add report generation |
| CS-002: Record Management System | ⚙️ In Progress | ![50%](https://progress-bar.dev/50) | Implement additional record types, enhance UI |
| CS-003: PUP - Student Information System | ⚙️ In Progress | ![40%](https://progress-bar.dev/40) | Add file I/O, implement search functionality |
| Shared Code | ⚙️ In Progress | ![60%](https://progress-bar.dev/60) | Optimize memory usage, enhance error handling |

---

## 🤝 Meet The Team

<div align="center">
  <h3>👨‍💻 Our Amazing Team 👩‍💻</h3>
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
    <td align="center"><a href="https://github.com/adiayouu"><img src="https://github.com/adiayouu.png" width="100px;" alt=""/><br /><sub><b>adiayouu</b></sub></a><br />Researcher</td>
    <td align="center"><a href="https://github.com/Aitsh28"><img src="https://github.com/Aitsh28.png" width="100px;" alt=""/><br /><sub><b>Aitsh28</b></sub></a><br />Technical Writer</td>
    <td align="center"><a href="https://github.com/PneumRejuvenator"><img src="https://github.com/PneumRejuvenator.png" width="100px;" alt=""/><br /><sub><b>PneumRejuvenator</b></sub></a><br />Backend Developer</td>
  </tr>
</table>
</div>

---

## 🙏 Acknowledgments

<div align="center">
  <h3>✨ Special Thanks ✨</h3>
</div>

<div align="center">
  <p>We extend our heartfelt gratitude to:</p>
  
  <p>👨‍🏫 Our Professor for guidance and support</p>
  <p>🏫 Polytechnic University of the Philippines</p>
  <p>🌐 The open-source community for providing invaluable tools and documentation</p>
  
  <hr style="width: 70%; border-top: 2px dashed #ccc;">
  <p>💻 <b>Made with ❤️ by BSIT 1-2 Group 1</b> 💻</p>
</div>