#include <stdlib.h>

#include "compiler.h"
#include "scanner.h"
#include "debug.h"
#include "common.h"
#include "object.h"
#include "error.h"

typedef struct {
    const char *source;
    Token current;
    Token previous;
    int errorCount;
    bool panicMode;
} Parser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,
    PREC_OR,
    PREC_AND,
    PREC_EQUALITY,
    PREC_COMPARISON,
    PREC_TERM,
    PREC_FACTOR,
    PREC_UNARY,
    PREC_CALL,
    PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)();

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;


static void number();

static void literal();

static void string();

static void grouping();

static void unary();

static void binary();

ParseRule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
  [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE}, 
  [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
  [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
  [TOKEN_EOS]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
  [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
  [TOKEN_MOD]           = {NULL,     binary, PREC_FACTOR},
  [TOKEN_BANG]          = {unary,    NULL,   PREC_NONE},
  [TOKEN_BANG_EQUAL]    = {NULL,     binary, PREC_EQUALITY},
  [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_DOUBLE_EQUAL]  = {NULL,     binary, PREC_EQUALITY},
  [TOKEN_GREATER]       = {NULL,     binary, PREC_EQUALITY},
  [TOKEN_GREATER_EQUAL] = {NULL,     binary, PREC_EQUALITY},
  [TOKEN_LESS]          = {NULL,     binary, PREC_EQUALITY},
  [TOKEN_LESS_EQUAL]    = {NULL,     binary, PREC_EQUALITY},
  [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_STRING]        = {string,   NULL,   PREC_NONE},
  [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
  [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FALSE]         = {literal,  NULL,   PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_DEF]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NIL]           = {literal,  NULL,   PREC_NONE},
  [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SELF]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_TRUE]          = {literal,  NULL,   PREC_NONE},
  [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

static ParseRule* getRule(TokenType type) {
    return &rules[type];
}

Parser parser;
CodeVec *compilingCode;

static CodeVec* currentCode() {
    return compilingCode;
}

static void reportError(const char *message) {
    if (parser.panicMode)
        return;
    parser.panicMode = true;
    parser.errorCount++;
    fprintf(stderr, "\033[31mParse Error\033[0m: %s\n", message);
}

static void advance() {
    parser.previous = parser.current;

    while (true) {
        parser.current = scanToken(false);

        if (parser.current.type == TOKEN_EOF)
            break;

        if (parser.current.type != TOKEN_ERROR)
            break;

        reportError(parser.current.start);
    }
}

static void parsePrecedence(Precedence precedence) {
    advance();

    Token leftOperand = parser.previous;

    ParseFn prefixRule = getRule(leftOperand.type)->prefix;
    if (prefixRule == NULL) {
        reportError("Expect expression");
        return;
    }

    prefixRule();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}

static bool consume(TokenType type) {
    if (parser.current.type == type) {
        advance();
        return false;
    }

    return true;
}

static void emitByte(uint8_t byte) {
    pushInstruction(currentCode(), byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static void emitReturn() {
    emitByte(OP_RETURN);
}

static void endCompiler() {
    emitReturn();
    #ifdef DEBUG_PRINT_CODE
        if (parser.errorCount == 0)
            printCodeVec(currentCode(), "code");
    #endif
}

static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

static uint8_t createConstant(Value value) {
    int constant = pushConstant(currentCode(), value);
    if (constant > UINT8_MAX) {
        reportError("Too many constants in one code block");
        return 0;
    }
    return constant;
}

static void emitConstant(Value value) {
    emitBytes(OP_CONSTANT, createConstant(value));
}

static void number() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

static void literal() {
    switch (parser.previous.type) {
        case TOKEN_FALSE:
            emitByte(OP_FALSE);
            break;
        case TOKEN_TRUE:
            emitByte(OP_TRUE);
            break;
        case TOKEN_NIL:
            emitByte(OP_NIL);
            break;
        default:
            return;
    }
}

static void string() {
    emitConstant(OBJ_VAL(copyString(parser.previous.start + 1, parser.previous.length - 2)));
}

static void grouping() {
    expression();
    if (consume(TOKEN_RIGHT_PAREN)) {
        reportError("Expect ')' here");
        const char *codeLine = getCodeLine(parser.source, parser.current.line);
        printCodeLine(codeLine, parser.current.line);
        printArrow(codeLine, parser.current.column, 1);
    }
}

static void unary() {
    TokenType operatorType = parser.previous.type;
    ParseRule *rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case TOKEN_MINUS:
            emitByte(OP_NEGATE);
            break;
        case TOKEN_BANG:
            emitByte(OP_NOT);
            break;
        default:
            return;
    }
}

static void binary() {
    TokenType operatorType = parser.previous.type;
    ParseRule *rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case TOKEN_BANG_EQUAL:
            emitByte(OP_NOT_EQUAL);
            break;
        case TOKEN_DOUBLE_EQUAL:
            emitByte(OP_EQUAL);
            break;
        case TOKEN_GREATER:
            emitByte(OP_GREATER);
            break;
        case TOKEN_GREATER_EQUAL:
            emitByte(OP_GREATER_EQUAL);
            break;
        case TOKEN_LESS:
            emitByte(OP_LESS);
            break;
        case TOKEN_LESS_EQUAL:
            emitByte(OP_LESS_EQUAL);
            break;
        case TOKEN_PLUS:
            emitByte(OP_ADD);
            break;
        case TOKEN_MINUS:
            emitByte(OP_SUBTRUCT);
            break;
        case TOKEN_STAR:
            emitByte(OP_MULTIPLY);
            break;
        case TOKEN_SLASH:
            emitByte(OP_DIVIDE);
            break;
        case TOKEN_MOD:
            emitByte(OP_MOD);
            break;
        default:
            return;
    }
}

int compile(const char *source, CodeVec *code) {
    initScanner(source);
    compilingCode = code;

    parser.errorCount = 0;
    parser.panicMode = false;
    parser.source = source;

    advance();
    expression();
    if (consume(TOKEN_EOS)) {
        reportError("Expect end of expression");
    }
    endCompiler();
    return parser.errorCount;
}