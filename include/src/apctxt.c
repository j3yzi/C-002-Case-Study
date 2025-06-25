#include "../headers/apctxt.h"

// Global configuration instance
Config g_config;

// Set default configuration values
void setDefaultConfig(void) {
    // Payroll settings
    g_config.regularHours = 160.0f;
    g_config.overtimeRate = 0.5f;
    
    // Academic settings
    g_config.passingGrade = 75.0f;
    g_config.minGrade = 0.0f;
    g_config.maxGrade = 100.0f;
}

// Save configuration to file
int saveConfig(const char* config_file) {
    FILE* file = fopen(config_file, "w");
    if (!file) {
        printf("Error: Could not create config file '%s'.\n", config_file);
        return -1;
    }
    
    fprintf(file, "# Configuration File\n");
    fprintf(file, "# Created on %s\n\n", __DATE__);
    
    fprintf(file, "[Payroll_Settings]\n");
    fprintf(file, "regular_hours = %.1f\n", g_config.regularHours);
    fprintf(file, "overtime_rate = %.1f\n\n", g_config.overtimeRate);
    
    fprintf(file, "[Academic_Settings]\n");
    fprintf(file, "passing_grade = %.1f\n", g_config.passingGrade);
    fprintf(file, "min_grade = %.1f\n", g_config.minGrade);
    fprintf(file, "max_grade = %.1f\n", g_config.maxGrade);
    
    fclose(file);
    printf("Configuration saved to '%s'.\n", config_file);
    return 0;
}

// Simple but functional configuration loader
int loadConfig(const char* config_file) {
    // Set defaults first
    setDefaultConfig();
    
    // Try to open the file
    FILE* file = fopen(config_file, "r");
    if (!file) {
        printf("Warning: Could not open config file '%s'. Creating default configuration.\n", config_file);
        // Create directory if it doesn't exist (extract path from config_file)
        char path[256] = {0};
        strcpy(path, config_file);
        char* lastSlash = strrchr(path, '/');
        if (!lastSlash) {
            // Try Windows path separator
            lastSlash = strrchr(path, '\\');
        }
        if (lastSlash) {
            *lastSlash = '\0';
            // Create directory using system command (Windows-specific)
            char mkdir_cmd[300];
            sprintf(mkdir_cmd, "mkdir %s 2>nul", path);
            system(mkdir_cmd);
        }
        
        // Create default configuration file
        int result = saveConfig(config_file);
        if (result == 0) {
            printf("Default configuration file created at '%s'.\n", config_file);
        }
        return result; // Return result of saveConfig
    }
    
    printf("Successfully opened config file: %s\n", config_file);
    
    char line[256];
    char current_section[64] = "";
    
    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        char* newline = strchr(line, '\n');
        if (newline) *newline = '\0';
        
        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        
        // Check for section
        if (line[0] == '[') {
            char* end = strchr(line + 1, ']');
            if (end) {
                *end = '\0';
                strcpy(current_section, line + 1);
                printf("Reading section: [%s]\n", current_section);
            }
            continue;
        }
        
        // Parse key=value
        char* equals = strchr(line, '=');
        if (!equals) continue;
        
        *equals = '\0';
        char* key = line;
        char* value = equals + 1;
        
        // Remove leading/trailing spaces
        while (*key == ' ') key++;
        while (*value == ' ') value++;
        
        // Parse based on section
        if (strcmp(current_section, "Payroll_Settings") == 0) {
            if (strcmp(key, "regular_hours") == 0) {
                g_config.regularHours = (float)atof(value);
                printf("Set regular_hours = %.1f\n", g_config.regularHours);
            } else if (strcmp(key, "overtime_rate") == 0) {
                g_config.overtimeRate = (float)atof(value);
                printf("Set overtime_rate = %.1f\n", g_config.overtimeRate);
            }
        } else if (strcmp(current_section, "Academic_Settings") == 0) {
            if (strcmp(key, "passing_grade") == 0) {
                g_config.passingGrade = (float)atof(value);
                printf("Set passing_grade = %.1f\n", g_config.passingGrade);
            } else if (strcmp(key, "min_grade") == 0) {
                g_config.minGrade = (float)atof(value);
            } else if (strcmp(key, "max_grade") == 0) {
                g_config.maxGrade = (float)atof(value);
            }
        }
    }
    
    fclose(file);
    printf("Configuration loaded. Final regular_hours = %.1f\n", g_config.regularHours);
    return 0;
}

// Print current configuration
void printCurrentConfig(void) {
    printf("=== Current Configuration ===\n");
    printf("Regular Hours: %.1f\n", g_config.regularHours);
    printf("Overtime Rate: %.1f\n", g_config.overtimeRate);
    printf("Passing Grade: %.1f\n", g_config.passingGrade);
    printf("============================\n");
}

// Convenience accessor functions
float getRegularHours(void) {
    return g_config.regularHours;
}

float getOvertimeRate(void) {
    return g_config.overtimeRate;
}

float getPassingGrade(void) {
    return g_config.passingGrade;
} 