#ifndef AUTH_H
#define AUTH_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char username[50];
    char* role;
    char hashedPassword[100];
} User;

bool doesUserExist(const char* username, const char* filename);
void createUser(const char* username, const char* role, const char* hashedPassword, const char* filename);
void deleteUser(const char* username, const char* filename);
void updateUser(const char* username, const char* newRole, const char* newHashedPassword, const char* filename);
int hashPassword(const char* password, char* buffer, size_t buffer_size);
bool verifyPassword(const char* password, const char* hashedPassword);
void hashPasswordInPlace(char* password, size_t buffer_size);
int authenticateUser(const char* username, const char* password, const char* filename, User** user_out);

#endif