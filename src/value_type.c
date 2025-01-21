#include "value_type.h"
#include "value_methods.h"
#include "vm.h"

Value Type_Init(Value callee, int argc, Value *argv) {
    return valueClass(argv[0]);
}

Value Type_Class(Value value) {
    return OBJ_VAL(vm.types.type);
}