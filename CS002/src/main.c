#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/headers/apctxt.h"
#include "ui/menuio.h"

// Initialize configuration

int main(void)
{   
    // Initialize the terminal
    appInitWinTerm("Academic Management System");
    
    runMenuLoop();
    return 0;
}