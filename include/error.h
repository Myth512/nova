#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

void printError(const char *format, ...);

void printCodeLine(const char *source, int line);

void printArrow(int offset, int line);

#endif