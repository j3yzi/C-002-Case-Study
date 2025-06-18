#include "../../include/employee.h"
#include "../../include/appctxt.h"

int exportEmployeeDataToFile(EmployeeNode* head, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        return 0; 
    }
      EmployeeNode *current = head;
    while (current != NULL) {
        // This writes all data fields but stops before the 'next' pointer.
        // This is correct, assuming 'next' is the last member and there's no padding.
        size_t written = fwrite(current, sizeof(EmployeeNode) - sizeof(EmployeeNode*), 1, file);

        if (written != 1) {
            perror("Error writing record to file");
            fclose(file);
            return 0;
        }
        
        current = current->next; // Advance to next node
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
    EmployeeNode *tail = NULL;
    EmployeeNode tempNode;

    while (fread(&tempNode, sizeof(EmployeeNode) - sizeof(EmployeeNode*), 1, file) == 1) {
        
        tempNode.next = NULL;
        EmployeeNode* newNode = (EmployeeNode*)malloc(sizeof(EmployeeNode));

        if (newNode == NULL) {
            perror("Failed to allocate memory for new employee node");
            fclose(file);
            return NULL;
        }

        *newNode = tempNode;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode; 
        }
    }

    fclose(file);
    return head;

}