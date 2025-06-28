/**
 * @file apctxt.h
 * @brief Application Context and Configuration Management
 * 
 * This header file defines the core data structures and functions for managing
 * the application's configuration, menu system, form validation, and terminal
 * operations. It serves as the central hub for application-wide settings and
 * utility functions.
 * 
 * @author C002 - Group 1
 * @version 1.0
 * @date 2024
 */

#ifndef APCTXT_H
#define APCTXT_H

// Standard C library includes
#include <stdio.h>      // For file operations and I/O
#include <stdlib.h>     // For memory allocation and general utilities
#include <string.h>     // For string manipulation functions
#include <stdbool.h>    // For boolean data type support
#include <conio.h>      // For console I/O operations
#include <windows.h>    // For Windows-specific terminal functions

/**
 * @struct Config
 * @brief Configuration structure for business logic values
 * 
 * This structure holds configurable values that affect the business logic
 * of the application, such as payroll calculations and academic grading.
 * These values are typically loaded from a configuration file.
 */
typedef struct {
    // Payroll calculation settings
    float regularHours;    // Standard work hours per period (e.g., 40 hours/week)
    float overtimeRate;    // Multiplier for overtime pay (e.g., 1.5 for time-and-a-half)
    
    // Academic grading settings
    float passingGrade;    // Minimum grade required to pass (e.g., 75.0)
    float minGrade;        // Minimum possible grade (e.g., 0.0)
    float maxGrade;        // Maximum possible grade (e.g., 100.0)
} Config;

// Global configuration instance - accessible throughout the application
extern Config g_config;

/**
 * @name Configuration Management Functions
 * @brief Functions for loading, saving, and accessing configuration data
 * @{
 */

/**
 * @brief Sets default configuration values
 * 
 * Initializes the global configuration with sensible default values
 * in case the configuration file cannot be loaded.
 */
void setDefaultConfig(void);

/**
 * @brief Saves current configuration to file
 * @param config_file Path to the configuration file
 * @return 0 on success, non-zero on error
 */
int saveConfig(const char* config_file);

/**
 * @brief Loads configuration from file
 * @param config_file Path to the configuration file
 * @return 0 on success, non-zero on error
 */
int loadConfig(const char* config_file);

/**
 * @brief Prints current configuration to console
 * 
 * Displays all current configuration values for debugging purposes.
 */
void printCurrentConfig(void);

/** @} */ // End of Configuration Management Functions

/**
 * @name Configuration Accessor Functions
 * @brief Convenient functions to access configuration values
 * @{
 */

/**
 * @brief Gets the standard work hours setting
 * @return Regular hours value from configuration
 */
float getRegularHours(void);

/**
 * @brief Gets the overtime rate multiplier
 * @return Overtime rate value from configuration
 */
float getOvertimeRate(void);

/**
 * @brief Gets the passing grade threshold
 * @return Passing grade value from configuration
 */
float getPassingGrade(void);

/** @} */ // End of Configuration Accessor Functions

/**
 * @name File System Utility Functions
 * @brief Cross-platform file system operations
 * @{
 */

/**
 * @brief Creates a directory (replacement for system() calls)
 * @param dirPath Path of the directory to create
 * @return 0 on success, non-zero on error
 */
int appCreateDirectory(const char* dirPath);

/**
 * @brief Lists files in a directory matching a pattern
 * @param directory Directory path to search
 * @param pattern File pattern to match (e.g., "*.dat")
 * @return Number of files found, or negative on error
 */
int appListFiles(const char* directory, const char* pattern);

/** @} */ // End of File System Utility Functions

/**
 * @struct AppConfig
 * @brief Application metadata structure
 * 
 * Contains basic information about the application such as name,
 * version, license, and creator information.
 */
typedef struct {
    char* appName;      // Name of the application
    char* appVersion;   // Version string (e.g., "1.0")
    char* license;      // License type (e.g., "MIT License")
    char* creator;      // Creator or development team name
} AppConfig;

/**
 * @struct Header
 * @brief Simple key-value pair structure
 * 
 * Used for storing header information or configuration pairs.
 */
typedef struct {
    const char* name;   // Header name/key
    const char* string; // Header value/content
} Header;

/**
 * @struct MenuOption
 * @brief Individual menu option configuration
 * 
 * Defines a single menu option with its appearance, behavior, and callback.
 * Includes color settings for different states (normal, highlighted, disabled).
 */
typedef struct {
    char key;                    // Keyboard shortcut for this option
    const char* text;            // Display text for the option
    const char* description;     // Detailed description shown in help area
    bool isDisabled;            // Whether this option is currently disabled
    bool isSelected;            // Whether this option is currently selected
    
    // Color configuration for different states
    int highlightTextColor;     // Text color when option is highlighted
    int highlightBgColor;       // Background color when option is highlighted
    int textColor;              // Normal text color
    int bgColor;                // Normal background color
    int disabledTextColor;      // Text color when option is disabled
    int disabledBgColor;        // Background color when option is disabled
    
    void (*onSelect)(void);     // Callback function when option is selected
} MenuOption;

/**
 * @struct Menu
 * @brief Complete menu structure
 * 
 * Represents a menu with multiple options, including the menu title
 * and an array of menu options.
 */
typedef struct {
    int id;                     // Unique identifier for this menu
    const char* name;           // Menu title/name
    MenuOption* options;        // Array of menu options
    int optionCount;           // Number of options in the menu
} Menu;

/**
 * @enum IValidationType
 * @brief Input validation types for form fields
 * 
 * Defines the different types of validation that can be applied
 * to user input fields in forms.
 */
typedef enum {
    IV_NONE,                    // No validation required
    IV_OPTIONAL,                // Optional field (can be empty)
    IV_CHOICES,                 // Must be one of predefined choices
    IV_RANGE_INT,               // Integer within specified range
    IV_RANGE_FLT,               // Float within specified range
    IV_MAX_LEN,                 // Maximum length constraint
    IV_MAX_LEN_CHARS,           // Maximum character length
    IV_ALPHA_ONLY,              // Alphabetic characters only
    IV_ALPHA_ONLY_MAX_LEN,      // Alphabetic with max length
    IV_OPTIONAL_ALPHA_ONLY_MAX_LEN, // Optional alphabetic with max length
    IV_ALNUM_ONLY_MAX_LEN,      // Alphanumeric with max length
    IV_EXACT_LEN                // Exact length required
} IValidationType;

/**
 * @union IValidationParams
 * @brief Parameters for different validation types
 * 
 * Union that holds different parameter sets depending on the
 * validation type being used.
 */
typedef union {
    // For IV_CHOICES validation
    struct {
        const char** choices;   // Array of valid choices
        int count;              // Number of choices available
    } choices;
    
    // For IV_RANGE_INT validation
    struct {
        long min;               // Minimum allowed value
        long max;               // Maximum allowed value
    } rangeInt;
    
    // For IV_RANGE_FLT validation
    struct {
        double min;             // Minimum allowed value
        double max;             // Maximum allowed value
    } rangeFloat;
    
    // For length-based validations
    struct {
        int maxLength;          // Maximum allowed length
    } maxLengthChars;
} IValidationParams;

/**
 * @struct appFormField
 * @brief Form field definition for user input
 * 
 * Defines a single form field with its prompt, buffer, and validation rules.
 * Used by the form system to collect and validate user input.
 */
typedef struct {
    const char* prompt;         // Text prompt shown to user
    char* buffer;               // Buffer to store user input
    int bufferSize;             // Size of the input buffer
    IValidationType validationType; // Type of validation to apply
    IValidationParams validationParams; // Parameters for validation
} appFormField;

/**
 * @struct appState
 * @brief Application state structure
 * 
 * Holds the current state of the application, such as whether
 * certain features are enabled or disabled.
 */
typedef struct {
    bool isEnabled;             // General enabled/disabled state
} appState;

/**
 * @name Menu System Functions
 * @brief Functions for creating and managing menus
 * @{
 */

/**
 * @brief Sets text and background colors for menu display
 * @param textColor Color code for text (0-15)
 * @param bgColor Color code for background (0-15)
 */
void appMenuSetColor(int textColor, int bgColor);

/**
 * @brief Displays a menu on the screen
 * @param menu Pointer to the menu structure to display
 */
void appDisplayMenu(const Menu* menu);

/**
 * @brief Initializes and runs an interactive menu
 * @param m Pointer to the menu to initialize
 * @return Character key of selected option, or 0 on error
 */
char initMenu(Menu* m);

/**
 * @brief Displays a yes/no prompt to the user
 * @param prompt Text prompt to display
 * @return true if user selected yes, false if no
 */
bool appYesNoPrompt(const char* prompt);

/** @} */ // End of Menu System Functions

/**
 * @name Input Validation Functions
 * @brief Functions for validating user input
 * @{
 */

/**
 * @brief Enables ANSI escape sequence support in console
 * 
 * Required for color support and cursor positioning in Windows console.
 */
void enableAnsiSupport();

/**
 * @brief Reads a line of input from the user
 * @param buffer Buffer to store the input
 * @param size Size of the buffer
 */
void readLine(char* buffer, int size);

/**
 * @brief Validates input against specified criteria
 * @param input Input string to validate
 * @param type Type of validation to perform
 * @param params Parameters for the validation
 * @param fieldName Name of the field (for error messages)
 * @return true if input is valid, false otherwise
 */
bool isValid(const char* input, IValidationType type, IValidationParams params, const char* fieldName);

/**
 * @brief Gets validated input from user using form fields
 * @param fields Array of form field definitions
 * @param fieldCount Number of fields in the array
 */
void appGetValidatedInput(appFormField* fields, int fieldCount);

/** @} */ // End of Input Validation Functions

/**
 * @struct winTermSize
 * @brief Terminal window size structure
 */
typedef struct {
    int height;                 // Terminal height in characters
    int width;                  // Terminal width in characters
} winTermSize;

/**
 * @struct winTermCursorPos
 * @brief Terminal cursor position structure
 */
typedef struct {
    int x;                      // Horizontal cursor position (column)
    int y;                      // Vertical cursor position (row)
} winTermCursorPos;

/**
 * @name Windows Terminal Functions
 * @brief Functions for controlling the Windows terminal
 * @{
 */

/**
 * @brief Initializes the Windows terminal with a title
 * @param title Title to set for the terminal window
 */
void appInitWinTerm(const char* title);

/**
 * @brief Sets the cursor position in the terminal
 * @param x Column position (0-based)
 * @param y Row position (0-based)
 */
void winTermSetCursor(int x, int y);

/**
 * @brief Clears the current line in the terminal
 */
void winTermClearLine();

/**
 * @brief Clears the entire terminal screen
 */
void winTermClearScreen();

/**
 * @brief Gets the current cursor position
 * @param position Pointer to structure to store cursor position
 */
void winTermGetCursorPosition(winTermCursorPos* position);

/**
 * @brief Resets terminal colors to default
 */
void winTermResetColors();

/** @} */ // End of Windows Terminal Functions

/**
 * @name Utility Helper Functions
 * @brief Inline helper functions for common operations
 * @{
 */

/**
 * @brief Creates a menu option with standard settings
 * @param key Keyboard shortcut character
 * @param text Display text for the option
 * @param description Detailed description
 * @param disabled Whether the option should be disabled
 * @return Configured MenuOption structure
 */
static inline MenuOption createMenuOption(char key, const char* text, const char* description, bool disabled) {
    return (MenuOption){
        key, text, description, disabled, false, 
        9, 0,  // highlight colors (bright white on black)
        7, 0,  // normal colors (light gray on black)
        8, 0,  // disabled colors (dark gray on black)
        NULL   // no callback function
    };
}

/**
 * @brief Waits for user to press any key
 * @param message Optional message to display (NULL for default)
 */
static inline void waitForKeypress(const char* message) {
    printf("%s", message ? message : "Press any key to continue...");
    _getch();
}

/** @} */ // End of Utility Helper Functions

#endif // APCTXT_H