#ifndef TOKEN_H
#define TOKEN_H

#include "memory.h"

typedef enum {
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_BRACKET, TOKEN_RIGHT_BRACKET,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
    TOKEN_SLASH, TOKEN_STAR, TOKEN_MOD,

    TOKEN_MINUS_EQUAL, TOKEN_PLUS_EQUAL,
    TOKEN_SLASH_EQUAL, TOKEN_STAR_EQUAL, TOKEN_MOD_EQUAL,
    TOKEN_BANG, TOKEN_BANG_EQUAL,
    TOKEN_EQUAL, TOKEN_DOUBLE_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,

    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,

    TOKEN_VAR, TOKEN_IF, TOKEN_ELSE, TOKEN_FOR, TOKEN_WHILE,
    TOKEN_AND, TOKEN_OR, TOKEN_TRUE, TOKEN_FALSE, TOKEN_NIL,
    TOKEN_DEF, TOKEN_RETURN, TOKEN_CLASS, TOKEN_SELF, TOKEN_SUPER, 
    TOKEN_PRINT,

    TOKEN_ERROR, TOKEN_EOS, TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    int length;
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