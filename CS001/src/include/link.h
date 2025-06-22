#ifndef LINK_H
#define LINK_H

#include "../../../include/models/employee.h"

// empio.c
int getEmployeeDataFromUser(Employee** employee);

// menuio.c
void handleCreateEmployeeList();
void handleAddEmployee();
int menuLoop();
void checkStates();

// payroll.c
void calculatePayroll(Employee* employee);
void calculateBasicPay(Employee* employee);
void calculateOvertimePay(Employee* employee);
void calculateDeductions(Employee* employee);


#endif