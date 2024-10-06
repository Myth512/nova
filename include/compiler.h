#ifndef COMPILER_H
#define COMPILER_H

#include "code.h"
#include "object.h"

ObjFunction* compile(const char *source);

#endif