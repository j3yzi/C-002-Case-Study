#include "../headers/apctxt.h"

static HANDLE hConsole;
static CONSOLE_SCREEN_BUFFER_INFO originalConsoleInfo;

/**
 * @brief Initializes the Windows terminal environment.
 * @brief This function gets the standard output handle, saves the original console attributes for later restoration,
 * @brief and sets the console window title.
 * @param title The string to be set as the console window title.
 */
void appInitWinTerm(const char* title) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &originalConsoleInfo);
    SetConsoleTitle(title);
}

/**
 * @brief Sets the console cursor position to the specified coordinates.
 * @param x The x-coordinate (column).
 * @param y The y-coordinate (row).
 */
void winTermSetCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

/**
 * @brief Clears the current line from the cursor's position to the end.
 */
void winTermClearLine() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count, cellCount;
    COORD homeCoords = {0, 0};
    
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    cellCount = csbi.dwSize.X;
    homeCoords.Y = csbi.dwCursorPosition.Y;
    homeCoords.X = 0;
    
    FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hConsole, homeCoords);
}

/**
 * @brief Clears the entire console screen.
 */
void winTermClearScreen() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    COORD homeCoords = {0, 0};
    
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    
    FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hConsole, homeCoords);
}

/**
 * @brief Gets the current position of the console cursor.
 * @param position A pointer to a winTermCursorPos struct where the cursor's X and Y coordinates will be stored.
 */
void winTermGetCursorPosition(winTermCursorPos* position) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    position->x = csbi.dwCursorPosition.X;
    position->y = csbi.dwCursorPosition.Y;
}

/**
 * @brief Resets the console text attributes (colors) to their original values.
 */
void winTermResetColors() {
    SetConsoleTextAttribute(hConsole, originalConsoleInfo.wAttributes);
}

// void winTermPrintLine(char ch, int width) {
//     for (int i = 0; i < width; i++) printf("%c", ch);
//     printf("\n");
// }

// void winTermPrintCentered(const char* text, int width) {
//     int len = strlen(text);
//     int padding = (width - len) / 2;
//     for (int i = 0; i < padding; i++) printf(" ");
//     printf("%s\n", text);
// }