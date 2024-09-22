#ifndef TOKEN_H
#define TOKEN_H

#include "memory.h"

typedef enum {
    T_LEFT_PAREN, T_RIGHT_PAREN,
    T_LEFT_BRACE, T_RIGHT_BRACE,
    T_LEFT_BRACKET, T_RIGHT_BRACKET,
    T_COMMA, T_DOT, T_MINUS, T_PLUS,
    T_SLASH, T_STAR, T_MOD,

    T_BANG, T_BANG_EQUAL,
    T_EQUAL, T_DOUBLE_EQUAL,
    T_GREATER, T_GREATER_EQUAL,
    T_LESS, T_LESS_EQUAL,

    T_IDENTIFIER, T_STRING, T_NUMBER,

    T_VAR, T_IF, T_ELSE, T_FOR, T_WHILE,
    T_AND, T_OR, T_TRUE, T_FALSE, T_NIL,
    T_DEF, T_RETURN, T_CLASS, T_SELF, T_SUPER, 
    T_PRINT,

    T_ERROR, T_EOS, T_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    int lenght;
    int line;
    int column;
} Token;

typedef struct {
    int size;
    int capacity;
    Token *tokens;
    Token *top;
} TokenStack;

void TokenStackPush(TokenStack *stack, Token token);

Token TokenStackPop(TokenStack *stack);

#endif