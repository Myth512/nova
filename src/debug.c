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
        case OP_ADD:
            return simpleInstruction("ADD", offset);
        case OP_SUBTRUCT:
            return simpleInstruction("SUBTRUCT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("DIVIDE", offset);
        case OP_NEGATE:
            return simpleInstruction("NEGATE", offset);
        case OP_RETURN:
            return simpleInstruction("RETURN", offset);
        default:
            printf("Unknown opcode %d\n", opcode);
            return offset + 1;
    }
}

static char* decodeTokenType(TokenType type) {
    switch (type) {
        case T_LEFT_PAREN:
            return "LEFT PAREN";
        case T_RIGHT_PAREN:
            return "RIGHT PAREN";
        case T_LEFT_BRACE:
            return "LEFT BRACE";
        case T_RIGHT_BRACE:
            return "RIGHT BRACE";
        case T_LEFT_BRACKET:
            return "LEFT BRACKET";
        case T_RIGHT_BRACKET:
            return "RIGHT BRACKET";
        case T_COMMA:
            return "COMMA";
        case T_DOT:
            return "DOT";
        case T_MINUS:
            return "MINUS";
        case T_PLUS:
            return "PLUS";
        case T_SLASH:
            return "SLASH";
        case T_STAR:
            return "STAR";
        case T_MOD:
            return "MOD";
        case T_BANG:
            return "BANG";
        case T_BANG_EQUAL:
            return "BANG EQUAL";
        case T_EQUAL:
            return "EQUAL";
        case T_DOUBLE_EQUAL:
            return "DOUBLE EQUAL";
        case T_GREATER:
            return "GREATER";
        case T_GREATER_EQUAL:
            return "GREATER EQUAL";
        case T_LESS:
            return "LESS";
        case T_LESS_EQUAL:
            return "LESS EQUAL";
        case T_IDENTIFIER:
            return "IDENTIFIER";
        case T_STRING:
            return "STRING";
        case T_NUMBER:
            return "NUMBER";
        case T_VAR:
            return "VAR";
        case T_IF:
            return "IF";
        case T_ELSE:
            return "ELSE";
        case T_FOR:
            return "FOR";
        case T_WHILE:
            return "WHILE";
        case T_AND:
            return "AND";
        case T_OR:
            return "OR";
        case T_TRUE:
            return "TRUE";
        case T_FALSE:
            return "FALSE";
        case T_NIL:
            return "NIL";
        case T_DEF:
            return "DEF";
        case T_RETURN:
            return "RETURN";
        case T_CLASS:
            return "CLASS";
        case T_SELF:
            return "SELF";
        case T_SUPER:
            return "SUPER";
        case T_PRINT:
            return "PRINT";
        case T_ERROR:
            return "ERROR";
        case T_EOS:
            return "EOS";
        case T_EOF:
            return "EOF";
        default:
            return "Unknow Token";
    }
}

void printToken(Token token) {
    printf("line: %d \tcolumn: %d \ttype: %s \tdata: \t\"", token.line, token.column, decodeTokenType(token.type));
    for (int i = 0; i < token.lenght; i++) {
        char *c = token.start;
        if (*c == '\n') {
            putchar('\\');
            putchar('n');
        } else {
            putchar(*c);
        }
        c++;
    }
    putchar('\"');
    putchar('\n');
}