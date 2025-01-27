#ifndef COMPILER_H
#define COMPILER_H

#include "code.h"
#include "object.h"
#include "object_function.h"
#include "object_module.h"

ObjModule* compile(const char *source, const char *path, const char *name);

void setBasePath(char *path);

void markCompilerRoots();

#endif