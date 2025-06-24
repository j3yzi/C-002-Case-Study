#include "../headers/apctxt.h"

void enableAnsiSupport();
void readLine(char* buffer, int size);
bool isValid(const char* input, IValidationType type, IValidationParams params, const char* fieldName);
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
                // Define ENABLE_VIRTUAL_TERMINAL_PROCESSING if not defined (for older Windows SDKs)
                #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
                    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
                #endif
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
 * @brief Extracts a more user-friendly field name from the prompt
 * @param prompt The full prompt string (e.g., "Enter employee first name: ")
 * @return A simplified field name (e.g., "First Name")
 */
const char* extractFieldName(const char* prompt) {
    // Common field mappings for better user experience
    if (strstr(prompt, "first name") || strstr(prompt, "First Name")) return "First Name";
    if (strstr(prompt, "last name") || strstr(prompt, "Last Name")) return "Last Name";
    if (strstr(prompt, "middle name") || strstr(prompt, "Middle Name")) return "Middle Name";
    if (strstr(prompt, "employee number") || strstr(prompt, "Employee Number")) return "Employee Number";
    if (strstr(prompt, "student number") || strstr(prompt, "Student Number")) return "Student Number";
    if (strstr(prompt, "hourly rate") || strstr(prompt, "Hourly Rate")) return "Hourly Rate";
    if (strstr(prompt, "hours worked") || strstr(prompt, "Hours Worked")) return "Hours Worked";
    if (strstr(prompt, "deductions") || strstr(prompt, "Deductions")) return "Deductions";
    if (strstr(prompt, "year level") || strstr(prompt, "Year Level")) return "Year Level";
    if (strstr(prompt, "course") || strstr(prompt, "Course")) return "Course";
    if (strstr(prompt, "prelim") || strstr(prompt, "Prelim")) return "Prelim Grade";
    if (strstr(prompt, "midterm") || strstr(prompt, "Midterm")) return "Midterm Grade";
    if (strstr(prompt, "final") || strstr(prompt, "Final")) return "Final Exam Grade";
    if (strstr(prompt, "grade")) return "Grade";
    if (strstr(prompt, "age") || strstr(prompt, "Age")) return "Age";
    if (strstr(prompt, "phone") || strstr(prompt, "Phone")) return "Phone Number";
    if (strstr(prompt, "email") || strstr(prompt, "Email")) return "Email Address";
    if (strstr(prompt, "address") || strstr(prompt, "Address")) return "Address";
    
    // If no specific mapping found, try to extract from the prompt
    // Look for patterns like "Enter [field]: " or "Enter your [field]: "
    const char* enterPos = strstr(prompt, "Enter ");
    if (enterPos) {
        enterPos += 6; // Skip "Enter "
        if (strncmp(enterPos, "your ", 5) == 0) {
            enterPos += 5; // Skip "your "
        }
        // Find the end of the field name (before colon or space)
        const char* endPos = strchr(enterPos, ':');
        if (!endPos) endPos = strchr(enterPos, ' ');
        if (endPos) {
            static char fieldBuffer[64];
            int len = endPos - enterPos;
            if (len > 0 && len < 63) {
                strncpy(fieldBuffer, enterPos, len);
                fieldBuffer[len] = '\0';
                // Capitalize first letter
                if (fieldBuffer[0] >= 'a' && fieldBuffer[0] <= 'z') {
                    fieldBuffer[0] = fieldBuffer[0] - 32;
                }
                return fieldBuffer;
            }
        }
    }
    
    return "Input"; // Fallback
}

/**
 * @brief Validates an input string against a specified validation type and parameters.
 * @param input The input string to validate.
 * @param type The type of validation to perform (e.g., integer range, choice from a list).
 * @param params A union containing the specific parameters for the validation type.
 * @param fieldName The name of the field being validated for specific error messages.
 * @return Returns true if the input is valid, false otherwise.
 */
bool isValid(const char* input, IValidationType type, IValidationParams params, const char* fieldName) {
    // Special case: IV_OPTIONAL allows empty input
    if (type == IV_OPTIONAL && input[0] == '\0') {
        return true;
    }
    
    if (input[0] == '\0') {
        printf("   [Error] %s cannot be empty. Please enter a value.\n", fieldName);
        return false;
    }

    switch (type) {
        case IV_NONE:
        case IV_OPTIONAL:
            return true;

        case IV_MAX_LEN:
            if (strlen(input) > (size_t)params.rangeInt.max) {
                printf("   [Error] %s cannot exceed %ld characters. Current length: %zu\n", 
                       fieldName, params.rangeInt.max, strlen(input));
                return false;
            }
            return true; 

        case IV_CHOICES:
            for (int i = 0; i < params.choices.count; i++) {
                if (strcmp(input, params.choices.choices[i]) == 0) {
                    return true;
                }
            }
            printf("   [Error] Invalid %s '%s'. Valid options are: ", fieldName, input);
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
                if (end == input || *end != '\0') {
                    printf("   [Error] %s must be a valid decimal number. '%s' is not a valid format.\n", 
                           fieldName, input);
                    return false;
                }
                if (val < params.rangeFloat.min || val > params.rangeFloat.max) {
                    printf("   [Error] %s must be between %.2f and %.2f. You entered: %.2f\n", 
                           fieldName, params.rangeFloat.min, params.rangeFloat.max, val);
                    return false;
                }
                return true;
            }

        case IV_RANGE_INT:
            {
                char* end;
                long val = strtol(input, &end, 10);
                if (end == input || *end != '\0') {
                    printf("   [Error] %s must be a valid whole number. '%s' is not a valid format.\n", 
                           fieldName, input);
                    return false;
                }
                if (val < params.rangeInt.min || val > params.rangeInt.max) {
                    printf("   [Error] %s must be between %ld and %ld. You entered: %ld\n", 
                           fieldName, params.rangeInt.min, params.rangeInt.max, val);
                    return false;
                }
                return true;
            }
            
        case IV_MAX_LEN_CHARS:
            if (strlen(input) > (size_t)params.maxLengthChars.maxLength) {
                printf("   [Error] %s cannot exceed %d characters. Current length: %zu\n", 
                       fieldName, params.maxLengthChars.maxLength, strlen(input));
                return false;
            }
            return true;
            
        case IV_ALPHA_ONLY:
            {
                for (int i = 0; input[i] != '\0'; i++) {
                    char c = input[i];
                    // Allow only alphabetic characters and spaces
                    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
                        printf("   [Error] %s can only contain letters and spaces. Invalid character: '%c'\n", 
                               fieldName, c);
                        return false;
                    }
                }
                return true;
            }
            
        case IV_ALPHA_ONLY_MAX_LEN:
            {
                // First check length
                if (strlen(input) > (size_t)params.maxLengthChars.maxLength) {
                    printf("   [Error] %s cannot exceed %d characters. Current length: %zu\n", 
                           fieldName, params.maxLengthChars.maxLength, strlen(input));
                    return false;
                }
                
                // Then check alphabetic characters only
                for (int i = 0; input[i] != '\0'; i++) {
                    char c = input[i];
                    // Allow only alphabetic characters and spaces
                    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
                        printf("   [Error] %s can only contain letters and spaces. Invalid character: '%c'\n", 
                               fieldName, c);
                        return false;
                    }
                }
                return true;
            }
            
        case IV_OPTIONAL_ALPHA_ONLY_MAX_LEN:
            {
                // If empty, it's valid (optional field)
                if (input[0] == '\0') {
                    return true;
                }
                
                // If not empty, validate as alphabetic with max length
                // First check length
                if (strlen(input) > (size_t)params.maxLengthChars.maxLength) {
                    printf("   [Error] %s cannot exceed %d characters. Current length: %zu\n", 
                           fieldName, params.maxLengthChars.maxLength, strlen(input));
                    return false;
                }
                
                // Then check alphabetic characters only
                for (int i = 0; input[i] != '\0'; i++) {
                    char c = input[i];
                    // Allow only alphabetic characters and spaces
                    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ')) {
                        printf("   [Error] %s can only contain letters and spaces. Invalid character: '%c'\n", 
                               fieldName, c);
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
        const char* fieldName = extractFieldName(fields[i].prompt);
        
        while (!validInput) {
            printf("%s", fields[i].prompt);
            readLine(tempBuffer, sizeof(tempBuffer));
            validInput = isValid(tempBuffer, fields[i].validationType, fields[i].validationParams, fieldName);
            if (validInput) {
                // Copy up to bufferSize-1 chars to the actual field buffer
                strncpy(fields[i].buffer, tempBuffer, fields[i].bufferSize - 1);
                fields[i].buffer[fields[i].bufferSize - 1] = '\0';
            }
        }
    }
}