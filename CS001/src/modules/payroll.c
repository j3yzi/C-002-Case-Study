#include "../../include/employee.h"

void calculatePayroll(Employee* employee) {
    calculateBasicPay(employee);
    calculateOvertimePay(employee);
    calculateDeductions(employee);

    employee->payroll.netPay = employee->payroll.basicPay + 
                              employee->payroll.overtimePay - 
                              employee->payroll.deductions;
}

void calculateBasicPay(Employee* employee) {
    if (employee->employment.hoursWorked >= REGULAR_HOURS) {
        employee->payroll.basicPay = employee->employment.basicRate * REGULAR_HOURS;
    } else {
        employee->payroll.basicPay = employee->employment.hoursWorked * employee->employment.basicRate;
    }
}

void calculateOvertimePay(Employee* employee) {
    if (employee->employment.hoursWorked > REGULAR_HOURS) {
        float overtimeHours = employee->employment.hoursWorked - REGULAR_HOURS;
        employee->payroll.overtimePay = overtimeHours * (employee->employment.basicRate * (1.0 + OVERTIME_RATE));
    } else {
        employee->payroll.overtimePay = 0.0;
    }
}

void calculateDeductions(Employee* employee) {
    float deductionRate;
    
    if (employee->employment.status == STATUS_REGULAR) {
        deductionRate = 0.078533;  // 7.8533% for regular employees
    } else if (employee->employment.status == STATUS_CASUAL) {
        deductionRate = 0.10;      // 10% for casual employees
    } else {
        deductionRate = 0.05;
    }
    
    employee->payroll.deductions = (employee->payroll.basicPay + employee->payroll.overtimePay) * deductionRate;
}