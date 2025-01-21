#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>

#include "token.h"

void initScanner(const char *source);

Token scanToken(bool skip);

#endif