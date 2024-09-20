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