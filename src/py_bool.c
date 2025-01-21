#include "py_bool.h"
#include "value_int.h"
#include "vm.h"

Value PyBool_Equal(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Equal(self, value);
}

Value PyBool_NotEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_NotEqual(self, value);
}

Value PyBool_Greater(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Greater(self, value);
}

Value PyBool_GreaterEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_GreaterEqual(self, value);
}

Value PyBool_Less(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Less(self, value);
}

Value PyBool_LessEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_LessEqual(self, value);
}

Value PyBool_Add(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Add(self, value);
}

Value PyBool_Subtract(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Subtract(self, value);
}

Value PyBool_RightSubtract(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightSubtract(self, value);
}

Value PyBool_Multiply(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Multiply(self, value);
}

Value PyBool_TrueDivide(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_TrueDivide(self, value);
}

Value PyBool_RightTrueDivide(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightTrueDivide(self, value);
}

Value PyBool_FloorDivide(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_FloorDivide(self, value);
}

Value PyBool_RightFloorDivide(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightFloorDivide(self, value);
}

Value PyBool_Modulo(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Modulo(self, value);
}

Value PyBool_RightModulo(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightModulo(self, value);
}

Value PyBool_Power(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Power(self, value);
}

Value PyBool_RightPower(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightPower(self, value);
}

Value PyBool_Positive(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return Int_Positive(self);
}

Value PyBool_Negative(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return Int_Negative(self);
}

Value PyBool_And(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_And(self, value);
}

Value PyBool_Xor(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Xor(self, value);
}

Value PyBool_Or(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Or(self, value);
}

Value PyBool_Invert(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return Int_Invert(self);
}

Value PyBool_LeftShift(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_LeftShift(self, value);
}

Value PyBool_RightLeftShift(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightLeftShift(self, value);
}

Value PyBool_RightShift(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightShift(self, value);
}

Value PyBool_RightRightShift(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightRightShift(self, value);
}

Value PyBool_GetAttribute(int argc, int kwargc) {
    static char *keywords[] = {"self", "name"};
    Value self;
    ObjString *name;
    PARSE_ARGS(&self, &name);

    return Bool_GetAttribute(self, name);
}

Value PyBool_Class(Value self) {
    return Bool_Class(self);
}