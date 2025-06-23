#ifndef MENUIO_H
#define MENUIO_H

#include "../../../include/headers/list.h"

void initMenuIO(list* employeeList);
void handleCreateEmployeeList();
void handleAddEmployee();
void handleDisplayPayroll();
int menuLoop();
void checkStates();

#endif 