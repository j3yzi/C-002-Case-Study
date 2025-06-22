#ifndef PAYROLL_H
#define PAYROLL_H

#include "../../../include/models/employee.h"

void calculatePayroll(Employee* employee);
void calculateBasicPay(Employee* employee);
void calculateOvertimePay(Employee* employee);
void calculateDeductions(Employee* employee);

#endif 