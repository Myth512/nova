#include "py_list.h"
#include "object.h"
#include "object_array.h"
#include "error.h"
#include "vm.h"

Value novaArrayPush(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[0]);
    pushValue(&array->vec, argv[1]);
    return NONE_VAL;
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
    return NONE_VAL;
}

Value novaArrayReverse(int argc, Value *argv) {
    ObjArray *array = AS_ARRAY(argv[0]);
    reverseValueVec(&array->vec);
    return NONE_VAL;
}

static void swap(ObjArray* array, int l, int r) {
    Value tmp = array->vec.values[l];
    array->vec.values[l] = array->vec.values[r];
    array->vec.values[r] = tmp;
}

// Hoare partition
static int partition(ObjArray *array, int l, int r, bool reverse) {
    // bool (*leftFunc)(Value, Value) = valueLessEqual;
    // bool (*rightFunc)(Value, Value) = valueGreaterEqual;
    // if (reverse) {
    //     leftFunc = valueGreaterEqual;
    //     rightFunc = valueLessEqual;
    // }

    // Value pivot = array->vec.values[r];
    // int p = r;
    // while (l < r) {
    //     while (l < r && leftFunc(array->vec.values[l], pivot))
    //         l++;
    //     while (l < r && rightFunc(array->vec.values[r], pivot))
    //         r--;
    //     swap(array, l, r);
    // }
    // swap(array, l, p);
    // return l;
}

static void quicksort(ObjArray *array, int left, int right, bool reverse) {
    if (left >= right)
        return;

    int p = partition(array, left, right, reverse);
    quicksort(array, left, p - 1, reverse);
    quicksort(array, p + 1, right, reverse);
}

Value novaArraySort(int argc, Value *argv) {
    bool reverse = false;
    if (argc == 1) {
        if (IS_BOOL(argv[1])) {
            reverse = AS_BOOL(argv[1]);
        } else {
            reportRuntimeError("Reverse must be boolean");
        }
    } else if (argc > 1) {
        reportRuntimeError("At most 1 arguments");
    }

    ObjArray *array = AS_ARRAY(argv[0]);
    quicksort(array, 0, array->vec.size - 1, reverse);
    return NONE_VAL;
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
