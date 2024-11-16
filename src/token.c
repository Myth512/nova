#include "token.h"
#include "memory.h"

void TokenVecInit(TokenVec *vec) {
    vec->tokens = NULL; 
    vec->size = 0;
    vec->capacity = 0;
}

void TokenVecPush(TokenVec *vec, Token token) {
    if (vec->size == vec->capacity) {
        size_t oldSize = vec->size;
        vec->capacity = GROW_CAPACITY(vec->capacity);
        vec->tokens = GROW_VEC(Token, vec->tokens, oldSize, vec->capacity);
    }

    vec->tokens[vec->size++] = token;
}

void TokenVecFree(TokenVec *vec) {
    if (vec->tokens)
        free(vec->tokens);
}