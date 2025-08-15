# Student Grading System

A terminal-based management system implemented in C for tracking and analyzing student performance.

## Project Overview

This Student Grading System allows teachers and administrators to manage student records, compute averages and letter grades, and generate statistical reports. The system demonstrates proficiency in C programming fundamentals including functions, conditional logic, loops, arrays, and text file handling.

## Prerequisites

- GCC compiler (MinGW for Windows, or gcc on Linux/macOS)
- Terminal/Command Prompt access
- No additional libraries required - uses only standard C libraries

## Installation and Compilation

### Windows (PowerShell or Command Prompt)
```
gcc -o student_grading_system student_grading_system.c
```

### Linux/macOS (Terminal)
```
gcc -o student_grading_system student_grading_system.c
```

## Running the Program

### Windows
```
.\student_grading_system
```

### Linux/macOS
```
./student_grading_system
```

## Key Features

1. **Student Management**
   - Add new student records with ID, name, and subject marks
   - List all active students with formatted display
   - Search for specific students by ID
   - Update existing student information
   - Delete students (logical deletion)

2. **Grade Processing**
   - Calculate student average from subject marks
   - Assign letter grades based on average (A ≥85, B ≥70, C ≥55, D ≥40, F <40)

3. **Reporting**
   - Generate class summary reports
   - Display class average
   - Show highest and lowest performing students
   - Present grade distribution statistics

4. **Data Persistence**
   - Load student records from file on startup
   - Save student data to file on exit
   - Generate optional class reports to separate file

## File Structure

- **student_grading_system.c** - Main source code file
- **students.txt** - Data storage file (pipe-delimited format)
- **class_report.txt** - Generated report file (when requested)

## Data Format

Student records are stored in the following format:
```
<id>|<name>|<marks>|<average>|<grade>|<active>
```

Example:
```
2021001|Alice Perera|85,78,90|84.33|A|1
```
