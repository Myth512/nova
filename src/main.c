#include <stdlib.h>

#include "common.h"
#include "code.h"
#include "debug.h"
#include "scanner.h"
#include "vm.h"

void repl() {
	char line[1024];
	while (true) {
		printf("> ");

		if (!fgets(line, sizeof(line), stdin)) {
			printf("\n");
			break;
		}

		interpret(line, "");
	}
}

void runFile(const char *path) {
	char *source = readFile(path);
	InterpretResult result = interpret(source, path);
	free(source);

	if (result == INTERPRET_COMPILE_ERROR)
		exit(65);
	if (result == INTERPRET_RUNTIME_ERROR) {
		printf("runtime error\n");
		exit(70);
	}
}

int main(int argc, const char *argv[]) {
	initVM(argv[1]);

	if (argc == 1) {
		repl();
	} else if (argc == 2) {
		runFile(argv[1]);
	} else {
		fprintf(stderr, "Usage: nova [path]\n");
	}

	freeVM();
	return 0;
}
