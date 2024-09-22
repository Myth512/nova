#include "scanner.h"

typedef struct {
    const char *source;
    const char *start;
    const char *current;
    int line;
    int column;
    TokenType lastToken;
} Scanner;

Scanner scanner;

static bool reachEnd() {
    return *scanner.current == '\0';
}

static char advance() {
    scanner.current++;
    scanner.column++;
    return scanner.current[-1];
}

static bool match(char expected) {
    if (reachEnd())
        return false;
    if (*scanner.current != expected)
        return false;
    
    scanner.current++;
    return true;
}

static char peek() {
    return *scanner.current;
}

static char peekNext() {
    if (reachEnd())
        return '\0';
    return scanner.current[1];
}

static void skipWhitespace(bool skipNewLine) {
    while (true) {
        char c = peek();
        // printf("%c\n", c);
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                if (skipNewLine) {
                    scanner.line++;
                    scanner.column = 0;
                    advance();
                    break;
                } else {
                    return;
                }
            // case '/':
            //     if (peekNext() == '/') {
            //         while (peek() != '\n' && !reachEnd())
            //             advance();
            //     } else if (peekNext() == '*') {
            //         int nestCount = 1;
            //         advance();
            //         while (nestCount && !reachEnd()) {
            //             if (peek() == '/' && peekNext() == '*')
            //                 nestCount++;
            //             if (peek() == '*' && peekNext() == '/')
            //                 nestCount--;
            //             advance(); 
            //         } 
            //         advance();
            //     } else {
            //         return;
            //     }
            default:
                return;
        }
    }
}

static Token createToken(TokenType type) {
    scanner.lastToken = type;
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.lenght = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    token.column = scanner.column;
    return token;
}

static Token createErrorToken(const char *message) {
    Token token;
    token.type = T_ERROR;
    token.start = message;
    token.lenght = (int)strlen(message);
    token.line = scanner.line;
    token.column = scanner.column;
    return token;
}

void initScanner(const char *source) {
    scanner.source = source;
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
    scanner.column = 0;
}

Token scanToken(bool skipNewline) {
    skipWhitespace(skipNewline || scanner.lastToken == T_EOS);

    scanner.start = scanner.current;

    if (reachEnd())
        return createToken(T_EOF);

    char c = advance();

    switch (c) {
        case '(':
            return createToken(T_LEFT_PAREN);
        case ')':
            return createToken(T_RIGHT_PAREN);
        case '{':
            return createToken(T_LEFT_BRACE);
        case '}':
            return createToken(T_RIGHT_BRACE);
        case '[':
            return createToken(T_LEFT_BRACKET);
        case ']':
            return createToken(T_RIGHT_BRACKET);
        case ';':
            return createToken(T_EOS);
        case ',':
            return createToken(T_COMMA);
        case '.':
            return createToken(T_DOT);
        case '-':
            return createToken(T_MINUS);
        case '+':
            return createToken(T_PLUS);
        case '/':
            return createToken(T_SLASH);
        case '*':
            return createToken(T_STAR);
        case '%':
            return createToken(T_MOD);
        case '!':
            return createToken(match('=') ? T_BANG_EQUAL : T_BANG);
        case '=':
            return createToken(match('=') ? T_DOUBLE_EQUAL : T_EQUAL);
        case '<':
            return createToken(match('=') ? T_LESS_EQUAL : T_LESS);
        case '>':
            return createToken(match('=') ? T_GREATER_EQUAL : T_GREATER);
        case '\n':
            Token token = createToken(T_EOS);
            scanner.line++;
            scanner.column = 0;
            return token; 
        case '\\':
            return scanToken(true);
    }
    return createErrorToken("Unknown token");
}