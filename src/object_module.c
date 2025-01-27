#include "object_module.h"
#include "object_string.h"
#include "vm.h"

ObjModule *allocateModule() {
    ObjModule *module = (ObjModule*)allocateObject(sizeof(ObjModule), VAL_MODULE);
    tableInit(&module->globals);
    return module;
}

Value Module_GetAttribute(Value obj, ObjString *name) {
    ObjModule *module = AS_MODULE(obj);
    return tableGet(&module->globals, OBJ_VAL(name));
}

Value Module_Call(Value callee, int argc, int kwargc, Value *argv) {
}

int Module_ToStr(Value value, char *buffer, size_t size) {
    writeToBuffer(buffer, size, "<module '%s'>", AS_MODULE(value)->function->name->chars);
}