#ifndef MENUIO_H
#define MENUIO_H

#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/list.h"

void initMenuIO(list* employeeList);
void checkStates(void);
int menuLoop(void);

#endif 