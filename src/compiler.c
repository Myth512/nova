#include <stdlib.h>
#include <string.h>

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
    Token exprStart;
    int errorCount;
    bool panicMode;
} Parser;

typedef struct {
    Token name;
    int depth;
} Local;

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

typedef void (*ParseFn)(bool canAssign, bool skipNewline);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    ParseFn postfix;
    Precedence precedence;
} ParseRule;

static void number(bool canAssign, bool skipNewline);

static void literal(bool canAssign, bool skipNewline);

static void string(bool canAssign, bool skipNewline);

static void fstring(bool canAssign, bool skipNewline);

static void grouping(bool canAssign, bool skipNewline);

static void nullary(bool canAssign, bool skipNewline);

static void unary(bool canAssign, bool skipNewline);

static void binary(bool canAssign, bool skipNewline);

static void absolute(bool canAssign, bool skipNewline);

static void variable(bool canAssign, bool skipNewline);

static void declaration();

static void and_(bool canAssign, bool skipNewline);

static void or_(bool canAssign, bool skipNewline);

static void statement(bool skipNewline);

static void call(bool canAssign, bool skipNewline);

ParseRule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping, call,   NULL,    PREC_CALL},
  [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   NULL,    PREC_NONE}, 
  [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_COMMA]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_DOT]           = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_PLUS_PLUS]     = {unary,    NULL,   nullary, PREC_NONE},
  [TOKEN_MINUS_MINUS]   = {unary,    NULL,   NULL,    PREC_NONE},
  [TOKEN_MINUS]         = {unary,    binary, NULL,    PREC_TERM},
  [TOKEN_PLUS]          = {NULL,     binary, NULL,    PREC_TERM},
  [TOKEN_EOS]           = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_SLASH]         = {NULL,     binary, NULL,    PREC_FACTOR},
  [TOKEN_STAR]          = {NULL,     binary, NULL,    PREC_FACTOR},
  [TOKEN_MOD]           = {NULL,     binary, NULL,    PREC_FACTOR},
  [TOKEN_POWER]         = {NULL,     binary, NULL,    PREC_POWER},
  [TOKEN_BANG]          = {unary,    NULL,   nullary, PREC_NONE},
  [TOKEN_BANG_EQUAL]    = {NULL,     binary, NULL,    PREC_EQUALITY},
  [TOKEN_EQUAL]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_DOUBLE_EQUAL]  = {NULL,     binary, NULL,    PREC_EQUALITY},
  [TOKEN_GREATER]       = {NULL,     binary, NULL,    PREC_EQUALITY},
  [TOKEN_GREATER_EQUAL] = {NULL,     binary, NULL,    PREC_EQUALITY},
  [TOKEN_LESS]          = {NULL,     binary, NULL,    PREC_EQUALITY},
  [TOKEN_LESS_EQUAL]    = {NULL,     binary, NULL,    PREC_EQUALITY},
  [TOKEN_IDENTIFIER]    = {variable, NULL,   NULL,    PREC_NONE},
  [TOKEN_STRING]        = {string,   NULL,   NULL,    PREC_NONE},
  [TOKEN_FSTRING]       = {fstring,  NULL,   NULL,    PREC_NONE},
  [TOKEN_NUMBER]        = {number,   NULL,   NULL,    PREC_NONE},
  [TOKEN_AND]           = {NULL,     and_,   NULL,    PREC_AND},
  [TOKEN_CLASS]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_FALSE]         = {literal,  NULL,   NULL,    PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_DEF]           = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_NIL]           = {literal,  NULL,   NULL,    PREC_NONE},
  [TOKEN_OR]            = {NULL,     or_,    NULL,    PREC_NONE},
  [TOKEN_RETURN]        = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_SUPER]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_SELF]          = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_TRUE]          = {literal,  NULL,   NULL,    PREC_NONE},
  [TOKEN_VAR]           = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_WHILE]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_ERROR]         = {NULL,     NULL,   NULL,    PREC_NONE},
  [TOKEN_PIPE]          = {absolute, NULL,   NULL,    PREC_NONE},
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

static void initCompiler(Compiler *compiler, FunctionType type) {
    compiler->enclosing = current;
    compiler->function = createFunction();
    compiler->type = type;
    compiler->localCount = 0;
    compiler->scopeDepth = 0;
    current = compiler;
    if (type != TYPE_TOP_LEVEL) {
        current->function->name = copyString(parser.previous.start, parser.previous.length);
    }

    // reserve 0th slot for internal use
    Local* local = &current->locals[current->localCount++];
    local->depth = 0;
    local->name.start = "";
    local->name.length = 0;
}

static bool reportError(const char *message) {
    if (parser.panicMode)
        return false;
    parser.panicMode = true;
    parser.errorCount++;
    fprintf(stderr, "\033[31mParse Error\033[0m: %s\n", message);
    return true;
}

static void advance(bool skipNewline) {
    parser.previous = parser.current;

    while (true) {
        parser.current = scanToken(skipNewline);

        #ifdef DEBUG_PRINT_TOKENS
            printToken(&parser.current);
        #endif

        if (parser.current.type == TOKEN_EOF)
            break;

        if (parser.current.type != TOKEN_ERROR)
            break;

        reportError(parser.current.start);
    }
}

static void synchronize() {
    parser.panicMode = false;

    while (parser.current.type != TOKEN_EOF) {
        if (parser.previous.type == TOKEN_EOS)
            return;

        switch (parser.current.type) {
            case TOKEN_CLASS:
            case TOKEN_DEF:
            case TOKEN_VAR:
            case TOKEN_FOR:
            case TOKEN_IF:
            case TOKEN_WHILE:
            case TOKEN_RETURN:
                return;
            default:
                break;
        }

        advance(false);
    }
}

static bool check(TokenType type) {
    return parser.current.type == type;
}

static bool match(TokenType type, bool skipNewline) {
    if (!check(type))
        return false;
    advance(skipNewline);
    return true;
}

static void parsePrecedence(Precedence precedence, bool skipNewline) {
    advance(skipNewline);

    Token leftOperand = parser.previous;

    ParseFn prefixRule = getRule(leftOperand.type)->prefix;
    if (prefixRule == NULL) {
        reportError("Expect expression");
        printTokenInCode(parser.source, &leftOperand);
        return;
    }

    bool canAssign = precedence <= PREC_ASSIGNMENT;
    prefixRule(canAssign, skipNewline);

    ParseFn postfixRule = getRule(parser.current.type)->postfix;
    if (postfixRule != NULL) {
        postfixRule(canAssign, skipNewline);
    }

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance(skipNewline);
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule(canAssign, skipNewline);
    }
}

static bool consume(TokenType type) {
    if (parser.current.type == type) {
        advance(false);
        return true;
    }

    return false;
}

static void emitByte(uint8_t byte) {
    pushInstruction(currentCode(), byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static int emitJump(uint8_t instruction) {
    emitByte(instruction);
    emitBytes(0xff, 0xff);
    return currentCode()->size - 2;
}

static void emitLoop(int loopStart) {
    emitByte(OP_LOOP);

    int offset = currentCode()->size - loopStart + 2;

    emitByte((offset >> 8) & 0xff);
    emitByte(offset & 0xff);
}

static void emitReturn() {
    emitByte(OP_NIL);
    emitByte(OP_RETURN);
}

static ObjFunction* endCompiler() {
    emitReturn();
    ObjFunction *function = current->function;

    #ifdef DEBUG_PRINT_CODE
        if (parser.errorCount == 0)
            printCodeVec(currentCode(), function->name != NULL ? function->name->chars : "<top level>");
    #endif

    current = current->enclosing;
    return function;
}

static void expression(bool skipNewline) {
    parser.exprStart = parser.current;
    parsePrecedence(PREC_ASSIGNMENT, skipNewline);
}

static void expressionStatement() {
    expression(false);
    if (!consume(TOKEN_EOS)) {
        if (check(TOKEN_EQUAL)) {
            if (reportError("Invalid assignment target")) {
                int line = parser.exprStart.line;
                int column = parser.exprStart.column;
                int length = parser.previous.column + parser.previous.length - column;
                printHighlightedPartInCode(parser.source, line, column, length);
            }
        } else {
            reportError("Expect eos after expression");
        }
    }
    emitByte(OP_POP);
}

static uint8_t createConstant(Value value) {
    ValueVec *constants = &currentCode()->constants;
    int size = constants->size;

    // for (int i = 0; i < 10 && i < size; i++)
    //     if (compareValues(value, constants->values[size-i-1]))
    //         return size - i - 1;

    int id = pushConstant(currentCode(), value);
    if (id > UINT8_MAX) {
        reportError("Too many constants in one code block");
        return 0;
    }
    return id;
}

static void emitConstant(Value value) {
    emitBytes(OP_CONSTANT, createConstant(value));
}

static void patchJump(int offset) {
    int jump = currentCode()->size - offset - 2;

    if (jump > UINT16_MAX) {
        reportError("Too much code to jump over");
    }

    currentCode()->code[offset] = (jump >> 8) & 0xff;
    currentCode()->code[offset + 1] = jump & 0xff;
}

static uint8_t identifierConstant(Token *name) {
    return createConstant(OBJ_VAL(copyString(name->start, name->length)));
}

static void addLocal(Token name) {
    if (current->localCount == UINT8_MAX + 1) {
        reportError("Too many variable in function");
        return;
    }
    Local *local = &current->locals[current->localCount++];
    local->name = name;
    local->depth = -1;
}

static bool identifierEqual(Token *a, Token *b) {
    if (a->length != b->length)
        return false;
    
    return memcmp(a->start, b->start, a->length) == 0;
}

static void declareVariable() {
    if (current->scopeDepth == 0)
        return;
    
    Token *name = &parser.previous;
    for (int i = current->localCount - 1; i >= 0; i--) {
        Local *local = &current->locals[i];
        if (local->depth != -1 && local->depth < current->scopeDepth)
            break;
        
        if (identifierEqual(name, &local->name))
            reportError("Already a variable with this name in this scope");
    }
    addLocal(*name);
}

static uint8_t parseVariable(const char *errorMessage) {
    if (!consume(TOKEN_IDENTIFIER)) {
        if (check(TOKEN_NUMBER)) {
            reportError("Variable name can't start with digit");
            printHighlightedPartInCode(parser.source, parser.current.line, parser.current.column, 1);
        } else {
            reportError(errorMessage);
        }
    }

    declareVariable();
    if (current->scopeDepth > 0)
        return 0;

    return identifierConstant(&parser.previous);
}

static void markInitialized() {
    if (current->scopeDepth == 0)
        return;
    current->locals[current->localCount - 1].depth = current->scopeDepth;
}

static void defineVariable(uint8_t global) {
    if (current->scopeDepth > 0)
        markInitialized();
    else
        emitBytes(OP_DEFINE_GLOBAL, global);
}

static void and_(bool canAssign, bool skipNewline) {
    int endJump = emitJump(OP_JUMP_IF_FALSE);

    emitByte(OP_POP);
    parsePrecedence(PREC_AND, skipNewline);

    patchJump(endJump);
}

static void or_(bool canAssign, bool skipNewline) {
    int elseJump = emitJump(OP_JUMP_IF_FALSE);
    int endJump = emitJump(OP_JUMP);

    patchJump(elseJump);
    emitByte(OP_POP);

    parsePrecedence(PREC_OR, skipNewline);
    patchJump(endJump);
}

static void number(bool canAssign, bool skipNewline) {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

static void literal(bool canAssign, bool skipNewline) {
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

static int resolveLocal(Compiler *compiler, Token *name) {
    for (int i = compiler->localCount - 1; i >= 0; i--) {
        Local *local = &compiler->locals[i];
        if (identifierEqual(name, &local->name)) {
            if (local->depth == -1) {
                if (reportError("Can't use local variable in it's own initializer")) {
                    printTokenInCode(parser.source, &parser.previous);
                }
            }
            return i;
        }
    }

    return -1;
}

static void namedVariable(Token name, bool canAssign) {
    uint8_t getOp, setOp;
    int arg = resolveLocal(current, &name); 
    if (arg != -1) {
        getOp = OP_GET_LOCAL;
        setOp = OP_SET_LOCAL;
    } else {
        arg = identifierConstant(&name);
        getOp = OP_GET_GLOBAL;
        setOp = OP_SET_GLOBAL;
    }

    TokenType type = parser.current.type;

    if (type == TOKEN_PLUS_PLUS  || type == TOKEN_MINUS_MINUS) {
        advance(false);
        emitBytes(getOp, (uint8_t)arg);
        if (type == TOKEN_PLUS_PLUS)
            emitByte(OP_INCREMENT);
        else if (type == TOKEN_MINUS_MINUS)
            emitByte(OP_DECREMENT);
        emitBytes(setOp, (uint8_t)arg);
        if (type == TOKEN_PLUS_PLUS)
            emitByte(OP_DECREMENT);
        if (type == TOKEN_MINUS_MINUS)
            emitByte(OP_INCREMENT);
        return;
    }

    if (type == TOKEN_EQUAL
     || type == TOKEN_PLUS_EQUAL || type == TOKEN_MINUS_EQUAL
     || type == TOKEN_STAR_EQUAL || type == TOKEN_SLASH_EQUAL
     || type == TOKEN_MOD_EQUAL  || type == TOKEN_POWER_EQUAL) {
        if (canAssign) {
            advance(false);

            if (type != TOKEN_EQUAL)
                    emitBytes(getOp, (uint8_t)arg);

            expression(false);

            switch (type) {
                case TOKEN_PLUS_EQUAL:
                    emitByte(OP_ADD);
                    break;
                case TOKEN_MINUS_EQUAL:
                    emitByte(OP_SUBTRUCT);
                    break;
                case TOKEN_STAR_EQUAL:
                    emitByte(OP_MULTIPLY);
                    break;
                case TOKEN_SLASH_EQUAL:
                    emitByte(OP_DIVIDE);
                    break;
                case TOKEN_MOD_EQUAL:
                    emitByte(OP_MOD);
                    break;
                case TOKEN_POWER_EQUAL:
                    emitByte(OP_POWER);
                    break;
            }
            emitBytes(setOp, (uint8_t)arg);

        } else {
            if (reportError("Invalid assignment target")) {
                int line = parser.exprStart.line;
                int column = parser.exprStart.column;
                int length = parser.previous.column + parser.previous.length - column;
                printHighlightedPartInCode(parser.source, line, column, length);
            }
        }
    } else {
        emitBytes(getOp, (uint8_t)arg);
    }
}

static void variable(bool canAssign, bool skipNewline) {
    namedVariable(parser.previous, canAssign);
}

static void string(bool canAssign, bool skipNewline) {
    emitConstant(OBJ_VAL(copyString(parser.previous.start, parser.previous.length)));
}

static void rawString() {
    emitConstant(OBJ_VAL(createRawString(parser.previous.start, parser.previous.length)));
}

static void fstring(bool canAssign, bool skipNewline) {
    int count = 0;

    while (parser.previous.type != TOKEN_STRING) {
        if (parser.previous.length > 0) {
            rawString();
            count++;
        }
        if (check(TOKEN_STRING) || check(TOKEN_FSTRING)) {
            if (reportError("Expect expression inside braces")) 
                print2HighlightedPartsInCode(parser.source,
                                             parser.current.line, parser.current.column - 1, 1,
                                             parser.current.line, parser.current.column - 2, 1);
            return;
        }
        expression(canAssign);
        count++;
        advance(false);
    }

    if (parser.previous.length > 0) {
        rawString();
        count++;
    }
    emitBytes(OP_BUILD_FSTRING, (uint8_t)count);

    return;
}

static void grouping(bool canAssign, bool skipNewline) {
    Token groupingStart = parser.previous;

    expression(true);

    if (!consume(TOKEN_RIGHT_PAREN)) {
        switch (parser.current.type) {
            case TOKEN_RIGHT_BRACE:
                if (reportError("closing brace '}' does not match opening parenthesis '('"))
                    print2HighlightedPartsInCode(parser.source,
                                                 groupingStart.line, groupingStart.column, 1,
                                                 parser.current.line, parser.current.column, 1);
                break;
            case TOKEN_RIGHT_BRACKET:
                if (reportError("closing bracket ']' does not match opening parenthesis '('"))
                    print2HighlightedPartsInCode(parser.source,
                                                 groupingStart.line, groupingStart.column, 1,
                                                 parser.current.line, parser.current.column, 1);
                break;
            default:
                if (reportError("opening parenthesis does not closed"))
                    printTokenInCode(parser.source, &groupingStart);
                break;
        }
    }
}

static void absolute(bool canAssign, bool skipNewline) {
    expression(true);

    if (!consume(TOKEN_PIPE))
        printf("unlucky\n");
    
    emitByte(OP_ABS);
}

static void unary(bool canAssign, bool skipNewline) {
    TokenType operatorType = parser.previous.type;
    ParseRule *rule = getRule(operatorType);

    if (operatorType == TOKEN_PLUS_PLUS || operatorType == TOKEN_MINUS_MINUS) {
        advance(skipNewline);
        uint8_t getOp, setOp;
        Token name = parser.previous;
        int arg = resolveLocal(current, &name);

        if (arg != -1) {
            getOp = OP_GET_LOCAL;
            setOp = OP_SET_LOCAL;
        } else {
            arg = identifierConstant(&name);
            getOp = OP_GET_GLOBAL;
            setOp = OP_SET_GLOBAL;
        }
        emitBytes(getOp, (uint8_t)arg);

        if (operatorType == TOKEN_PLUS_PLUS) 
            emitByte(OP_INCREMENT);
        else
            emitByte(OP_DECREMENT);

        emitBytes(setOp, (uint8_t)arg);
        return;
    }

    parsePrecedence((Precedence)(rule->precedence + 1), skipNewline);

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

static void binary(bool canAssign, bool skipNewline) {
    TokenType operatorType = parser.previous.type;
    ParseRule *rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1), skipNewline);

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
        case TOKEN_POWER:
            emitByte(OP_POWER);
            break;
        case TOKEN_EOS:
            break;
        default:
            return;
    }
}

static void nullary(bool canAssign, bool skipNewline) {
    TokenType operatorType = parser.current.type;
    advance(skipNewline);

    switch (operatorType) {
        case TOKEN_BANG:
            emitByte(OP_FACTORIAL);
            break;
        default:
            return;
    }
}

static uint8_t argumentList() {
    uint8_t argCount = 0;
    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            expression(true);
            if (argCount == 255)
                reportError("Can't have more than 255 arguments");
            argCount++;
        } while (match(TOKEN_COMMA, true));
    }
    if (!consume(TOKEN_RIGHT_PAREN)) {
        reportError("Expect ')' after arguments");
    }
    return argCount;
}

static void call(bool canAssign, bool skipNewline) {
    uint8_t argCount = argumentList();
    emitBytes(OP_CALL, argCount);
}

static void beginScope() {
    current->scopeDepth++;
}

static void endScope() {
    current->scopeDepth--;

    while (current->localCount > 0 &&
           current->locals[current->localCount - 1].depth > current->scopeDepth) {
            emitByte(OP_POP);
            current->localCount--;
    }
}

static void block() {
    Token blockStart = parser.previous;

    while (!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF)) {
        declaration();
    }

    // printToken(&parser.previous);
    // printToken(&parser.current);
    if (!consume(TOKEN_RIGHT_BRACE)) {
        switch (parser.previous.type) {
            case TOKEN_RIGHT_PAREN:
                if (reportError("Expect '}' instead of ')'")) 
                    printHighlightedPartInCode(parser.source, parser.previous.line, parser.previous.column, 1);
                break;
            case TOKEN_RIGHT_BRACKET:
                if (reportError("Expect '}' instead of ']'")) 
                    printHighlightedPartInCode(parser.source, parser.previous.line, parser.previous.column, 1);
                break;
            default:
                if (reportError("Unclosed '{' starting here")) 
                    printHighlightedPartInCode(parser.source, blockStart.line, blockStart.column, 1);
                break;
        }
    }
}

static void function(FunctionType type) {
    Compiler compiler;
    initCompiler(&compiler, type);
    beginScope();

    if (!consume(TOKEN_LEFT_PAREN)) {
        reportError("Expect '(' after function name");
    }
    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            current->function->arity++;
            if (current->function->arity > 255) {
                reportError("Can't have more than 255 parameters");
            }
            uint8_t constant = parseVariable("Expect parameter name");
            defineVariable(constant);
        } while (match(TOKEN_COMMA, false));
    }
    if (!consume(TOKEN_RIGHT_PAREN)) {
        reportError("Expect ')' after parameters");
    }
    if (!consume(TOKEN_LEFT_BRACE)) {
        reportError("Expect '{' after function body}");
    }
    block();

    ObjFunction *function = endCompiler();
    emitBytes(OP_CONSTANT, createConstant(OBJ_VAL(function)));
}

static void ifStatement() {
    expression(true);

    int jumpToNextBranch = emitJump(OP_JUMP_IF_FALSE_AND_POP);
    int jumpToEnd = -1;

    statement(true);

    if (check(TOKEN_ELIF)) {
        jumpToEnd = emitJump(OP_JUMP);
        while (match(TOKEN_ELIF, true)) {
            patchJump(jumpToNextBranch);
            expression(true);
            jumpToNextBranch = emitJump(OP_JUMP_IF_FALSE_AND_POP);
            statement(true);
            emitLoop(jumpToEnd - 1);
        }
    }

    patchJump(jumpToNextBranch);

    if (match(TOKEN_ELSE, true)) {
        if (jumpToEnd == -1)
            jumpToEnd = emitJump(OP_JUMP);
        statement(true);
    }

    if (jumpToEnd != -1)
        patchJump(jumpToEnd);
}

static void whileStatement() {
    int loopStart = currentCode()->size;
    expression(true);

    int exitJump = emitJump(OP_JUMP_IF_FALSE);
    emitByte(OP_POP);
    statement(true);
    emitLoop(loopStart);

    patchJump(exitJump);
    emitByte(OP_POP);
}

static void returnStatement() {
    if (current->type == TYPE_TOP_LEVEL) {
        reportError("Can't return from top-level code");
    }
    if (match(TOKEN_EOS, false)) {
        emitReturn();
    } else {
        expression(false);
        if (!consume(TOKEN_EOS)) {
            reportError("Expect eos after value");
        }
        emitByte(OP_RETURN);
    }
}

static void statement(bool skipNewline) {
    if (match(TOKEN_IF, skipNewline)) {
        ifStatement();
    } else if (match(TOKEN_WHILE, skipNewline)) {
        whileStatement();
    } else if (match(TOKEN_LEFT_BRACE, skipNewline)) {
        beginScope();
        block();
        endScope();
    } else if (match(TOKEN_RETURN, skipNewline)) {
        returnStatement();
    } else {
        expressionStatement();
    }
}

static void varDeclaration() {
    uint8_t global = parseVariable("Expect variable name.");

    if (match(TOKEN_EQUAL, false))
        expression(false);
    else
        emitByte(OP_NIL);
    
    if (!consume(TOKEN_EOS)) {
        if (reportError("Expect eos after variable declaration"))
            printHighlightedPartInCode(parser.source, parser.previous.line, parser.previous.column + parser.previous.length, 1);
    }

    defineVariable(global);
}

static void funcDeclaration() {
    uint8_t global = parseVariable("Expect function name");
    markInitialized();
    function(TYPE_FUNCTION);
    defineVariable(global);
}

static void declaration() {
    if (match(TOKEN_EOS, false))
        return;
    
    if (match(TOKEN_DEF, false)) {
        funcDeclaration();
    } else if (match(TOKEN_VAR, false))
        varDeclaration();
    else
        statement(false);

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

    while (!match(TOKEN_EOF, false))
        declaration();

    ObjFunction *function = endCompiler();
    return parser.errorCount != 0 ? NULL : function;
}