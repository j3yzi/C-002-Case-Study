#include "../headers/apctxt.h"

AppConfig* createAppConfig(const char* appName, const char* appVersion, const char* license, const char* creator) {
    AppConfig* config = (AppConfig*)malloc(sizeof(AppConfig));
    if (config == NULL) {
        fprintf(stderr, "Memory allocation failed for AppConfig\n");
        return NULL;
    }
    
    config->appName = appName;
    config->appVersion = appVersion;
    config->license = license;
    config->creator = creator;

    return config;
}

AppConfig* loadAppConfig(const char* filePath) {
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open configuration file: %s\n", filePath);
        return NULL;
    }

    AppConfig* config = (AppConfig*)malloc(sizeof(AppConfig));
    if (config == NULL) {
        fprintf(stderr, "Memory allocation failed for AppConfig\n");
        fclose(file);
        return NULL;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "appName=", 8) == 0) {
            config->appName = strdup(buffer + 8);
            config->appName[strcspn(config->appName, "\n")] = 0; // Remove newline
        } else if (strncmp(buffer, "appVersion=", 11) == 0) {
            config->appVersion = strdup(buffer + 11);
            config->appVersion[strcspn(config->appVersion, "\n")] = 0; // Remove newline
        } else if (strncmp(buffer, "license=", 8) == 0) {
            config->license = strdup(buffer + 8);
            config->license[strcspn(config->license, "\n")] = 0; // Remove newline
        } else if (strncmp(buffer, "creator=", 8) == 0) {
            config->creator = strdup(buffer + 8);
            config->creator[strcspn(config->creator, "\n")] = 0; // Remove newline
        }
    }

    fclose(file);
    return config;
}