#include <stdio.h>
#include <string.h>

#include "scanner.h"
#include "error.h"
#include "keywords.h"

typedef struct {
    const char *source;
    const char *start;
    const char *current;
    int line;
    int column;
    int startLine;
    int startColumn;
    int indentationStack[256];
    int indentationPointer;
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

static void pushIndent(int level) {
    scanner.indentationStack[scanner.indentationPointer++] = level;
}

static int popIndent() {
    return scanner.indentationStack[--scanner.indentationPointer];
}

static int peekIndent() {
    return scanner.indentationStack[scanner.indentationPointer - 1];
}

static void skipWhitespace() {
    while (true) {
        char c = peek();

        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                advance();
                break;
            default:
                return;
        }
    }
}

static Token createToken(TokenType type) {
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
        }
    }

    return createToken(TOKEN_NUMBER);
}

static Token scanIdentifier() {
    while (isAlpha(peek()) || isDigit(peek()))
        advance();

    int length = scanner.current - scanner.start;
    char str[256];
    strncpy(str, scanner.start, length);
    str[2] = '\0';
    const struct Keyword *keyword = in_keyword_set(scanner.start, length);

    if (keyword)
        return createToken(keyword->type);
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
    pushIndent(0);
}

Token scanToken() {
    skipWhitespace();

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
        case '[':
            return createToken(TOKEN_LEFT_BRACKET);
        case ']':
            return createToken(TOKEN_RIGHT_BRACKET);
        case '{':
            return createToken(TOKEN_LEFT_BRACE);
        case '}':
            return createToken(TOKEN_RIGHT_BRACE);
        case '+':
            if (match('='))
                return createToken(TOKEN_PLUS_EQUAL);
            return createToken(TOKEN_PLUS);
        case '-':
            if (match('='))
                return createToken(TOKEN_MINUS_EQUAL);
            return createToken(TOKEN_MINUS);
        case '*':
            if (match('='))
                return createToken(TOKEN_STAR_EQUAL);
            if (match('*'))
                return createToken(TOKEN_DOUBLE_STAR);
            return createToken(TOKEN_STAR);
        case '/':
            if (match('='))
                return createToken(TOKEN_SLASH_EQUAL);
            return createToken(TOKEN_SLASH);
        case '%':
            if (match('='))
                return createToken(TOKEN_PERCENT_EQUAL);
            return createToken(TOKEN_PERCENT);
        case '@':
            if (match('='))
                return createToken(TOKEN_AT_EQUAL);
            return createToken(TOKEN_AT);
        case '&':
            if (match('='))
                return createToken(TOKEN_AMPERSAND_EQUAL);
            return createToken(TOKEN_AMPERSAND);
        case '|':
            if (match('='))
                return createToken(TOKEN_PIPE_EQUAL);
            return createToken(TOKEN_PIPE);
        case '^':
            if (match('='))
                return createToken(TOKEN_CARET_EQUAL);
            return createToken(TOKEN_CARET);
        case '~':
            return createToken(TOKEN_TILDE);
        case '=':
            if (match('='))
                return createToken(TOKEN_DOUBLE_EQUAL);
            return createToken(TOKEN_EQUAL);
        case '!':
            if (match('='))
                return createToken(TOKEN_BANG_EQUAL);
            break;
        case '<':
            if (match('='))
                return createToken(TOKEN_LESS_EQUAL);
            if (match('<')) {
                if (match('='))
                    return createToken(TOKEN_LEFT_SHIFT_EQUAL);
                return createToken(TOKEN_LEFT_SHIFT);
            }
            return createToken(TOKEN_LESS);
        case '>':
            if (match('='))
                return createToken(TOKEN_GREATER_EQUAL);
            if (match('<')) {
                if (match('='))
                    return createToken(TOKEN_RIGHT_SHIFT_EQUAL);
                return createToken(TOKEN_RIGHT_SHIFT);
            }
            return createToken(TOKEN_GREATER);
        case ':':
            if (match('='))
                return createToken(TOKEN_COLON_EQUAL);
            return createToken(TOKEN_COLON);
        case ';':
            return createToken(TOKEN_SEMICOLON);
        case '.':
            return createToken(TOKEN_DOT);
        case ',':
            return createToken(TOKEN_COMMA);
        case '\n': {
            int indent = 0;

            while (match(' '))
                indent++;
            
            int prevIndect = peekIndent();

            if (prevIndect == indent) {
                return createToken(TOKEN_NEWLINE);
            } else if (prevIndect < indent) {
                popIndent();
                return createToken(TOKEN_DEDENT);
            } else {
                pushIndent(indent);
                return createToken(TOKEN_INDENT);
            }
        }
    }

    return createErrorToken("Invalid syntax");
}