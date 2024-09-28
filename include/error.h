#ifndef ERROR_H
#define ERROR_H

#include "token.h"

void printError(const char *format, ...);

const char* getCodeLine(const char *source, int line);

void printCodeLine(const char *codeLine, int line);

void printArrow(const char *codeLine, int start, int length);

void printHightlightedWordInCode(const char *codeLine, int line, int column, int length);

void printTokenInCode(const char *source, Token *token);

void printHighlightedPartInCode(const char *source, int line, int column, int length);

#endif