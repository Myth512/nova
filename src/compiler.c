#include <stdlib.h>
#include <string.h>

#include "compiler.h"
#include "memory.h"
#include "token.h"
#include "scanner.h"
#include "debug.h"
#include "common.h"
#include "object.h"
#include "error.h"

typedef struct {
    const char *source;
    Token current;
    Token next;
    int errorCount;
    bool panicMode;
} Parser;

typedef struct {
    Token name;
    int depth;
    bool isCaptured;
} Local;

typedef struct {
    uint8_t index;
    bool isLocal;
} Upvalue;

typedef enum {
    TYPE_FUNCTION,
    TYPE_TOP_LEVEL
} FunctionType;

typedef struct Compiler {
    struct Compiler *enclosing;
    ObjFunction *function;
    FunctionType type;
    Local locals[UINT8_MAX + 1];
    int localCount;
    Upvalue upvalues[UINT8_MAX + 1];
    int scopeDepth;
} Compiler;

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

typedef void (*ParseFn)();

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    ParseFn postfix;
    Precedence precedence;
} ParseRule;

static void literal();

static void number();

static void string();

static void fstring();

static void array();

static void grouping();

static void unary();

static void binary();

static void getVariable();

static void and();

static void or();

static void statement(int breakPointer, int continuePointer);

static void call();

static void declaration(int breakPointer, int continuePointer);

ParseRule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping, call,   NULL,    PREC_CALL},
  [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   NULL,    PREC_NONE}, 
  [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_LEFT_BRACKET]  = {array,     NULL,   NULL,    PREC_NONE},
  [TOKEN_RIGHT_BRACKET] = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_COMMA]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_DOT]           = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_PLUS]          = {unary,     binary, NULL,    PREC_TERM},
  [TOKEN_MINUS]         = {unary,    binary, NULL,    PREC_TERM},
  [TOKEN_STAR]          = {NULL,     binary, NULL,    PREC_FACTOR},
  [TOKEN_CARET]         = {NULL,     binary, NULL,    PREC_POWER},
  [TOKEN_SLASH]         = {NULL,     binary, NULL,    PREC_FACTOR},
  [TOKEN_PERCENT]       = {NULL,     binary, NULL,    PREC_FACTOR},
  [TOKEN_NOT]           = {unary,    NULL,   NULL, PREC_NONE},
  [TOKEN_PLUS_PLUS]     = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_MINUS_MINUS]   = {NULL,     NULL,   NULL,    PREC_NONE},
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
  [TOKEN_IDENTIFIER]    = {getVariable, NULL,   NULL,    PREC_NONE},
  [TOKEN_STRING]        = {string,   NULL,   NULL,    PREC_NONE},
  [TOKEN_FSTRING]       = {fstring,  NULL,   NULL,    PREC_NONE},
  [TOKEN_NUMBER]        = {number,   NULL,   NULL,    PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_ELIF]          = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_AND]           = {NULL,     and,   NULL,    PREC_AND},
  [TOKEN_OR]            = {NULL,     or,    NULL,    PREC_OR},
  [TOKEN_TRUE]          = {literal,  NULL,   NULL,    PREC_NONE},
  [TOKEN_FALSE]         = {literal,  NULL,   NULL,    PREC_NONE},
  [TOKEN_NIL]           = {literal,  NULL,   NULL,    PREC_NONE},
  [TOKEN_DEF]           = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_RETURN]        = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_CLASS]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_SELF]          = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_SUPER]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_LINE_BREAK]    = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_SEMICOLON]     = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_ERROR]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_EOF]           = {NULL,     NULL,   NULL,    PREC_NONE},
  };

static ParseRule* getRule(TokenType type) {
    return &rules[type];
}

Parser parser;
Compiler *current = NULL;
CodeVec *compilingCode;

static CodeVec* currentCode() {
    return &current->function->code;
}

static void advance(bool skipLineBreak) {
    if (skipLineBreak && parser.next.type == TOKEN_LINE_BREAK)
        parser.current = scanToken(true);
    else
        parser.current = parser.next;
    #ifdef DEBUG_PRINT_TOKENS
        printToken(&parser.current);
    #endif
    parser.next = scanToken(false);
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
            case TOKEN_LINE_BREAK:
            case TOKEN_SEMICOLON:
                return;
            default:
                break;
        }

        advance(false);
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

static bool check(TokenType type) {
    return parser.current.type == type;
}

static bool checkNext(TokenType type) {
    return parser.next.type == type;
}

static bool match(TokenType type, bool skipLineBreak) {
    if (check(type)) {
        advance(skipLineBreak);
        return true;
    }
    return false;
}

static bool consume(TokenType type, bool skipLineBreak) {
    while (skipLineBreak && parser.current.type == TOKEN_LINE_BREAK)
        advance(false);
    if (parser.current.type == type) {
        advance(false);
        return true;
    }
    return false;
}

static void emitByte(uint8_t byte, Token token) {
    pushInstruction(currentCode(), byte, token.line, token.column, token.length);
}

static void emitBytes(uint8_t byte1, uint8_t byte2, Token token) {
    emitByte(byte1, token);
    emitByte(byte2, token); 
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

static void initCompiler(Compiler *compiler, FunctionType type) {
    compiler->enclosing = current;
    compiler->function = createFunction();
    compiler->type = type;
    compiler->localCount = 0;
    compiler->scopeDepth = 0;
    current = compiler;
    if (type != TYPE_TOP_LEVEL) {
        current->function->name = copyString(parser.current.start, parser.current.length);
    }

    // reserve 0th slot for internal use
    Local* local = &current->locals[current->localCount++];
    local->depth = 0;
    local->isCaptured = false;
    local->name.start = "";
    local->name.length = 0;
}

static ObjFunction* endCompiler() {
    ObjFunction *function = current->function;
    if (function->code.size == 0 ||
        function->code.code[function->code.size - 1] != OP_RETURN)
            emitBytes(OP_NIL, OP_RETURN, (Token){0});

    #ifdef DEBUG_PRINT_CODE
        if (parser.errorCount == 0)
            printCodeVec(currentCode(), function->name != NULL ? function->name->chars : "<top level>");
    #endif

    current = current->enclosing;
    return function;
}

static void parseExpression(Precedence precedence) {
    void (*prefixFunc)() = getRule(parser.current.type)->prefix;

    if (prefixFunc == NULL) {
        reportError("Expect expression", &parser.current);
        exit(1);
        return;
    }

    prefixFunc();

    while (precedence < getRule(parser.current.type)->precedence) {
        void (*infixFunc)() = getRule(parser.current.type)->infix;
        infixFunc();
    }
}

static void expression() {
    parseExpression(PREC_ASSIGNMENT);
}

static void literal() {
    switch (parser.current.type) {
        case TOKEN_TRUE:
            emitByte(OP_TRUE, parser.current); 
            break;
        case TOKEN_FALSE:
            emitByte(OP_FALSE, parser.current);
            break;
        case TOKEN_NIL:
            emitByte(OP_NIL, parser.current);
            break;
        default:
            return;
    }
    advance(false);
}

static void number() {
    double value = strtod(parser.current.start, NULL);
    emitConstant(NUMBER_VAL(value));
    advance(false);
}

static void string() {
    emitConstant(OBJ_VAL(copyString(parser.current.start, parser.current.length)));
    advance(false);
}

static void rawString() {
    emitConstant(OBJ_VAL(createRawString(parser.current.start, parser.current.length)));
    advance(false);
}

static void array() {
    size_t size = 0;
    advance(true);
    if (!match(TOKEN_RIGHT_BRACKET, true)) {
        do {
            expression();
            size++;
        } while (consume(TOKEN_COMMA, true));
        if (!consume(TOKEN_RIGHT_BRACKET, true))
            reportError("Expect ']'", &parser.current);
    }
    emitBytes(OP_BUILD_ARRAY, (uint8_t)size, (Token){0});
}

static void fstring() {
    int count = 0;

    while (!check(TOKEN_STRING)) {
        if (parser.current.length > 0) {
            rawString();
            count++;
        } else {
            advance(false);
        }
        expression();
        count++;
    }

    if (parser.current.length > 0) {
        rawString();
        count++;
    } else {
        advance(false);
    }
    emitBytes(OP_BUILD_FSTRING, (uint8_t)count, (Token){0});
}

static void grouping() {
    advance(true);
    parseExpression(PREC_ASSIGNMENT);

    if (!consume(TOKEN_RIGHT_PAREN, true))
        reportError("Opening parenthesis does not closed", NULL);
}

static void unary() {
    Token operator = parser.current;
    advance(false);

    Precedence precedence = getRule(operator.type)->precedence + 1;
    parseExpression(precedence);

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

static void binary() {
    Token operator = parser.current;
    advance(true);
    parseExpression(getRule(operator.type)->precedence);

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
    expression();
    
    if (parser.current.type == TOKEN_LINE_BREAK || parser.current.type == TOKEN_SEMICOLON)
        advance(false);
    else
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

static void beginScope() {
    current->scopeDepth++;
}

static void endScope() {
    current->scopeDepth--;

    while (current->localCount > 0 &&
           current->locals[current->localCount - 1].depth > current->scopeDepth) {
            if (current->locals[current->localCount - 1].isCaptured)
                emitByte(OP_CLOSE_UPVALUE, (Token){0});
            else
                emitByte(OP_POP, (Token){0});
            current->localCount--;
    }
}

static void block(int breakPoiner, int continuePointer) {
    advance(true);

    while (parser.current.type != TOKEN_RIGHT_BRACE && parser.current.type != TOKEN_EOF)
        declaration(breakPoiner, continuePointer);
    
    if (!consume(TOKEN_RIGHT_BRACE, true))
        reportError("Unclosed '{'", NULL);
}

// ======================================
//               Variables
// ======================================    

static void createLocal(Token name) {
    for (int i = current->localCount - 1; i >= 0; i--) {
        Local *local = &current->locals[i];
        if (local->depth < current->scopeDepth)
            break;
        
        if (identifierEqual(&name, &local->name))
            reportError("Already a variable with this name in this scope", NULL);
    }

    if (current->localCount == UINT8_MAX + 1) {
        reportError("Too many variable in function", NULL);
    }
    Local *local = &current->locals[current->localCount++];
    local->name = name;
    local->depth = current->scopeDepth;
    local->isCaptured = false;
}

static void createGlobal(Token name) {
    uint8_t arg = identifierConstant(&name);
    emitBytes(OP_DEFINE_GLOBAL, arg, name);
}

static void createVariable(Token name) {
    if (current->scopeDepth > 0)
        createLocal(name);
    else 
        createGlobal(name);
}

static int resolveLocal(Compiler *compiler, Token *name) {
    for (int i = compiler->localCount - 1; i >= 0; i--) {
        Local *local = &compiler->locals[i];
        if (identifierEqual(name, &local->name))
            return i;
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
    if (compiler->enclosing == NULL)
        return -1;
    
    int offset = resolveLocal(compiler->enclosing, name);
    if (offset != -1) {
        compiler->enclosing->locals[offset].isCaptured = true;
        return addUpvalue(compiler, (uint8_t)offset, true);
    }
    
    offset = resolveUpvalue(compiler->enclosing, name);
    if (offset != -1)
        return addUpvalue(compiler, (uint8_t)offset, false);

    return -1;
}

static void resolveVariable(Token *name, uint8_t *getOp, uint8_t *setOp, uint8_t *arg) {
    int offset = resolveLocal(current, name);
    if (offset != -1) {
        *getOp = OP_GET_LOCAL;
        *setOp = OP_SET_LOCAL;
        *arg = offset;
        return;
    }
    
    offset = resolveUpvalue(current, name);
    if (offset != -1) {
        *getOp = OP_GET_UPVALUE;
        *setOp = OP_SET_UPVALUE;
        *arg = offset;
        return;
    }

    *getOp = OP_GET_GLOBAL;
    *setOp = OP_SET_GLOBAL;
    *arg = identifierConstant(name);
}

static void getVariable() {
    Token *name = &parser.current;
    uint8_t getOp, setOp, arg;
    resolveVariable(name, &getOp, &setOp, &arg);
    emitBytes(getOp, arg, *name);
    advance(false);
}

void variableDeclaration(Token name) {
    expression();
    createVariable(name);
}

void setVariable(Token name, Token operator) {
    uint8_t getOp, setOp, arg;
    resolveVariable(&name, &getOp, &setOp, &arg);

    if (operator.type != TOKEN_EQUAL)
        emitBytes(getOp, arg, name);
    
    if (operator.type != TOKEN_PLUS_PLUS && operator.type != TOKEN_MINUS_MINUS)
        expression();

    switch (operator.type) {
        case TOKEN_PLUS_PLUS:
            emitByte(OP_INCREMENT, operator);
            break;
        case TOKEN_MINUS_MINUS:
            emitByte(OP_DECREMENT, operator);
            break;
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
            break;
    }
    emitBytes(setOp, arg, name);
}

static void singleVariable() {
    Token name = parser.current;
    Token operator = parser.next;

    switch (operator.type) {
        case TOKEN_COLON_EQUAL:
            advance(false);
            advance(true);
            variableDeclaration(name);
            break;
        case TOKEN_EQUAL:
        case TOKEN_PLUS_PLUS:
        case TOKEN_MINUS_MINUS:
        case TOKEN_PLUS_EQUAL:
        case TOKEN_MINUS_EQUAL:
        case TOKEN_STAR_EQUAL:
        case TOKEN_SLASH_EQUAL:
        case TOKEN_CARET_EQUAL:
        case TOKEN_PERCENT_EQUAL:
            advance(false);
            advance(false);
            setVariable(name, operator);
            break;
        default:
            expression();
            emitByte(OP_POP, (Token){0});
            break;
    }
}

static void parseMultipleVariables(TokenVec *variables) {
    do {
        if (!check(TOKEN_IDENTIFIER))
            reportError("Expect variable name after comma", &parser.current);
        
        TokenVecPush(variables, parser.current);
        advance(false);
    } while (match(TOKEN_COMMA, true));

    Token operator = parser.current;
    advance(true);
    if (operator.type == TOKEN_LINE_BREAK || operator.type == TOKEN_SEMICOLON)
        return;

    for (int i = 0; i < variables->size; i++) {
        Token name = variables->tokens[i];

        if (operator.type == TOKEN_COLON_EQUAL) {
            variableDeclaration(name);
        } else if (operator.type == TOKEN_EQUAL) {
            setVariable(name, operator);
        } else {
            reportError("Unsupported operator for multiple variables", &operator);
            return;
        }

        if (i != variables->size - 1 && !consume(TOKEN_COMMA, false)) {
            reportError("Number of variables does not match number of values", &parser.current);
            return;
        }
    }
}

static void multipleVariables() {
    TokenVec variables;
    TokenVecInit(&variables);
    parseMultipleVariables(&variables);
    TokenVecFree(&variables);
}

static void variable() {
    if (checkNext(TOKEN_COMMA))
        multipleVariables();
    else
        singleVariable();

    if (!consume(TOKEN_LINE_BREAK, false) && !consume(TOKEN_SEMICOLON, false))
        reportError("Expect eos", &parser.current);
}

// ======================================
//            Control flow
// ======================================    

static void and() {
    advance(true);
    int endJump = emitJump(OP_JUMP_FALSE);

    emitByte(OP_POP, (Token){0});
    parseExpression(PREC_AND);

    patchJump(endJump);
}

static void or(){
    advance(true);
    int endJump = emitJump(OP_JUMP_TRUE);

    emitByte(OP_POP, (Token){0});
    parseExpression(PREC_OR);

    patchJump(endJump);
}

static void ifStatement(int breakPointer, int continuePointer) {
    advance(false);
    expression();

    int jumpToNextBranch = emitJump(OP_JUMP_FALSE_POP);
    int jumpToEnd = -1;

    if (parser.current.type == TOKEN_LINE_BREAK) {
        advance(false);
        statement(breakPointer, continuePointer);
    } else if (parser.current.type == TOKEN_LEFT_BRACE) {
        statement(breakPointer, continuePointer);
    } else {
        reportError("Expect new line or block after condition", &parser.current);
    }

    if (parser.current.type == TOKEN_ELIF) {
        jumpToEnd = emitJump(OP_JUMP);
        while (match(TOKEN_ELIF, false)) {
            patchJump(jumpToNextBranch);
            expression();
            jumpToNextBranch = emitJump(OP_JUMP_FALSE_POP);
            statement(-1, -1);
            emitLoop(OP_LOOP, jumpToEnd - 1);
        }
    }

    if (jumpToEnd == -1)
        jumpToEnd = emitJump(OP_JUMP);
    patchJump(jumpToNextBranch);

    if (parser.current.type == TOKEN_ELSE) {

        advance(false);
        if (parser.current.type == TOKEN_LINE_BREAK) {
            advance(false);
            statement(-1, -1);
        } else if (parser.current.type == TOKEN_LEFT_BRACE) {
            statement(-1, -1);
        } else {
            reportError("Expect new line or block after condition", &parser.current);
        }
    }

    if (jumpToEnd != -1)
        patchJump(jumpToEnd);
}

static void breakStatement(int breakPointer) {
    if (breakPointer == -1)
        reportError("Can't use 'break' outside of loop body", &parser.current);

    advance(false);
    
    emitLoop(OP_LOOP, breakPointer - 1);

    if (!consume(TOKEN_LINE_BREAK, false) && !consume(TOKEN_SEMICOLON, false))
        reportError("Expect eos after 'break'", &parser.current);
}

static void continueStatement(int continuePointer) {
    if (continuePointer == -1)
        reportError("Can't use 'continue' outside of loop body", &parser.current);
    
    advance(false);

    emitLoop(OP_LOOP, continuePointer);

    if (!consume(TOKEN_LINE_BREAK, false) && !consume(TOKEN_SEMICOLON, false))
        reportError("Expect eos after 'continue'", &parser.current);
}

static int breakJump() {
    emitByte(OP_JUMP, (Token){0});
    emitBytes(0, 3, (Token){0});
    return emitJump(OP_JUMP); 
}

static void forLoop() {
    bool hasInit, hasCondition, hasPost;
    int conditionPointer, postPointer, bodyPointer;
    int jumpToStart, jumpToEndIfFalse, jumpToEnd, jumpToBody;

    jumpToEnd = breakJump();

    if (!check(TOKEN_SEMICOLON)) {
        hasInit = true;
        beginScope();
        declaration(-1, -1);
    } else {
        hasInit = false;
        advance(true);
    }

    if (!check(TOKEN_SEMICOLON)) {
        hasCondition = true;
        conditionPointer = currentCode()->size;
        expression();
        jumpToEndIfFalse = emitJump(OP_JUMP_FALSE_POP);
        if (!check(TOKEN_SEMICOLON)) {
            reportError("Expect ';' after condition", &parser.current);
        }
        advance(true);
    } else {
        hasCondition = false;
        advance(true);
    }

    if (!check(TOKEN_LEFT_BRACE)) {
        hasPost = true;

        jumpToBody = emitJump(OP_JUMP);
        postPointer = currentCode()->size;

        singleVariable();

        if (hasCondition)
            emitLoop(OP_LOOP, conditionPointer);
    } else {
        hasPost = false;
    }

    bodyPointer = currentCode()->size;
    if (hasPost)
        patchJump(jumpToBody);

    if (hasPost)
        jumpToStart = postPointer;
    else if (hasCondition)
        jumpToStart = conditionPointer;
    else
        jumpToStart = bodyPointer;

    declaration(jumpToEnd, jumpToStart);

    emitLoop(OP_LOOP, jumpToStart);

    if (hasCondition)
        patchJump(jumpToEndIfFalse);
    patchJump(jumpToEnd);
    
    if (hasInit)
        endScope();
}

static void whileLoop() {
    int jumpToEnd, jumpToEndIfFalse;
    int conditionPointer;

    jumpToEnd = breakJump();
    conditionPointer = currentCode()->size;

    expression();
    jumpToEndIfFalse = emitJump(OP_JUMP_FALSE_POP);

    declaration(jumpToEnd, conditionPointer);

    emitLoop(OP_LOOP, conditionPointer);

    patchJump(jumpToEnd);
    patchJump(jumpToEndIfFalse);
}

static void unconditionalLoop() {
    int jumpToEnd;
    int startPointer;

    jumpToEnd = breakJump();
    startPointer = currentCode()->size;

    declaration(jumpToEnd, startPointer);

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
    advance(false);

    if (check(TOKEN_LEFT_BRACE)) {
        unconditionalLoop();
        return;
    }

    if (check(TOKEN_IDENTIFIER)) {
        if (checkNext(TOKEN_COLON_EQUAL) || checkNext(TOKEN_EQUAL) || checkNext(TOKEN_COMMA)) {
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
    Compiler compiler;
    initCompiler(&compiler, type);
    advance(false);
    beginScope();

    if (parser.current.type != TOKEN_LEFT_PAREN) {
        reportError("Expect '(' after function name", &parser.current);
    }
    advance(true);

    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            current->function->arity++;
            if (current->function->arity > 255) {
                reportError("Can't have more than 255 parameters", &parser.current);
            }
            Token name = parser.current;
            advance(false);
            createLocal(name);
        } while (match(TOKEN_COMMA, true));
    }

    if (!consume(TOKEN_RIGHT_PAREN, true))
        reportError("Expect ')' after parameters", &parser.current);
    
    if (!consume(TOKEN_LEFT_BRACE, true))
        reportError("Expect '{' after function body}", &parser.current);

    block(-1, -1);

    ObjFunction *function = endCompiler();
    emitBytes(OP_CLOSURE, createConstant(OBJ_VAL(function)), (Token){0});

    for (int i = 0; i < function->upvalueCount; i++) {
        emitByte(compiler.upvalues[i].isLocal ? 1 : 0, (Token){0});
        emitByte(compiler.upvalues[i].index, (Token){0});
    }
}

static void declareFunction(Token name) {
    if (current->scopeDepth > 0)
        createLocal(name);
}

static void defineFunction(Token name) {
    if (current->scopeDepth == 0)
        createGlobal(name);
}

static void funcDeclaration() {
    advance(false);
    Token name = parser.current;
    declareFunction(name);
    function(TYPE_FUNCTION);
    defineFunction(name);
}

static uint8_t parseArguments() {
    uint8_t argc = 0;
    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            expression(true);
            if (argc == 255)
                reportError("Can't have more than 255 arguments", &parser.current);
            argc++;
        } while (match(TOKEN_COMMA, true));
    }

    if (!consume(TOKEN_RIGHT_PAREN, true))
        reportError("Expect ')' after arguments", &parser.current);

    return argc;
}

static void call() {
    Token name = parser.current;
    advance(true);
    uint8_t argc = parseArguments();
    emitBytes(OP_CALL, argc, name);
}

static void returnStatement() {
    advance(false);
    if (current->type == TYPE_TOP_LEVEL)
        reportError("Can't return from top-level code", &parser.current);
    
    if (match(TOKEN_LINE_BREAK, false) || match(TOKEN_SEMICOLON, false)) {
        emitBytes(OP_NIL, OP_RETURN, (Token){0});
    } else {
        expression(false);
        if (!consume(TOKEN_LINE_BREAK, false) && !consume(TOKEN_SEMICOLON, false)) {
            reportError("Expect eos after value", &parser.current);
        }
        emitByte(OP_RETURN, parser.current);
    }
}

static void statement(int breakPointer, int continuePointer) {
    switch (parser.current.type) {
        case TOKEN_LINE_BREAK:
        case TOKEN_SEMICOLON:
            advance(false);
            break;
        case TOKEN_IDENTIFIER:
            variable();
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
        case TOKEN_LEFT_BRACE:
            beginScope();
            block(breakPointer, continuePointer);
            endScope();
            break;
        case TOKEN_RETURN:
            returnStatement();
            break;
        default:
            expressionStatement();
            break;
    }
}

static void declaration(int breakPointer, int continuePointer) {
    switch (parser.current.type) {
        case TOKEN_DEF:
            funcDeclaration();
            break;
        default:
            statement(breakPointer, continuePointer);
            break;
    }
    if (parser.panicMode)
        synchronize();
}

ObjFunction* compile(const char *source) {
    initScanner(source);
    Compiler compiler;
    initCompiler(&compiler, TYPE_TOP_LEVEL);

    parser.errorCount = 0;
    parser.panicMode = false;
    parser.source = source;

    advance(false);
    advance(false);

    while (parser.current.type != TOKEN_EOF)
        declaration(-1, -1);

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