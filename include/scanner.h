#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>

#include "token.h"

typedef struct Scanner {
    struct Scanner *enclosing;
    const char *source;
    const char *start;
    const char *current;
    int line;
    int column;
    int startLine;
    int startColumn;
    int indentationStack[256];
    int indentationPointer;
    int indent;
    bool inFormattedString;
    char stop;
} Scanner;

char *readFile(const char *path);

void initScanner(Scanner *s, const char *source);

void endScanner();

Token scanToken(bool skip);

#endif