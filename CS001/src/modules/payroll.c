#include "payroll.h"
#include "../../../include/models/employee.h"

// Define constants for payroll calculations
#define regularHours 160
#define overtimeRate 0.5

void calculatePayroll(Employee* employee) {
    if (!employee) return;
    
    calculateBasicPay(employee);
    calculateOvertimePay(employee);
    calculateDeductions(employee);

    employee->payroll.netPay = employee->payroll.basicPay + 
                              employee->payroll.overtimePay - 
                              employee->payroll.deductions;
}

void calculateBasicPay(Employee* employee) {
    if (!employee) return;
    
    if (employee->employment.hoursWorked >= regularHours) {
        employee->payroll.basicPay = employee->employment.basicRate * regularHours;
    } else {
        employee->payroll.basicPay = employee->employment.hoursWorked * employee->employment.basicRate;
    }
}

void calculateOvertimePay(Employee* employee) {
    if (!employee) return;
    
    if (employee->employment.hoursWorked > regularHours) {
        float overtimeHours = employee->employment.hoursWorked - regularHours;
        employee->payroll.overtimePay = overtimeHours * (employee->employment.basicRate * (1.0 + overtimeRate));
    } else {
        employee->payroll.overtimePay = 0.0;
    }
}

void calculateDeductions(Employee* employee) {
    if (!employee) return;
    
    float deductionRate;
    
    if (employee->employment.status == statusRegular) {
        deductionRate = 0.078533;  // 7.8533% for regular employees
    } else if (employee->employment.status == statusCasual) {
        deductionRate = 0.10;      // 10% for casual employees
    } else {
        deductionRate = 0.05;
    }
    
    employee->payroll.deductions = (employee->payroll.basicPay + employee->payroll.overtimePay) * deductionRate;
}