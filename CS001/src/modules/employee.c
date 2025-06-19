#include "../../include/employee.h"

void addInitialEmployees() {
    list *employeeList = (list*)malloc(sizeof(list));
    employeeList = createList(SINGLY);

    for (int i = 0; i < MAX_EMPLOYEE_CREATION_COUNT; i++)
    {
        Employee* newEmployeeData = (Employee*)malloc(sizeof(Employee)); 
        
        getEmployeeDataFromUser(&newEmployeeData);

        createEmployee(newEmployeeData, &employeeList);    
    }
}

void createEmployee(Employee* newEmployeeData, list** l) {
    addNode(l, newEmployeeData);
}

// void updateEmployee(Employee* employee, void* data) {

// }

void deleteEmployee(Employee* employee) {
    if (employee != NULL) {
        free(employee);
    }
}