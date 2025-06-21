#include "../../include/apctxt.h"

static HANDLE hConsole;
static CONSOLE_SCREEN_BUFFER_INFO originalConsoleInfo;

void appInitWinTerm(const char* title) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &originalConsoleInfo);
    SetConsoleTitle(title);
}

void winTermSetCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

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

void winTermGetCursorPosition(winTermCursorPos* position) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    position->x = csbi.dwCursorPosition.X;
    position->y = csbi.dwCursorPosition.Y;
}

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