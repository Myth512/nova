#include "object_super.h"
#include "object_instance.h"
#include "vm.h"

ObjSuper *allocateSuper(Value self, Value class) {
    ObjSuper *super = (ObjSuper*)allocateObject(sizeof(ObjSuper), VAL_SUPER);
    super->self = self;
    super->class = class;
    return super;
}

Value Super_Init(Value callee, int argc, Value *argv) {
    Value self = frame->slots[0];
    Value class = OBJ_VAL(AS_INSTANCE(self)->class);
    ObjSuper *super = allocateSuper(self, class);
    Value res = OBJ_VAL(super);
    return res;
}

Value Super_GetAttr(Value value, ObjString *name) {
    Value self = AS_SUPER(value)->self;
    Value class = AS_SUPER(value)->class;
    ObjClosure *method = AS_CLOSURE(Class_GetAttr(AS_CLASS(class)->super, name));
    return OBJ_VAL(createMethod(self, method));
}

int Super_ToStr(Value value, char *buffer, const size_t size) {
    writeToBuffer(buffer, size, "<super: ");
}