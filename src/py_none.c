#include "py_none.h"
#include "value_int.h"
#include "value_none.h"
#include "vm.h"

Value PyNone_Equal(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return None_Equal(self, value);
}

Value PyNone_NotEqual(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return None_Equal(self, value);
}

Value PyNone_GetAttribute(int argc, int kwargc) {
    static char *keywords[] = {"self", "value"};
    Value self, value;
    PARSE_ARGS(&self, &value);

    return None_Equal(self, value);
}

Value PyNone_Hash(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return INT_VAL(None_Hash(self));
}

Value PyNone_ToBool(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return BOOL_VAL(None_ToBool(self));
}

Value PyNone_Class(Value self) {
    return None_Class(self);
}