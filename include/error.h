#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

#include "token.h"

void printError(const char *format, ...);

void printTokenInCode(const char *source, Token token);

#endif