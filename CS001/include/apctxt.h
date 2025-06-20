#ifndef APCTXT_H
#define APCTXT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

typedef struct {
    const char* appName;
    const char* appVersion;
    const char* license;
    const char* creator;
} AppConfig;

typedef struct {
    const char* name;
    const char* string;
} Header;

typedef struct {
    char key;
    const char* text;
} MenuOption;

typedef enum {
    IV_NONE,
    IV_CHOICES,
    IV_RANGE_INT,
    IV_RANGE_FLT,
    IV_MAX_LEN,
    IV_MAX_LEN_CHARS,
} IValidationType;

typedef union {
    // V_CHOICES
    struct {
        const char** choices;
        int count;
    } choices;
    // V_RANGE_INT
    struct {
        long min;
        long max;
    } rangeInt;
    // V_RANGE_FLT
    struct {
        double min;
        double max;
    } rangeFloat;
    struct {
        int maxLength;
    } maxLengthChars;
} IValidationParams;

typedef struct {
    const char* prompt;
    char* buffer;
    int bufferSize;
    IValidationType validationType;
    IValidationParams validationParams;
} appFormField;


void appClearScreen();
void appPrintHeader(const Header* header);

// Validation functions
static void read_line(char* buffer, int size);
static bool is_valid(char* input, IValidationType type, IValidationParams params);
void appGetValidatedInput(appFormField* fields, int fieldCount);

char appDisplayMenu(MenuOption options[], int optionCount, const char* disabledOpts);
void appGetValidatedInput(appFormField* fields, int fieldCount);

void setColor(int text, int bg);
void addMenu ();
void printMenu(int selected);
int runMenuLoop();

#endif

