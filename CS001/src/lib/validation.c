#include "../../include/apctxt.h"

static void enableAnsiSupport();
static void readLine(char* buffer, int size);
static bool isValid(char* input, IValidationType type, IValidationParams params);
void appGetValidatedInput(appFormField* fields, int fieldCount);

// Enable ANSI escape sequences for Windows
static void enableAnsiSupport() {
    #ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
    #endif
}

static void readLine(char* buffer, int size) {
    memset(buffer, 0, size);
    if (fgets(buffer, size, stdin)) {
        char* newline = strchr(buffer, '\n');
        if (newline) {
            *newline = '\0';
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}

static bool isValid(char* input, IValidationType type, IValidationParams params) {
    if (input[0] == '\0') {
        printf("   [Error] Empty input. Please enter a value.\n");
        return false;
    }

    switch (type) {
        case IV_NONE:
            return true;

        case IV_MAX_LEN:
            if (strlen(input) > (size_t)params.rangeInt.max) {
                printf("   [Error] Input too long. Maximum length is %ld characters.\n", params.rangeInt.max);
                return false;
            }
            return true; 

        case IV_CHOICES:
            for (int i = 0; i < params.choices.count; i++) {
                if (strcmp(input, params.choices.choices[i]) == 0) {
                    return true;
                }
            }
            printf("   [Error] Invalid choice '%s'. Please enter one of these: ", input);
            for (int i = 0; i < params.choices.count; i++) {
                printf("%s", params.choices.choices[i]);
                if (i < params.choices.count - 1) {
                    printf(", ");
                }
            }
            printf("\n");
            return false;

        case IV_RANGE_FLT:
            {
                char* end;
                double val = strtod(input, &end);
                if (end == input || *end != '\0') { // Check if conversion was successful
                    printf("   [Error] Invalid float format.\n");
                    return false;
                }
                if (val < params.rangeFloat.min || val > params.rangeFloat.max) {
                    printf("   [Error] Value out of range. Must be between %.2f and %.2f.\n", params.rangeFloat.min, params.rangeFloat.max);
                    return false;
                }
                return true;
            }

        case IV_RANGE_INT:
            {
                char* end;
                long val = strtol(input, &end, 10);
                if (end == input || *end != '\0') { // Check if conversion was successful
                    printf("   [Error] Invalid integer format.\n");
                    return false;
                }
                if (val < params.rangeInt.min || val > params.rangeInt.max) {
                    printf("   [Error] Value out of range. Must be between %ld and %ld.\n", params.rangeInt.min, params.rangeInt.max);
                    return false;
                }
                return true;
            }        case IV_MAX_LEN_CHARS:
            if (strlen(input) > (size_t)params.maxLengthChars.maxLength) {
                printf("   [Error] Input too long. Maximum length is %d characters.\n", params.maxLengthChars.maxLength);
                return false;
            }
            return true;
        default:
            return true;
    }
}

void appGetValidatedInput(appFormField* fields, int fieldCount) {
    static bool ansiEnabled = false;
    if (!ansiEnabled) {
        enableAnsiSupport();
        ansiEnabled = true;
    }
    char tempBuffer[256];
    for (int i = 0; i < fieldCount; i++) {
        bool valid_input = false;
        while (!valid_input) {
            printf("%s", fields[i].prompt);
            readLine(tempBuffer, sizeof(tempBuffer));
            valid_input = isValid(tempBuffer, fields[i].validationType, fields[i].validationParams);
            if (valid_input) {
                // Copy up to bufferSize-1 chars to the actual field buffer
                strncpy(fields[i].buffer, tempBuffer, fields[i].bufferSize - 1);
                fields[i].buffer[fields[i].bufferSize - 1] = '\0';
            }
        }
    }
}