#ifndef MENUIO_H
#define MENUIO_H

#include "../../../include/headers/apctxt.h"
#include "../../../include/headers/list.h"

void initMenuIO(list* studentList);
void checkStates(void);
int runStudentMenuLoop(void);

#endif 