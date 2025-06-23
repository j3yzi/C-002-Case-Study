#include "../headers/apctxt.h"

void enableAnsiSupport();
void readLine(char* buffer, int size);
bool isValid(const char* input, IValidationType type, IValidationParams params);
void appGetValidatedInput(appFormField* fields, int fieldCount);

/**
 * @brief Enables ANSI escape sequence processing on Windows consoles.
 * @brief This allows for features like colored text. It has no effect on other platforms.
 */
void enableAnsiSupport() {
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

/**
 * @brief Reads a line of input from stdin, safely handling buffer size and trailing newline.
 * @param buffer The character buffer to store the read line into.
 * @param size The total size of the buffer.
 */
void readLine(char* buffer, int size) {
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

/**
 * @brief Validates an input string against a specified validation type and parameters.
 * @param input The input string to validate.
 * @param type The type of validation to perform (e.g., integer range, choice from a list).
 * @param params A union containing the specific parameters for the validation type.
 * @return Returns true if the input is valid, false otherwise.
 */
bool isValid(const char* input, IValidationType type, IValidationParams params) {
    // Special case: IV_OPTIONAL allows empty input
    if (type == IV_OPTIONAL && input[0] == '\0') {
        return true;
    }
    
    if (input[0] == '\0') {
        printf("   [Error] Empty input. Please enter a value.\n");
        return false;
    }

    switch (type) {
        case IV_NONE:
        case IV_OPTIONAL:
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
            
        case IV_ALPHA_ONLY:
            {
                for (int i = 0; input[i] != '\0'; i++) {
                    char c = input[i];
                    // Allow only alphabetic characters and spaces
                    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
                        printf("   [Error] Invalid characters. Only letters and spaces are allowed.\n");
                        return false;
                    }
                }
                return true;
            }
            
        case IV_ALPHA_ONLY_MAX_LEN:
            {
                // First check length
                if (strlen(input) > (size_t)params.maxLengthChars.maxLength) {
                    printf("   [Error] Input too long. Maximum length is %d characters.\n", params.maxLengthChars.maxLength);
                    return false;
                }
                
                // Then check alphabetic characters only
                for (int i = 0; input[i] != '\0'; i++) {
                    char c = input[i];
                    // Allow only alphabetic characters and spaces
                    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
                        printf("   [Error] Invalid characters. Only letters and spaces are allowed.\n");
                        return false;
                    }
                }
                return true;
            }
            
        default:
            return true;
    }
}

/**
 * @brief Prompts the user for input for a series of form fields and validates the input for each.
 * @param fields An array of appFormField structs, each defining a prompt and validation criteria.
 * @param fieldCount The number of fields in the array.
 */
void appGetValidatedInput(appFormField* fields, int fieldCount) {
    static bool ansiEnabled = false;
    if (!ansiEnabled) {
        enableAnsiSupport();
        ansiEnabled = true;
    }
    char tempBuffer[256];
    for (int i = 0; i < fieldCount; i++) {
        bool validInput = false;
        while (!validInput) {
            printf("%s", fields[i].prompt);
            readLine(tempBuffer, sizeof(tempBuffer));
            validInput = isValid(tempBuffer, fields[i].validationType, fields[i].validationParams);
            if (validInput) {
                // Copy up to bufferSize-1 chars to the actual field buffer
                strncpy(fields[i].buffer, tempBuffer, fields[i].bufferSize - 1);
                fields[i].buffer[fields[i].bufferSize - 1] = '\0';
            }
        }
    }
}