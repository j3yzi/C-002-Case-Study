#include "../headers/config.h"

// Global configuration instance
Config g_config;

// Set default configuration values
void setDefaultConfig(void) {
    // Payroll settings
    g_config.regular_hours = 160.0f;
    g_config.overtime_rate = 0.5f;
    
    // Academic settings
    g_config.passing_grade = 75.0f;
    g_config.min_grade = 0.0f;
    g_config.max_grade = 100.0f;
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
    fprintf(file, "regular_hours = %.1f\n", g_config.regular_hours);
    fprintf(file, "overtime_rate = %.1f\n\n", g_config.overtime_rate);
    
    fprintf(file, "[Academic_Settings]\n");
    fprintf(file, "passing_grade = %.1f\n", g_config.passing_grade);
    fprintf(file, "min_grade = %.1f\n", g_config.min_grade);
    fprintf(file, "max_grade = %.1f\n", g_config.max_grade);
    
    fclose(file);
    printf("Configuration saved to '%s'.\n", config_file);
    return 0;
}

// Simple but functional configuration loader
int loadConfig(const char* config_file) {
    // Set defaults first
    setDefaultConfig();
    
    // Try to open the file
    FILE* file = fopen(config_file, "r");    if (!file) {
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
                g_config.regular_hours = (float)atof(value);
                printf("Set regular_hours = %.1f\n", g_config.regular_hours);
            } else if (strcmp(key, "overtime_rate") == 0) {
                g_config.overtime_rate = (float)atof(value);
                printf("Set overtime_rate = %.1f\n", g_config.overtime_rate);
            }
        } else if (strcmp(current_section, "Academic_Settings") == 0) {
            if (strcmp(key, "passing_grade") == 0) {
                g_config.passing_grade = (float)atof(value);
                printf("Set passing_grade = %.1f\n", g_config.passing_grade);
            } else if (strcmp(key, "min_grade") == 0) {
                g_config.min_grade = (float)atof(value);
            } else if (strcmp(key, "max_grade") == 0) {
                g_config.max_grade = (float)atof(value);
            }
        }
    }
    
    fclose(file);
    printf("Configuration loaded. Final regular_hours = %.1f\n", g_config.regular_hours);
    return 0;
}

// Print current configuration
void printCurrentConfig(void) {
    printf("=== Current Configuration ===\n");
    printf("Regular Hours: %.1f\n", g_config.regular_hours);
    printf("Overtime Rate: %.1f\n", g_config.overtime_rate);
    printf("Passing Grade: %.1f\n", g_config.passing_grade);
    printf("============================\n");
}

// Convenience accessor functions
float getRegularHours(void) {
    return g_config.regular_hours;
}

float getOvertimeRate(void) {
    return g_config.overtime_rate;
}

float getPassingGrade(void) {
    return g_config.passing_grade;
}