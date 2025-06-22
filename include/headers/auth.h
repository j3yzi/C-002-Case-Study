#ifndef AUTH_H
#define AUTH_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char username[50];
    char role[20];
    char hashedPassword[100];
} User;

bool doesUserExist(const char* username, const char* filename);
void createUser(const char* username, const char* role, const char* hashedPassword, const char* filename);
void deleteUser(const char* username, const char* filename);
void updateUser(const char* username, const char* role, const char* hashedPassword, const char* filename);
char* hashPassword(const char* password);
bool verifyPassword(const char* password, const char* hashedPassword);
void hashPasswordInPlace(char* password);
User* authenticateUser(const char* username, const char* password, const char* filename);

#endif