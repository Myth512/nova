#include <stdio.h>
#include <string.h>

#include "scanner.h"
#include "error.h"

typedef struct {
    const char *source;
    const char *start;
    const char *current;
    int line;
    int column;
    int startLine;
    int startColumn;
    bool inFormattedString;
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

static void skipChar() {
    scanner.startColumn++;
    scanner.start++;
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
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;
    token.column = scanner.column;
    return token;
}

static Token scanString() {
    if (peek() == '{') {
        scanner.inFormattedString = true;
        Token token = createToken(TOKEN_FSTRING);
        advance();
        return token; 
    }

    while (peek() != '"' && peek() != '\n' && !reachEnd()) {
        if (peek() != '\\' && peekNext() == '{') {
            scanner.inFormattedString = true;
            advance();
            Token token = createToken(TOKEN_FSTRING);
            advance();
            return token; 
        }
        if (peek() == '\\' && peekNext() == '"')
            advance();
        advance();
    }

    if (peek() == '\n' || reachEnd()) {
        return createErrorToken("Unterminated string, staring here");
    }

    Token token = createToken(TOKEN_STRING);
    advance();
    scanner.inFormattedString = false;
    return token; 
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
            return createErrorToken("Missing fractional part after '.");
            // fprintf(stderr, "Scan Error: Missing fractional part after '.'\n");
            // const char *codeLine = getCodeLine(scanner.source, scanner.line);
            // printHightlightedWordInCode(codeLine, scanner.line, scanner.column, 1);
            // printArrow(codeLine, scanner.column, 1);
        }
    }

    return createToken(TOKEN_NUMBER);
}

static TokenType checkKeyword(int start, int length, const char *rest, TokenType type) {
    if (scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start, rest, length) == 0)
        return type;
    return TOKEN_IDENTIFIER;
}

static Token scanIdentifier() {
    while (isAlpha(peek()) || isDigit(peek()))
        advance();
    
    switch (scanner.start[0]) {
        case 'a':
            return createToken(checkKeyword(1, 2, "nd", TOKEN_AND));
        case 'c':
            return createToken(checkKeyword(1, 4, "lass", TOKEN_CLASS));
        case 'd':
            return createToken(checkKeyword(1, 2, "ef", TOKEN_DEF));
        case 'e':
            if (scanner.current - scanner.start > 3) {
                if (scanner.start[1] == 'l' && scanner.start[2] == 's' && scanner.start[3] == 'e')
                    return createToken(TOKEN_ELSE);
                if (scanner.start[1] == 'l' && scanner.start[2] == 'i' && scanner.start[3] == 'f')
                    return createToken(TOKEN_ELIF);
            }
            return createToken(TOKEN_IDENTIFIER);
        case 'i':
            return createToken(checkKeyword(1, 1, "f", TOKEN_IF));
        case 'n':
            return createToken(checkKeyword(1, 2, "il", TOKEN_NIL));
        case 'o':
            return createToken(checkKeyword(1, 1, "r", TOKEN_OR));
        case 'p':
            return createToken(checkKeyword(1, 4, "rint", TOKEN_PRINT));
        case 'r':
            return createToken(checkKeyword(1, 5, "eturn", TOKEN_RETURN));
        case 'v':
            return createToken(checkKeyword(1, 2, "ar", TOKEN_VAR));
        case 'w':
            return createToken(checkKeyword(1, 4, "hile", TOKEN_WHILE));
        case 't':
            return createToken(checkKeyword(1, 3, "rue", TOKEN_TRUE));
        case 's':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'e':
                        return createToken(checkKeyword(2, 2, "lf", TOKEN_SELF));
                    case 'u':
                        return createToken(checkKeyword(2, 3, "per", TOKEN_SUPER));
                }
            }
            break;
        case 'f':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a':
                        return createToken(checkKeyword(2, 3, "lse", TOKEN_FALSE));
                    case 'o':
                        return createToken(checkKeyword(2, 1, "r", TOKEN_FOR));
                }
            }
            break;
    }
    return createToken(TOKEN_IDENTIFIER);
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
    skipWhitespace(skipNewline || scanner.lastToken == TOKEN_EOS);

    scanner.start = scanner.current;
    scanner.startLine = scanner.line;
    scanner.startColumn = scanner.column;

    if (reachEnd())
        return createToken(TOKEN_EOF);

    char c = advance();

    if (isDigit(c))
        return scanNumber();
    
    if (isAlpha(c))
        return scanIdentifier();

    switch (c) {
        case '(':
            return createToken(TOKEN_LEFT_PAREN);
        case ')':
            return createToken(TOKEN_RIGHT_PAREN);
        case '{':
            return createToken(TOKEN_LEFT_BRACE);
        case '}':
            if (scanner.inFormattedString) {
                skipChar();
                return scanString(); 
            }
            return createToken(TOKEN_RIGHT_BRACE);
        case '[':
            return createToken(TOKEN_LEFT_BRACKET);
        case ']':
            return createToken(TOKEN_RIGHT_BRACKET);
        case ';':
            return createToken(TOKEN_EOS);
        case ',':
            return createToken(TOKEN_COMMA);
        case '.':
            return createToken(TOKEN_DOT);
        case '-':
            if (match('='))
                return createToken(TOKEN_MINUS_EQUAL);
            if (match('-'))
                return createToken(TOKEN_MINUS_MINUS);
            return createToken(TOKEN_MINUS);
        case '+':
            if (match('='))
                return createToken(TOKEN_PLUS_EQUAL);
            if (match('+'))
                return createToken(TOKEN_PLUS_PLUS);
            return createToken(TOKEN_PLUS);
        case '/':
            return createToken(match('=') ? TOKEN_SLASH_EQUAL : TOKEN_SLASH);
        case '*':
            return createToken(match('=') ? TOKEN_STAR_EQUAL : TOKEN_STAR);
        case '%':
            return createToken(match('=') ? TOKEN_MOD_EQUAL : TOKEN_MOD);
        case '^':
            return createToken(match('=') ? TOKEN_POWER_EQUAL : TOKEN_POWER);
        case '|':
            return createToken(TOKEN_PIPE);
        case '!':
            return createToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return createToken(match('=') ? TOKEN_DOUBLE_EQUAL : TOKEN_EQUAL);
        case '<':
            return createToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return createToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '\n':
            scanner.line++;
            scanner.column = 1;
            return createToken(TOKEN_EOS);
        case '\\':
            return scanToken(true);
        case '"':
            skipChar();
            return scanString();
    }
    return createErrorToken("Unknown token");
}