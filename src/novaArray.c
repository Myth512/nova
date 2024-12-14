#include "novaArray.h"
#include "object.h"

Value arrayPush(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[0]);
    pushValue(&array->values, argv[1]);
    return NIL_VAL;
}

Value arrayPop(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[-1]);
    if (array->values.size == 0) {
        reportRuntimeError("Can't pop from empty array");
    }
    return popValue(&array->values);
}

Value arrayInsert(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[-1]);
    int index = AS_NUMBER(argv[0]);
    insertValue(&array->values, index, argv[1]);
    return NIL_VAL;
}

Value arrayReverse(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[-1]);
    reverseValueVec(&array->values);
    return NIL_VAL;
}

Value arraySort(int argc, Value *argv) {

}

Value arrayConcatinate(int argc, Value *argv) {

}

Value arrayEqual(int argc, Value *argv) {

}