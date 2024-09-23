#include "compiler.h"

void compile(const char *source) {
    initScanner(source);
    while (true) {
        Token token = scanToken(false);
        printToken(token);
        printTokenInCode(source, token);

        if (token.type == T_EOF)
            break;
    }
    // printCodeLine(source, 13);
    // printArrow(7, 13);

    // printCodeLine(source, 9);
    // printArrow(10, 9);
}