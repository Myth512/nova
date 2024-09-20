#include "common.h"
#include "code.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char *argv[]) {
	initVM();
	CodeVec vec;
	initCodeVec(&vec);

	int constant = pushConstant(&vec, 1.2);
	pushInstruction(&vec, OP_CONSTANT, 123);
	pushInstruction(&vec, constant, 123);

	int constant2 = pushConstant(&vec, 69);
	pushInstruction(&vec, OP_CONSTANT, 420);
	pushInstruction(&vec, constant2, 420);

	pushInstruction(&vec, OP_ADD, 111);
	pushInstruction(&vec, OP_RETURN, 123);

	interpret(&vec);
	freeVM();
	freeCodeVec(&vec);

	return 0;
}
