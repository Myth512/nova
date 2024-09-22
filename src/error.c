#include "error.h"

void printError(const char *format, ...) {
    return;
}

void printCodeLine(const char *source, int line) {
    printf("%d | ", line);
    int curLine = 1;
    char *lineStart = source;
    for (; lineStart != '\0' && curLine < line; lineStart++)
        curLine += (*lineStart == '\n');

    for (; *lineStart != '\0' && *lineStart != '\n'; lineStart++)
        putchar(*lineStart);
    putchar('\n');
}

void printArrow(int offset, int line) {
    while (line /= 10)
        putchar(' ');
    printf("  |");
    for (int i = 0; i < offset; i++) {
        putchar(' ');
    }
    putchar('^');
    putchar('\n');
}