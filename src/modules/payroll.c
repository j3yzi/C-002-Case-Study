#include <stdio.h>
#include "payroll.h"
#include "../../include/models/employee.h"
#include "../../include/headers/apctxt.h"

void calculatePayroll(Employee* employee) {
    if (!employee) return;
    
    calculateBasicPay(employee);
    printf("DEBUG: Basic Pay = %.2f\n", employee->payroll.basicPay);
    
    calculateOvertimePay(employee);
    printf("DEBUG: Overtime Pay = %.2f\n", employee->payroll.overtimePay);
    
    calculateDeductions(employee);
    printf("DEBUG: Deductions = %.2f\n", employee->payroll.deductions);

    employee->payroll.netPay = employee->payroll.basicPay + 
                              employee->payroll.overtimePay - 
                              employee->payroll.deductions;
    printf("DEBUG: Net Pay = %.2f\n", employee->payroll.netPay);
    
    // Check if employee is regular and gets a special bonus
    if (employee->employment.status == statusRegular) {
        printf("DEBUG: Employee is Regular\n");
    } else {
        printf("DEBUG: Employee is Casual\n");
    }
}

void calculateBasicPay(Employee* employee) {
    if (!employee) return;
    
    float configRegularHours = getRegularHours();
    employee->payroll.basicPay = employee->employment.basicRate * configRegularHours;
   
    
    // Ensure basic pay doesn't exceed maximum limit
    if (employee->payroll.basicPay > 999999.0f) {
        printf("WARNING: Basic pay calculation exceeded maximum limit of 999999.00\n");
        printf("         Capping basic pay at 999999.00\n");
        employee->payroll.basicPay = 999999.0f;
    }
}

void calculateOvertimePay(Employee* employee) {
    if (!employee) return;
    
    float configRegularHours = getRegularHours();
    if (employee->employment.hoursWorked > configRegularHours) {
        float overtimeHours = employee->employment.hoursWorked - configRegularHours;
        // Overtime rate is configurable bonus more than basic rate (multiply, not add)
        float overtimeRateBonus = getOvertimeRate();
        float overtimeHourlyRate = employee->employment.basicRate * (1.0 + overtimeRateBonus);
        employee->payroll.overtimePay = overtimeHours * overtimeHourlyRate;
    } else {
        employee->payroll.overtimePay = 0.0;
    }
}

void calculateDeductions(Employee* employee) {
    if (!employee) return;
    
    float configRegularHours = getRegularHours();
    
    // Deductions logic: if hours worked is less than regular hours,
    // deduct basic rate * lack of hours
    if (employee->employment.hoursWorked < configRegularHours) {
        float lackOfHours = configRegularHours - employee->employment.hoursWorked;
        employee->payroll.deductions = employee->employment.basicRate * lackOfHours;
        
        // Ensure deductions don't exceed maximum limit
        if (employee->payroll.deductions > 99999.99f) {
            printf("WARNING: Deductions calculation exceeded maximum limit of 99999.99\n");
            printf("         Capping deductions at 99999.99\n");
            employee->payroll.deductions = 99999.99f;
        }
    } else {
        // No deductions if regular hours are met or exceeded
        employee->payroll.deductions = 0.0;
    }
} 