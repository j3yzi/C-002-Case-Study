#include "../../include/employee.h"

void calculateBasicPay(EmployeeNode* employee) {
    if (employee == NULL) return;
    
    /* Basic pay calculation:
     * - Basic pay is only given if at least 160 hours are rendered
     */
    
}

void calculateOvertimePay(EmployeeNode* employee) {
    if (employee == NULL) return;
    
    /* NOTE TO DEVELOPERS:
     * Overtime pay calculation: Only applies to hours worked beyond 160 hours
     */
}

void calculateDeductions(EmployeeNode* employee) {
    if (employee == NULL) return;
    
    /* Deduction calculation:
     * - For every hour less than 160, deduct the equivalent of basicRate per hour
     */
    
    
}

void calculateNetPay(EmployeeNode* employee) {
    if (employee == NULL) return;
    
    /* NOTE TO DEVELOPERS:
     * Net pay is the final amount after all calculations:
     * Net Pay = Basic Pay + Overtime Pay - Deductions
     */
    
}

// void calculatePayroll(EmployeeNode* employee) {
//     if (employee == NULL) return;
    
//     /* NOTE TO DEVELOPERS:
//      * This function orchestrates the entire payroll calculation process.
//      * It should call the other calculation functions in the proper order.
//      * Can be used for a single employee or looped through the entire list.
//      */
    
//     calculateBasicPay(employee);
//     calculateOvertimePay(employee);
//     calculateDeductions(employee);
//     calculateNetPay(employee);
// }