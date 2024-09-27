#ifndef DEBUG_H
#define DEBUG_H

#include "token.h"
#include "code.h"

void printCodeVec(CodeVec *codeVec, const char *title);

void printValueVec(ValueVec *vec);

int printInstruction(CodeVec *codeVec, int offset);

void printToken(Token *token);

#endif