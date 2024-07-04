#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_EMPLOYEES 100
#define MAX_NAME_LENGTH 50
#define MAX_POSITION_LENGTH 50
#define MAX_LOCATION_LENGTH 50
#define EMPLOYEE_FILE_NAME "employees.dat"
#define SALARY_FILE_NAME "salary.dat"

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#else
    #define CLEAR_COMMAND "clear"
#endif

typedef struct {
    char name[MAX_NAME_LENGTH];
    int age;
    double salary;
    char position[MAX_POSITION_LENGTH];
    char location[MAX_LOCATION_LENGTH];
    int attendance;
} Employee;

typedef struct {
    char name[MAX_NAME_LENGTH];
    double amount;
} SalaryRecord;

Employee employees[MAX_EMPLOYEES];
int employeeCount = 0;

void clearScreen() {
    system(CLEAR_COMMAND);
}

void saveEmployeesToFile() {
    FILE *file = fopen(EMPLOYEE_FILE_NAME, "wb");
    if (file != NULL) {
        fwrite(&employeeCount, sizeof(int), 1, file);
        fwrite(employees, sizeof(Employee), employeeCount, file);
        fclose(file);
    } else {
        printf("Unable to save data to file.\n");
    }
}

void loadEmployeesFromFile() {
    FILE *file = fopen(EMPLOYEE_FILE_NAME, "rb");
    if (file != NULL) {
        fread(&employeeCount, sizeof(int), 1, file);
        fread(employees, sizeof(Employee), employeeCount, file);
        fclose(file);
    }
}

void saveSalaryRecord(SalaryRecord record) {
    FILE *file = fopen(SALARY_FILE_NAME, "ab");
    if (file != NULL) {
        fwrite(&record, sizeof(SalaryRecord), 1, file);
        fclose(file);
    } else {
        printf("Unable to save salary record to file.\n");
    }
}

void viewSalaryHistory() {
    clearScreen();
    FILE *file = fopen(SALARY_FILE_NAME, "rb");
    if (file != NULL) {
        SalaryRecord record;
        printf("_________________________________\n");
        printf("|%-20s %-10s|\n", "Name", "Amount");
        printf("|_______________________________|\n");
        while (fread(&record, sizeof(SalaryRecord), 1, file)) {
            printf("|%-20s %-10.2f|\n", record.name, record.amount);
        printf("|_______________________________|\n");
        }
        fclose(file);
    } else {
        printf("Unable to read salary history from file.\n");
    }
}


void createEmployee() {
    clearScreen();
    if (employeeCount < MAX_EMPLOYEES) {
        Employee newEmployee;
        int nameExists;
        do {
            nameExists = 0;
            printf("Enter name: ");
            scanf(" %[^\n]", newEmployee.name);
            if (findEmployeeIndex(newEmployee.name) != -1) {
                printf("Name already exists. Please enter a new name.\n");
                nameExists = 1;
            }
        } while (nameExists);
        
        printf("Enter age: ");
        scanf("%d", &newEmployee.age);
        
        printf("Enter salary: ");
        scanf("%lf", &newEmployee.salary);
        
        printf("Enter position: ");
        scanf(" %[^\n]", newEmployee.position);
        
        printf("Enter location: ");
        scanf(" %[^\n]", newEmployee.location);
        
        printf("Enter attendance: ");
        scanf("%d", &newEmployee.attendance);
        
        employees[employeeCount] = newEmployee;
        employeeCount++;
        saveEmployeesToFile();
        printf("Employee added successfully.\n");
    } else {
        printf("Employee list is full.\n");
    }
}
void readEmployees() {
    clearScreen();
    printf("________________________________________________________________________________________________\n");
    printf("|%-3s %-20s %-5s %-10s %-20s %-20s %-10s|\n", "No.", "Name", "Age", "Salary", "Position", "Location", "Attendance");
    printf("|______________________________________________________________________________________________|\n");
    int i;
    for ( i = 0; i < employeeCount; i++) {
        printf("|%-3d %-20s %-5d %-10.2f %-20s %-20s %-10d|\n", i + 1, employees[i].name, employees[i].age, employees[i].salary, employees[i].position, employees[i].location, employees[i].attendance);
        printf("|______________________________________________________________________________________________|\n");
  }
}

int findEmployeeIndex(char name[]) {
    int i;
    for ( i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void updateEmployee() {
    clearScreen();
    char name[MAX_NAME_LENGTH];
    
    printf("Enter the name of the employee to update: ");
    scanf(" %[^\n]", name);
    
    int index = findEmployeeIndex(name);
    if (index != -1) {
        printf("Enter new age: ");
        scanf("%d", &employees[index].age);
        
        printf("Enter new salary: ");
        scanf("%lf", &employees[index].salary);
        
        printf("Enter new position: ");
        scanf(" %[^\n]", employees[index].position);
        
        printf("Enter new location: ");
        scanf(" %[^\n]", employees[index].location);
        
        printf("Enter new attendance: ");
        scanf("%d", &employees[index].attendance);
        
        saveEmployeesToFile();
        printf("Employee updated successfully.\n");
    } else {
        printf("Employee not found.\n");
    }
}

void updateAttendance() {
    clearScreen();
    char name[MAX_NAME_LENGTH];
    
    printf("Enter the name of the employee to update attendance: ");
    scanf(" %[^\n]", name);
    
    int index = findEmployeeIndex(name);
    if (index != -1) {
        printf("Enter new attendance: ");
        scanf("%d", &employees[index].attendance);
        
        saveEmployeesToFile();
        printf("Attendance updated successfully.\n");
    } else {
        printf("Employee not found.\n");
    }
}

void deleteEmployee() {
    clearScreen();
    char name[MAX_NAME_LENGTH];
    
    printf("Enter the name of the employee to delete: ");
    scanf(" %[^\n]", name);
    
    int index = findEmployeeIndex(name);
    if (index != -1) {
        int i;
        for ( i = index; i < employeeCount - 1; i++) {
            employees[i] = employees[i + 1];
        }
        employeeCount--;
        saveEmployeesToFile();
        printf("Employee deleted successfully.\n");
    } else {
        printf("Employee not found.\n");
    }
}

void paySalary() {
    clearScreen();
    char confirm;
    int i;
    
    for ( i = 0; i < employeeCount; i++) {
        double salaryPaid = (employees[i].salary / 30) * employees[i].attendance;
        
        printf("Employee: %s\n", employees[i].name);
        printf("Attendance: %d\n", employees[i].attendance);
        printf("Calculated Salary: %.2f\n", salaryPaid);
        printf("Do you want to pay this salary? (y/n): ");
        scanf(" %c", &confirm);
        
        if (confirm == 'y' || confirm == 'Y') {
            SalaryRecord record;
            strcpy(record.name, employees[i].name);
            record.amount = salaryPaid;
            
            saveSalaryRecord(record);
            
            employees[i].attendance = 0;
            printf("Salary paid and attendance reset for %s.\n", employees[i].name);
        } else {
            printf("Salary not paid for %s.\n", employees[i].name);
        }
        printf("\n");
    }
    saveEmployeesToFile();
    printf("Salary payment process completed.\n");
}

int main() {
    loadEmployeesFromFile();
    int choice;
    
    while (1) {
        clearScreen();
        printf("\nEmployee Management System\n");
        printf("1. Add Employee\n");
        printf("2. View Employees\n");
        printf("3. Update Employee\n");
        printf("4. Delete Employee\n");
        printf("5. Update Attendance\n");
        printf("6. Pay Salary\n");
        printf("7. View Salary History\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                createEmployee();
                break;
            case 2:
                readEmployees();
                break;
            case 3:
                updateEmployee();
                break;
            case 4:
                deleteEmployee();
                break;
            case 5:
                updateAttendance();
                break;
            case 6:
                paySalary();
                break;
            case 7:
                viewSalaryHistory();
                break;
            case 8:
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        printf("Press Enter to continue...");
        while (getchar() != '\n');  // Clear the input buffer
        getchar();  // Wait for Enter key
    }
}
