#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "memory.h"
#include "token.h"
#include "scanner.h"
#include "debug.h"
#include "common.h"
#include "value_int.h"
#include "value_float.h"
#include "object.h"
#include "object_string.h"
#include "object_list.h"
#include "error.h"

#define NO_ARG -1

typedef struct {
    const char *source;
    Token current;
    Token next;
    int errorCount;
    bool panicMode;
} Parser;

typedef struct {
    Token name;
    bool isCaptured;
} Local;

typedef struct {
    uint8_t index;
    bool isLocal;
} Upvalue;

typedef enum {
    TYPE_FUNCTION,
    TYPE_METHOD,
    TYPE_INITIALIZER,
    TYPE_TOP_LEVEL
} FunctionType;

typedef struct Compiler {
    struct Compiler *enclosing;
    ObjFunction *function;
    FunctionType type;
    Local locals[UINT8_MAX + 1];
    int localCount;
    Upvalue upvalues[UINT8_MAX + 1];
} Compiler;

typedef struct ClassCompiler {
    struct ClassCompiler *enclosing;
} ClassCompiler;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,
    PREC_BOOL_OR,
    PREC_BOOL_AND,
    PREC_NOT,
    PREC_COMPARISON,
    PREC_BITWISE_OR,
    PREC_BITWISE_XOR,
    PREC_BITWISE_AND,
    PREC_SHIFT,
    PREC_TERM,
    PREC_FACTOR,
    PREC_UNARY,
    PREC_POWER,
    PREC_CALL,
    PREC_PRIMARY
} Precedence;

typedef enum {
    VAR_MODE_READ,
    VAR_MODE_ASSIGNMENT,
    VAR_MODE_DECLARATION
} VarAccessMode;

typedef void (*ParseFn)(bool, bool);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

static void literal(bool canAssign, bool allowTuple);

static void number(bool canAssign, bool allowTuple);

static void string(bool canAssign, bool allowTuple);

static void rstring(bool canAssign, bool allowTuple);

static void fstring(bool canAssign, bool allowTuple);

static void list(bool canAssign, bool allowTuple);

static void tuple(bool canAssign, bool allowTuple);

static void dict(bool canAssign, bool allowTuple);

static void grouping(bool canAssign, bool allowTuple);

static void unary(bool canAssign, bool allowTuple);

static void binary(bool canAssign, bool allowTuple);

static void variable(bool canAssign, bool allowTuple);

static void and(bool canAssign, bool allowTuple);

static void or(bool canAssign, bool allowTuple);

static void call(bool canAssign, bool allowTuple);

static void item(bool canAssign, bool allowTuple);

static void dot(bool canAssign, bool allowTuple);

static void lambda(bool canAssign, bool allowTuple);

static void statement(int breakPointer, int continuePointer);

ParseRule rules[TOKEN_COUNT] = {
    [TOKEN_LEFT_PAREN]    = {grouping, call,   PREC_CALL},
    [TOKEN_LEFT_BRACKET]  = {list,     item,   PREC_CALL},
    [TOKEN_LEFT_BRACE]    = {dict,     NULL,   PREC_NONE},
    [TOKEN_COMMA]         = {NULL,     tuple,  PREC_PRIMARY},
    [TOKEN_DOT]           = {NULL,     dot,    PREC_CALL},
    [TOKEN_DOUBLE_EQUAL]  = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_BANG_EQUAL]    = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_GREATER]       = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_GREATER_EQUAL] = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_LESS]          = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_LESS_EQUAL]    = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_PLUS]          = {unary,    binary, PREC_TERM},
    [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
    [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
    [TOKEN_DOUBLE_STAR]   = {NULL,     binary, PREC_POWER},
    [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
    [TOKEN_DOUBLE_SLASH]  = {NULL,     binary, PREC_FACTOR},
    [TOKEN_PERCENT]       = {NULL,     binary, PREC_FACTOR},
    [TOKEN_AMPERSAND]     = {NULL,     binary, PREC_BITWISE_AND},
    [TOKEN_CARET]         = {NULL,     binary, PREC_BITWISE_XOR},
    [TOKEN_PIPE]          = {NULL,     binary, PREC_BITWISE_OR},
    [TOKEN_LEFT_SHIFT]    = {NULL,     binary, PREC_SHIFT},
    [TOKEN_RIGHT_SHIFT]   = {NULL,     binary, PREC_SHIFT},
    [TOKEN_TILDE]         = {unary,    NULL,   PREC_UNARY},
    [TOKEN_LAMBDA]        = {lambda,   NULL,   PREC_NONE},
    [TOKEN_NOT]           = {unary,    NULL,   PREC_NONE},
    [TOKEN_IDENTIFIER]    = {variable, NULL,   PREC_NONE},
    [TOKEN_STRING]        = {string,   NULL,   PREC_NONE},
    [TOKEN_RSTRING]       = {rstring,  NULL,   PREC_NONE},
    [TOKEN_FSTRING]       = {fstring,  NULL,   PREC_NONE},
    [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
    [TOKEN_AND]           = {NULL,     and,    PREC_BOOL_AND},
    [TOKEN_OR]            = {NULL,     or,     PREC_BOOL_OR},
    [TOKEN_TRUE]          = {literal,  NULL,   PREC_NONE},
    [TOKEN_FALSE]         = {literal,  NULL,   PREC_NONE},
    [TOKEN_NONE]          = {literal,  NULL,   PREC_NONE},
};

static ParseRule* getRule(TokenType type) {
    return &rules[type];
}

Parser parser;
Compiler *current = NULL;
ClassCompiler *currentClass = NULL;
CodeVec *compilingCode;

static CodeVec* currentCode() {
    return &current->function->code;
}

static void advance() {
    parser.current = parser.next;
    parser.next = scanToken();
    #ifdef DEBUG_PRINT_TOKENS
        printToken(&parser.current);
    #endif
}

static void synchronize() {
    parser.panicMode = false;

    while (parser.current.type != TOKEN_EOF) {
        switch (parser.current.type) {
            case TOKEN_CLASS:
            case TOKEN_DEF:
            case TOKEN_FOR:
            case TOKEN_IF:
            case TOKEN_RETURN:
            case TOKEN_NEWLINE:
            case TOKEN_SEMICOLON:
            case TOKEN_INDENT:
            case TOKEN_DEDENT:
                return;
            default:
                break;
        }

        advance();
    }
}

static bool reportError(const char *message, Token *token) {
    if (parser.panicMode)
        return false;
    parser.panicMode = true;
    parser.errorCount++;
    fprintf(stderr, "\033[31mParse Error\033[0m: %s\n", message);
    if (token != NULL)
        printTokenInCode(parser.source, token);
    return true;
}

static bool check(TokenType type, bool skip) {
    if (skip)
        while (parser.current.type == TOKEN_NEWLINE)
            advance();
    return parser.current.type == type;
}

static bool checkNext(TokenType type, bool skip) {
    if (skip)
        while (parser.next.type == TOKEN_NEWLINE)
            parser.next = scanToken();
    return parser.next.type == type;
}

static bool consume(TokenType type, bool skip) {
    if (!check(type, skip))
        return false;
    advance();
    return true;
}

static bool consumeEOS() {
    return consume(TOKEN_NEWLINE, false) || consume(TOKEN_SEMICOLON, false) || consume(TOKEN_EOF, false);
}

static void emitByte(uint8_t byte, Token token) {
    pushInstruction(currentCode(), byte, token.line, token.column, token.length);
}

static void emitBytes(uint8_t byte1, uint8_t byte2, Token token) {
    emitByte(byte1, token);
    emitByte(byte2, token); 
}

static void emitAssignment(uint8_t op, int arg, Token token) {
    if (arg != NO_ARG)
        emitBytes(op, (uint8_t)arg, token);
    else
        emitByte(op, token);
}

static int emitJump(uint8_t instruction) {
    emitByte(instruction, (Token){0});
    emitBytes(0xff, 0xff, (Token){0});
    return currentCode()->size - 2;
}

static void emitLoop(uint8_t instruction, int loopStart) {
    emitByte(instruction, (Token){0});

    int offset = currentCode()->size - loopStart + 2;

    emitByte((offset >> 8) & 0xff, (Token){0});
    emitByte(offset & 0xff, (Token){0});
}

static void emitReturn() {
    if (current->type == TYPE_INITIALIZER) {
        emitBytes(OP_GET_LOCAL, 0, (Token){0});
    } else {
        emitByte(OP_NONE, (Token){0});
    }
    emitByte(OP_RETURN, (Token){0});
}

static uint8_t createConstant(Value value) {
    ValueVec *constants = &currentCode()->constants;
    int size = constants->size;

    int id = pushConstant(currentCode(), value);
    if (id > UINT8_MAX) {
        reportError("Too many constants in one code block", NULL);
        return 0;
    }
    return id;
}

static void emitConstant(Value value) {
    emitBytes(OP_CONSTANT, createConstant(value), (Token){0});
}

static void initCompiler(Compiler *compiler, FunctionType type, Token name) {
    compiler->enclosing = current;
    compiler->function = createFunction();
    compiler->type = type;
    compiler->localCount = 0;
    current = compiler;
    if (type != TYPE_TOP_LEVEL) {
        current->function->name = copyString(name.start, name.length);
    }
}

static ObjFunction* endCompiler() {
    ObjFunction *function = current->function;
    if (function->code.size == 0 ||
        function->code.code[function->code.size - 1] != OP_RETURN)
            emitReturn();

    #ifdef DEBUG_PRINT_CODE
        if (parser.errorCount == 0)
            printCodeVec(currentCode(), function->name != NULL ? function->name->chars : "<top level>");
    #endif

    ObjTuple *names = allocateTuple(current->localCount);
    for (int i = 0; i < current->localCount; i++) {
        Token name = current->locals[i].name;
        names->values[i] = STRING_VAL(copyString(name.start, name.length));
    }
    function->localNames = names;

    current = current->enclosing;
    return function;
}

static void parseExpression(Precedence precedence, bool canAssign, bool allowTuple) {
    ParseFn prefixFunc = getRule(parser.current.type)->prefix;

    if (prefixFunc == NULL) {
        reportError("Expect expression", &parser.current);
        exit(1);
        return;
    }

    canAssign &= precedence <= PREC_ASSIGNMENT;

    prefixFunc(canAssign, allowTuple);

    while (precedence < getRule(parser.current.type)->precedence) {
        ParseFn infixFunc = getRule(parser.current.type)->infix;
        if (infixFunc == tuple && !allowTuple)
            return;
        infixFunc(canAssign, allowTuple);
    }
}

static void expression(bool tuple) {
    parseExpression(PREC_ASSIGNMENT, false, tuple);
}

static void literal(bool canAssign, bool allowTuple) {
    (void)canAssign;

    switch (parser.current.type) {
        case TOKEN_TRUE:
            emitByte(OP_TRUE, parser.current); 
            break;
        case TOKEN_FALSE:
            emitByte(OP_FALSE, parser.current);
            break;
        case TOKEN_NONE:
            emitByte(OP_NONE, parser.current);
            break;
        default:
            return;
    }
    advance();
}

static void number(bool canAssign, bool allowTuple) {
    (void)canAssign;

    double value = strtod(parser.current.start, NULL);
    if ((long long)value == value)
        emitConstant(INT_VAL(value));
    else
        emitConstant(FLOAT_VAL(value));
    advance();
}

static void string(bool canAssign, bool allowTuple) {
    (void)canAssign;

    emitConstant(STRING_VAL(copyEscapedString(parser.current.start, parser.current.length)));
    advance();
}

static void rstring(bool canAssign, bool allowTuple) {
    (void)canAssign;

    emitConstant(STRING_VAL(copyString(parser.current.start, parser.current.length)));
    advance();
}

static void list(bool canAssign, bool allowTuple) {
    (void)canAssign;

    size_t size = 0;
    advance(true);
    if (!consume(TOKEN_RIGHT_BRACKET, false)) {
        do {
            expression(false);
            size++;
        } while (consume(TOKEN_COMMA, true));
        if (!consume(TOKEN_RIGHT_BRACKET, true))
            reportError("Expect ']'", &parser.current);
    }
    emitBytes(OP_BUILD_LIST, (uint8_t)size, (Token){0});
}

static void tuple(bool canAssign, bool allowTuple) {
    (void)canAssign;
    if (!allowTuple)
        return;
    advance();

    size_t size = 1;
    do {
        expression(false);
        size++;
    } while (consume(TOKEN_COMMA, false));

    emitBytes(OP_BUILD_TUPLE, (uint8_t)size, (Token){0});
}

static void dict(bool canAssign, bool allowTuple) {
    advance();

    size_t size = 0;
    if (!consume(TOKEN_RIGHT_BRACE, false)) {
        do  {
            expression(false);
            if (!consume(TOKEN_COLON, false))
                reportError("':' expected after dictionary key", &parser.current);
            expression(false);
            size++;
        } while (consume(TOKEN_COMMA, false));

        if (!consume(TOKEN_RIGHT_BRACE, false))
            reportError("Expect '}'", &parser.current);
    }

    emitBytes(OP_BUILD_DICT, (uint8_t)size, (Token){0});
}

static void fstring(bool canAssign, bool allowTuple) {
    (void)canAssign;

    int count = 0;

    while (!check(TOKEN_STRING, false)) {
        if (parser.current.length > 0) {
            string(true, true);
            count++;
        } else {
            advance();
        }
        expression(false);
        count++;
    }

    if (parser.current.length > 0) {
        string(true, true);
        count++;
    } else {
        advance();
    }
    emitBytes(OP_BUILD_FSTRING, (uint8_t)count, (Token){0});
}

static void grouping(bool canAssign, bool allowTuple) {
    (void)canAssign;

    advance();
    expression(true);

    if (!consume(TOKEN_RIGHT_PAREN, true))
        reportError("Opening parenthesis does not closed", NULL);
}

static void unary(bool canAssign, bool allowTuple) {
    (void)canAssign;

    Token operator = parser.current;
    advance();

    Precedence precedence = getRule(operator.type)->precedence + 1;
    expression(allowTuple);

    switch (operator.type) {
        case TOKEN_PLUS:
            emitByte(OP_POSITIVE, operator);
            break;
        case TOKEN_MINUS:
            emitByte(OP_NEGATIVE, operator);
            break;
        case TOKEN_TILDE:
            emitByte(OP_INVERT, operator);
            break;
        case TOKEN_NOT:
            emitByte(OP_NOT, operator);
            break;
        default:
            return;
    }
}

static void binary(bool canAssign, bool allowTuple) {
    (void)canAssign;
    
    Token operator = parser.current;
    advance();
    parseExpression(getRule(operator.type)->precedence, false, allowTuple);

    switch (operator.type) {
        case TOKEN_BANG_EQUAL:
            emitByte(OP_NOT_EQUAL, operator);
            break;
        case TOKEN_DOUBLE_EQUAL:
            emitByte(OP_EQUAL, operator);
            break;
        case TOKEN_GREATER:
            emitByte(OP_GREATER, operator);
            break;
        case TOKEN_GREATER_EQUAL:
            emitByte(OP_GREATER_EQUAL, operator);
            break;
        case TOKEN_LESS:
            emitByte(OP_LESS, operator);
            break;
        case TOKEN_LESS_EQUAL:
            emitByte(OP_LESS_EQUAL, operator);
            break;
        case TOKEN_PLUS:
            emitByte(OP_ADD, operator);
            break;
        case TOKEN_MINUS:
            emitByte(OP_SUBTRUCT, operator);
            break;
        case TOKEN_STAR:
            emitByte(OP_MULTIPLY, operator);
            break;
        case TOKEN_DOUBLE_STAR:
            emitByte(OP_POWER, operator);
            break;
        case TOKEN_SLASH:
            emitByte(OP_TRUE_DIVIDE, operator);
            break;
        case TOKEN_DOUBLE_SLASH:
            emitByte(OP_FLOOR_DIVIDE, operator);
            break;
        case TOKEN_PERCENT:
            emitByte(OP_MOD, operator);
            break;
        case TOKEN_AMPERSAND:
            emitByte(OP_BITWISE_AND, operator);
            break;
        case TOKEN_CARET:
            emitByte(OP_BITWISE_XOR, operator);
            break;
        case TOKEN_PIPE:
            emitByte(OP_BITWISE_OR, operator);
            break;
        case TOKEN_LEFT_SHIFT:
            emitByte(OP_LEFT_SHIFT, operator);
            break;
        case TOKEN_RIGHT_SHIFT:
            emitByte(OP_RIGHT_SHIFT, operator);
            break;
        default:
            return;
    }
}

static bool identifierEqual(Token *a, Token *b) {
    if (a->length != b->length)
        return false;
    
    return memcmp(a->start, b->start, a->length) == 0;
}

static uint8_t identifierConstant(Token *name) {
    return createConstant(STRING_VAL(copyString(name->start, name->length)));
}

static void expressionStatement() {
    parseExpression(PREC_ASSIGNMENT, true, true);
    
    if (!consumeEOS())
        reportError("Expect eos after statement", &parser.current);

    emitByte(OP_POP, (Token){0});
}

static void patchJump(int offset) {
    int jump = currentCode()->size - offset - 2;

    if (jump > UINT16_MAX)
        reportError("Too much code to jump over", NULL);

    currentCode()->code[offset] = (jump >> 8) & 0xff;
    currentCode()->code[offset + 1] = jump & 0xff;
}

static void block(int breakPoiner, int continuePointer) {
    while (!check(TOKEN_DEDENT, false) && !check(TOKEN_EOF, false))
        statement(breakPoiner, continuePointer);
    
    consume(TOKEN_DEDENT, false);
}

static void oneLineBlock(int breakPointer, int continuePointer) {
    while (!check(TOKEN_NEWLINE, false) && !check(TOKEN_EOF, false))
        statement(breakPointer, continuePointer);
    
    consume(TOKEN_NEWLINE, false);
}

// ======================================
//               Variables
// ======================================    

static int createLocal(Token name) {
    Local local;
    local.name = name;
    local.isCaptured = false;
    current->locals[current->localCount++] = local;
    return current->localCount - 1;
}

static int resolveLocal(Compiler *compiler, Token *name, bool new) {
    for (int i = 0; i < compiler->localCount; i++) {
        Local *local = &compiler->locals[i];
        if (identifierEqual(name, &local->name))
            return i;
    }
    if (new)
        return createLocal(*name);
    return -1;
}

static int addUpvalue(Compiler *compiler, uint8_t index, bool isLocal) {
    int upvalueCount  = compiler->function->upvalueCount;

    for (int i = 0; i < upvalueCount; i++) {
        Upvalue *upvalue = &compiler->upvalues[i];
        if (upvalue->index == index && upvalue->isLocal == isLocal)
            return i;
    }

    if (upvalueCount == UINT8_MAX + 1) {
        reportError("Too many closure variables in function", NULL);
        return 0;
    }

    compiler->upvalues[upvalueCount].isLocal = isLocal;
    compiler->upvalues[upvalueCount].index = index;
    return compiler->function->upvalueCount++;
}

static int resolveUpvalue(Compiler *compiler, Token *name) {
    // if (compiler->enclosing == NULL)
    //     return -1;
    
    // int offset = resolveLocal(compiler->enclosing, name);
    // if (offset != -1) {
    //     compiler->enclosing->locals[offset].isCaptured = true;
    //     return addUpvalue(compiler, (uint8_t)offset, true);
    // }
    
    // offset = resolveUpvalue(compiler->enclosing, name);
    // if (offset != -1)
    //     return addUpvalue(compiler, (uint8_t)offset, false);

    // return -1;
}

static void resolveVariableAssignment(Token *name, uint8_t *getOp, uint8_t *setOp, uint8_t *arg) {
    if (current->type == TYPE_FUNCTION) {
        *getOp = OP_GET_LOCAL;
        *setOp = OP_SET_LOCAL;
        *arg = resolveLocal(current, name, true);
        return;
    }
    *getOp = OP_GET_GLOBAL;
    *setOp = OP_SET_GLOBAL;
    *arg = identifierConstant(name);
}

static void resolveVariableReference(Token *name, uint8_t *getOp, uint8_t *arg) {
    if (current->type != TYPE_TOP_LEVEL) {
        int offset = resolveLocal(current, name, false);
        if (offset != -1) {
            *getOp = OP_GET_LOCAL;
            *arg = offset;
            return;
        }
    }
    *getOp = OP_GET_GLOBAL;
    *arg = identifierConstant(name);
}

static void declareVariable(Token name) {
    if (current->type != TYPE_TOP_LEVEL)
        resolveLocal(current, &name, true);
}

static void defineVariable(Token name) {
    uint8_t getOp, setOp, arg;
    resolveVariableAssignment(&name, &getOp, &setOp, &arg);
    emitBytes(setOp, arg, name);
    if (setOp == OP_SET_GLOBAL)
        emitByte(OP_POP, (Token){0});
}

static bool isAssignment(Token operator) {
    return TOKEN_EQUAL <= operator.type && operator.type <= TOKEN_RIGHT_SHIFT_EQUAL;
}

static void assignment(uint8_t getOp, uint8_t setOp, int arg, Token operator) {
    if (operator.type != TOKEN_EQUAL)
        emitAssignment(getOp, arg, operator);
    
    expression(false);

    switch (operator.type) {
        case TOKEN_PLUS_EQUAL:
            emitByte(OP_ADD, operator);
            break;
        case TOKEN_MINUS_EQUAL:
            emitByte(OP_SUBTRUCT, operator);
            break;
        case TOKEN_STAR_EQUAL:
            emitByte(OP_MULTIPLY, operator);
            break;
        case TOKEN_DOUBLE_STAR_EQUAL:
            emitByte(OP_POWER, operator);
            break;
        case TOKEN_SLASH_EQUAL:
            emitByte(OP_TRUE_DIVIDE, operator);
            break;
        case TOKEN_DOUBLE_SLASH_EQUAL:
            emitByte(OP_FLOOR_DIVIDE, operator);
            break;
        case TOKEN_PERCENT_EQUAL:
            emitByte(OP_MOD, operator);
            break;
        case TOKEN_AMPERSAND_EQUAL:
            emitByte(OP_BITWISE_AND, operator);
            break;
        case TOKEN_CARET_EQUAL:
            emitByte(OP_BITWISE_XOR, operator);
            break;
        case TOKEN_PIPE_EQUAL:
            emitByte(OP_BITWISE_OR, operator);
            break;
        case TOKEN_LEFT_SHIFT_EQUAL:
            emitByte(OP_LEFT_SHIFT, operator);
            break;
        case TOKEN_RIGHT_SHIFT_EQUAL:
            emitByte(OP_RIGHT_SHIFT, operator);
            break;
        default:
            // to remove warnings
            break;
    }

    emitAssignment(setOp, arg, operator);
}

static void variable(bool canAssign, bool allowTuple) {
    uint8_t getOp, setOp, arg;
    Token name = parser.current;
    advance();
    Token operator = parser.current;

    if (isAssignment(operator)) {
        advance();
        if (!canAssign)
            reportError("Variable assignment is now allowed here", &operator);
        resolveVariableAssignment(&name, &getOp, &setOp, &arg);
        assignment(getOp, setOp, arg, operator);
    } else {
        resolveVariableReference(&name, &getOp, &arg);
        emitBytes(getOp, arg, name);
    }
}

// ======================================
//            Control flow
// ======================================    

static void and(bool canAssign, bool allowTuple) {
    (void)canAssign;

    advance();
    int endJump = emitJump(OP_JUMP_FALSE);

    emitByte(OP_POP, (Token){0});
    parseExpression(PREC_BOOL_AND, false, true);

    patchJump(endJump);
}

static void or(bool canAssign, bool allowTuple){
    (void)canAssign;

    advance();
    int endJump = emitJump(OP_JUMP_TRUE);

    emitByte(OP_POP, (Token){0});
    parseExpression(PREC_BOOL_OR, false, true);

    patchJump(endJump);
}

static void parseBlock(int breakPointer, int continuePointer) {
    if (!consume(TOKEN_COLON, false))
        reportError("Expect ':' after condition", &parser.current);
    
    if (consume(TOKEN_NEWLINE, false)) {
        if (!consume(TOKEN_INDENT, false))
            reportError("Expect indentation block", &parser.current);
        block(breakPointer, continuePointer);
    } else {
        oneLineBlock(breakPointer, continuePointer);
    }
}

static void ifStatement(int breakPointer, int continuePointer) {
    advance(false);
    expression(false);

    int jumpToNextBranch = emitJump(OP_JUMP_FALSE_POP);
    int jumpToEnd = -1;

    parseBlock(breakPointer, continuePointer);

    if (parser.current.type == TOKEN_ELIF) {
        jumpToEnd = emitJump(OP_JUMP);
        while (consume(TOKEN_ELIF, false)) {
            patchJump(jumpToNextBranch);
            expression(false);
            jumpToNextBranch = emitJump(OP_JUMP_FALSE_POP);

            parseBlock(breakPointer, continuePointer);

            emitLoop(OP_LOOP, jumpToEnd - 1);
        }
    }

    if (jumpToEnd == -1)
        jumpToEnd = emitJump(OP_JUMP);
    patchJump(jumpToNextBranch);

    if (consume(TOKEN_ELSE, false))
        parseBlock(breakPointer, continuePointer);

    if (jumpToEnd != -1)
        patchJump(jumpToEnd);
}

static void breakStatement(int breakPointer) {
    if (breakPointer == -1)
        reportError("Can't use 'break' outside of loop body", &parser.current);

    advance();
    
    emitLoop(OP_LOOP, breakPointer - 1);

    if (!consumeEOS())
        reportError("Expect eos after 'break'", &parser.current);
}

static void continueStatement(int continuePointer) {
    if (continuePointer == -1)
        reportError("Can't use 'continue' outside of loop body", &parser.current);
    
    advance();

    emitLoop(OP_LOOP, continuePointer);

    if (!consumeEOS())
        reportError("Expect eos after 'continue'", &parser.current);
}

static int breakJump() {
    emitByte(OP_JUMP, (Token){0});
    emitBytes(0, 3, (Token){0});
    return emitJump(OP_JUMP); 
}

static void whileStatement() {
    advance();

    int jumpToEnd, jumpToEndIfFalse;
    int conditionPointer;

    jumpToEnd = breakJump();
    conditionPointer = currentCode()->size;

    expression(false);
    jumpToEndIfFalse = emitJump(OP_JUMP_FALSE_POP);

    parseBlock(jumpToEnd, conditionPointer);

    emitLoop(OP_LOOP, conditionPointer);

    patchJump(jumpToEnd);
    patchJump(jumpToEndIfFalse);
}

// ======================================
//              Functions 
// ======================================    

static void parseArgs(Compiler *compiler, FunctionType type, Token name) {
    int arity = 0;
    int defaultStart = -1;
    int defaultCount = 0;
    int extraArgs = -1;
    int extraKwargs = -1;

    TokenVec names;
    TokenVecInit(&names);

    if (!check(TOKEN_RIGHT_PAREN, false)) {
        do {
            if (arity > 255) {
                reportError("Can't have more than 255 parameters", &parser.current);
            }

            if (consume(TOKEN_STAR, false)) {
                if (extraArgs != -1)
                    reportError("* argument may appear only once", &parser.current);
                extraArgs = arity;
            }

            if (consume(TOKEN_DOUBLE_STAR, false)) {
                extraKwargs = arity;
            }

            TokenVecPush(&names, parser.current);
            advance();

            if (consume(TOKEN_EQUAL, true)) {
                if (defaultCount == 0)
                    defaultStart = arity;
                if (extraArgs == arity)
                    reportError("var-positional argument cannot have default value", &parser.current);
                if (extraKwargs == arity)
                    reportError("var-keyword argument cannot have default value", &parser.current);
                expression(false);
                defaultCount++;
            } else if (defaultCount > 0 && extraArgs == -1)
                reportError("Non-default argument follows default argument", &parser.current);
            arity++;

        } while (consume(TOKEN_COMMA, true));
    }

    emitBytes(OP_BUILD_TUPLE, (uint8_t)defaultCount, parser.current);

    initCompiler(compiler, type, name);

    current->function->arity = arity;
    current->function->defaultStart = defaultStart;
    current->function->extraArgs = extraArgs;
    current->function->extraKwargs = extraKwargs;

    for (int i = 0; i < names.size; i++)
        createLocal(names.tokens[i]);
    
    TokenVecFree(&names);

}

static void function(FunctionType type) {
    Compiler compiler;
    Token name = parser.current;
    advance();

    if (!consume(TOKEN_LEFT_PAREN, false))
        reportError("Expect '(' after function name", &parser.current);

    parseArgs(&compiler, type, name);

    if (!consume(TOKEN_RIGHT_PAREN, true))
        reportError("Expect ')' after parameters", &parser.current);
    
    parseBlock(-1, -1);

    ObjFunction *function = endCompiler();

    emitBytes(OP_CLOSURE, createConstant(STRING_VAL(function)), (Token){0});

    for (int i = 0; i < function->upvalueCount; i++) {
        emitByte(compiler.upvalues[i].isLocal ? 1 : 0, (Token){0});
        emitByte(compiler.upvalues[i].index, (Token){0});
    }
}

static void funcDeclaration() {
    advance();
    Token name = parser.current;
    declareVariable(name);
    function(TYPE_FUNCTION);
    defineVariable(name);
}

static void lambda(bool canAssign, bool AllowTuple) {
    Compiler compiler;
    Token name; 
    name.start = "<lambda>";
    name.length = 8;
    advance();

    parseArgs(&compiler, TYPE_FUNCTION, name);

    if (!consume(TOKEN_COLON, true))
        reportError("Expect ':' after parameters", &parser.current);

    expression(false);
    emitByte(OP_RETURN, name);

    ObjFunction *function = endCompiler();

    emitBytes(OP_CLOSURE, createConstant(STRING_VAL(function)), (Token){0});

    for (int i = 0; i < function->upvalueCount; i++) {
        emitByte(compiler.upvalues[i].isLocal ? 1 : 0, (Token){0});
        emitByte(compiler.upvalues[i].index, (Token){0});
    }
}

static uint16_t parseArguments() {
    uint8_t argc = 0;
    uint8_t kwargc = 0;

    if (!check(TOKEN_RIGHT_PAREN, false)) {
        do {
            if (check(TOKEN_IDENTIFIER, false) && checkNext(TOKEN_EQUAL, false)) {
                rstring(false, false);
                advance();
                expression(false);
                kwargc++;
            } else{
                if (kwargc > 0)
                    reportError("positional argument follows keyword argument", &parser.current);
                expression(false);
                argc++;
            }
        } while (consume(TOKEN_COMMA, true));
    }

    if (!consume(TOKEN_RIGHT_PAREN, true))
        reportError("Expect ')' after arguments", &parser.current);

    return (argc << 8) | kwargc;
}

static void call(bool canAssign, bool allowTuple) {
    (void)canAssign;

    Token name = parser.current;
    advance();
    uint16_t args = parseArguments();
    uint8_t argc = args >> 8;
    uint8_t kwargc = args & 0xff;
    emitBytes(OP_CALL, argc, name);
    emitByte(kwargc, name);
}

static void method() {
    Token name = parser.current;
    
    uint8_t constant = identifierConstant(&name);
    FunctionType type = TYPE_METHOD;
    if (name.length == 8 && memcmp(name.start, "__init__", 8) == 0)
        type = TYPE_INITIALIZER;
    function(type);
    emitBytes(OP_METHOD, constant, (Token){0});
}

static void classDeclaration() {
    advance();
    Token name = parser.current;
    if (!consume(TOKEN_IDENTIFIER, false))
        reportError("Expect class name", &parser.current);
    
    uint8_t nameConstant = identifierConstant(&name);
    
    declareVariable(name);
    emitBytes(OP_CLASS, nameConstant, name);
    defineVariable(name);

    ClassCompiler classCompiler;
    classCompiler.enclosing = currentClass;
    currentClass = &classCompiler;

    uint8_t getOp, arg;
    resolveVariableReference(&name, &getOp, &arg); 
    emitBytes(getOp, arg, name);

    if (!consume(TOKEN_COLON, false))
        reportError("expect ':'", &parser.current);
    
    if (consume(TOKEN_NEWLINE, false)) {
        if (!consume(TOKEN_INDENT, false))
            reportError("expect indent", &parser.current);
        
        while (!check(TOKEN_DEDENT, false) && !check(TOKEN_EOF, false))
            if (consume(TOKEN_DEF, false))
                method();
        
        consume(TOKEN_DEDENT, false);
    } 
    
    emitByte(OP_POP, (Token){0});

    currentClass = currentClass->enclosing;
}

static void dot(bool canAssign, bool allowTuple) {
    advance();
    Token name = parser.current;
    if (!consume(TOKEN_IDENTIFIER, false)) {
        reportError("Expect property name after '.'", &parser.current);
    }
    uint8_t arg = identifierConstant(&name);

    Token operator = parser.current;
    if (isAssignment(operator)) {
        if (!canAssign)
            reportError("Assignment is not allowed here", &operator);
        
        advance();
        assignment(OP_GET_ATTRIBUTE, OP_SET_ATTRIBUTE, arg, operator);
    } else {
        emitBytes(OP_GET_ATTRIBUTE, arg, name);
    }
}

static void item(bool canAssign, bool allowTuple) {
    (void)canAssign;

    advance(true);
    Token index = parser.current;
    expression(false);
    advance(false);

    Token operator = parser.current;

    if (isAssignment(operator)) {
        advance(true);
        if (!canAssign)
            reportError("Assignment is not allowed here", &operator);

        assignment(OP_GET_ITEM_NO_POP, OP_SET_ITEM, NO_ARG, operator);
    } else {
        emitByte(OP_GET_ITEM, index);
    }
}

static void returnStatement() {
    advance(false);
    if (current->type == TYPE_TOP_LEVEL)
        reportError("Can't return from top-level code", &parser.current);
    
    if (consumeEOS()) {
        emitReturn();
    } else {
        expression(false);
        if (!consumeEOS()) {
            reportError("Expect eos after value", &parser.current);
        }
        emitByte(OP_RETURN, parser.current);
    }
}

static void statement(int breakPointer, int continuePointer) {
    switch (parser.current.type) {
        case TOKEN_NEWLINE:
        case TOKEN_SEMICOLON:
        case TOKEN_PASS:
            advance();
            break;
        case TOKEN_DEF:
            funcDeclaration();
            break;
        case TOKEN_CLASS:
            classDeclaration();
            break;
        case TOKEN_IF:
            ifStatement(breakPointer, continuePointer);
            break;
        case TOKEN_WHILE:
            whileStatement();
            break;
        case TOKEN_BREAK:
            breakStatement(breakPointer);
            break;
        case TOKEN_CONTINUE:
            continueStatement(continuePointer);
            break;
        case TOKEN_RETURN:
            returnStatement();
            break;
        default:
            expressionStatement();
            break;
    }
    if (parser.panicMode)
        synchronize();
}

ObjFunction* compile(const char *source) {
    initScanner(source);
    Compiler compiler;
    initCompiler(&compiler, TYPE_TOP_LEVEL, parser.current);

    parser.errorCount = 0;
    parser.panicMode = false;
    parser.source = source;

    advance();
    advance();

    while (parser.current.type != TOKEN_EOF)
        statement(-1, -1);

    ObjFunction *function = endCompiler();
    function->defaultStart = 0;
    function->defaults = allocateTuple(0);
    return parser.errorCount != 0 ? NULL : function;
}

void markCompilerRoots() {
    Compiler *compiler = current;
    while (compiler != NULL) {
        markObject((Obj*)compiler->function);
        compiler = compiler->enclosing;
    }
}