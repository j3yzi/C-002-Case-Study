/**
 * @file apclrs.h
 * @brief Text color definitions for console applications
 * @date June 21, 2025
 * 
 * This header provides color code macros for ANSI color formatting
 * in console applications. These can be used to enhance the user interface
 * across different case study projects.
 */

#ifndef __APCLRS_H__
#define __APCLRS_H__

/* Text Colors */
#define TXT_RESET   "\033[0m"       /* Reset to default color */
#define TXT_BLACK   "\033[30m"      /* Black text */
#define TXT_RED     "\033[31m"      /* Red text */
#define TXT_GREEN   "\033[32m"      /* Green text */
#define TXT_YELLOW  "\033[33m"      /* Yellow text */
#define TXT_BLUE    "\033[34m"      /* Blue text */
#define TXT_MAGENTA "\033[35m"      /* Magenta text */
#define TXT_CYAN    "\033[36m"      /* Cyan text */
#define TXT_WHITE   "\033[37m"      /* White text */

/* Bright/Bold Text Colors */
#define TXT_BRIGHT_BLACK   "\033[90m"   /* Bright Black (Gray) text */
#define TXT_BRIGHT_RED     "\033[91m"   /* Bright Red text */
#define TXT_BRIGHT_GREEN   "\033[92m"   /* Bright Green text */
#define TXT_BRIGHT_YELLOW  "\033[93m"   /* Bright Yellow text */
#define TXT_BRIGHT_BLUE    "\033[94m"   /* Bright Blue text */
#define TXT_BRIGHT_MAGENTA "\033[95m"   /* Bright Magenta text */
#define TXT_BRIGHT_CYAN    "\033[96m"   /* Bright Cyan text */
#define TXT_BRIGHT_WHITE   "\033[97m"   /* Bright White text */

/* Background Colors */
#define BG_BLACK   "\033[40m"      /* Black background */
#define BG_RED     "\033[41m"      /* Red background */
#define BG_GREEN   "\033[42m"      /* Green background */
#define BG_YELLOW  "\033[43m"      /* Yellow background */
#define BG_BLUE    "\033[44m"      /* Blue background */
#define BG_MAGENTA "\033[45m"      /* Magenta background */
#define BG_CYAN    "\033[46m"      /* Cyan background */
#define BG_WHITE   "\033[47m"      /* White background */

/* Bright Background Colors */
#define BG_BRIGHT_BLACK   "\033[100m"   /* Bright Black (Gray) background */
#define BG_BRIGHT_RED     "\033[101m"   /* Bright Red background */
#define BG_BRIGHT_GREEN   "\033[102m"   /* Bright Green background */
#define BG_BRIGHT_YELLOW  "\033[103m"   /* Bright Yellow background */
#define BG_BRIGHT_BLUE    "\033[104m"   /* Bright Blue background */
#define BG_BRIGHT_MAGENTA "\033[105m"   /* Bright Magenta background */
#define BG_BRIGHT_CYAN    "\033[106m"   /* Bright Cyan background */
#define BG_BRIGHT_WHITE   "\033[107m"   /* Bright White background */

/* Text Styles */
#define TXT_BOLD      "\033[1m"     /* Bold text */
#define TXT_DIM       "\033[2m"     /* Dim text */
#define TXT_ITALIC    "\033[3m"     /* Italic text */
#define TXT_UNDERLINE "\033[4m"     /* Underlined text */
#define TXT_BLINK     "\033[5m"     /* Blinking text */
#define TXT_REVERSE   "\033[7m"     /* Reversed colors */
#define TXT_HIDDEN    "\033[8m"     /* Hidden text */

/* Common UI Elements */
#define UI_HEADER     TXT_BRIGHT_CYAN
#define UI_SUBHEADER  TXT_CYAN
#define UI_HIGHLIGHT  TXT_BRIGHT_YELLOW
#define UI_SUCCESS    TXT_BRIGHT_GREEN
#define UI_WARNING    TXT_BRIGHT_YELLOW
#define UI_ERROR      TXT_BRIGHT_RED
#define UI_INFO       TXT_BRIGHT_BLUE
#define UI_PROMPT     TXT_BRIGHT_MAGENTA
#define UI_INPUT      TXT_BRIGHT_WHITE
#define UI_DISABLED   TXT_DIM

/**
 * @brief Usage example:
 * 
 * #include "apclrs.h"
 * #include <stdio.h>
 * 
 * int main() {
 *     printf(UI_HEADER "Application Header\n" TXT_RESET);
 *     printf(UI_SUBHEADER "Menu Options:\n" TXT_RESET);
 *     printf(UI_HIGHLIGHT "1. " TXT_RESET "Option One\n");
 *     printf(UI_ERROR "Error: " TXT_RESET "Something went wrong\n");
 *     return 0;
 * }
 */

#endif /* __APCLRS_H__ */
