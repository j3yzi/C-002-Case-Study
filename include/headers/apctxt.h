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
    bool isDisabled;
    bool isSelected;
    int highlightTextColor; // Color for highlighting the option
    int highlightBgColor;   // Background color for highlighting the option
    int textColor;      // Color for the text of the option
    int bgColor;       // Background color for the option
    int disabledTextColor; // Color for disabled options
    int disabledBgColor; // Background Color for disabled options
    void (*onSelect)(void);
} MenuOption;

typedef struct {
    int id;
    const char* name;
    MenuOption* options;
    int optionCount;
} Menu;

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

typedef struct {
    bool isEnabled;
} appState;

// Menu functions
static void appMenuSetColor(int textColor, int bgColor);
static void appDisplayMenu(Menu* menu);
char initMenu(Menu* m);

// Validation functions
static void enableAnsiSupport();
static void readLine(char* buffer, int size);
static bool isValid(char* input, IValidationType type, IValidationParams params);
void appGetValidatedInput(appFormField* fields, int fieldCount);

typedef struct {
    int height;
    int width;
} winTermSize;

typedef struct {
    int x;
    int y;
} winTermCursorPos;

// Window Terminal functions
void appInitWinTerm(const char* title);
void winTermSetCursor(int x, int y);
void winTermClearLine();
void winTermClearScreen();
void winTermGetCursorPosition(winTermCursorPos* position);
void winTermResetColors();

// void winTermPrintLine(char ch, int width);
// void winTermPrintCentered(const char* text, int width);

#endif // APCTXT_H