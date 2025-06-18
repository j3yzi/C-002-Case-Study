#include "../../include/employee.h"

char status;
int hoursWorked;
int basicRate;
int basicPay;
int overtimePay;
int netPay;
int deductionRate;

int statusJob(){
    if (status == 'R' || status == 'r'){
        basicRate = 645;
        deductionRate = 0.078533;
    } else if (status == 'C' || status == 'c'){
        basicRate = 300;
        deductionRate = 0.10;
    } else {
        printf("No known status available");
        return 1;
    }
}

void basicPayment (){
    if (hoursWorked >= 160){
        basicPay = basicRate * 160;
    } else {
        basicPay = hoursWorked * basicRate;
    }
}

void overtimePayment(){
    if (hoursWorked > 160){
        //Overtime this
        overtimePay = (hoursWorked = hoursWorked - 160)*(basicRate * 1.5);
    } else {
        overtimePay == NULL;
    }
}

void calculatePayroll() {
    netPay = basicPay + overtimePay - deductionRate;
    //placeholder
    //printf("calculatePayroll() called!\n");
}

