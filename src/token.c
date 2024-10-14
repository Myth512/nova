#include "token.h"
#include "memory.h"

void TokenQueueInit(TokenQueue *queue) {
    queue->tokens = NULL; 
    queue->size = 0;
    queue->capacity = 0; 
}

void TokenQueuePush(TokenQueue *queue, Token token) {
    if (queue->size + 1 >= queue->capacity) {
        int oldCapacity = queue->capacity;
        queue->capacity = GROW_CAPACITY(oldCapacity);
        queue->tokens = GROW_VEC(Token, queue->tokens, oldCapacity, queue->capacity);
        queue->start = queue->start;
    }
    queue->tokens[queue->size++] = token;
}

Token TokenQueuePop(TokenQueue *queue) {
    Token token = *queue->start;
    queue->start++;
    return token; 
}