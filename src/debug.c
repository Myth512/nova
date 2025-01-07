#include <stdio.h>

#include "debug.h"
#include "object.h"
#include "value.h"
#include "value_methods.h"
#include "object_function.h"

static const char *TokenTypeToString[] = {
    [TOKEN_AMPERSAND] = "AMPERSAND",
    [TOKEN_AMPERSAND_EQUAL] = "AMPERNAND_EQUAL",
    [TOKEN_AND] = "AND",
    [TOKEN_AS] = "AS",
    [TOKEN_ASSERT] = "ASSERT",
    [TOKEN_ASYNC] = "ASYNC",
    [TOKEN_AT] = "AT",
    [TOKEN_AT_EQUAL] = "AT EQUAL",
    [TOKEN_AWAIT] = "AWAIT",
    [TOKEN_BANG_EQUAL] = "BANG EQUAL",
    [TOKEN_BREAK] = "BREAK",
    [TOKEN_CARET] = "CARET",
    [TOKEN_CARET_EQUAL] = "CARET EQAUL",
    [TOKEN_CLASS] = "CLASS",
    [TOKEN_COLON] = "COLON",
    [TOKEN_COLON_EQUAL] = "COLON EQAUL",
    [TOKEN_COMMA] = "COMMA",
    [TOKEN_CONTINUE] = "CONTINUE",
    [TOKEN_DEDENT] = "DEDENT",
    [TOKEN_DEF] = "DEF",
    [TOKEN_DEL] = "DEL",
    [TOKEN_DOT] = "DOT",
    [TOKEN_DOUBLE_EQUAL] = "DOUBLE EQUAL",
    [TOKEN_DOUBLE_SLASH] = "DOUBLE SLASH",
    [TOKEN_DOUBLE_SLASH_EQUAL] = "DOUBLE SLASH EQUAL",
    [TOKEN_DOUBLE_STAR] = "DOUBLE STAR",
    [TOKEN_DOUBLE_STAR_EQUAL] = "DOUBLE STAR EQUAL",
    [TOKEN_ELIF] = "ELIF",
    [TOKEN_ELSE] = "ELSE",
    [TOKEN_EOF] = "EOF",
    [TOKEN_EQUAL] = "EQUAL",
    [TOKEN_ERROR] = "ERROR",
    [TOKEN_EXCEPT] = "EXCEPT",
    [TOKEN_FALSE] = "FALSE",
    [TOKEN_FINALLY] = "FINALLY",
    [TOKEN_FOR] = "FOR",
    [TOKEN_FROM] = "FROM",
    [TOKEN_FSTRING] = "FSTRING",
    [TOKEN_GLOBAL] = "GLOBAL",
    [TOKEN_GREATER] = "GREATER",
    [TOKEN_GREATER_EQUAL] = "GREATER EQUAL",
    [TOKEN_IDENTIFIER] = "IDENTIFIER",
    [TOKEN_IF] = "IF",
    [TOKEN_IMPORT] = "IMPORT",
    [TOKEN_IN] = "IN",
    [TOKEN_INDENT] = "INDENT",
    [TOKEN_IS] = "IS",
    [TOKEN_LAMBDA] = "LAMBDA",
    [TOKEN_LEFT_BRACE] = "LEFT BRACE",
    [TOKEN_LEFT_BRACKET] = "LEFT BRACKET",
    [TOKEN_LEFT_PAREN] = "LEFT PAREN",
    [TOKEN_LEFT_SHIFT] = "LEFT SHIFT",
    [TOKEN_LEFT_SHIFT_EQUAL] = "LEFT SHIFT EQUAL",
    [TOKEN_LESS] = "LESS",
    [TOKEN_LESS_EQUAL] = "LESS EQUAL",
    [TOKEN_MINUS] = "MINUS",
    [TOKEN_MINUS_EQUAL] = "MINUS EQUAL",
    [TOKEN_NEWLINE] = "NEWLINE",
    [TOKEN_NONE] = "NONE",
    [TOKEN_NONLOCAL] = "NONLOCAL",
    [TOKEN_NOT] = "NOT",
    [TOKEN_NUMBER] = "NUMBER",
    [TOKEN_OR] = "OR",
    [TOKEN_PASS] = "PASS",
    [TOKEN_PERCENT] = "PERCENT",
    [TOKEN_PIPE] = "PIPE",
    [TOKEN_PIPE_EQUAL] = "PIPE EQUAL",
    [TOKEN_PLUS] = "PLUS",
    [TOKEN_PLUS_EQUAL] = "PLUS EQUAL",
    [TOKEN_RAISE] = "RAISE",
    [TOKEN_RETURN] = "RETURN",
    [TOKEN_RIGHT_BRACE] = "RIGHT BRACE",
    [TOKEN_RIGHT_BRACKET] = "RIGHT BRACKET",
    [TOKEN_RIGHT_PAREN] = "RIGHT PAREN",
    [TOKEN_RIGHT_SHIFT] = "RIGHT SHIFT",
    [TOKEN_RIGHT_SHIFT_EQUAL] = "RIGHT SHIFT EQUAL",
    [TOKEN_SEMICOLON] = "SEMICOLON",
    [TOKEN_SLASH] = "SLASH",
    [TOKEN_SLASH_EQUAL] = "SLASH EQUAL",
    [TOKEN_STAR] = "STAR",
    [TOKEN_STAR_EQUAL] = "STAR EQUAL",
    [TOKEN_STRING] = "STRING",
    [TOKEN_TILDE] = "TILDE",
    [TOKEN_TRUE] = "TRUE",
    [TOKEN_TRY] = "TRY",
    [TOKEN_WHILE] = "WHILE",
    [TOKEN_WITH] = "WITH",
    [TOKEN_YIELD] = "YIELD"
};

static const char *ValueTypeToString[] = {
    [VAL_NONE] = "<type none>",
    [VAL_BOOL] = "<type bool>",
    [VAL_INT] = "<type int>",
    [VAL_FLOAT] = "<type float>",
    [VAL_UNDEFINED] = "<type undefined>",
    [VAL_NOT_IMPLEMENTED] = "<type not implemented>",
    [VAL_STRING] = "<type string>",
    [VAL_LIST] = "<type list>",
    [VAL_TUPLE] = "<type tuple>",
    [VAL_FUNCTION] = "<type function>",
    [VAL_CLOSURE] = "<type closure>",
    [VAL_UPVALUE] = "<type upvalue>",
    [VAL_NATIVE] = "<type native>",
    [VAL_CLASS] = "<type class>",
    [VAL_NATIVE_CLASS] = "<type native class>",
    [VAL_METHOD] = "<type method>",
    [VAL_NATIVE_METHOD] = "<type native method>",
    [VAL_INSTANCE] = "<type instance>"
};

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

static int callInstruction(const char *name, CodeVec *vec, int offset) {
    uint8_t slot = vec->code[offset + 1];
    uint8_t slot2 = vec->code[offset + 2];
    printf("%-16s %4d %4d\n", name, slot, slot2);
    return offset + 3;
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
        case OP_NONE:
            return simpleInstruction("NONE", offset);
        case OP_POP:
            return simpleInstruction("POP", offset);
        case OP_FALSE:
            return simpleInstruction("FALSE", offset);
        case OP_TRUE:
            return simpleInstruction("TRUE", offset);
        case OP_GET_GLOBAL:
            return varInstruction("GET GLOBAL", vec, offset);
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
        case OP_SET_ITEM:
            return simpleInstruction("SET AT", offset);
        case OP_GET_ITEM:
            return simpleInstruction("GET AT", offset);
        case OP_GET_ITEM_NO_POP:
            return simpleInstruction("GET AT NO POP", offset);
        case OP_ADD:
            return simpleInstruction("ADD", offset);
        case OP_SUBTRUCT:
            return simpleInstruction("SUBTRUCT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("MULTIPLY", offset);
        case OP_POWER:
            return simpleInstruction("POWER", offset);
        case OP_TRUE_DIVIDE:
            return simpleInstruction("TRUE DIVIDE", offset);
        case OP_FLOOR_DIVIDE:
            return simpleInstruction("FLOOR DIVIDE", offset);
        case OP_MOD:
            return simpleInstruction("MOD", offset);
        case OP_POSITIVE:
            return simpleInstruction("POSITIVE", offset);
        case OP_NEGATIVE:
            return simpleInstruction("NEGATIVE", offset);
        case OP_BITWISE_AND:
            return simpleInstruction("AND", offset);
        case OP_BITWISE_XOR:
            return simpleInstruction("XOR", offset);
        case OP_BITWISE_OR:
            return simpleInstruction("OR", offset);
        case OP_LEFT_SHIFT:
            return simpleInstruction("LEFT SHIFT", offset);
        case OP_RIGHT_SHIFT:
            return simpleInstruction("RIGHT SHIFT", offset);
        case OP_INVERT:
            return simpleInstruction("INVERT", offset);
        case OP_NOT:
            return simpleInstruction("NOT", offset);
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
        case OP_CONTAINS:
            return simpleInstruction("CONTAINS", offset);
        case OP_IS:
            return simpleInstruction("IS", offset);
        case OP_BUILD_FSTRING:
            return argInstruction("BUILD FSTRING", vec, offset);
        case OP_BUILD_LIST:
            return argInstruction("BUILD LIST", vec, offset);
        case OP_BUILD_TUPLE:
            return argInstruction("BUILD TUPLE", vec, offset);
        case OP_BUILD_DICT:
            return argInstruction("BUILD DICT", vec, offset);
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
        case OP_SETUP_TRY:
            return jumpInstruction("SETUP TRY", 1, vec, offset);
        case OP_CALL:
            return callInstruction("CALL", vec, offset);
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
        case OP_GET_ATTRIBUTE:
            return constantInstruction("GET ATTRIBUTE", vec, offset);
        case OP_SET_ATTRIBUTE:
            return constantInstruction("SET ATTRIBUTE", vec, offset);
        default:
            printf("Unknown opcode %d\n", opcode);
            return offset + 1;
    }
}

const char* decodeValueType(Value value) {
    if (value.type < 0 || value.type > VAL_INSTANCE)
        return "<unknown type>";
    return ValueTypeToString[value.type];
 }

static char* decodeTokenType(TokenType type) {
    if (type < 0 || type > TOKEN_DEDENT)
        return "UNKNOWN TOKEN";
    return TokenTypeToString[type];
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