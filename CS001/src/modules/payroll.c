#include "payroll.h"
#include "../../../include/models/employee.h"

// Define constants for payroll calculations
#define regularHours 160

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
        // Overtime rate is 0.5 MORE than basic rate (basicRate + 0.5)
        float overtimeHourlyRate = employee->employment.basicRate + 0.5;
        employee->payroll.overtimePay = overtimeHours * overtimeHourlyRate;
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