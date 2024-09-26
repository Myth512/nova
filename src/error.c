#include <stdio.h>

#include "error.h"
#include "token.h"

void printError(const char *format, ...) {
    return;
}

const char* getCodeLine(const char *source, int line) {
    int curLine = 1;
    const char *lineStart = source;
    for (; *lineStart != '\0' && curLine < line; lineStart++)
        curLine += (*lineStart == '\n');
    return lineStart;
}


void printHightlightedWordInCode(const char *codeLine, int line, int column, int length){
    printf("%4d | ", line);
    const char *c = codeLine;
    for (int i = 1; *c != '\0' && i < column; c++, i++)
        putchar(*c);

    printf("\033[31m");
    for (int i = 0; *c != '\n' && *c != '\0' && i < length; c++, i++)
        putchar(*c);
    printf("\033[0m");
    for (; *c != '\0' && *c != '\n'; c++)
        putchar(*c);
    putchar('\n');
}


void printCodeLine(const char *codeLine, int line) {
    printf("%4d | ", line);

    for (const char *c = codeLine; *c != '\0' && *c != '\n'; c++)
        putchar(*c);
    putchar('\n');
}

void printArrow(const char *codeLine, int start, int length) {
    printf("     | ");
    int offset = 0;
    for (const char *c = codeLine; *c != '\0' && offset < start - 1; c++, offset++) {
        if (*c == '\t')
            putchar('\t');
        else
            putchar(' ');
    }
    printf("\033[31m");
    putchar('^');
    for (int i = 0; i < length - 1; i++)
        putchar('~');
    printf("\033[0m\n");
}

void printTokenInCode(const char *source, Token *token) {
    const char *codeLine = getCodeLine(source, token->line);
    printHightlightedWordInCode(codeLine, token->line, token->column, token->length);
    printArrow(codeLine, token->column, token->length);
}