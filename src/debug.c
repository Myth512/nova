#include <stdio.h>

#include "debug.h"

static int simpleInstruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char *name, CodeVec *vec, int offset) {
    uint8_t constant = vec->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(vec->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

void printCodeVec(CodeVec *vec, const char *title) {
    printf("--- %s ---\n", title);

    for (int offset = 0; offset < vec->size;) {
        offset = printInstruction(vec, offset);
    }
}

void printValueVec(ValueVec *vec) {
    for (int i = 0; i < vec->size; i++) {
        printValue(vec->values[i]);
        printf(" ");
    }
    printf("\n");
}

int printInstruction(CodeVec *vec, int offset) {
    printf("%04d ", offset);
    if (offset > 0 && vec->lines[offset] == vec->lines[offset - 1])
        printf("     | ");
    else
        printf("%4d | ", vec->lines[offset]);

    uint8_t opcode = vec->code[offset];
    switch (opcode) {
        case OP_CONSTANT:
            return constantInstruction("CONSTANT", vec, offset);
        case OP_NIL:
            return simpleInstruction("NIL", offset);
        case OP_POP:
            return simpleInstruction("POP", offset);
        case OP_FALSE:
            return simpleInstruction("FALSE", offset);
        case OP_TRUE:
            return simpleInstruction("TRUE", offset);
        case OP_GET_GLOBAL:
            return constantInstruction("GET GLOBAL", vec, offset);
        case OP_DEFINE_GLOBAL:
            return constantInstruction("DEFINE GLOBAL", vec, offset);
        case OP_SET_GLOBAL:
            return constantInstruction("SET GLOBAL", vec, offset);
        case OP_ADD:
            return simpleInstruction("ADD", offset);
        case OP_SUBTRUCT:
            return simpleInstruction("SUBTRUCT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("DIVIDE", offset);
        case OP_MOD:
            return simpleInstruction("MOD", offset);
        case OP_NOT:
            return simpleInstruction("NOT", offset);
        case OP_NEGATE:
            return simpleInstruction("NEGATE", offset);
        case OP_PRINT:
            return simpleInstruction("PRINT", offset);
        case OP_RETURN:
            return simpleInstruction("RETURN", offset);
        default:
            printf("Unknown opcode %d\n", opcode);
            return offset + 1;
    }
}

static char* decodeTokenType(TokenType type) {
    switch (type) {
        case TOKEN_LEFT_PAREN:
            return "LEFT PAREN";
        case TOKEN_RIGHT_PAREN:
            return "RIGHT PAREN";
        case TOKEN_LEFT_BRACE:
            return "LEFT BRACE";
        case TOKEN_RIGHT_BRACE:
            return "RIGHT BRACE";
        case TOKEN_LEFT_BRACKET:
            return "LEFT BRACKET";
        case TOKEN_RIGHT_BRACKET:
            return "RIGHT BRACKET";
        case TOKEN_COMMA:
            return "COMMA";
        case TOKEN_DOT:
            return "DOT";
        case TOKEN_MINUS:
            return "MINUS";
        case TOKEN_PLUS:
            return "PLUS";
        case TOKEN_SLASH:
            return "SLASH";
        case TOKEN_STAR:
            return "STAR";
        case TOKEN_MOD:
            return "MOD";
        case TOKEN_MINUS_EQUAL:
            return "MINUS EQUAL";
        case TOKEN_PLUS_EQUAL:
            return "PLUS EQUAL";
        case TOKEN_SLASH_EQUAL:
            return "SLASH EQUAL";
        case TOKEN_STAR_EQUAL:
            return "STAR EQUAL";
        case TOKEN_MOD_EQUAL:
            return "MOD EQUAL";
        case TOKEN_BANG:
            return "BANG";
        case TOKEN_BANG_EQUAL:
            return "BANG EQUAL";
        case TOKEN_EQUAL:
            return "EQUAL";
        case TOKEN_DOUBLE_EQUAL:
            return "DOUBLE EQUAL";
        case TOKEN_GREATER:
            return "GREATER";
        case TOKEN_GREATER_EQUAL:
            return "GREATER EQUAL";
        case TOKEN_LESS:
            return "LESS";
        case TOKEN_LESS_EQUAL:
            return "LESS EQUAL";
        case TOKEN_IDENTIFIER:
            return "IDENTIFIER";
        case TOKEN_STRING:
            return "STRING";
        case TOKEN_NUMBER:
            return "NUMBER";
        case TOKEN_VAR:
            return "VAR";
        case TOKEN_IF:
            return "IF";
        case TOKEN_ELSE:
            return "ELSE";
        case TOKEN_FOR:
            return "FOR";
        case TOKEN_WHILE:
            return "WHILE";
        case TOKEN_AND:
            return "AND";
        case TOKEN_OR:
            return "OR";
        case TOKEN_TRUE:
            return "TRUE";
        case TOKEN_FALSE:
            return "FALSE";
        case TOKEN_NIL:
            return "NIL";
        case TOKEN_DEF:
            return "DEF";
        case TOKEN_RETURN:
            return "RETURN";
        case TOKEN_CLASS:
            return "CLASS";
        case TOKEN_SELF:
            return "SELF";
        case TOKEN_SUPER:
            return "SUPER";
        case TOKEN_PRINT:
            return "PRINT";
        case TOKEN_ERROR:
            return "ERROR";
        case TOKEN_EOS:
            return "EOS";
        case TOKEN_EOF:
            return "EOF";
        default:
            return "Unknow Token";
    }
}

void printToken(Token *token) {
    printf("line: %d \tcolumn: %d \tlen: %d \ttype: %s \tdata: \t\"", token->line, token->column, token->length, decodeTokenType(token->type));
    const char *c = token->start;
    for (int i = 0; i < token->length; i++) {
        if (*c == '\n') {
            putchar('\\');
            putchar('n');
        } else {
            putchar(*c);
        }
        c++;
    }
    putchar('"');
    putchar('\n');
}