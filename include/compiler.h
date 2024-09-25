#ifndef COMPILER_H
#define COMPILER_H

#include "scanner.h"
#include "debug.h"
#include "code.h"
#include "error.h"
#include "code.h"
#include "common.h"
#include "object.h"

int compile(const char *source, CodeVec *code);

#endif