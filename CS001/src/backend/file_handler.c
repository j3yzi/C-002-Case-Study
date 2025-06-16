#include "../../include/employee.h"

int saveEmployeeDataToFile(EmployeeNode* head, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return 0; 
    }
    
    EmployeeNode *current = head;
    while (current != NULL) {
    
        size_t written = fwrite(current, sizeof(EmployeeNode), 1, file);

        if (written != 1) {
            perror("Error writing to file");
            fclose(file);
            return 0; 
        }

        current = current->next;
    }

    fclose(file);
    printf("Data successfully saved to %s\n", filename);
    return 1;
}

EmployeeNode* loadEmployeeDataFromFile (const char* filename) {
    FILE* file = fopen(filename, "rb");
    
    if (file == NULL) {
        perror("Error opening file for reading");
        return NULL;
    }

    EmployeeNode *head = NULL;
    EmployeeNode *current = NULL;
    EmployeeNode tempNode;

    while (fread(&tempNode, sizeof(EmployeeNode), 1, file) == 1) {
        EmployeeNode* newNode = (EmployeeNode*)malloc(sizeof(EmployeeNode));

        if (newNode == NULL) {
            perror("Failed to allocate memory for new employee node");
            fclose(file);
            return NULL;
        }

        *newNode = tempNode;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            current = newNode;
        } else {
            current->next = newNode;
            current = newNode; 
        }
    }

    fclose(file);
    return head;

}