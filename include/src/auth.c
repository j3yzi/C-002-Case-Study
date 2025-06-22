#include "../headers/auth.h"
#include <time.h>

// Initialize random seed for salt generation
static void initRandom() {
    static bool initialized = false;
    if (!initialized) {
        srand(time(NULL));
        initialized = true;
    }
}

bool doesUserExist(const char* username, const char* filename) {
    // Check for NULL username or filename
    if (username == NULL || filename == NULL) {
        return false;
    }
    
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening user data file");
        return false;  // Return false when file doesn't exist
    }

    char line[256];
    char fileUsername[50];
    char role[20];
    char hashedPassword[100];
    
    // Skip comments and empty lines
    while (fgets(line, sizeof(line), file)) {
        // Skip comment lines and empty lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }
        
        // Parse the line: username|role|hashed_password
        if (sscanf(line, "%49[^|]|%19[^|]|%99s", fileUsername, role, hashedPassword) == 3) {
            if (strcmp(username, fileUsername) == 0) {
                fclose(file);
                return true;
            }
        }
    }
    
    fclose(file);
    return false;
}

void createUser(const char* username, const char* role, const char* hashedPassword, const char* filename) {
    // Check for NULL parameters
    if (username == NULL || role == NULL || hashedPassword == NULL || filename == NULL) {
        printf("Error: Invalid parameters for user creation\n");
        return;
    }
    
    FILE* file = fopen(filename, "a");

    if (file == NULL) {
        perror("Error opening user data file");
        printf("Note: Make sure the directory exists\n");
        return;
    }

    fprintf(file, "%s|%s|%s\n", username, role, hashedPassword);

    fclose(file);
    printf("User created successfully!\n");
}

void deleteUser(const char* username, const char* filename) {
    // Check for NULL parameters
    if (username == NULL || filename == NULL) {
        printf("Error: Invalid parameters for user deletion\n");
        return;
    }
    
    // Create temp filename in the same directory
    char tempFilename[256];
    strcpy(tempFilename, filename);
    strcat(tempFilename, ".temp");
    
    FILE* file = fopen(filename, "r");
    FILE* tempFile = fopen(tempFilename, "w");

    if (file == NULL || tempFile == NULL) {
        perror("Error opening files");
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        return;
    }

    char line[256];
    char fileUsername[50];
    char role[20];
    char hashedPassword[100];
    bool userFound = false;

    // Copy all lines except the one to delete
    while (fgets(line, sizeof(line), file)) {
        // Skip comment lines and empty lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            fprintf(tempFile, "%s", line);
            continue;
        }

        if (sscanf(line, "%49[^|]|%19[^|]|%99s", fileUsername, role, hashedPassword) == 3) {
            if (strcmp(username, fileUsername) == 0) {
                userFound = true;
                // Skip this line (don't write it to temp file)
            } else {
                fprintf(tempFile, "%s", line);
            }
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Replace original file with temp file
    remove(filename);
    rename(tempFilename, filename);

    if (userFound) {
        printf("User deleted successfully!\n");
    } else {
        printf("User not found!\n");
    }
}

void updateUser(const char* username, const char* newRole, const char* newHashedPassword, const char* filename) {
    // Check for NULL parameters
    if (username == NULL || newRole == NULL || newHashedPassword == NULL || filename == NULL) {
        printf("Error: Invalid parameters for user update\n");
        return;
    }
    
    // Create temp filename in the same directory
    char tempFilename[256];
    strcpy(tempFilename, filename);
    strcat(tempFilename, ".temp");
    
    FILE* file = fopen(filename, "r");
    FILE* tempFile = fopen(tempFilename, "w");

    if (file == NULL || tempFile == NULL) {
        perror("Error opening files");
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        return;
    }

    char line[256];
    char fileUsername[50];
    char role[20];
    char hashedPassword[100];
    bool userFound = false;

    // Copy all lines, updating the target user
    while (fgets(line, sizeof(line), file)) {
        // Skip comment lines and empty lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            fprintf(tempFile, "%s", line);
            continue;
        }

        if (sscanf(line, "%49[^|]|%19[^|]|%99s", fileUsername, role, hashedPassword) == 3) {
            if (strcmp(username, fileUsername) == 0) {
                // Write updated user data
                fprintf(tempFile, "%s|%s|%s\n", username, newRole, newHashedPassword);
                userFound = true;
            } else {
                fprintf(tempFile, "%s", line);
            }
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Replace original file with temp file
    remove(filename);
    rename(tempFilename, filename);

    if (userFound) {
        printf("User updated successfully!\n");
    } else {
        printf("User not found!\n");
    }
}

char* hashPassword(const char* password) {
    // Check for NULL password
    if (password == NULL) {
        return NULL;
    }
    
    // Allocate memory for the hashed password
    char* hashedPassword = malloc(100 * sizeof(char));
    if (hashedPassword == NULL) {
        return NULL;
    }
    
    int len = strlen(password);
    int hashIndex = 0;
    
    // Caesar cipher based hashing with variable shifts
    for (int i = 0; i < len && hashIndex < 99; i++) {
        char c = password[i];
        
        // Use position-based shift for Caesar cipher
        int shift = (i + 1) * 3 + 7; // Variable shift based on position
        
        // Apply Caesar cipher shift
        if (c >= 'a' && c <= 'z') {
            c = ((c - 'a' + shift) % 26) + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            c = ((c - 'A' + shift) % 26) + 'A';
        } else if (c >= '0' && c <= '9') {
            c = ((c - '0' + shift) % 10) + '0';
        } else {
            // For special characters, shift within printable ASCII range
            c = ((c - 33 + shift) % 94) + 33;
        }
        
        hashedPassword[hashIndex++] = c;
        
        // Add Caesar-shifted salt character
        if (hashIndex < 99) {
            char salt = 'A' + ((i * 5 + shift) % 26);
            hashedPassword[hashIndex++] = salt;
        }
    }
    
    // Add final Caesar-shifted characters based on password length
    while (hashIndex < 99 && hashIndex < len * 2 + 8) {
        char finalChar = 'a' + ((len + hashIndex) % 26);
        hashedPassword[hashIndex++] = finalChar;
    }
    
    hashedPassword[hashIndex] = '\0';
    return hashedPassword;
}

bool verifyPassword(const char* password, const char* hashedPassword) {
    // Check for NULL parameters
    if (password == NULL || hashedPassword == NULL) {
        return false;
    }
    
    char* newHash = hashPassword(password);
    if (newHash == NULL) {
        return false;
    }
    
    bool result = (strcmp(newHash, hashedPassword) == 0);
    free(newHash);
    return result;
}

void hashPasswordInPlace(char* password) {
    // Check for NULL password
    if (password == NULL) {
        return;
    }
    
    char* hashed = hashPassword(password);
    if (hashed != NULL) {
        strncpy(password, hashed, 99);
        password[99] = '\0';
        free(hashed);
    }
}

User* authenticateUser(const char* username, const char* password, const char* filename) {
    // Check for NULL parameters
    if (username == NULL || password == NULL || filename == NULL) {
        return NULL;
    }
    
    // Check if user exists first
    if (!doesUserExist(username, filename)) {
        return NULL;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening user data file");
        return NULL;
    }
    
    char line[256];
    char fileUsername[50];
    char role[20];
    char hashedPassword[100];
    
    // Search for the user and verify password
    while (fgets(line, sizeof(line), file)) {
        // Skip comment lines and empty lines
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }
        
        // Parse the line: username|role|hashed_password
        if (sscanf(line, "%49[^|]|%19[^|]|%99s", fileUsername, role, hashedPassword) == 3) {
            if (strcmp(username, fileUsername) == 0) {
                // Found the user, now verify password
                if (verifyPassword(password, hashedPassword)) {
                    // Password is correct, create and return User struct
                    User* user = malloc(sizeof(User));
                    if (user != NULL) {
                        strncpy(user->username, fileUsername, sizeof(user->username) - 1);
                        user->username[sizeof(user->username) - 1] = '\0';
                        strncpy(user->role, role, sizeof(user->role) - 1);
                        user->role[sizeof(user->role) - 1] = '\0';
                        strncpy(user->hashedPassword, hashedPassword, sizeof(user->hashedPassword) - 1);
                        user->hashedPassword[sizeof(user->hashedPassword) - 1] = '\0';
                    }
                    fclose(file);
                    return user;
                } else {
                    // Password is incorrect
                    fclose(file);
                    return NULL;
                }
            }
        }
    }
    
    fclose(file);
    return NULL;
}
