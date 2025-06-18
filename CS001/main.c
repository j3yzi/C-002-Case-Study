#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/employee.h"
#include "include/appctxt.h"

int main(void)
{
    // Initialize context
    AppContext appContext;
    appContext.head = NULL;
    appContext.tail = NULL;
    appContext.current = NULL;
    appContext.count = 0;
    appContext.dataFile = "data/record.bin";
    appContext.isInitialList = true;

    // Run the main menu loop
    runMenuLoop(&appContext);
    
    // Clean up before exiting
    freeEmployeeList(&appContext);
    
    return 0;
}