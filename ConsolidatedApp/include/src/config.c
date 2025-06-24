#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/apctxt.h"

/**
 * @brief Creates and allocates memory for an AppConfig structure.
 * @brief This function dynamically allocates an AppConfig struct and populates it with copies of the provided strings.
 * @param config A double pointer to the AppConfig struct to be created.
 * @param appName The name of the application.
 * @param appVersion The version of the application.
 * @param license The license information for the application.
 * @param creator The creator of the application.
 * @return Returns 0 on success, -1 on memory allocation failure.
 */
static int createAppConfig(AppConfig** config, const char* appName, const char* appVersion, const char* license, const char* creator) {
    *config = (AppConfig*)malloc(sizeof(AppConfig));
    if (*config == NULL) {
        return -1;
    }
    
    (*config)->appName = strdup(appName);
    (*config)->appVersion = strdup(appVersion);
    (*config)->license = strdup(license);
    (*config)->creator = strdup(creator);

    if (!(*config)->appName || !(*config)->appVersion || !(*config)->license || !(*config)->creator) {
        // strdup failed, clean up allocated memory
        free((*config)->appName);
        free((*config)->appVersion);
        free((*config)->license);
        free((*config)->creator);
        free(*config);
        *config = NULL;
        return -1;
    }

    return 0;
}

/**
 * @brief Loads application configuration from a file.
 * @brief This function reads a key-value pair configuration file and populates an AppConfig struct.
 * @param config A double pointer to the AppConfig struct to be created and populated.
 * @param filePath The path to the configuration file.
 * @return Returns 0 on success, -1 on failure (e.g., file not found, memory allocation error).
 */
static int loadAppConfig(AppConfig** config, const char* filePath) {
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open configuration file: %s\n", filePath);
        return -1;
    }

    *config = (AppConfig*)malloc(sizeof(AppConfig));
    if (*config == NULL) {
        fprintf(stderr, "Memory allocation failed for AppConfig\n");
        fclose(file);
        return -1;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "appName=", 8) == 0) {
            (*config)->appName = strdup(buffer + 8);
            if ((*config)->appName) (*config)->appName[strcspn((*config)->appName, "\n")] = 0;
        } else if (strncmp(buffer, "appVersion=", 11) == 0) {
            (*config)->appVersion = strdup(buffer + 11);
            if ((*config)->appVersion) (*config)->appVersion[strcspn((*config)->appVersion, "\n")] = 0;
        } else if (strncmp(buffer, "license=", 8) == 0) {
            (*config)->license = strdup(buffer + 8);
            if ((*config)->license) (*config)->license[strcspn((*config)->license, "\n")] = 0;
        } else if (strncmp(buffer, "creator=", 8) == 0) {
            (*config)->creator = strdup(buffer + 8);
            if ((*config)->creator) (*config)->creator[strcspn((*config)->creator, "\n")] = 0;
        }
    }

    fclose(file);
    return 0;
}