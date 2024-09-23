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

static bool isAlpha(char c) {
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           c == '_';
}

static bool isDigit(char c) {
    return '0' <= c && c <= '9';
}

static void skipWhitespace(bool skipNewLine) {
    while (true) {
        char c = peek();

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
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !reachEnd())
                        advance();
                } else if (peekNext() == '*') {
                    int nestCount = 1;
                    advance();
                    while (nestCount && !reachEnd()) {
                        if (peek() == '\n') {
                            scanner.line++;
                            scanner.column = 0;
                        }
                        if (peek() == '/' && peekNext() == '*')
                            nestCount++;
                        if (peek() == '*' && peekNext() == '/')
                            nestCount--;
                        advance(); 
                    } 
                    advance();
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

static Token createToken(TokenType type, int column) {
    scanner.lastToken = type;
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    token.column = column;
    return token;
}

static Token createErrorToken(const char *message) {
    Token token;
    token.type = T_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;
    token.column = scanner.column;
    return token;
}

static Token scanString() {
    int line = scanner.line;
    int column = scanner.column;

    while (peek() != '"' && !reachEnd()) {
        if (peek() == '\n') {
            scanner.line++;
            scanner.column = 0;
        }
        advance();
    }

    if (reachEnd()) {
        fprintf(stderr, "Scan Error: Unterminated string, starting here:\n");
        printCodeLine(scanner.source, line);
        printArrow(column, line);
    }

    advance();
    return createToken(T_STRING, column);
}

static Token scanNumber() {
    int column = scanner.column;

    while (isDigit(peek()))
        advance();

    if (peek() == '.') {
        if (isDigit(peekNext())) {
            advance();
            while (isDigit(peek()))
                advance();
        } else {
            fprintf(stderr, "Scan Error: Missing fractional part after '.'\n");
            printCodeLine(scanner.source, scanner.line);
            printArrow(scanner.column, scanner.line);
        }
    }

    return createToken(T_NUMBER, column);
}

static TokenType checkKeyword(int start, int length, const char *rest, TokenType type) {
    if (scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start, rest, length) == 0)
        return type;
    return T_IDENTIFIER;
}

static Token scanIdentifier() {
    int column = scanner.column;

    while (isAlpha(peek()) || isDigit(peek()))
        advance();
    
    switch (scanner.start[0]) {
        case 'a':
            return createToken(checkKeyword(1, 2, "nd", T_AND), column);
        case 'c':
            return createToken(checkKeyword(1, 4, "lass", T_CLASS), column);
        case 'd':
            return createToken(checkKeyword(1, 2, "ef", T_DEF), column);
        case 'e':
            return createToken(checkKeyword(1, 3, "lse", T_ELSE), column);
        case 'i':
            return createToken(checkKeyword(1, 1, "f", T_IF), column);
        case 'n':
            return createToken(checkKeyword(1, 2, "il", T_NIL), column);
        case 'o':
            return createToken(checkKeyword(1, 1, "r", T_OR), column);
        case 'p':
            return createToken(checkKeyword(1, 4, "rint", T_PRINT), column);
        case 'r':
            return createToken(checkKeyword(1, 5, "eturn", T_RETURN), column);
        case 'v':
            return createToken(checkKeyword(1, 2, "ar", T_VAR), column);
        case 'w':
            return createToken(checkKeyword(1, 4, "hile", T_WHILE), column);
        case 't':
            return createToken(checkKeyword(1, 3, "rue", T_TRUE), column);
        case 's':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'e':
                        return createToken(checkKeyword(2, 2, "lf", T_SELF), column);
                    case 'u':
                        return createToken(checkKeyword(2, 3, "per", T_SUPER), column);
                }
            }
            break;
        case 'f':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a':
                        return createToken(checkKeyword(2, 3, "lse", T_FALSE), column);
                    case 'o':
                        return createToken(checkKeyword(2, 1, "r", T_FOR), column);
                }
            }
            break;
    }
    return createToken(T_IDENTIFIER, column);
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

    int column = scanner.column;

    scanner.start = scanner.current;

    if (reachEnd())
        return createToken(T_EOF, 0);

    char c = advance();

    if (isDigit(c))
        return scanNumber();
    
    if (isAlpha(c))
        return scanIdentifier();

    switch (c) {
        case '(':
            return createToken(T_LEFT_PAREN, column);
        case ')':
            return createToken(T_RIGHT_PAREN, column);
        case '{':
            return createToken(T_LEFT_BRACE, column);
        case '}':
            return createToken(T_RIGHT_BRACE, column);
        case '[':
            return createToken(T_LEFT_BRACKET, column);
        case ']':
            return createToken(T_RIGHT_BRACKET, column);
        case ';':
            return createToken(T_EOS, column);
        case ',':
            return createToken(T_COMMA, column);
        case '.':
            return createToken(T_DOT, column);
        case '-':
            return createToken(match('=') ? T_MINUS_EQUAL : T_MINUS, column);
        case '+':
            return createToken(match('=') ? T_PLUS_EQUAL : T_PLUS, column);
        case '/':
            return createToken(match('=') ? T_SLASH_EQUAL : T_SLASH, column);
        case '*':
            return createToken(match('=') ? T_STAR_EQUAL : T_STAR, column);
        case '%':
            return createToken(match('=') ? T_MOD_EQUAL : T_MOD, column);
        case '!':
            return createToken(match('=') ? T_BANG_EQUAL : T_BANG, column);
        case '=':
            return createToken(match('=') ? T_DOUBLE_EQUAL : T_EQUAL, column);
        case '<':
            return createToken(match('=') ? T_LESS_EQUAL : T_LESS, column);
        case '>':
            return createToken(match('=') ? T_GREATER_EQUAL : T_GREATER, column);
        case '\n':
            Token token = createToken(T_EOS, column);
            scanner.line++;
            scanner.column = 0;
            return token; 
        case '\\':
            return scanToken(true);
        case '"':
            return scanString();
    }
    return createErrorToken("Unknown token");
}