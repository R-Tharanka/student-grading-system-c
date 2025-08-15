#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 200    // Maximum number of students the system can store
#define NUM_SUBJECTS 3      // Default number of subjects per student
#define MAX_ID_LENGTH 20    // Maximum length for student ID
#define MAX_NAME_LENGTH 50  // Maximum length for student name
#define DATA_FILENAME "students.txt"  // Default filename for student data
#define REPORT_FILENAME "class_report.txt"  // Default filename for class report

// Student structure definition
typedef struct {
    char id[MAX_ID_LENGTH];
    char name[MAX_NAME_LENGTH];
    int marks[NUM_SUBJECTS];
    float average;
    char grade;
    int active;  // 1 = active, 0 = deleted
} Student;

// Function prototypes
void displayMenu();
void loadFromFile(const char *filename, Student arr[], int *count);
void saveToFile(const char *filename, Student arr[], int count);
void addStudent(Student arr[], int *count);
void listStudents(Student arr[], int count);
int findStudentIndexByID(Student arr[], int count, const char *id);
void updateStudent(Student arr[], int count);
void deleteStudent(Student arr[], int *count);
void searchStudent(Student arr[], int count);
float calculateAverage(int marks[], int n);
char calculateGrade(float avg);
void generateReport(Student arr[], int count);
void clearInputBuffer();
int getIntegerInput(int min, int max);
void waitForEnter();

int main() {
    Student students[MAX_STUDENTS];
    int studentCount = 0;
    int choice;
    
    // Load existing data from file
    loadFromFile(DATA_FILENAME, students, &studentCount);
    
    // Main program loop
    do {
        displayMenu();
        printf("Enter your choice: ");
        choice = getIntegerInput(1, 8);
        
        switch (choice) {
            case 1:
                addStudent(students, &studentCount);
                break;
            case 2:
                listStudents(students, studentCount);
                break;
            case 3:
                searchStudent(students, studentCount);
                break;
            case 4:
                updateStudent(students, studentCount);
                break;
            case 5:
                deleteStudent(students, &studentCount);
                break;
            case 6:
                generateReport(students, studentCount);
                break;
            case 7:
                saveToFile(DATA_FILENAME, students, studentCount);
                printf("\nData saved successfully to %s\n", DATA_FILENAME);
                waitForEnter();
                break;
            case 8:
                saveToFile(DATA_FILENAME, students, studentCount);
                printf("\nData saved to %s. Exiting program...\n", DATA_FILENAME);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);
    
    return 0;
}

// Display the main menu
void displayMenu() {
    system("cls || clear"); // Clear screen (works on Windows and Unix)
    
    printf("\n");
    printf("===============================================\n");
    printf("          STUDENT GRADING SYSTEM              \n");
    printf("===============================================\n");
    printf("\n");
    printf("1. Add Student\n");
    printf("2. List Students\n");
    printf("3. Search Student\n");
    printf("4. Update Student\n");
    printf("5. Delete Student\n");
    printf("6. Generate Class Report\n");
    printf("7. Save Data\n");
    printf("8. Exit\n");
    printf("\n");
}

// Load student data from file
void loadFromFile(const char *filename, Student arr[], int *count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Warning: Could not open file %s for reading.\n", filename);
        printf("Starting with empty data set.\n");
        waitForEnter();
        *count = 0;
        return;
    }
    
    *count = 0;
    char line[256];
    char *token;
    
    while (fgets(line, sizeof(line), file) != NULL && *count < MAX_STUDENTS) {
        // Remove newline character
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        // Parse student ID
        token = strtok(line, "|");
        if (token == NULL) continue;
        strncpy(arr[*count].id, token, MAX_ID_LENGTH - 1);
        arr[*count].id[MAX_ID_LENGTH - 1] = '\0';
        
        // Parse student name
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy(arr[*count].name, token, MAX_NAME_LENGTH - 1);
        arr[*count].name[MAX_NAME_LENGTH - 1] = '\0';
        
        // Parse student marks
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        
        char *markToken = strtok(token, ",");
        int i = 0;
        while (markToken != NULL && i < NUM_SUBJECTS) {
            arr[*count].marks[i] = atoi(markToken);
            markToken = strtok(NULL, ",");
            i++;
        }
        
        // Fill remaining marks with 0 if less than NUM_SUBJECTS were provided
        while (i < NUM_SUBJECTS) {
            arr[*count].marks[i] = 0;
            i++;
        }
        
        // Parse average
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        arr[*count].average = atof(token);
        
        // Parse grade
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        arr[*count].grade = token[0];
        
        // Parse active status
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        arr[*count].active = atoi(token);
        
        (*count)++;
    }
    
    fclose(file);
    printf("Successfully loaded %d student records from %s\n", *count, filename);
}

// Save student data to file
void saveToFile(const char *filename, Student arr[], int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s for writing.\n", filename);
        waitForEnter();
        return;
    }
    
    for (int i = 0; i < count; i++) {
        // Write student data to file in the required format
        fprintf(file, "%s|%s|", arr[i].id, arr[i].name);
        
        // Write marks separated by commas
        for (int j = 0; j < NUM_SUBJECTS; j++) {
            fprintf(file, "%d", arr[i].marks[j]);
            if (j < NUM_SUBJECTS - 1) {
                fprintf(file, ",");
            }
        }
        
        // Write average, grade, and active status
        fprintf(file, "|%.2f|%c|%d\n", arr[i].average, arr[i].grade, arr[i].active);
    }
    
    fclose(file);
}

// Add a new student record
void addStudent(Student arr[], int *count) {
    if (*count >= MAX_STUDENTS) {
        printf("\nError: Maximum number of students (%d) reached.\n", MAX_STUDENTS);
        waitForEnter();
        return;
    }
    
    system("cls || clear");
    printf("\n=== Add New Student ===\n\n");
    
    // Get student ID
    while (1) {
        printf("Enter Student ID: ");
        if (scanf("%19s", arr[*count].id) != 1) {
            printf("Error reading student ID. Try again.\n");
            clearInputBuffer();
            continue;
        }
        
        // Check if ID is empty
        if (strlen(arr[*count].id) == 0) {
            printf("Error: Student ID cannot be empty.\n");
            continue;
        }
        
        // Check for duplicate IDs
        if (findStudentIndexByID(arr, *count, arr[*count].id) != -1) {
            printf("Error: A student with this ID already exists.\n");
            continue;
        }
        
        break;
    }
    
    // Clear input buffer after reading ID
    clearInputBuffer();
    
    // Get student name
    printf("Enter Student Name: ");
    if (fgets(arr[*count].name, MAX_NAME_LENGTH, stdin) == NULL) {
        printf("Error reading name. Using 'Unknown'.\n");
        strcpy(arr[*count].name, "Unknown");
    } else {
        // Remove newline character if present
        size_t len = strlen(arr[*count].name);
        if (len > 0 && arr[*count].name[len - 1] == '\n') {
            arr[*count].name[len - 1] = '\0';
        }
    }
    
    // Get marks for each subject
    printf("\nEnter marks for %d subjects:\n", NUM_SUBJECTS);
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        printf("Enter mark for Subject %d (0-100): ", i + 1);
        arr[*count].marks[i] = getIntegerInput(0, 100);
    }
    
    // Calculate average and grade
    arr[*count].average = calculateAverage(arr[*count].marks, NUM_SUBJECTS);
    arr[*count].grade = calculateGrade(arr[*count].average);
    arr[*count].active = 1;  // Set as active
    
    printf("\nStudent added successfully.");
    printf("\nAverage: %.2f, Grade: %c\n", arr[*count].average, arr[*count].grade);
    
    (*count)++;
    waitForEnter();
}

// List all active students
void listStudents(Student arr[], int count) {
    system("cls || clear");
    printf("\n=== Student List ===\n\n");
    
    printf("%-15s %-25s %-10s %-6s\n", "ID", "Name", "Average", "Grade");
    printf("--------------------------------------------------------\n");
    
    int activeCount = 0;
    
    for (int i = 0; i < count; i++) {
        if (arr[i].active) {
            printf("%-15s %-25s %-10.2f %-6c\n", 
                   arr[i].id, arr[i].name, arr[i].average, arr[i].grade);
            activeCount++;
        }
    }
    
    if (activeCount == 0) {
        printf("No active students found.\n");
    } else {
        printf("--------------------------------------------------------\n");
        printf("Total: %d active students\n", activeCount);
    }
    
    waitForEnter();
}

// Find a student by ID, returns the index or -1 if not found
int findStudentIndexByID(Student arr[], int count, const char *id) {
    for (int i = 0; i < count; i++) {
        if (strcmp(arr[i].id, id) == 0 && arr[i].active) {
            return i;
        }
    }
    return -1;
}

// Update student information
void updateStudent(Student arr[], int count) {
    char id[MAX_ID_LENGTH];
    int index;
    
    system("cls || clear");
    printf("\n=== Update Student ===\n\n");
    
    if (count == 0) {
        printf("No students in the system.\n");
        waitForEnter();
        return;
    }
    
    printf("Enter Student ID to update: ");
    scanf("%19s", id);
    clearInputBuffer();
    
    index = findStudentIndexByID(arr, count, id);
    
    if (index == -1) {
        printf("Student with ID '%s' not found or inactive.\n", id);
        waitForEnter();
        return;
    }
    
    // Display current information
    printf("\nCurrent Information:\n");
    printf("ID: %s\n", arr[index].id);
    printf("Name: %s\n", arr[index].name);
    printf("Marks: ");
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        printf("%d", arr[index].marks[i]);
        if (i < NUM_SUBJECTS - 1) printf(", ");
    }
    printf("\nAverage: %.2f\n", arr[index].average);
    printf("Grade: %c\n", arr[index].grade);
    
    // Update name
    printf("\nEnter new name (press Enter to keep current): ");
    char newName[MAX_NAME_LENGTH];
    if (fgets(newName, MAX_NAME_LENGTH, stdin) != NULL) {
        // Remove newline character if present
        size_t len = strlen(newName);
        if (len > 0 && newName[len - 1] == '\n') {
            newName[len - 1] = '\0';
        }
        
        // If not empty, update the name
        if (strlen(newName) > 0) {
            strncpy(arr[index].name, newName, MAX_NAME_LENGTH - 1);
            arr[index].name[MAX_NAME_LENGTH - 1] = '\0';
        }
    }
    
    // Update marks
    printf("\nUpdate marks? (1 for Yes, 0 for No): ");
    int updateMarks = getIntegerInput(0, 1);
    
    if (updateMarks) {
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            printf("Enter new mark for Subject %d (0-100): ", i + 1);
            arr[index].marks[i] = getIntegerInput(0, 100);
        }
        
        // Recalculate average and grade
        arr[index].average = calculateAverage(arr[index].marks, NUM_SUBJECTS);
        arr[index].grade = calculateGrade(arr[index].average);
    }
    
    printf("\nStudent updated successfully.\n");
    printf("New Average: %.2f, New Grade: %c\n", arr[index].average, arr[index].grade);
    waitForEnter();
}

// Delete a student (mark as inactive)
void deleteStudent(Student arr[], int *count) {
    char id[MAX_ID_LENGTH];
    int index;
    
    system("cls || clear");
    printf("\n=== Delete Student ===\n\n");
    
    if (*count == 0) {
        printf("No students in the system.\n");
        waitForEnter();
        return;
    }
    
    printf("Enter Student ID to delete: ");
    scanf("%19s", id);
    clearInputBuffer();
    
    index = findStudentIndexByID(arr, *count, id);
    
    if (index == -1) {
        printf("Student with ID '%s' not found or already inactive.\n", id);
        waitForEnter();
        return;
    }
    
    // Display student information before deletion
    printf("\nStudent Information:\n");
    printf("ID: %s\n", arr[index].id);
    printf("Name: %s\n", arr[index].name);
    printf("Average: %.2f\n", arr[index].average);
    printf("Grade: %c\n", arr[index].grade);
    
    printf("\nAre you sure you want to delete this student? (1 for Yes, 0 for No): ");
    int confirm = getIntegerInput(0, 1);
    
    if (confirm) {
        // Logical deletion - mark as inactive
        arr[index].active = 0;
        printf("\nStudent has been marked as inactive.\n");
    } else {
        printf("\nDeletion cancelled.\n");
    }
    
    waitForEnter();
}

// Search for a student by ID and display details
void searchStudent(Student arr[], int count) {
    char id[MAX_ID_LENGTH];
    int index;
    
    system("cls || clear");
    printf("\n=== Search Student ===\n\n");
    
    if (count == 0) {
        printf("No students in the system.\n");
        waitForEnter();
        return;
    }
    
    printf("Enter Student ID to search: ");
    scanf("%19s", id);
    clearInputBuffer();
    
    index = findStudentIndexByID(arr, count, id);
    
    if (index == -1) {
        printf("Student with ID '%s' not found or inactive.\n", id);
        waitForEnter();
        return;
    }
    
    printf("\nStudent Details:\n");
    printf("ID: %s\n", arr[index].id);
    printf("Name: %s\n", arr[index].name);
    printf("Marks: ");
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        printf("%d", arr[index].marks[i]);
        if (i < NUM_SUBJECTS - 1) printf(", ");
    }
    printf("\nAverage: %.2f\n", arr[index].average);
    printf("Grade: %c\n", arr[index].grade);
    
    waitForEnter();
}

// Calculate the average of marks
float calculateAverage(int marks[], int n) {
    if (n <= 0) return 0.0f;
    
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += marks[i];
    }
    
    return (float)sum / n;
}

// Determine the grade based on average
char calculateGrade(float avg) {
    if (avg >= 85) return 'A';
    else if (avg >= 70) return 'B';
    else if (avg >= 55) return 'C';
    else if (avg >= 40) return 'D';
    else return 'F';
}

// Generate class report
void generateReport(Student arr[], int count) {
    system("cls || clear");
    printf("\n=== Class Report ===\n\n");
    
    int activeCount = 0;
    float totalAverage = 0.0f;
    float highestAvg = 0.0f;
    float lowestAvg = 100.0f;
    char highestID[MAX_ID_LENGTH] = "";
    char lowestID[MAX_ID_LENGTH] = "";
    int gradeCount[5] = {0}; // A, B, C, D, F counts
    
    // Calculate statistics
    for (int i = 0; i < count; i++) {
        if (arr[i].active) {
            activeCount++;
            totalAverage += arr[i].average;
            
            // Check for highest average
            if (arr[i].average > highestAvg) {
                highestAvg = arr[i].average;
                strncpy(highestID, arr[i].id, MAX_ID_LENGTH - 1);
                highestID[MAX_ID_LENGTH - 1] = '\0';
            }
            
            // Check for lowest average
            if (arr[i].average < lowestAvg) {
                lowestAvg = arr[i].average;
                strncpy(lowestID, arr[i].id, MAX_ID_LENGTH - 1);
                lowestID[MAX_ID_LENGTH - 1] = '\0';
            }
            
            // Count grades
            switch (arr[i].grade) {
                case 'A': gradeCount[0]++; break;
                case 'B': gradeCount[1]++; break;
                case 'C': gradeCount[2]++; break;
                case 'D': gradeCount[3]++; break;
                case 'F': gradeCount[4]++; break;
            }
        }
    }
    
    // Calculate class average
    float classAverage = (activeCount > 0) ? (totalAverage / activeCount) : 0.0f;
    
    // Display report on screen
    printf("Total Active Students: %d\n\n", activeCount);
    
    if (activeCount > 0) {
        printf("Class Average: %.2f\n\n", classAverage);
        
        printf("Highest Average: %.2f (Student ID: %s)\n", highestAvg, highestID);
        printf("Lowest Average: %.2f (Student ID: %s)\n\n", lowestAvg, lowestID);
        
        printf("Grade Distribution:\n");
        printf("A: %d students (%.1f%%)\n", gradeCount[0], (float)gradeCount[0]/activeCount*100);
        printf("B: %d students (%.1f%%)\n", gradeCount[1], (float)gradeCount[1]/activeCount*100);
        printf("C: %d students (%.1f%%)\n", gradeCount[2], (float)gradeCount[2]/activeCount*100);
        printf("D: %d students (%.1f%%)\n", gradeCount[3], (float)gradeCount[3]/activeCount*100);
        printf("F: %d students (%.1f%%)\n", gradeCount[4], (float)gradeCount[4]/activeCount*100);
    } else {
        printf("No active students to generate report.\n");
    }
    
    // Save report to file
    printf("\nSave report to file? (1 for Yes, 0 for No): ");
    int saveReport = getIntegerInput(0, 1);
    
    if (saveReport) {
        FILE *reportFile = fopen(REPORT_FILENAME, "w");
        if (reportFile == NULL) {
            printf("Error: Could not create report file %s.\n", REPORT_FILENAME);
        } else {
            fprintf(reportFile, "STUDENT GRADING SYSTEM - CLASS REPORT\n");
            fprintf(reportFile, "======================================\n\n");
            
            fprintf(reportFile, "Total Active Students: %d\n\n", activeCount);
            
            if (activeCount > 0) {
                fprintf(reportFile, "Class Average: %.2f\n\n", classAverage);
                
                fprintf(reportFile, "Highest Average: %.2f (Student ID: %s)\n", highestAvg, highestID);
                fprintf(reportFile, "Lowest Average: %.2f (Student ID: %s)\n\n", lowestAvg, lowestID);
                
                fprintf(reportFile, "Grade Distribution:\n");
                fprintf(reportFile, "A: %d students (%.1f%%)\n", gradeCount[0], (float)gradeCount[0]/activeCount*100);
                fprintf(reportFile, "B: %d students (%.1f%%)\n", gradeCount[1], (float)gradeCount[1]/activeCount*100);
                fprintf(reportFile, "C: %d students (%.1f%%)\n", gradeCount[2], (float)gradeCount[2]/activeCount*100);
                fprintf(reportFile, "D: %d students (%.1f%%)\n", gradeCount[3], (float)gradeCount[3]/activeCount*100);
                fprintf(reportFile, "F: %d students (%.1f%%)\n", gradeCount[4], (float)gradeCount[4]/activeCount*100);
            } else {
                fprintf(reportFile, "No active students to generate report.\n");
            }
            
            fclose(reportFile);
            printf("Report saved to %s successfully.\n", REPORT_FILENAME);
        }
    }
    
    waitForEnter();
}

// Clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Get integer input within a range
int getIntegerInput(int min, int max) {
    int value;
    char input[100];
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input. Please try again: ");
            continue;
        }
        
        // Check if input is empty (just newline)
        if (input[0] == '\n') {
            printf("No input. Please enter a number between %d and %d: ", min, max);
            continue;
        }
        
        // Check if input is not a number
        int isValid = 1;
        for (int i = 0; input[i] != '\n' && input[i] != '\0'; i++) {
            if (!isdigit(input[i])) {
                isValid = 0;
                break;
            }
        }
        
        if (!isValid) {
            printf("Invalid input. Please enter a number between %d and %d: ", min, max);
            continue;
        }
        
        value = atoi(input);
        
        if (value >= min && value <= max) {
            return value;
        } else {
            printf("Number out of range. Please enter a number between %d and %d: ", min, max);
        }
    }
}

// Wait for user to press Enter
void waitForEnter() {
    printf("\nPress Enter to continue...");
    clearInputBuffer();
    getchar();
}
