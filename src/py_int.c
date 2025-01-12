#include "py_int.h"
#include "value_int.h"
#include "object_string.h"
#include "vm.h"

Value PyInt_Equal(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Equal(self, value);
}

Value PyInt_NotEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_NotEqual(self, value);
}

Value PyInt_Greater(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Greater(self, value);
}

Value PyInt_GreaterEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_GreaterEqual(self, value);
}

Value PyInt_Less(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Less(self, value);
}

Value PyInt_LessEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_LessEqual(self, value);
}

Value PyInt_Add(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Add(self, value);
}

Value PyInt_Subtract(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Subtract(self, value);
}

Value PyInt_RightSubtract(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightSubtract(self, value);
}

Value PyInt_Multiply(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Multiply(self, value);
}

Value PyInt_TrueDivide(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_TrueDivide(self, value);
}

Value PyInt_RightTrueDivide(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightTrueDivide(self, value);
}

Value PyInt_FloorDivide(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_FloorDivide(self, value);
}

Value PyInt_RightFloorDivide(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightFloorDivide(self, value);
}

Value PyInt_Modulo(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Modulo(self, value);
}

Value PyInt_RightModulo(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightModulo(self, value);
}

Value PyInt_Power(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Power(self, value);
}

Value PyInt_RightPower(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightPower(self, value);
}

Value PyInt_Positive(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return Int_Positive(self);
}

Value PyInt_Negative(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return Int_Negative(self);
}

Value PyInt_And(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_And(self, value);
}

Value PyInt_Xor(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Xor(self, value);
}

Value PyInt_Or(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_Or(self, value);
}

Value PyInt_Invert(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return Int_Invert(self);
}

Value PyInt_LeftShift(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_LeftShift(self, value);
}

Value PyInt_RightLeftShift(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightLeftShift(self, value);
}

Value PyInt_RightShift(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightShift(self, value);
}

Value PyInt_RightRightShift(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return Int_RightRightShift(self, value);
}

Value PyInt_GetAttribute(int argc, int kwargc) {
    static char *keywords[] = {"self", "name"};
    Value self;
    ObjString *name;
    PARSE_ARGS(&self, &name);

    return Int_GetAttribute(self, name);
}

Value PyInt_Class(Value self) {
    return Int_Class(self);
}