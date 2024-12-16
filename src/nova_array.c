#include "nova_array.h"
#include "object.h"
#include "object_array.h"
#include "error.h"
#include "vm.h"

Value arrayPush(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[0]);
    pushValue(&array->values, argv[1]);
    return NIL_VAL;
}

Value arrayPop(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[0]);
    if (array->values.size == 0) {
        reportRuntimeError("Can't pop from empty array");
        printErrorInCode();
    }
    return popValue(&array->values);
}

Value arrayInsert(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[0]);
    int index = AS_NUMBER(argv[1]);
    insertValue(&array->values, index, argv[2]);
    return NIL_VAL;
}

Value arrayReverse(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[0]);
    reverseValueVec(&array->values);
    return NIL_VAL;
}

Value arraySort(int argc, Value *argv) {

}

// bool compareArrays(ObjArray *a, ObjArray *b) {
//     if (a->values.size != b->values.size)
//         return false;
//     for (int i = 0; i < a->values.size; i++) {
//         if (!compareValues(a->values.values[i], b->values.values[i]))
//             return false;
//     }
//     return true;
// }

Value arrayEqual(int argc, Value *argv) {
    // ObjArray *a = AS_ARRAY(argv[0]);
    // ObjArray *b = AS_ARRAY(argv[1]);
    // return BOOL_VAL(compareArrays(a, b));
}

Value arrayNotEqual(int argc, Value *argv) {
    // ObjArray *a = AS_ARRAY(argv[0]);
    // ObjArray *b = AS_ARRAY(argv[1]);
    // return BOOL_VAL(!compareArrays(a, b));
}

Value arrayGreater(int argc, Value *argv) {

}

Value arrayGreaterEqual(int argc, Value *argv) {

}

Value arrayLess(int argc, Value *argv) {

}

Value arrayLessEqual(int argc, Value *argv) {

}

Value arrayAdd(int argc, Value *argv) {

}

Value arrayMultiply(int argc, Value *argv) {

}

Value arrayGetAt(int argc, Value *argv) {

}

Value arraySetAt(int argc, Value *argv) {

}

Value arrayLen(int argc, Value *argv) {

}
