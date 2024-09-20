#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#include "code.h"

void printCodeVec(CodeVec *codeVec, const char *title);

int printInstruction(CodeVec *codeVec, int offset);

#endif