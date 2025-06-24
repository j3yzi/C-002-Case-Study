#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Configuration for business logic values (not struct sizes)
typedef struct {
    // Payroll settings
    float regular_hours;
    float overtime_rate;
    
    // Academic settings
    float passing_grade;
    float min_grade;
    float max_grade;
} Config;

// Global configuration instance
extern Config g_config;

// Configuration functions
int loadConfig(const char* config_file);
int saveConfig(const char* config_file);
void setDefaultConfig(void);
void printCurrentConfig(void);

// Convenience functions for accessing configurable values
float getRegularHours(void);
float getOvertimeRate(void);
float getPassingGrade(void);

#endif // CONFIG_H