#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "token.h"
#include "error.h"

void initScanner(const char *source);

Token scanToken(bool skipNewline);

#endif