#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stdio.h>

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_VEC(type, pointer, oldSize, newSize) \
    (type*)reallocate(pointer, sizeof(type) * (oldSize), sizeof(type) * (newSize))

#define FREE_VEC(type, pointer, oldSize) \
    reallocate(pointer, sizeof(type) * (oldSize), 0)

void* reallocate(void *pointer, size_t oldSize, size_t newSize);

#endif