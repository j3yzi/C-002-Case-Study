#ifndef APCTXT_H
#define APCTXT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

// Configuration for business logic values (not struct sizes)
typedef struct {
    // Payroll settings
    float regularHours;
    float overtimeRate;
    
    // Academic settings
    float passingGrade;
    float minGrade;
    float maxGrade;
} Config;

// Global configuration instance
extern Config g_config;

// Configuration functions
void setDefaultConfig(void);
int saveConfig(const char* config_file);
int loadConfig(const char* config_file);
void printCurrentConfig(void);

// Convenience accessor functions
float getRegularHours(void);
float getOvertimeRate(void);
float getPassingGrade(void);

// File system utility functions (replacements for system() calls)
int appCreateDirectory(const char* dirPath);
int appListFiles(const char* directory, const char* pattern);

typedef struct {
    char* appName;
    char* appVersion;
    char* license;
    char* creator;
} AppConfig;

typedef struct {
    const char* name;
    const char* string;
} Header;

typedef struct {
    char key;
    const char* text;
    const char* description; // Description text to display in the side box
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
    IV_OPTIONAL,
    IV_CHOICES,
    IV_RANGE_INT,
    IV_RANGE_FLT,
    IV_MAX_LEN,
    IV_MAX_LEN_CHARS,
    IV_ALPHA_ONLY,
    IV_ALPHA_ONLY_MAX_LEN,
    IV_OPTIONAL_ALPHA_ONLY_MAX_LEN,
    IV_EXACT_LEN
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
void appMenuSetColor(int textColor, int bgColor);
void appDisplayMenu(const Menu* menu);
char initMenu(Menu* m);
bool appYesNoPrompt(const char* prompt);

// Validation functions
void enableAnsiSupport();
void readLine(char* buffer, int size);
bool isValid(const char* input, IValidationType type, IValidationParams params, const char* fieldName);
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

// Menu creation helper
static inline MenuOption createMenuOption(char key, const char* text, const char* description, bool disabled) {
    return (MenuOption){
        key, text, description, disabled, false, 
        9, 0,  // highlight colors
        7, 0,  // normal colors
        8, 0,  // disabled colors
        NULL   // no callback
    };
}

// Common user interaction helper
static inline void waitForKeypress(const char* message) {
    printf("%s", message ? message : "Press any key to continue...");
    _getch();
}

#endif // APCTXT_H