#ifndef MENUIO_H
#define MENUIO_H

#include "../../../include/headers/apctxt.h"
#include "../../../include/models/employee.h"
#include "../../../include/headers/list.h"

void initMenuIO(list* list);
int menuLoop();
void checkStates();

#endif 