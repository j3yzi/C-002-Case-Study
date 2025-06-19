#include "../../include/apctxt.h"


static void read_line(char* buffer, int size) {
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

static bool is_valid(char* input, IValidationType type, IValidationParams params) {
    if (input[0] == '\0') {
        printf("   [Error] Empty input. Please enter a value.\n");
        return false;
    }

    switch (type) {
        case IV_NONE:
            return true;

        case IV_MAX_LEN:
            if (strlen(input) > params.rangeInt.max) {
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
        default:
            return true;
    }
}

void appGetValidatedInput(appFormField* fields, int fieldCount) {
    for (int i = 0; i < fieldCount; i++) {
        bool valid_input = false;
        int attempts = 0;
        
        while (!valid_input) {
            if (attempts > 0) {
                printf("\n");
            }
            printf("%s", fields[i].prompt);
            
            read_line(fields[i].buffer, fields[i].bufferSize);
            
            valid_input = is_valid(fields[i].buffer, fields[i].validationType, fields[i].validationParams);
            
            attempts++;
        }
    }
}