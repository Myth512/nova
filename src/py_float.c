#include "py_float.h"
#include "value_float.h"
#include "value_int.h"
#include "vm.h"

Value PyFloat_Equal(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_Equal(self, value);
}

Value PyFloat_NotEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_NotEqual(self, value);
}

Value PyFloat_Greater(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_Greater(self, value);
}

Value PyFloat_GreaterEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_GreaterEqual(self, value);
}

Value PyFloat_Less(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_Less(self, value);
}

Value PyFloat_LessEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_LessEqual(self, value);
}

Value PyFloat_Add(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_Add(self, value);
}

Value PyFloat_Subtract(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_Subtract(self, value);
}

Value PyFloat_RightSubtract(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_RightSubtract(self, value);
}

Value PyFloat_Multiply(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_Multiply(self, value);
}

Value PyFloat_TrueDivide(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_TrueDivide(self, value);
}

Value PyFloat_RightTrueDivide(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_RightTrueDivide(self, value);
}

Value PyFloat_FloorDivide(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_FloorDivide(self, value);
}

Value PyFloat_RightFloorDivide(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_RightFloorDivide(self, value);
}

Value PyFloat_Modulo(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_Modulo(self, value);
}

Value PyFloat_RightModulo(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_RightModulo(self, value);
}

Value PyFloat_Power(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_Power(self, value);
}

Value PyFloat_RightPower(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Float_RightPower(self, value);
}

Value PyFloat_Positive(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return Float_Positive(self);
}

Value PyFloat_Negative(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return Float_Negative(self);
}

Value PyFloat_Class(Value value) {
    return Float_Class(value);
}

Value PyFloat_Init(Value callee, int argc, Value *argv) {
    return Float_Init(callee, argc, argv);
}

Value PyFloat_GetAttribute(int argc, int kwargc) {
    static char *keywords[] = {"self", "name"};
    Value self;
    ObjString *name;
    PARSE_ARGS(&self, &name);

    return Float_GetAttribute(self, name);
}

Value PyFloat_Hash(Value value) {
    return INT_VAL(Float_Hash(value));
}

Value PyFloat_ToBool(Value value) {
    return BOOL_VAL(Float_ToBool(value));
}

Value PyFloat_ToInt(Value value) {
    return INT_VAL(Float_ToInt(value));
}

Value PyFloat_ToFloat(Value value) {
    return FLOAT_VAL(Float_ToFloat(value));
}
