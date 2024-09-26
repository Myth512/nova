#include "token.h"
#include "memory.h"

void TokenStackPush(TokenStack *stack, Token token) {
    if (stack->size + 1 >= stack->capacity) {
        int oldCapacity = stack->capacity;
        stack->capacity = GROW_CAPACITY(oldCapacity);
        stack->tokens = GROW_VEC(Token, stack->tokens, oldCapacity, stack->capacity);
    }

    *stack->top = token;
    stack->top++;
}

Token TokenStackPop(TokenStack *stack) {
    stack->top--;
    return *stack->top;
}