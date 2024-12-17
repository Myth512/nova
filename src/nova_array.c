#include "nova_array.h"
#include "object.h"
#include "object_array.h"
#include "error.h"
#include "vm.h"

Value novaArrayPush(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[0]);
    pushValue(&array->vec, argv[1]);
    return NIL_VAL;
}

Value novaArrayPop(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[0]);
    if (array->vec.size == 0)
        reportRuntimeError("Can't pop from empty array");
    return popValue(&array->vec);
}

Value novaArrayInsert(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[0]);
    int index = AS_NUMBER(argv[1]);
    insertValue(&array->vec, index, argv[2]);
    return NIL_VAL;
}

Value novaArrayReverse(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[0]);
    reverseValueVec(&array->vec);
    return NIL_VAL;
}

Value novaArraySort(int argc, Value *argv) {

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

Value novaArrayEqual(int argc, Value *argv) {
    // ObjArray *a = AS_ARRAY(argv[0]);
    // ObjArray *b = AS_ARRAY(argv[1]);
    // return BOOL_VAL(compareArrays(a, b));
}

Value novaArrayNotEqual(int argc, Value *argv) {
    // ObjArray *a = AS_ARRAY(argv[0]);
    // ObjArray *b = AS_ARRAY(argv[1]);
    // return BOOL_VAL(!compareArrays(a, b));
}

Value novaArrayGreater(int argc, Value *argv) {

}

Value novaArrayGreaterEqual(int argc, Value *argv) {

}

Value novaArrayLess(int argc, Value *argv) {

}

Value novaArrayLessEqual(int argc, Value *argv) {

}

Value novaArrayAdd(int argc, Value *argv) {

}

Value novaArrayMultiply(int argc, Value *argv) {

}

Value novaArrayGetAt(int argc, Value *argv) {

}

Value novaArraySetAt(int argc, Value *argv) {

}

Value novaArrayLen(int argc, Value *argv) {

}
