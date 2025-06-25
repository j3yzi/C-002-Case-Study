#include "../headers/apctxt.h"

Config g_config; // Global configuration instance

void setDefaultConfig(void) { // Set default configuration values
    g_config.regularHours = 160.0f;     // Payroll settings
    g_config.overtimeRate = 0.5f;
    g_config.passingGrade = 75.0f;     // Academic settings
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
    fprintf(file, "# PUP Information Management System Configuration\n");
    fprintf(file, "# Created on %s\n\n", __DATE__);
    fprintf(file, "[Payroll_Settings]\n");
    fprintf(file, "regular_hours = %.1f\n", g_config.regularHours);
    fprintf(file, "overtime_rate = %.1f\n\n", g_config.overtimeRate);
    fprintf(file, "[Academic_Settings]\n");
    fprintf(file, "passing_grade = %.1f\n", g_config.passingGrade);
    fprintf(file, "min_grade = %.1f\n", g_config.minGrade);
    fprintf(file, "max_grade = %.1f\n\n", g_config.maxGrade);
    
    // Add Programs section with default program codes
    fprintf(file, "[Programs]\n");
    fprintf(file, "# Format: program_code = Program Name\n");
    fprintf(file, "BSIT = Bachelor of Science in Information Technology\n");
    fprintf(file, "BSCS = Bachelor of Science in Computer Science\n");
    fprintf(file, "BSIS = Bachelor of Science in Information Systems\n");
    fprintf(file, "BSA = Bachelor of Science in Accountancy\n");
    fprintf(file, "BSBA = Bachelor of Science in Business Administration\n");
    
    fclose(file);
    return 0;
}

int loadConfig(const char* config_file) {
    setDefaultConfig(); // Set defaults first

    FILE* file = fopen(config_file, "r");
    if (!file) {
        char path[256] = {0};
        strcpy(path, config_file);
        char* lastSlash = strrchr(path, '/'); // Create directory if it doesn't exist (extract path from config_file)
        if (!lastSlash) {
            lastSlash = strrchr(path, '\\'); // Try Windows path separator
        }
        if (lastSlash) {
            *lastSlash = '\0';
            char mkdir_cmd[300];
            sprintf(mkdir_cmd, "mkdir %s 2>nul", path); // Create directory using system command (Windows-specific)
            system(mkdir_cmd);
        }
        int result = saveConfig(config_file); // Create default configuration file
        if (result == 0) {
            printf("Default configuration file created at '%s'.\n", config_file);
        }
        return result;
    }
    
    char line[256];
    char current_section[64] = "";
    
    while (fgets(line, sizeof(line), file)) {
        char* newline = strchr(line, '\n'); // Remove newline
        if (newline) *newline = '\0';
        if (line[0] == '\0' || line[0] == '#') continue; // Skip empty lines and comments
        if (line[0] == '[') { 
            char* end = strchr(line + 1, ']');
            if (end) {
                *end = '\0';
                strcpy(current_section, line + 1);
            }
            continue;
        }
        
        char* equals = strchr(line, '='); // Parse key=value
        if (!equals) continue;
        *equals = '\0';
        char* key = line;
        char* value = equals + 1;
        
        while (*key == ' ') key++;
        while (*value == ' ') value++;
        
        if (strcmp(current_section, "Payroll_Settings") == 0) { // Get payroll settings
            if (strcmp(key, "regular_hours") == 0) {
                g_config.regularHours = (float)atof(value);
            } else if (strcmp(key, "overtime_rate") == 0) {
                g_config.overtimeRate = (float)atof(value);
            }
        } else if (strcmp(current_section, "Academic_Settings") == 0) { // Get academic settings
            if (strcmp(key, "passing_grade") == 0) {
                g_config.passingGrade = (float)atof(value);
            } else if (strcmp(key, "min_grade") == 0) {
                g_config.minGrade = (float)atof(value);
            } else if (strcmp(key, "max_grade") == 0) {
                g_config.maxGrade = (float)atof(value);
            }
        }
    }
    fclose(file);
    return 0;
}

void printCurrentConfig(void) {
    printf("=== Current Configuration ===\n");
    printf("Regular Hours: %.1f\n", g_config.regularHours);
    printf("Overtime Rate: %.1f\n", g_config.overtimeRate);
    printf("Passing Grade: %.1f\n", g_config.passingGrade);
    printf("============================\n");
}

float getRegularHours(void) {
    return g_config.regularHours;
}

float getOvertimeRate(void) {
    return g_config.overtimeRate;
}

float getPassingGrade(void) {
    return g_config.passingGrade;
} 