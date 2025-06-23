#ifndef MENUIO_H
#define MENUIO_H

#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/list.h"

void initMenuIO(list* students, list* employees);
void checkStates(void);
int runMenuLoop(void);

#endif 