#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "memory.h"
#include "token.h"
#include "scanner.h"
#include "debug.h"
#include "common.h"
#include "object.h"
#include "object_string.h"
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
    PREC_OR,
    PREC_AND,
    PREC_EQUALITY,
    PREC_COMPARISON,
    PREC_TERM,
    PREC_FACTOR,
    PREC_POWER,
    PREC_UNARY,
    PREC_CALL,
    PREC_PRIMARY
} Precedence;

typedef enum {
    VAR_MODE_READ,
    VAR_MODE_ASSIGNMENT,
    VAR_MODE_DECLARATION
} VarAccessMode;

typedef void (*ParseFn)(bool);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    ParseFn postfix;
    Precedence precedence;
} ParseRule;

static void literal(bool canAssign);

static void number(bool canAssign);

static void string(bool canAssign);

static void fstring(bool canAssign);

static void array(bool canAssign);

static void grouping(bool canAssign);

static void unary(bool canAssign);

static void binary(bool canAssign);

static void variable(bool canAssign);

static void and(bool canAssign);

static void or(bool canAssign);

static void statement(int breakPointer, int continuePointer);

static void call(bool canAssign);

static void at(bool canAssign);

static void dot(bool canAssign);

ParseRule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping, call,   NULL,    PREC_CALL},
  [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   NULL,    PREC_NONE}, 
  [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_LEFT_BRACKET]  = {array,    at,     NULL,    PREC_CALL},
  [TOKEN_RIGHT_BRACKET] = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_COMMA]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_DOT]           = {NULL,     dot,   NULL,     PREC_CALL},
  [TOKEN_PLUS]          = {unary,    binary, NULL,    PREC_TERM},
  [TOKEN_MINUS]         = {unary,    binary, NULL,    PREC_TERM},
  [TOKEN_STAR]          = {NULL,     binary, NULL,    PREC_FACTOR},
  [TOKEN_CARET]         = {NULL,     binary, NULL,    PREC_POWER},
  [TOKEN_SLASH]         = {NULL,     binary, NULL,    PREC_FACTOR},
  [TOKEN_PERCENT]       = {NULL,     binary, NULL,    PREC_FACTOR},
  [TOKEN_NOT]           = {unary,    NULL,   NULL, PREC_NONE},
  [TOKEN_PLUS_EQUAL]    = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_MINUS_EQUAL]   = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_STAR_EQUAL]    = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_CARET_EQUAL]   = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_SLASH_EQUAL]   = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_PERCENT_EQUAL] = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_EQUAL]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_DOUBLE_EQUAL]  = {NULL,     binary, NULL,    PREC_EQUALITY},
  [TOKEN_BANG_EQUAL]    = {NULL,     binary, NULL,    PREC_EQUALITY},
  [TOKEN_COLON_EQUAL]   = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_GREATER]       = {NULL,     binary, NULL,    PREC_EQUALITY},
  [TOKEN_GREATER_EQUAL] = {NULL,     binary, NULL,    PREC_EQUALITY},
  [TOKEN_LESS]          = {NULL,     binary, NULL,    PREC_EQUALITY},
  [TOKEN_LESS_EQUAL]    = {NULL,     binary, NULL,    PREC_EQUALITY},
  [TOKEN_IDENTIFIER]    = {variable, NULL,   NULL,    PREC_NONE},
  [TOKEN_STRING]        = {string,   NULL,   NULL,    PREC_NONE},
//   [TOKEN_FSTRING]       = {fstring,  NULL,   NULL,    PREC_NONE},
  [TOKEN_NUMBER]        = {number,   NULL,   NULL,    PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_ELIF]          = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_AND]           = {NULL,     and,   NULL,    PREC_AND},
  [TOKEN_OR]            = {NULL,     or,    NULL,    PREC_OR},
  [TOKEN_TRUE]          = {literal,  NULL,   NULL,    PREC_NONE},
  [TOKEN_FALSE]         = {literal,  NULL,   NULL,    PREC_NONE},
  [TOKEN_NONE]          = {literal,  NULL,   NULL,    PREC_NONE},
  [TOKEN_DEF]           = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_RETURN]        = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_CLASS]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_NEWLINE]    = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_ERROR]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_EOF]           = {NULL,     NULL,   NULL,    PREC_NONE},
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
        emitByte(OP_NIL, (Token){0});
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

    current = current->enclosing;
    return function;
}

static void parseExpression(Precedence precedence, bool canAssign) {
    ParseFn prefixFunc = getRule(parser.current.type)->prefix;

    if (prefixFunc == NULL) {
        reportError("Expect expression", &parser.current);
        exit(1);
        return;
    }

    canAssign &= precedence <= PREC_ASSIGNMENT;

    prefixFunc(canAssign);

    while (precedence < getRule(parser.current.type)->precedence) {
        ParseFn infixFunc = getRule(parser.current.type)->infix;
        infixFunc(canAssign);
    }
}

static void expression() {
    parseExpression(PREC_ASSIGNMENT, false);
}

static void literal(bool canAssign) {
    (void)canAssign;

    switch (parser.current.type) {
        case TOKEN_TRUE:
            emitByte(OP_TRUE, parser.current); 
            break;
        case TOKEN_FALSE:
            emitByte(OP_FALSE, parser.current);
            break;
        case TOKEN_NONE:
            emitByte(OP_NIL, parser.current);
            break;
        default:
            return;
    }
    advance();
}

static void number(bool canAssign) {
    (void)canAssign;

    double value = strtod(parser.current.start, NULL);
    emitConstant(NUMBER_VAL(value));
    advance();
}

static void string(bool canAssign) {
    (void)canAssign;

    emitConstant(OBJ_VAL(copyEscapedString(parser.current.start, parser.current.length)));
    advance();
}

static void rawString() {
    emitConstant(OBJ_VAL(createRawString(parser.current.start, parser.current.length)));
    advance();
}

static void array(bool canAssign) {
    // (void)canAssign;

    // size_t size = 0;
    // advance(true);
    // if (!match(TOKEN_RIGHT_BRACKET, false)) {
    //     do {
    //         expression();
    //         size++;
    //     } while (consume(TOKEN_COMMA, true));
    //     if (!consume(TOKEN_RIGHT_BRACKET, true))
    //         reportError("Expect ']'", &parser.current);
    // }
    // emitBytes(OP_BUILD_ARRAY, (uint8_t)size, (Token){0});
}

static void fstring(bool canAssign) {
    (void)canAssign;

    int count = 0;

    while (!check(TOKEN_STRING, false)) {
        if (parser.current.length > 0) {
            rawString();
            count++;
        } else {
            advance();
        }
        expression();
        count++;
    }

    if (parser.current.length > 0) {
        rawString();
        count++;
    } else {
        advance();
    }
    emitBytes(OP_BUILD_FSTRING, (uint8_t)count, (Token){0});
}

static void grouping(bool canAssign) {
    (void)canAssign;

    advance();
    parseExpression(PREC_ASSIGNMENT, false);

    if (!consume(TOKEN_RIGHT_PAREN, true))
        reportError("Opening parenthesis does not closed", NULL);
}

static void unary(bool canAssign) {
    (void)canAssign;

    Token operator = parser.current;
    advance();

    Precedence precedence = getRule(operator.type)->precedence + 1;
    parseExpression(precedence, false);

    switch (operator.type) {
        case TOKEN_PLUS:
            return;
        case TOKEN_MINUS:
            emitByte(OP_NEGATE, operator);
            break;
        case TOKEN_NOT:
            emitByte(OP_NOT, operator);
            break;
        default:
            return;
    }
}

static void binary(bool canAssign) {
    (void)canAssign;
    
    Token operator = parser.current;
    advance();
    parseExpression(getRule(operator.type)->precedence, false);

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
        case TOKEN_CARET:
            emitByte(OP_POWER, operator);
            break;
        case TOKEN_SLASH:
            emitByte(OP_DIVIDE, operator);
            break;
        case TOKEN_PERCENT:
            emitByte(OP_MOD, operator);
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
    return createConstant(OBJ_VAL(copyString(name->start, name->length)));
}

static void expressionStatement() {
    parseExpression(PREC_ASSIGNMENT, true);
    
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
    if (!consume(TOKEN_INDENT, false))

    while (!check(TOKEN_DEDENT, false) && !check(TOKEN_EOF, false))
        statement(breakPoiner, continuePointer);
    
    consume(TOKEN_DEDENT, false);
}

// ======================================
//               Variables
// ======================================    

static int resolveLocal(Compiler *compiler, Token *name, bool new) {
    for (int i = 0; i < compiler->localCount; i++) {
        Local *local = &compiler->locals[i];
        if (identifierEqual(name, &local->name))
            return i;
    }
    if (new) {
        Local *local = &current->locals[current->localCount++];
        local->name = *name;
        local->isCaptured = false;
        return current->localCount - 1;
    }
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

static void resolveVariable(Token *name, uint8_t *getOp, uint8_t *setOp, uint8_t *arg) {
    // int offset = resolveLocal(current, name);
    // if (current->type == TYPE_FUNCTION)
    //     *setOp = OP_
    // if (offset != -1) {
    //     *getOp = OP_GET_LOCAL;
    //     *setOp = OP_SET_LOCAL;
    //     *arg = offset;
    //     return;
    // }
    
    // offset = resolveUpvalue(current, name);
    // if (offset != -1) {
    //     *getOp = OP_GET_UPVALUE;
    //     *setOp = OP_SET_UPVALUE;
    //     *arg = offset;
    //     return;
    // }

    // *getOp = OP_GET_GLOBAL;
    // *setOp = OP_SET_GLOBAL;
    // *arg = identifierConstant(name);
}

static void resolveVariableAssignment(Token *name, uint8_t *setOp, uint8_t *arg) {
    if (current->type == TYPE_FUNCTION) {
        *setOp = OP_SET_LOCAL;
        *arg = resolveLocal(current, name, true);
        return;
    }

    *setOp = OP_SET_GLOBAL;
    *arg = identifierConstant(name);
}

static void resolveVariableReference(Token *name, uint8_t *getOp, uint8_t *arg) {
    if (current->type == TYPE_FUNCTION) {
        int offset = resolveLocal(current, name, false);
        if (offset != -1) {
            *getOp = OP_SET_LOCAL;
            *arg = offset;
            return;
        }
    }

    *getOp = OP_GET_GLOBAL;
    *arg = identifierConstant(name);
}

static bool isAssignment(Token operator) {
    switch (operator.type) {
        case TOKEN_EQUAL:
        case TOKEN_PLUS_EQUAL:
        case TOKEN_MINUS_EQUAL:
        case TOKEN_STAR_EQUAL:
        case TOKEN_SLASH_EQUAL:
        case TOKEN_CARET_EQUAL:
        case TOKEN_PERCENT_EQUAL:
            return true;
        default:
            return false;
    }
}

static void assignment(uint8_t getOp, uint8_t setOp, int arg, Token operator) {
    if (operator.type != TOKEN_EQUAL)
        emitAssignment(getOp, arg, operator);
    
    expression();

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
        case TOKEN_SLASH_EQUAL:
            emitByte(OP_DIVIDE, operator);
            break;
        case TOKEN_CARET_EQUAL:
            emitByte(OP_POWER, operator);
            break;
        case TOKEN_PERCENT_EQUAL:
            emitByte(OP_MOD, operator);
            break;
        default:
            // to remove warnings
            break;
    }

    emitAssignment(setOp, arg, operator);
}

static void variable(bool canAssign) {
    uint8_t getOp, setOp, arg;
    Token name = parser.current;
    advance(false);
    Token operator = parser.current;

    if (consume(TOKEN_EQUAL, false)) {
        if (!canAssign)
            reportError("Variable assignment is now allowed here", &operator);
        resolveVariableAssignment(&name, &setOp, &arg);
        expression();
        emitBytes(setOp, arg, name);
    } else {
        resolveVariableReference(&name, &setOp, &arg);
        emitBytes(getOp, arg, name);
    }
}

// ======================================
//            Control flow
// ======================================    

static void and(bool canAssign) {
    (void)canAssign;

    advance();
    int endJump = emitJump(OP_JUMP_FALSE);

    emitByte(OP_POP, (Token){0});
    parseExpression(PREC_AND, false);

    patchJump(endJump);
}

static void or(bool canAssign){
    (void)canAssign;

    advance();
    int endJump = emitJump(OP_JUMP_TRUE);

    emitByte(OP_POP, (Token){0});
    parseExpression(PREC_OR, false);

    patchJump(endJump);
}

static void ifStatement(int breakPointer, int continuePointer) {
    // advance(false);
    // expression();

    // int jumpToNextBranch = emitJump(OP_JUMP_FALSE_POP);
    // int jumpToEnd = -1;

    // if (parser.current.type == TOKEN_LINE_BREAK) {
    //     advance(false);
    //     statement(breakPointer, continuePointer);
    // } else if (parser.current.type == TOKEN_LEFT_BRACE) {
    //     statement(breakPointer, continuePointer);
    // } else {
    //     reportError("Expect new line or block after condition", &parser.current);
    // }

    // if (parser.current.type == TOKEN_ELIF) {
    //     jumpToEnd = emitJump(OP_JUMP);
    //     while (match(TOKEN_ELIF, false)) {
    //         patchJump(jumpToNextBranch);
    //         expression();
    //         jumpToNextBranch = emitJump(OP_JUMP_FALSE_POP);
    //         statement(-1, -1);
    //         emitLoop(OP_LOOP, jumpToEnd - 1);
    //     }
    // }

    // if (jumpToEnd == -1)
    //     jumpToEnd = emitJump(OP_JUMP);
    // patchJump(jumpToNextBranch);

    // if (parser.current.type == TOKEN_ELSE) {

    //     advance(false);
    //     if (parser.current.type == TOKEN_LINE_BREAK) {
    //         advance(false);
    //         statement(-1, -1);
    //     } else if (parser.current.type == TOKEN_LEFT_BRACE) {
    //         statement(-1, -1);
    //     } else {
    //         reportError("Expect new line or block after condition", &parser.current);
    //     }
    // }

    // if (jumpToEnd != -1)
    //     patchJump(jumpToEnd);
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

static void forLoop() {
    // bool hasInit, hasCondition, hasPost;
    // int conditionPointer, postPointer, bodyPointer;
    // int jumpToStart, jumpToEndIfFalse, jumpToEnd, jumpToBody;

    // jumpToEnd = breakJump();

    // if (!check(TOKEN_SEMICOLON)) {
    //     hasInit = true;
    //     beginScope();
    //     declaration(-1, -1);
    // } else {
    //     hasInit = false;
    //     advance(true);
    // }

    // if (!check(TOKEN_SEMICOLON)) {
    //     hasCondition = true;
    //     conditionPointer = currentCode()->size;
    //     expression();
    //     jumpToEndIfFalse = emitJump(OP_JUMP_FALSE_POP);
    //     if (!check(TOKEN_SEMICOLON)) {
    //         reportError("Expect ';' after condition", &parser.current);
    //     }
    //     advance(true);
    // } else {
    //     hasCondition = false;
    //     advance(true);
    // }

    // if (!check(TOKEN_LEFT_BRACE)) {
    //     hasPost = true;

    //     jumpToBody = emitJump(OP_JUMP);
    //     postPointer = currentCode()->size;

    //     parseExpression(PREC_ASSIGNMENT, true);
    //     emitByte(OP_POP, (Token){0});

    //     if (hasCondition)
    //         emitLoop(OP_LOOP, conditionPointer);
    // } else {
    //     hasPost = false;
    // }

    // bodyPointer = currentCode()->size;
    // if (hasPost)
    //     patchJump(jumpToBody);

    // if (hasPost)
    //     jumpToStart = postPointer;
    // else if (hasCondition)
    //     jumpToStart = conditionPointer;
    // else
    //     jumpToStart = bodyPointer;

    // declaration(jumpToEnd, jumpToStart);

    // emitLoop(OP_LOOP, jumpToStart);

    // if (hasCondition)
    //     patchJump(jumpToEndIfFalse);
    // patchJump(jumpToEnd);
    
    // if (hasInit)
    //     endScope();
}

static void whileLoop() {
    int jumpToEnd, jumpToEndIfFalse;
    int conditionPointer;

    jumpToEnd = breakJump();
    conditionPointer = currentCode()->size;

    expression();
    jumpToEndIfFalse = emitJump(OP_JUMP_FALSE_POP);

    statement(jumpToEnd, conditionPointer);

    emitLoop(OP_LOOP, conditionPointer);

    patchJump(jumpToEnd);
    patchJump(jumpToEndIfFalse);
}

static void unconditionalLoop() {
    int jumpToEnd;
    int startPointer;

    jumpToEnd = breakJump();
    startPointer = currentCode()->size;

    statement(jumpToEnd, startPointer);

    if (consume(TOKEN_LEFT_PAREN, true)) {
        expression();
        emitLoop(OP_LOOP_TRUE_POP, startPointer);

        if (!consume(TOKEN_RIGHT_PAREN, true))
            reportError("Expect ')' after condition", &parser.current);
    } else {
        emitLoop(OP_LOOP, startPointer);
    }

    patchJump(jumpToEnd);
}

static void forStatement() {
    advance();

    if (check(TOKEN_LEFT_BRACE, false)) {
        unconditionalLoop();
        return;
    }

    if (check(TOKEN_IDENTIFIER, false)) {
        if (checkNext(TOKEN_COLON_EQUAL, false) || checkNext(TOKEN_EQUAL, false) || checkNext(TOKEN_COMMA, false)) {
            forLoop();
            return;
        } else {
            whileLoop();
            return;
        }
    }
}

// ======================================
//              Functions 
// ======================================    

static void function(FunctionType type) {
    // Compiler compiler;
    // Token name = parser.current;
    // advance(false);

    // if (parser.current.type != TOKEN_LEFT_PAREN) {
    //     reportError("Expect '(' after function name", &parser.current);
    // }
    // advance(true);
    
    // int defaultCount = 0;
    // int minArity = 0;
    // int maxArity = 0;
    // TokenVec names;
    // TokenVecInit(&names);

    // if (!check(TOKEN_RIGHT_PAREN, false)) {
    //     do {
    //         if (current->function->maxArity > 255) {
    //             reportError("Can't have more than 255 parameters", &parser.current);
    //         }

    //         TokenVecPush(&names, parser.current);
    //         advance(false);

    //         if (consume(TOKEN_EQUAL, true)) {
    //             expression();
    //             defaultCount++;
    //         } else if (defaultCount > 0)
    //             reportError("Non-default argument follows default argument", &parser.current);
    //         else 
    //             minArity++;
    //         maxArity++;

    //     } while (match(TOKEN_COMMA, true));
    // }

    // emitBytes(OP_BUILD_ARRAY, (uint8_t)defaultCount, parser.current);

    // if (!consume(TOKEN_RIGHT_PAREN, true))
    //     reportError("Expect ')' after parameters", &parser.current);
    
    // if (!consume(TOKEN_LEFT_BRACE, true))
    //     reportError("Expect '{' after function body}", &parser.current);

    // initCompiler(&compiler, type, name);
    // beginScope();

    // current->function->minArity = minArity;
    // current->function->maxArity = maxArity;

    // for (int i = 0; i < names.size; i++)
    //     createLocal(names.tokens[i]);
    
    // TokenVecFree(&names);

    // block(-1, -1);

    // ObjFunction *function = endCompiler();

    // emitBytes(OP_CLOSURE, createConstant(OBJ_VAL(function)), (Token){0});

    // for (int i = 0; i < function->upvalueCount; i++) {
    //     emitByte(compiler.upvalues[i].isLocal ? 1 : 0, (Token){0});
    //     emitByte(compiler.upvalues[i].index, (Token){0});
    // }
}

static void funcDeclaration() {
    // advance();
    // Token name = parser.current;
    // declareVariable(name);
    // function(TYPE_FUNCTION);
    // defineVariable(name);
}

static uint8_t parseArguments() {
    uint8_t argc = 0;
    if (!check(TOKEN_RIGHT_PAREN, false)) {
        do {
            expression(true);
            if (argc == 255)
                reportError("Can't have more than 255 arguments", &parser.current);
            argc++;
        } while (consume(TOKEN_COMMA, true));
    }

    if (!consume(TOKEN_RIGHT_PAREN, true))
        reportError("Expect ')' after arguments", &parser.current);

    return argc;
}

static void call(bool canAssign) {
    (void)canAssign;

    Token name = parser.current;
    advance();
    uint8_t argc = parseArguments();
    emitBytes(OP_CALL, argc, name);
}

static void method() {
    Token name = parser.current;
    
    uint8_t constant = identifierConstant(&name);
    FunctionType type = TYPE_METHOD;
    if (name.length == 6 && memcmp(name.start, "_init_", 6) == 0)
        type = TYPE_INITIALIZER;
    function(type);
    emitBytes(OP_METHOD, constant, (Token){0});
}

static void classDeclaration() {
    // advance();
    // Token name = parser.current;
    // if (!consume(TOKEN_IDENTIFIER, false))
    //     reportError("Expect class name", &parser.current);
    
    // uint8_t nameConstant = identifierConstant(&name);
    
    // declareVariable(name);
    // emitBytes(OP_CLASS, nameConstant, name);
    // defineVariable(name);

    // ClassCompiler classCompiler;
    // classCompiler.enclosing = currentClass;
    // currentClass = &classCompiler;

    // uint8_t getOp, setOp, arg;
    // resolveVariable(&name, &getOp, &setOp, &arg); 
    // emitBytes(getOp, arg, name);
    
    // if (!consume(TOKEN_LEFT_BRACE, true)) 
    //     reportError("Expect '{' before class body", &parser.current);
    
    // while (!check(TOKEN_RIGHT_BRACE, false) && !check(TOKEN_EOF, false)) {
    //     if (consume(TOKEN_DEF, true))
    //         method();
    // }
    
    // if (!consume(TOKEN_RIGHT_BRACE, false))
    //     reportError("Expect '}' after class body", &parser.current);

    // emitByte(OP_POP, (Token){0});

    // currentClass = currentClass->enclosing;
}

static void dot(bool canAssign) {
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
        assignment(OP_GET_PROPERTY, OP_SET_PROPERTY, arg, operator);
    } else {
        emitBytes(OP_GET_PROPERTY, arg, name);
    }
}

static void at(bool canAssign) {
    // (void)canAssign;

    // advance(true);
    // Token index = parser.current;
    // expression();
    // advance(false);

    // Token operator = parser.current;

    // if (match(TOKEN_COLON_EQUAL, false)) {
    //     reportError("Invalid syntax", &operator);
    // }

    // if (isAssignment(operator)) {
    //     advance(true);
    //     if (!canAssign)
    //         reportError("Assignment is not allowed here", &operator);

    //     assignment(OP_GET_AT_NO_POP, OP_SET_AT, NO_ARG, operator);
    // } else {
    //     emitByte(OP_GET_AT, index);
    // }
}

static void returnStatement() {
    // advance(false);
    // if (current->type == TYPE_TOP_LEVEL)
    //     reportError("Can't return from top-level code", &parser.current);
    
    // if (consumeEOS()) {
    //     emitReturn();
    // } else {
    //     if (current->type == TYPE_INITIALIZER) 
    //         reportError("Can't return a value from an initializer", &parser.current);
        
    //     expression(false);
    //     if (!consume(TOKEN_LINE_BREAK, false) && !consume(TOKEN_SEMICOLON, false)) {
    //         reportError("Expect eos after value", &parser.current);
    //     }
    //     emitByte(OP_RETURN, parser.current);
    // }
}

static void statement(int breakPointer, int continuePointer) {
    switch (parser.current.type) {
        case TOKEN_NEWLINE:
        case TOKEN_SEMICOLON:
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
    return parser.errorCount != 0 ? NULL : function;
}

void markCompilerRoots() {
    Compiler *compiler = current;
    while (compiler != NULL) {
        markObject((Obj*)compiler->function);
        compiler = compiler->enclosing;
    }
}