#include <stdio.h>
#include <stdarg.h>

#include "error.h"
#include "token.h"

void reportRuntimeError(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "\033[31mRuntime Error\033[0m: ");
    vprintf(format, args);
    printf("\n");
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
    offset = 0;
    for (; offset < length / 2; offset++)
        putchar('~');
    putchar('^');
    for (; offset < length - 1; offset++)
        putchar('~');
    printf("\033[0m");
}

void printTokenInCode(const char *source, Token *token) {
    const char *codeLine = getCodeLine(source, token->line);
    printHightlightedWordInCode(codeLine, token->line, token->column, token->length);
    printArrow(codeLine, token->column, token->length);
    putchar('\n');
}

void printHighlightedPartInCode(const char *source, int line, int column, int length) {
    const char *codeLine = getCodeLine(source, line);
    printHightlightedWordInCode(codeLine, line, column, length);
    printArrow(codeLine, column, length);
    putchar('\n');
}

void print2HighlightedPartsInCode(const char *source, int line1, int column1, int length1, int line2, int column2, int length2) {
    if (line1 == line2) {
        const char *codeLine = getCodeLine(source, line1);
        if (column1 > column2) {
            int tmp = column1;
            column1 = column2;
            column2 = tmp;
        }

        printf("%4d | ", line1);
        const char *c = codeLine;
        int curColumn = 1;
        for (; *c != '\0' && curColumn < column1; c++, curColumn++)
            putchar(*c);

        printf("\033[31m");
        for (int i = 0; *c != '\n' && *c != '\0' && i < length1; c++, i++, curColumn++)
            putchar(*c);
        printf("\033[0m");

        for (; *c != '\0' && curColumn < column2; c++, curColumn++)
            putchar(*c);

        printf("\033[31m");
        for (int i = 0; *c != '\n' && *c != '\0' && i < length2; c++, i++, curColumn++)
            putchar(*c);
        printf("\033[0m");

        for (; *c != '\0' && *c != '\n'; c++)
            putchar(*c);

        putchar('\n');
        printArrow(codeLine, column2, length2);        
        putchar('\r');
        printArrow(codeLine, column1, length1);        
        putchar('\n');
    } else {
        printHighlightedPartInCode(source, line1, column1, length1);
        printHighlightedPartInCode(source, line2, column2, length2);
    }
}