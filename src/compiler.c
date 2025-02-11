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
#include "object_module.h"
#include "unistd.h"

#define NO_ARG -1

typedef struct Parser {
    struct Parser *enclosing;
    ObjModule *module;
    char *path;
    const char *source;
    Token current;
    Token next;
    TokenVec putback;
    int putPointer;
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
    Token nonlocalNames[UINT8_MAX + 1];
    int nonlocalCount;
    Token globalNames[UINT8_MAX + 1];
    int globalCount;
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

typedef void (*ParseFn)(bool, bool, bool, bool);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

static void literal(bool assign, bool tuple, bool skip, bool del);

static void number(bool assign, bool tuple, bool skip, bool del);

static void string(bool assign, bool tuple, bool skip, bool del);

static void rstring(bool assign, bool tuple, bool skip, bool del);

static void fstring(bool assign, bool tuple, bool skip, bool del);

static void list(bool assign, bool tuple, bool skip, bool del);

static void tuple(bool assign, bool tuple, bool skip, bool de);

static void dict(bool assign, bool tuple, bool skip, bool del);

static void grouping(bool assign, bool tuple, bool skip, bool del);

static void not(bool assign, bool tuple, bool skip, bool del);

static void unary(bool assign, bool tuple, bool skip, bool del);

static void binary(bool assign, bool tuple, bool skip, bool del);

static void is(bool assign, bool tuple, bool skip, bool del);

static void in(bool assign, bool tuple, bool skip, bool del);

static void variable(bool assign, bool tuple, bool skip, bool del);

static void and(bool assign, bool tuple, bool skip, bool del);

static void or(bool assign, bool tuple, bool skip, bool del);

static void call(bool assign, bool tuple, bool skip, bool del);

static void item(bool assign, bool tuple, bool skip, bool del);

static void dot(bool assign, bool tuple, bool skip, bool del);

static void lambda(bool assign, bool tuple, bool skip, bool del);

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
    [TOKEN_NOT]           = {not,      in,     PREC_COMPARISON},
    [TOKEN_IN]            = {NULL,     in,     PREC_COMPARISON},
    [TOKEN_IS]            = {NULL,     is,     PREC_COMPARISON},
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

static ParseRule* getRule(TokenType type, bool tuple) {
    if (!tuple && type == TOKEN_COMMA)
        return &rules[TOKEN_NONE];
    return &rules[type];
}

char *basePath;
Parser *parser;
Compiler *current = NULL;
ClassCompiler *currentClass = NULL;
CodeVec *compilingCode;

void setBasePath(char *path) {
    basePath = path;
}

static CodeVec* currentCode() {
    return &current->function->code;
}

static bool check(TokenType type) {
    return parser->current.type == type;
}

static void advance(bool skip) {
    if (parser->putPointer >= 0) {
        parser->current = parser->putback.tokens[parser->putPointer++];
        if (parser->putPointer == parser->putback.size)
            parser->putPointer = -1;
        return;
    }
    parser->current = parser->next; 
    if (skip && check(TOKEN_NEWLINE))
        parser->current = scanToken(true);
    parser->next = scanToken(false);
    #ifdef DEBUG_PRINT_TOKENS
        printToken(&parser->current);
    #endif
}

static void advancePut() {
    advance(false);
    TokenVecPush(&parser->putback, parser->current);
}

static void putback() {
    parser->putPointer = 0;
}

static void discardPutback() {
    parser->putback.size = 0;
}

static void synchronize() {
    parser->panicMode = false;

    while (parser->current.type != TOKEN_EOF) {
        switch (parser->current.type) {
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
        advance(false);
    }
}

static bool reportError(const char *message, Token *token) {
    if (parser->panicMode)
        return false;
    parser->panicMode = true;
    parser->errorCount++;
    fprintf(stderr, "\033[31mSyntax Error\033[0m: %s\n", message);
    if (token != NULL)
        printTokenInCode(parser->source, token);
    return true;
}

static bool checkNext(TokenType type, bool skip) {
    if (skip && (parser->next.type == TOKEN_NEWLINE || parser->next.type == TOKEN_INDENT || parser->next.type == TOKEN_DEDENT) )
        parser->next = scanToken(true);
    return parser->next.type == type;
}

static bool consume(TokenType type, bool skip) {
    if (!check(type))
        return false;
    advance(skip);
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
    } else if (current->type == TYPE_TOP_LEVEL)  {
        emitByte(OP_UNLOAD_MODULE, (Token){0});
        return;
    } else {
        emitByte(OP_NONE, (Token){0});
    }
    emitByte(OP_RETURN, (Token){0});
}

static bool fastValueEqual(Value a, Value b) {
    if (a.type != b.type)
        return false;

    switch(a.type) {
        case VAL_NONE:
            return true;
        case VAL_BOOL:
        case VAL_INT:
            return a.as.integer == b.as.integer;
        case VAL_FLOAT:
            return a.as.floating == b.as.floating;
        case VAL_STRING:
            return valueToBool(String_Equal(a, b));
    }
    return false;
}

static uint8_t createConstant(Value value) {
    ValueVec *constants = &currentCode()->constants;
    int size = constants->size;

    for (int i = 0; i < size; i++) {
        if (fastValueEqual(constants->values[i], value))
            return i;
    }

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

static void initParser(Parser *p, ObjModule *module, const char *source) {
    p->errorCount = 0;
    p->module = module;
    p->panicMode = false;
    p->source = source;
    TokenVecInit(&p->putback);
    p->putPointer = -1;
    p->enclosing = parser;
    parser = p;
}

static void endParser() {
    parser = parser->enclosing;
}

static void initCompiler(Compiler *compiler, FunctionType type, Token name) {
    compiler->enclosing = current;
    compiler->function = createFunction();
    compiler->type = type;
    compiler->localCount = 0;
    compiler->nonlocalCount = 0;
    compiler->globalCount = 0;
    current = compiler;
    if (type != TYPE_TOP_LEVEL) {
        current->function->name = copyString(name.start, name.length);
    }
}

static ObjFunction* endCompiler() {
    ObjFunction *function = current->function;
    function->module = parser->module;
    if (function->code.size == 0 ||
        function->code.code[function->code.size - 1] != OP_RETURN)
            emitReturn();

    #ifdef DEBUG_PRINT_CODE
        if (parser->errorCount == 0)
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

static void parseExpression(Precedence precedence, bool assign, bool tuple, bool skip, bool del) {
    ParseFn prefixFunc = getRule(parser->current.type, tuple)->prefix;

    if (prefixFunc == NULL) {
        reportError("Expect expression", &parser->current);
        exit(1);
        return;
    }

    assign &= precedence <= PREC_ASSIGNMENT;

    prefixFunc(assign, tuple, skip, del);

    while (precedence < getRule(parser->current.type, tuple)->precedence) {
        ParseFn infixFunc = getRule(parser->current.type, tuple)->infix;
        infixFunc(assign, tuple, skip, del);
    }
}

static void expression(bool tuple, bool skip) {
    parseExpression(PREC_ASSIGNMENT, false, tuple, skip, false);
}

static void literal(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete literal", &parser->current);

    switch (parser->current.type) {
        case TOKEN_TRUE:
            emitByte(OP_TRUE, parser->current); 
            break;
        case TOKEN_FALSE:
            emitByte(OP_FALSE, parser->current);
            break;
        case TOKEN_NONE:
            emitByte(OP_NONE, parser->current);
            break;
        default:
            return;
    }
    advance(skip);
}

static void number(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete literal", &parser->current);

    double value = strtod(parser->current.start, NULL);
    if ((long long)value == value)
        emitConstant(INT_VAL(value));
    else
        emitConstant(FLOAT_VAL(value));
    advance(skip);
}

static void string(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete literal", &parser->current);

    emitConstant(STRING_VAL(copyEscapedString(parser->current.start, parser->current.length)));
    advance(skip);
}

static void rstring(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete literal", &parser->current);

    emitConstant(STRING_VAL(copyString(parser->current.start, parser->current.length)));
    advance(skip);
}

static void list(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete literal", &parser->current);

    size_t size = 0;
    advance(true);
    do {
        if (check(TOKEN_RIGHT_BRACKET))
            break;
        expression(false, true);
        size++;
    } while (consume(TOKEN_COMMA, true));

    if (!consume(TOKEN_RIGHT_BRACKET, false))
        reportError("Expect ']'", &parser->current);

    emitBytes(OP_BUILD_LIST, (uint8_t)size, (Token){0});
}

static void tuple(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete literal", &parser->current);

    advance(true);

    size_t size = 1;
    do {
        if (check(TOKEN_NEWLINE) || check(TOKEN_RIGHT_PAREN))
            break;
        expression(false, skip);
        size++;
    } while (consume(TOKEN_COMMA, true));

    emitBytes(OP_BUILD_TUPLE, (uint8_t)size, (Token){0});
}

static void dict(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete literal", &parser->current);

    advance(true);

    size_t size = 0;
    do  {
        if (check(TOKEN_RIGHT_BRACE))
            break;
        expression(false, true);
        if (!consume(TOKEN_COLON, true))
            reportError("':' expected after dictionary key", &parser->current);
        expression(false, true);
        size++;
    } while (consume(TOKEN_COMMA, true));

    if (!consume(TOKEN_RIGHT_BRACE, false))
        reportError("Expect '}'", &parser->current);

    emitBytes(OP_BUILD_DICT, (uint8_t)size, (Token){0});
}

static void fstring(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete literal", &parser->current);

    int count = 0;

    while (!check(TOKEN_STRING)) {
        if (parser->current.length > 0) {
            string(true, true, skip, false);
            count++;
        } else {
            advance(skip);
        }
        expression(true, skip);
        count++;
    }

    if (parser->current.length > 0) {
        string(true, true, skip, false);
        count++;
    } else {
        advance(skip);
    }
    emitBytes(OP_BUILD_FSTRING, (uint8_t)count, (Token){0});
}

static void grouping(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete expression", &parser->current);

    advance(true);

    if (consume(TOKEN_RIGHT_PAREN, false)) {
        emitBytes(OP_BUILD_TUPLE, 0, (Token){0});
        return;
    }

    expression(true, true);

    if (!consume(TOKEN_RIGHT_PAREN, false))
        reportError("Opening parenthesis does not closed", NULL);
}

static void not(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete expression", &parser->current);
        
    Token operator = parser->current;
    advance(skip);

    expression(false, skip);

    emitByte(OP_NOT, operator);
}

static void unary(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete expression", &parser->current);
        
    Token operator = parser->current;
    advance(skip);

    parseExpression(getRule(operator.type, tuple)->precedence, false, false, skip, false);

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

static void binary(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete expression", &parser->current);

    Token operator = parser->current;
    advance(skip);

    parseExpression(getRule(operator.type, tuple)->precedence, false, false, skip, false);

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
        case TOKEN_IN:
            emitByte(OP_CONTAINS, operator);
            break;
        default:
            return;
    }
}

static void is(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete comparison", &parser->current);

    Token operator = parser->current;
    advance(skip);

    bool negate = consume(TOKEN_NOT, skip);

    parseExpression(PREC_COMPARISON, false, false, skip, false);

    emitByte(OP_IS, operator);
    if (negate)
        emitByte(OP_NOT, operator);
}

static void in(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete comparison", &parser->current);

    Token operator = parser->current;
    bool negate = consume(TOKEN_NOT, skip);

    if (!consume(TOKEN_IN, skip)) {
        reportError("invalid syntax", &operator);
        return;
    }
    
    parseExpression(PREC_COMPARISON, false, false, skip, false);

    emitByte(OP_CONTAINS, operator);
    if (negate)
        emitByte(OP_NOT, operator);
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
    parseExpression(PREC_ASSIGNMENT, true, true, false, false);
    
    if (!consumeEOS())
        reportError("Expect eos after statement", &parser->current);

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
    while (!check(TOKEN_DEDENT) && !check(TOKEN_EOF))
        statement(breakPoiner, continuePointer);
    
    consume(TOKEN_DEDENT, false);
}

static void oneLineBlock(int breakPointer, int continuePointer) {
    while (!check(TOKEN_NEWLINE) && !check(TOKEN_EOF))
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

static bool isGlobal(Compiler *compiler, Token *name) {
    for (int i = 0; i < compiler->globalCount; i++) {
        if (identifierEqual(name, &compiler->globalNames[i]))
            return true;
    }
    return false;
}

static bool isNonlocal(Compiler *compiler, Token *name) {
    for (int i = 0; i < compiler->nonlocalCount; i++) {
        if (identifierEqual(name, &compiler->nonlocalNames[i]))
            return true;
    }
    return false;
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
    if (compiler->enclosing == NULL)
        return -1;
    
    int offset = resolveLocal(compiler->enclosing, name, false);
    if (offset != -1) {
        compiler->enclosing->locals[offset].isCaptured = true;
        return addUpvalue(compiler, (uint8_t)offset, true);
    }
    
    offset = resolveUpvalue(compiler->enclosing, name);
    if (offset != -1)
        return addUpvalue(compiler, (uint8_t)offset, false);

    return -1;
}

static void resolveVariableAssignment(Token *name, uint8_t *getOp, uint8_t *setOp, uint8_t *arg) {
    if (current->type == TYPE_TOP_LEVEL || isGlobal(current, name)) {
        *getOp = OP_GET_GLOBAL;
        *setOp = OP_SET_GLOBAL;
        *arg = identifierConstant(name);
        return;
    }

    if (isNonlocal(current, name)) {
        *getOp = OP_GET_UPVALUE;
        *setOp = OP_SET_UPVALUE;
        *arg = resolveUpvalue(current, name);
        return;
    }

    *getOp = OP_GET_LOCAL;
    *setOp = OP_SET_LOCAL;
    *arg = resolveLocal(current, name, true);
}

static void resolveVariableReference(Token *name, uint8_t *getOp, uint8_t *delOp, uint8_t *arg) {
    if (current->type != TYPE_TOP_LEVEL && !isGlobal(current, name)) {
        int offset = resolveLocal(current, name, false);
        if (offset != -1) {
            *getOp = OP_GET_LOCAL;
            *delOp = OP_DEL_LOCAL;
            *arg = offset;
            return;
        }

        offset = resolveUpvalue(current, name);
        if (offset != -1) {
            *getOp = OP_GET_UPVALUE;
            *delOp = OP_DEL_UPVALUE;
            *arg = offset;
            return;
        }
    }

    *getOp = OP_GET_GLOBAL;
    *delOp = OP_DEL_GLOBAL;
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
    if (operator.type != TOKEN_EQUAL && operator.type != TOKEN_COLON_EQUAL)
        emitAssignment(getOp, arg, operator);
    
    expression(true, false);

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

static void unpack(Token name) {
    TokenVecPush(&parser->putback, name);
    TokenVecPush(&parser->putback, parser->current);
    int c = 1;
    while (check(TOKEN_COMMA)) {
        advancePut();
        if (check(TOKEN_IDENTIFIER))
            advancePut();
        else
            break;
        c++;
    }

    if (consume(TOKEN_EQUAL, false)) {
        expression(true, false);

        int j = 0;
        for (int i = 0; i < c; i++) {
            emitConstant(INT_VAL(i));
            emitByte(OP_GET_ITEM_NO_POP, (Token){0});
            Token name = parser->putback.tokens[j];
            j+=2;
            uint8_t getOp, setOp, arg;
            defineVariable(name);
            emitByte(OP_POP, (Token){0});
        }
        discardPutback();
    } else {
        putback();
    }
}

static void variable(bool assign, bool tuple, bool skip, bool del) {
    uint8_t getOp, setOp, delOp, arg;
    Token name = parser->current;
    advance(skip);
    Token operator = parser->current;

    if (isAssignment(operator)) {
        advance(skip);
        if (!assign)
            reportError("invalid syntax. Maybe you meant '==' or ':=' instead of '='?", &parser->current);
        resolveVariableAssignment(&name, &getOp, &setOp, &arg);
        assignment(getOp, setOp, arg, operator);
    } else if (operator.type == TOKEN_COLON_EQUAL) {
        advance(skip);
        resolveVariableAssignment(&name, &getOp, &setOp, &arg);
        assignment(getOp, setOp, arg, operator);
    } else if (del && !check(TOKEN_DOT) && !check(TOKEN_LEFT_BRACKET)) {
        resolveVariableReference(&name, &getOp, &delOp, &arg);
        emitBytes(delOp, arg, name);
    } else if (assign && check(TOKEN_COMMA) && parser->putPointer < 0) {
        unpack(name);
    } else {
        resolveVariableReference(&name, &getOp, &delOp, &arg);
        emitBytes(getOp, arg, name);
    }
}

// ======================================
//            Control flow
// ======================================    

static void and(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete expression", &parser->current);

    advance(skip);
    int endJump = emitJump(OP_JUMP_FALSE);

    emitByte(OP_POP, (Token){0});
    // question
    parseExpression(PREC_BOOL_AND, false, true, skip, false);

    patchJump(endJump);
}

static void or(bool assign, bool tuple, bool skip, bool del){
    if (del)
        reportError("cannot delete expression", &parser->current);

    advance(skip);
    int endJump = emitJump(OP_JUMP_TRUE);

    emitByte(OP_POP, (Token){0});
    parseExpression(PREC_BOOL_OR, false, true, skip, false);

    patchJump(endJump);
}

static void parseBlock(int breakPointer, int continuePointer) {
    if (!consume(TOKEN_COLON, false))
        reportError("Expect ':' after condition", &parser->current);
    
    if (consume(TOKEN_NEWLINE, false)) {
        if (!consume(TOKEN_INDENT, false))
            reportError("Expect indentation block", &parser->current);
        block(breakPointer, continuePointer);
    } else {
        oneLineBlock(breakPointer, continuePointer);
    }
}

static void ifStatement(int breakPointer, int continuePointer) {
    advance(false);
    expression(false, false);

    int jumpToNextBranch = emitJump(OP_JUMP_FALSE_POP);
    int jumpToEnd = -1;

    parseBlock(breakPointer, continuePointer);

    if (parser->current.type == TOKEN_ELIF) {
        jumpToEnd = emitJump(OP_JUMP);
        while (consume(TOKEN_ELIF, false)) {
            patchJump(jumpToNextBranch);
            expression(false, false);
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
        reportError("Can't use 'break' outside of loop body", &parser->current);

    advance(false);
    
    emitLoop(OP_LOOP, breakPointer - 1);

    if (!consumeEOS())
        reportError("Expect eos after 'break'", &parser->current);
}

static void continueStatement(int continuePointer) {
    if (continuePointer == -1)
        reportError("Can't use 'continue' outside of loop body", &parser->current);
    
    advance(false);

    emitLoop(OP_LOOP, continuePointer);

    if (!consumeEOS())
        reportError("Expect eos after 'continue'", &parser->current);
}

static int breakJump() {
    emitByte(OP_JUMP, (Token){0});
    emitBytes(0, 3, (Token){0});
    return emitJump(OP_JUMP); 
}

static void whileStatement() {
    advance(false);

    int jumpToEnd, jumpToEndIfFalse;
    int conditionPointer;

    jumpToEnd = breakJump();
    conditionPointer = currentCode()->size;

    expression(false, false);
    jumpToEndIfFalse = emitJump(OP_JUMP_FALSE_POP);

    parseBlock(jumpToEnd, conditionPointer);

    emitLoop(OP_LOOP, conditionPointer);

    patchJump(jumpToEndIfFalse);

    if (consume(TOKEN_ELSE, false))
        parseBlock(-1, -1);

    patchJump(jumpToEnd);
}

static void forStatement() {
    advance(false);

    Token name = parser->current;
    if (!consume(TOKEN_IDENTIFIER, false))
        reportError("Syntax error", &name);

    if (!consume(TOKEN_IN, false))
        reportError("Syntax error", &name);

    int jumpToEnd = breakJump();
    
    expression(true, false);
    emitByte(OP_BUILD_ITERATOR, name);

    int jumpToExcept = emitJump(OP_JUMP_NEXT);

    uint8_t getOp, setOp, arg;
    resolveVariableAssignment(&name, &getOp, &setOp, &arg);
    emitBytes(setOp, arg, name);
    emitByte(OP_POP, name);

    parseBlock(jumpToEnd, jumpToExcept - 1);

    emitLoop(OP_LOOP, jumpToExcept - 1);
    patchJump(jumpToExcept);

    if (consume(TOKEN_ELSE, false))
        parseBlock(-1, -1);

    patchJump(jumpToEnd);
    emitByte(OP_POP, name);
}

static void tryStatement(int breakPointer, int continuePointer) {
    advance(false);

    int jumpToExcept = emitJump(OP_SETUP_TRY);

    parseBlock(breakPointer, continuePointer);

    emitByte(OP_NONE, (Token){0});

    int jumpToElse = emitJump(OP_JUMP);
    int jumpToFinally = emitJump(OP_JUMP);

    patchJump(jumpToExcept);

    emitByte(OP_END_TRY, (Token){0});
    jumpToExcept = emitJump(OP_SETUP_TRY);

    if (!check(TOKEN_EXCEPT) && !check(TOKEN_FINALLY))
        reportError("expected 'except' or 'finally' block", &parser->current);

    int jumpToNextBranch = -1;

    while (consume(TOKEN_EXCEPT, false)) {
        if (jumpToNextBranch != -1)
            patchJump(jumpToNextBranch);

        Token name = (Token){0};

        if (!check(TOKEN_COLON)) {
            expression(false, false);
            emitByte(OP_IS_INSTANCE, (Token){0});
            jumpToNextBranch = emitJump(OP_JUMP_FALSE_POP);

            if (consume(TOKEN_AS, false)) {
                name = parser->current;
                if (!consume(TOKEN_IDENTIFIER, false))
                    reportError("invalid syntax", &parser->current);
                
                uint8_t getOp, setOp, arg;
                resolveVariableAssignment(&name, &getOp, &setOp, &arg);
                emitBytes(setOp, arg, name);
            }
        }

        parseBlock(breakPointer, continuePointer);

        if (name.start != NULL) {
            uint8_t getOp, delOp, arg;
            resolveVariableReference(&name, &getOp, &delOp, &arg);
            emitBytes(delOp, arg, name);
        }

        emitLoop(OP_LOOP, jumpToFinally - 1);
    }

    if (jumpToNextBranch != -1)
        patchJump(jumpToNextBranch);
    patchJump(jumpToExcept);

    emitByte(OP_FALSE, (Token){0});

    int jumpSkip = emitJump(OP_JUMP);

    patchJump(jumpToElse);

    if (consume(TOKEN_ELSE, false)) {
        parseBlock(breakPointer, continuePointer);
    }

    emitLoop(OP_LOOP, jumpToFinally - 1);

    patchJump(jumpToFinally);

    emitByte(OP_TRUE, (Token){0});

    patchJump(jumpSkip);

    emitByte(OP_END_TRY, (Token){0});

    if (consume(TOKEN_FINALLY, false)) {
        parseBlock(breakPointer, continuePointer);
    }

    int jumpToEnd = emitJump(OP_JUMP_TRUE_POP);

    emitByte(OP_RAISE, (Token){0});

    patchJump(jumpToEnd);

    emitByte(OP_POP, (Token){0});
}

static void raiseStatement() {
    advance(false);

    if (!consumeEOS())
        expression(false, false);

    emitByte(OP_RAISE, parser->current);
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

    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            if (arity > 255) {
                reportError("Can't have more than 255 parameters", &parser->current);
            }

            if (consume(TOKEN_STAR, true)) {
                if (extraArgs != -1)
                    reportError("* argument may appear only once", &parser->current);
                extraArgs = arity;
            }

            if (consume(TOKEN_DOUBLE_STAR, true)) {
                extraKwargs = arity;
            }

            TokenVecPush(&names, parser->current);
            advance(true);

            if (consume(TOKEN_EQUAL, true)) {
                if (defaultCount == 0)
                    defaultStart = arity;
                if (extraArgs == arity)
                    reportError("var-positional argument cannot have default value", &parser->current);
                if (extraKwargs == arity)
                    reportError("var-keyword argument cannot have default value", &parser->current);
                expression(false, true);
                defaultCount++;
            } else if (defaultCount > 0 && extraArgs == -1)
                reportError("Non-default argument follows default argument", &parser->current);
            arity++;

        } while (consume(TOKEN_COMMA, true));
    }

    emitBytes(OP_BUILD_TUPLE, (uint8_t)defaultCount, parser->current);

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
    Token name = parser->current;
    advance(false);

    if (!consume(TOKEN_LEFT_PAREN, false))
        reportError("Expect '(' after function name", &parser->current);

    parseArgs(&compiler, type, name);

    if (!consume(TOKEN_RIGHT_PAREN, true))
        reportError("Expect ')' after parameters", &parser->current);
    
    parseBlock(-1, -1);

    ObjFunction *function = endCompiler();

    emitBytes(OP_CLOSURE, createConstant(OBJ_VAL(function)), (Token){0});

    for (int i = 0; i < function->upvalueCount; i++) {
        emitByte(compiler.upvalues[i].isLocal ? 1 : 0, (Token){0});
        emitByte(compiler.upvalues[i].index, (Token){0});
    }
}

static void funcDeclaration() {
    advance(false);
    Token name = parser->current;
    declareVariable(name);
    function(TYPE_FUNCTION);
    defineVariable(name);
}

static void lambda(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete lambda", &parser->current);

    Compiler compiler;
    Token name; 
    name.start = "<lambda>";
    name.length = 8;
    advance(skip);

    parseArgs(&compiler, TYPE_FUNCTION, name);

    if (!consume(TOKEN_COLON, true))
        reportError("Expect ':' after parameters", &parser->current);

    expression(false, skip);
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

    if (check(TOKEN_NEWLINE))
        advance(true);

    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            if (check(TOKEN_IDENTIFIER) && checkNext(TOKEN_EQUAL, false)) {
                rstring(false, false, true, false);
                advance(true);
                expression(false, true);
                kwargc++;
            } else{
                if (kwargc > 0)
                    reportError("positional argument follows keyword argument", &parser->current);
                expression(false, true);
                argc++;
            }
        } while (consume(TOKEN_COMMA, true));
    }

    if (!consume(TOKEN_RIGHT_PAREN, false))
        reportError("Expect ')' after arguments", &parser->current);

    return (argc << 8) | kwargc;
}

static void call(bool assign, bool tuple, bool skip, bool del) {
    if (del)
        reportError("cannot delete function call", &parser->current);

    Token name = parser->current;
    advance(skip);
    uint16_t args = parseArguments();
    uint8_t argc = args >> 8;
    uint8_t kwargc = args & 0xff;
    emitBytes(OP_CALL, argc, name);
    emitByte(kwargc, name);
}

static void method() {
    Token name = parser->current;
    
    uint8_t constant = identifierConstant(&name);
    FunctionType type = TYPE_METHOD;
    if (name.length == 8 && memcmp(name.start, "__init__", 8) == 0)
        type = TYPE_INITIALIZER;
    function(type);
    emitBytes(OP_METHOD, constant, (Token){0});
}

static void classDeclaration() {
    advance(false);
    Token name = parser->current;
    Token super = (Token){.type=TOKEN_EOF}; 
    if (!consume(TOKEN_IDENTIFIER, false))
        reportError("Expect class name", &parser->current);
    
    if (consume(TOKEN_LEFT_PAREN, false)) {
        if (!consume(TOKEN_RIGHT_PAREN, false)) {
            super = parser->current;
            if (!consume(TOKEN_IDENTIFIER, false))
                reportError("Expect super class name", &parser->current);
            if (!consume(TOKEN_RIGHT_PAREN, false))    
                reportError("Expect ')' after super class name", &parser->current);
        }
    }

    uint8_t getOp, delOp, arg;
    if (super.type != TOKEN_EOF) {
        resolveVariableReference(&super, &getOp, &delOp, &arg);
        emitBytes(getOp, arg, super);
    } else {
        emitByte(OP_NONE, name);
    }
    
    uint8_t nameConstant = identifierConstant(&name);
    
    declareVariable(name);
    emitBytes(OP_CLASS, nameConstant, name);
    defineVariable(name);

    ClassCompiler classCompiler;
    classCompiler.enclosing = currentClass;
    currentClass = &classCompiler;

    resolveVariableReference(&name, &getOp, &delOp, &arg); 
    emitBytes(getOp, arg, name);

    if (!consume(TOKEN_COLON, false))
        reportError("expect ':'", &parser->current);
    
    if (consume(TOKEN_NEWLINE, false)) {
        if (!consume(TOKEN_INDENT, false))
            reportError("expect indent", &parser->current);
        
        while (!check(TOKEN_DEDENT) && !check(TOKEN_EOF))
            if (consume(TOKEN_DEF, false))
                method();
        
        consume(TOKEN_DEDENT, false);
    } 
    
    emitByte(OP_POP, (Token){0});

    currentClass = currentClass->enclosing;
}

static void dot(bool assign, bool tuple, bool skip, bool del) {
    advance(skip);
    Token name = parser->current;
    if (!consume(TOKEN_IDENTIFIER, skip)) {
        reportError("Expect property name after '.'", &parser->current);
    }
    uint8_t arg = identifierConstant(&name);

    Token operator = parser->current;
    if (isAssignment(operator)) {
        if (!assign)
            reportError("Assignment is not allowed here", &operator);
        
        advance(skip);
        assignment(OP_GET_ATTRIBUTE, OP_SET_ATTRIBUTE, arg, operator);
    } else if (del) {
        emitBytes(OP_DEL_ATTRIBUTE, arg, name);
    } else {
        emitBytes(OP_GET_ATTRIBUTE, arg, name);
    }
}

static void item(bool assign, bool tuple, bool skip, bool del) {
    advance(true);
    Token index = parser->current;
    bool isSlice = false;

    if (!check(TOKEN_COLON))
        expression(true, true);
    else {
        emitByte(OP_NONE, (Token){0});
        isSlice = true;
    }

    if (consume(TOKEN_COLON, false)) {
        if (!check(TOKEN_RIGHT_BRACKET) && !check(TOKEN_COLON))
            expression(true, true);
        else
            emitByte(OP_NONE, (Token){0});
        isSlice = true;
    }

    if (consume(TOKEN_COLON, false)) {
        if (!check(TOKEN_RIGHT_BRACKET))
            expression(true, true);
        else
            emitByte(OP_NONE, (Token){0});
    } else if (isSlice) {
        emitByte(OP_NONE, (Token){0});
    }
    advance(false);

    if (isSlice)
        emitByte(OP_BUILD_SLICE, (Token){0});

    Token operator = parser->current;

    if (isAssignment(operator)) {
        advance(true);
        if (!assign)
            reportError("Assignment is not allowed here", &operator);
        assignment(OP_GET_ITEM_NO_POP, OP_SET_ITEM, NO_ARG, operator);
    } else if (del && !check(TOKEN_LEFT_BRACKET)) {
        emitByte(OP_DEL_ITEM, index);
    } else {
        emitByte(OP_GET_ITEM, index);
    }
}

static void returnStatement() {
    advance(false);
    if (current->type == TYPE_TOP_LEVEL)
        reportError("Can't return from top-level code", &parser->current);
    
    if (consumeEOS()) {
        emitReturn();
    } else {
        expression(true, false);
        if (!consumeEOS()) {
            reportError("Expect eos after value", &parser->current);
        }
        emitByte(OP_RETURN, parser->current);
    }
}

static void delStatement() {
    advance(false);
    do {
        parseExpression(PREC_ASSIGNMENT, false, false, false, true);
    } while (consume(TOKEN_COMMA, false));
}

static void assertStatement() {
    advance(false);
    int line = parser->current.line;
    int column = parser->current.column;

    expression(false, false);

    if (consume(TOKEN_COMMA, false))
        expression(false, false);
    else
        emitByte(OP_NONE, parser->current);

    int len = parser->current.column - column;

    emitByte(OP_ASSERT, (Token){.line=line, .column=column, .length=len});

    if (!consumeEOS())
        reportError("invalid syntax", &parser->current);
}

static void globalStatement() {
    if (current->type == TYPE_TOP_LEVEL)
        reportError("global declaration not allowed at module level", &parser->current);
    advance(false);

    Token name = parser->current;
    if (!consume(TOKEN_IDENTIFIER, false))
        reportError("invalid syntax", &parser->current);

    if (isNonlocal(current, &name))
        reportError("name is nonlocal and global", &parser->current);
    
    if (resolveLocal(current, &name, false) != -1)
        reportError("name 'x' is assigned to before global declaration", &parser->current);

    current->globalNames[current->globalCount++] = name;
    consumeEOS();
}

static void nonlocalStatement() {
    if (current->type == TYPE_TOP_LEVEL)
        reportError("nonlocal declaration not allowed at module level", &parser->current);
    advance(false);

    Token name = parser->current;
    if (!consume(TOKEN_IDENTIFIER, false))
        reportError("invalid syntax", &parser->current);

    if (isGlobal(current, &name))
        reportError("name is nonlocal and global", &parser->current);

    if (resolveUpvalue(current, &name) == -1)
        reportError("no binding for nonlocal 'x' found", &parser->current);

    if (resolveLocal(current, &name, false) != -1)
        reportError("name 'x' is assigned to before global declaration", &parser->current);

    current->nonlocalNames[current->nonlocalCount++] = name;
    consumeEOS();
}

static void importStatement() {
    advance(false);
    Token name = parser->current;
    char filename[50];
    strncpy(filename, name.start, name.length);
    filename[name.length] = '\0';
    advance(false);
    char fullname[256];
    snprintf(fullname, sizeof(fullname), "%s%s%s%s", basePath, "/", filename, ".py");

    char *source = readFile(fullname);
    ObjModule *module = compile(source, fullname, filename);

    emitBytes(OP_CONSTANT, createConstant(OBJ_VAL(module)), parser->current);
    uint8_t getOp, setOp, arg;
    resolveVariableAssignment(&name, &getOp, &setOp, &arg);
    emitBytes(setOp, arg, name);

    emitByte(OP_LOAD_MODULE, parser->current);
    consumeEOS();
}

static void statement(int breakPointer, int continuePointer) {
    switch (parser->current.type) {
        case TOKEN_NEWLINE:
        case TOKEN_SEMICOLON:
        case TOKEN_PASS:
            advance(false);
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
        case TOKEN_FOR:
            forStatement();
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
        case TOKEN_TRY:
            tryStatement(breakPointer, continuePointer);
            break;
        case TOKEN_RAISE:
            raiseStatement();
            break;
        case TOKEN_DEL:
            delStatement();
            break;
        case TOKEN_ASSERT:
            assertStatement();
            break;
        case TOKEN_GLOBAL:
            globalStatement();
            break;
        case TOKEN_NONLOCAL:
            nonlocalStatement();
            break;
        case TOKEN_IMPORT:
            importStatement();
            break;
        default:
            expressionStatement();
            break;
    }
    if (parser->panicMode)
        synchronize();
}

ObjModule* compile(const char *source, const char *path, const char *name) {
    ObjModule *module = allocateModule(function);
    module->path = path;
    Scanner s;
    initScanner(&s, source);
    Parser p;
    initParser(&p, module, source);
    Compiler compiler;
    initCompiler(&compiler, TYPE_TOP_LEVEL, parser->current);

    advance(false);
    advance(false);

    while (parser->current.type != TOKEN_EOF)
        statement(-1, -1);

    ObjFunction *function = endCompiler();
    int errorCount = parser->errorCount;
    endParser();
    endScanner();
    function->defaultStart = 0;
    function->defaults = allocateTuple(0);
    function->name = copyString(name, 0);
    module->function = function;
    return errorCount != 0 ? NULL : module;
}

void markCompilerRoots() {
    Compiler *compiler = current;
    while (compiler != NULL) {
        markObject((Obj*)compiler->function);
        compiler = compiler->enclosing;
    }
}