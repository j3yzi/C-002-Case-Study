#include "../headers/auth.h"
#include <time.h>

/**
 * @brief Initializes the random number generator seed.
 * @brief This function should be called once at the start of the application to ensure that
 * @brief generated salts for password hashing are sufficiently random. It uses a static
 * @brief flag to ensure it only runs once.
 */
static void initRandom() {
    static bool initialized = false;
    if (!initialized) {
        srand(time(NULL));
        initialized = true;
    }
}

/**
 * @brief Checks if a user exists in the user data file.
 * @param username The username to search for.
 * @param filename The path to the user data file.
 * @return Returns true if the user is found, false otherwise or if an error occurs.
 */
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
        } else if (line[0] != '\n' && line[0] != '\r' && line[0] != '#') {
            fprintf(stderr, "Warning: Malformed line in user file: %s", line);
        }
    }
    
    fclose(file);
    return false;
}

/**
 * @brief Creates a new user and appends their details to the user data file.
 * @param username The username for the new user.
 * @param role The role assigned to the new user.
 * @param hashedPassword The pre-hashed password for the new user.
 * @param filename The path to the user data file.
 */
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

/**
 * @brief Deletes a user from the user data file.
 * @brief This function works by creating a temporary file, copying all users except the
 * @brief one to be deleted, and then replacing the original file with the temporary one.
 * @param username The username of the user to delete.
 * @param filename The path to the user data file.
 */
void deleteUser(const char* username, const char* filename) {
    // Check for NULL parameters
    if (username == NULL || filename == NULL) {
        printf("Error: Invalid parameters for user deletion\n");
        return;
    }
    
    // Create temp filename in the same directory
    char tempFilename[256];
    snprintf(tempFilename, sizeof(tempFilename), "%s.temp", filename);
    
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
            // Keep malformed lines as they are
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

/**
 * @brief Updates a user's role and/or password in the data file.
 * @brief Similar to deleteUser, this function uses a temporary file to rewrite the user data,
 * @brief applying the new role and password to the specified user.
 * @param username The username of the user to update.
 * @param newRole The new role to assign to the user.
 * @param newHashedPassword The new pre-hashed password for the user.
 * @param filename The path to the user data file.
 */
void updateUser(const char* username, const char* newRole, const char* newHashedPassword, const char* filename) {
    // Check for NULL parameters
    if (username == NULL || newRole == NULL || newHashedPassword == NULL || filename == NULL) {
        printf("Error: Invalid parameters for user update\n");
        return;
    }
    
    // Create temp filename in the same directory
    char tempFilename[256];
    snprintf(tempFilename, sizeof(tempFilename), "%s.temp", filename);
    
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

/**
 * @brief Hashes a password using a custom Caesar cipher-based algorithm.
 * @param password The plaintext password to hash.
 * @param buffer The buffer where the resulting hashed password will be stored.
 * @param buffer_size The size of the output buffer.
 * @return Returns 0 on success, -1 on failure (e.g., invalid arguments).
 */
int hashPassword(const char* password, char* buffer, size_t buffer_size) {
    if (password == NULL || buffer == NULL || buffer_size == 0) {
        return -1; // Invalid arguments
    }
    
    int len = strlen(password);
    int hashIndex = 0;
    
    for (int i = 0; i < len && hashIndex < buffer_size - 1; i++) {
        char c = password[i];
        
        int shift = (i + 1) * 3 + 7;
        
        if (c >= 'a' && c <= 'z') c = ((c - 'a' + shift) % 26) + 'a';
        else if (c >= 'A' && c <= 'Z') c = ((c - 'A' + shift) % 26) + 'A';
        else if (c >= '0' && c <= '9') c = ((c - '0' + shift) % 10) + '0';
        else c = ((c - 33 + shift) % 94) + 33;
        
        buffer[hashIndex++] = c;
        
        if (hashIndex < buffer_size - 1) {
            char salt = 'A' + ((i * 5 + shift) % 26);
            buffer[hashIndex++] = salt;
        }
    }
    
    while (hashIndex < buffer_size - 1 && hashIndex < len * 2 + 8) {
        char finalChar = 'a' + ((len + hashIndex) % 26);
        buffer[hashIndex++] = finalChar;
    }
    
    buffer[hashIndex] = '\0';
    return 0; // Success
}

/**
 * @brief Verifies a plaintext password against a stored hash.
 * @param password The plaintext password to verify.
 * @param hashedPassword The stored hash to compare against.
 * @return Returns true if the password matches the hash, false otherwise.
 */
bool verifyPassword(const char* password, const char* hashedPassword) {
    if (password == NULL || hashedPassword == NULL) {
        return false;
    }
    
    char buffer[100];
    if (hashPassword(password, buffer, sizeof(buffer)) != 0) {
        return false;
    }
    
    return strcmp(buffer, hashedPassword) == 0;
}

/**
 * @brief Hashes a password string in place.
 * @brief The original content of the password buffer will be overwritten with its hashed version.
 * @param password A pointer to the string buffer to be hashed.
 * @param buffer_size The size of the password buffer.
 */
void hashPasswordInPlace(char* password, size_t buffer_size) {
    if (password == NULL || buffer_size == 0) {
        return;
    }
    
    char temp[buffer_size];
    strcpy(temp, password);
    
    hashPassword(temp, password, buffer_size);
}

/**
 * @brief Authenticates a user by checking their username and password against the data file.
 * @brief If authentication is successful, this function allocates memory for a User struct
 * @brief containing the user's details. The caller is responsible for freeing this memory
 * @brief by calling freeUser().
 * @param username The username to authenticate.
 * @param password The plaintext password to verify.
 * @param filename The path to the user data file.
 * @param user_out A double pointer to a User struct that will be populated on success.
 * @return Returns 0 on success, -1 on failure (e.g., user not found, password mismatch, file error).
 */
int authenticateUser(const char* username, const char* password, const char* filename, User** user_out) {
    if (username == NULL || password == NULL || filename == NULL || user_out == NULL) {
        return -1; // Invalid arguments
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1; // File error
    }

    char line[256];
    char fileUsername[50];
    char role[20];
    char fileHashedPassword[100];

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        if (sscanf(line, "%49[^|]|%19[^|]|%99s", fileUsername, role, fileHashedPassword) == 3) {
            if (strcmp(username, fileUsername) == 0) {
                if (verifyPassword(password, fileHashedPassword)) {
                    fclose(file);
                    User* foundUser = (User*)malloc(sizeof(User));
                    if (foundUser == NULL) {
                        return -1; // Memory allocation failed
                    }
                    strcpy(foundUser->username, fileUsername);
                    foundUser->role = strdup(role);
                    if (foundUser->role == NULL) {
                        free(foundUser);
                        return -1;
                    }
                    strcpy(foundUser->hashedPassword, fileHashedPassword);
                    *user_out = foundUser;
                    return 0; // Success
                } else {
                    fclose(file);
                    return -1; // Password mismatch
                }
            }
        } else if (line[0] != '\n' && line[0] != '\r' && line[0] != '#') {
            fprintf(stderr, "Warning: Malformed line in user file: %s", line);
        }
    }

    fclose(file);
    return -1; // User not found
}
