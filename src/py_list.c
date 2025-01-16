#include "py_list.h"
#include "object_list.h"
#include "vm.h"

Value PyList_Append(int argc, int kwargc) {
    static char *keywords[] = {"self", "object"};
    Value self, object;
    PARSE_ARGS(&self, &object);

    return List_Append(self, object);
}

Value PyList_Clear(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyList_Copy(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyList_Count(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyList_Extend(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyList_Index(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyList_Insert(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyList_Pop(int argc, int kwargc) {
    static char *keywords[] = {"self"};
    Value self;
    PARSE_ARGS(&self);

    return List_Pop(self);
}

Value PyList_Remove(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyList_Reverse(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}

Value PyList_Sort(int argc, int kwargc) {
    return NOT_IMPLEMENTED_VAL;
}