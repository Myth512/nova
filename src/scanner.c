#include "scanner.h"

typedef struct {
    const char *source;
    const char *start;
    const char *current;
    int line;
    int column;
    int startLine;
    int startColumn;
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
    scanner.column++;
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
                            scanner.column = 1;
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

static Token createToken(TokenType type) {
    scanner.lastToken = type;
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.startLine;
    token.column = scanner.startColumn;
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
    while (peek() != '"' && !reachEnd()) {
        if (peek() == '\n') {
            scanner.line++;
            scanner.column = 0;
        }
        advance();
    }

    if (reachEnd()) {
        fprintf(stderr, "Scan Error: Unterminated string, starting here:\n");
        // printCodeLine(scanner.source, scanner.startLine);
        // printArrow(scanner.startColumn);
    }

    advance();
    return createToken(T_STRING);
}

static Token scanNumber() {
    while (isDigit(peek()))
        advance();

    if (peek() == '.') {
        if (isDigit(peekNext())) {
            advance();
            while (isDigit(peek()))
                advance();
        } else {
            fprintf(stderr, "Scan Error: Missing fractional part after '.'\n");
            // printCodeLine(scanner.source, scanner.startLine);
            // printArrow(scanner.startColumn);
        }
    }

    return createToken(T_NUMBER);
}

static TokenType checkKeyword(int start, int length, const char *rest, TokenType type) {
    if (scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start, rest, length) == 0)
        return type;
    return T_IDENTIFIER;
}

static Token scanIdentifier() {
    while (isAlpha(peek()) || isDigit(peek()))
        advance();
    
    switch (scanner.start[0]) {
        case 'a':
            return createToken(checkKeyword(1, 2, "nd", T_AND));
        case 'c':
            return createToken(checkKeyword(1, 4, "lass", T_CLASS));
        case 'd':
            return createToken(checkKeyword(1, 2, "ef", T_DEF));
        case 'e':
            return createToken(checkKeyword(1, 3, "lse", T_ELSE));
        case 'i':
            return createToken(checkKeyword(1, 1, "f", T_IF));
        case 'n':
            return createToken(checkKeyword(1, 2, "il", T_NIL));
        case 'o':
            return createToken(checkKeyword(1, 1, "r", T_OR));
        case 'p':
            return createToken(checkKeyword(1, 4, "rint", T_PRINT));
        case 'r':
            return createToken(checkKeyword(1, 5, "eturn", T_RETURN));
        case 'v':
            return createToken(checkKeyword(1, 2, "ar", T_VAR));
        case 'w':
            return createToken(checkKeyword(1, 4, "hile", T_WHILE));
        case 't':
            return createToken(checkKeyword(1, 3, "rue", T_TRUE));
        case 's':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'e':
                        return createToken(checkKeyword(2, 2, "lf", T_SELF));
                    case 'u':
                        return createToken(checkKeyword(2, 3, "per", T_SUPER));
                }
            }
            break;
        case 'f':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a':
                        return createToken(checkKeyword(2, 3, "lse", T_FALSE));
                    case 'o':
                        return createToken(checkKeyword(2, 1, "r", T_FOR));
                }
            }
            break;
    }
    return createToken(T_IDENTIFIER);
}

void initScanner(const char *source) {
    scanner.source = source;
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
    scanner.column = 1;
    scanner.startLine = 1;
    scanner.startColumn = 1;
}

Token scanToken(bool skipNewline) {
    skipWhitespace(skipNewline || scanner.lastToken == T_EOS);

    scanner.start = scanner.current;
    scanner.startLine = scanner.line;
    scanner.startColumn = scanner.column;

    if (reachEnd())
        return createToken(T_EOF);

    char c = advance();

    if (isDigit(c))
        return scanNumber();
    
    if (isAlpha(c))
        return scanIdentifier();

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
            return createToken(match('=') ? T_MINUS_EQUAL : T_MINUS);
        case '+':
            return createToken(match('=') ? T_PLUS_EQUAL : T_PLUS);
        case '/':
            return createToken(match('=') ? T_SLASH_EQUAL : T_SLASH);
        case '*':
            return createToken(match('=') ? T_STAR_EQUAL : T_STAR);
        case '%':
            return createToken(match('=') ? T_MOD_EQUAL : T_MOD);
        case '!':
            return createToken(match('=') ? T_BANG_EQUAL : T_BANG);
        case '=':
            return createToken(match('=') ? T_DOUBLE_EQUAL : T_EQUAL);
        case '<':
            return createToken(match('=') ? T_LESS_EQUAL : T_LESS);
        case '>':
            return createToken(match('=') ? T_GREATER_EQUAL : T_GREATER);
        case '\n':
            scanner.line++;
            scanner.column = 1;
            return createToken(T_EOS);
        case '\\':
            return scanToken(true);
        case '"':
            return scanString();
    }
    return createErrorToken("Unknown token");
}