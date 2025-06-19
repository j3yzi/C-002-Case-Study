#include "../../include/employee.h"
#include "../../include/list.h"

int exportEmployeeDataToFile(list* employeeList, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return 0;
    }
    
    // Start from the head of the list
    node* current = employeeList->head;
    
    // Write the number of records first
    int recordCount = employeeList->size;
    if (fwrite(&recordCount, sizeof(int), 1, file) != 1) {
        perror("Error writing record count");
        fclose(file);
        return 0;
    }
    
    // Write each employee record
    while (current != NULL) {
        Employee* emp = (Employee*)current->data;
        
        if (fwrite(emp, sizeof(Employee), 1, file) != 1) {
            perror("Error writing employee record");
            fclose(file);
            return 0;
        }
        
        current = current->next;
        if (current == employeeList->head && employeeList->type == SINGLY_CIRCULAR) {
            break;  // Break if we've gone full circle in a circular list
        }
    }
    
    fclose(file);
    printf("Data successfully saved to %s (%d records)\n", filename, recordCount);
    return 1;
}

list* loadEmployeeDataFromFile(const char* filename, ListType listType) {
    FILE* file = fopen(filename, "rb");
    
    if (file == NULL) {
        // If the file doesn't exist, this isn't necessarily an error
        // We'll just return an empty list
        list* newList = createList(listType);
        return newList;
    }
    
    // Create a new list
    list* employeeList = createList(listType);
    if (employeeList == NULL) {
        perror("Failed to create employee list");
        fclose(file);
        return NULL;
    }
    
    // Read record count
    int recordCount = 0;
    if (fread(&recordCount, sizeof(int), 1, file) != 1) {
        // If we can't read the record count, the file may be empty
        fclose(file);
        return employeeList; // Return empty list
    }
    
    // Read each employee record
    for (int i = 0; i < recordCount; i++) {
        Employee* emp = (Employee*)malloc(sizeof(Employee));
        if (emp == NULL) {
            perror("Failed to allocate memory for employee");
            fclose(file);
            destroyList(employeeList);
            return NULL;
        }
        
        if (fread(emp, sizeof(Employee), 1, file) != 1) {
            perror("Error reading employee record");
            free(emp);
            fclose(file);
            destroyList(employeeList);
            return NULL;
        }
        
        // Add the employee to the list
        addNode(&employeeList, emp);
    }
    
    fclose(file);
    printf("Loaded %d employee records from %s\n", employeeList->size, filename);
    return employeeList;
}
