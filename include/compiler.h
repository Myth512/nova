#ifndef COMPILER_H
#define COMPILER_H

#include "code.h"
#include "object.h"
#include "object_function.h"

ObjFunction* compile(const char *source);

void markCompilerRoots();

#endif