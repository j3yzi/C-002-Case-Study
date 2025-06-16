#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/employee.h"

void testFunctions ();

int main (void)
{
    // testFunctions();
}

void testFunctions ()
{
    // ### FRONTEND ###
    getEmployeeDataFromUser();
    displayReportHeader();
    displayEmployeeRecord();

    // ### BACKEND ###
    calculatePayroll();
    saveEmployeeDataToFile();
    loadEmployeeDataFromFile();
}