#include <stdio.h>

#include "debug.h"
#include "object.h"
#include "value.h"
#include "object_function.h"

static int simpleInstruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int argInstruction(const char *name, CodeVec *vec, int offset) {
    printf("%s\t%d\n", name, vec->code[offset+1]);
    return offset + 2;
}

static int byteInstruction(const char *name, CodeVec *vec, int offset) {
    uint8_t slot = vec->code[offset + 1];
    printf("%-16s %4d\n", name, slot);
    return offset + 2;
}

static int constantInstruction(const char *name, CodeVec *vec, int offset) {
    uint8_t id = vec->code[offset + 1];
    Value value = vec->constants.values[id];
    printf("%-10s %s %4d '", name, decodeValueType(value), id);
    valuePrint(value);
    printf("'\n");
    return offset + 2;
}

static int varInstruction(const char *name, CodeVec *vec, int offset) {
    uint8_t id = vec->code[offset + 1];
    Value value = vec->constants.values[id];
    printf("%-10s %4d '", name, id);
    valuePrint(value);
    printf("'\n");
    return offset + 2;
}

static int jumpInstruction(const char *name, int sign, CodeVec *vec, int offset) {
    uint16_t jump = (uint16_t)(vec->code[offset + 1] << 8);
    jump |= vec->code[offset + 2];
    printf("%-16s %4d -> %d\n", name, offset, offset + 3 + sign * jump);
    return offset + 3;
}

void printCodeVec(CodeVec *vec, const char *title) {
    printf("--- %s ---\n", title);

    for (int offset = 0; offset < vec->size;) {
        offset = printInstruction(vec, offset);
    }
}

void printValueVec(ValueVec *vec) {
    for (int i = 0; i < vec->size; i++) {
        putchar('"');
        valuePrint(vec->values[i]);
        putchar('"');
        printf(" ");
    }
    printf("\n");
}

int printInstruction(CodeVec *vec, int offset) {
    printf("%04d ", offset);
    if (offset > 0 && vec->lines[offset] == vec->lines[offset - 1])
        printf("     %4d | ", vec->columns[offset]);
    else
        printf("%4d %4d | ", vec->lines[offset], vec->columns[offset]);

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
            return varInstruction("GET GLOBAL", vec, offset);
        case OP_DEFINE_GLOBAL:
            return varInstruction("DEFINE GLOBAL", vec, offset);
        case OP_SET_GLOBAL:
            return varInstruction("SET GLOBAL", vec, offset);
        case OP_GET_LOCAL:
            return byteInstruction("GET LOCAL", vec, offset);
        case OP_SET_LOCAL:
            return byteInstruction("SET LOCAL", vec, offset);
        case OP_GET_UPVALUE:
            return byteInstruction("GET UPVALUE", vec, offset);
        case OP_SET_UPVALUE:
            return byteInstruction("SET UPVALUE", vec, offset);
        case OP_SET_AT:
            return simpleInstruction("SET AT", offset);
        case OP_GET_AT:
            return simpleInstruction("GET AT", offset);
        case OP_GET_AT_NO_POP:
            return simpleInstruction("GET AT NO POP", offset);
        case OP_INCREMENT:
            return simpleInstruction("INCREMENT", offset);
        case OP_DECREMENT:
            return simpleInstruction("DECREMENT", offset);
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
        case OP_POWER:
            return simpleInstruction("POWER", offset);
        case OP_NOT:
            return simpleInstruction("NOT", offset);
        case OP_NEGATE:
            return simpleInstruction("NEGATE", offset);
        case OP_EQUAL:
            return simpleInstruction("EQUAL", offset);
        case OP_NOT_EQUAL:
            return simpleInstruction("NOT EQUAL", offset);
        case OP_GREATER:
            return simpleInstruction("GREATER", offset);
        case OP_GREATER_EQUAL:
            return simpleInstruction("GREATER EQUAL", offset);
        case OP_LESS:
            return simpleInstruction("LESS", offset);
        case OP_LESS_EQUAL:
            return simpleInstruction("LESS EQUAL", offset);
        case OP_RETURN:
            return simpleInstruction("RETURN", offset);
        case OP_BUILD_FSTRING:
            return argInstruction("BUILD FSTRING", vec, offset);
        case OP_BUILD_ARRAY:
            return argInstruction("BUILD ARRAY", vec, offset);
        case OP_JUMP:
            return jumpInstruction("JUMP", 1, vec, offset);
        case OP_JUMP_TRUE:
            return jumpInstruction("JUMP TRUE", 1, vec, offset);
        case OP_JUMP_TRUE_POP:
            return jumpInstruction("JUMP TRUE POP", 1, vec, offset);
        case OP_JUMP_FALSE:
            return jumpInstruction("JUMP FALSE", 1, vec, offset);
        case OP_JUMP_FALSE_POP:
            return jumpInstruction("JUMP FALSE POP", 1, vec, offset);
        case OP_LOOP:
            return jumpInstruction("LOOP", -1, vec, offset);
        case OP_LOOP_TRUE_POP:
            return jumpInstruction("LOOP TRUE POP", -1, vec, offset);
        case OP_CALL:
            return byteInstruction("CALL", vec, offset);
        case OP_CLOSURE: {
            offset++;
            uint8_t constant = vec->code[offset++];
            printf("%-16s %4d ", "CLOSURE", constant);
            valuePrint(vec->constants.values[constant]);
            printf("\n");

            ObjFunction *function = AS_FUNCTION(vec->constants.values[constant]);
            for (int i = 0; i < function->upvalueCount; i++) {
                int isLocal = vec->code[offset++];
                int index = vec->code[offset++];
                printf("%04d      |              %s %d\n", offset - 2, isLocal ? "local" : "upvalue", index);
            }
            return offset;
        }
        case OP_CLOSE_UPVALUE:
            return simpleInstruction("CLOSE UPVALUE", offset);
        case OP_CLASS:
            return constantInstruction("CLASS", vec, offset);
        case OP_METHOD:
            return constantInstruction("METHOD", vec, offset);
        case OP_GET_PROPERTY:
            return constantInstruction("GET PROPERTY", vec, offset);
        case OP_SET_PROPERTY:
            return constantInstruction("SET PROPERTY", vec, offset);
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
        case TOKEN_PERCENT:
            return "PERCENT";
        case TOKEN_CARET:
            return "CARET";
        case TOKEN_MINUS_EQUAL:
            return "MINUS EQUAL";
        case TOKEN_PLUS_EQUAL:
            return "PLUS EQUAL";
        case TOKEN_SLASH_EQUAL:
            return "SLASH EQUAL";
        case TOKEN_STAR_EQUAL:
            return "STAR EQUAL";
        case TOKEN_PERCENT_EQUAL:
            return "PERCENT EQUAL";
        case TOKEN_CARET_EQUAL:
            return "CARET EQUAL";
        case TOKEN_PLUS_PLUS:
            return "PLUS PLUS";
        case TOKEN_MINUS_MINUS:
            return "MINUS MINUS";
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
        case TOKEN_FSTRING:
            return "FSTRING";
        case TOKEN_NUMBER:
            return "NUMBER";
        case TOKEN_IF:
            return "IF";
        case TOKEN_ELSE:
            return "ELSE";
        case TOKEN_ELIF:
            return "ELIF";
        case TOKEN_FOR:
            return "FOR";
        case TOKEN_BREAK:
            return "BREAK";
        case TOKEN_CONTINUE:
            return "CONTINUE";
        case TOKEN_NOT:
            return "NOT";
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
        case TOKEN_ERROR:
            return "ERROR";
        case TOKEN_LINE_BREAK:
            return "LINE BREAK";
        case TOKEN_SEMICOLON:
            return "SEMICOLON";
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