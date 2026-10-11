#include <string.h>
#define NOB_IMPLEMENTATION
#define BUILD_FOLDER "./build/"
#define SRC_FOLDER   "./src/"
#define LIBS_FOLDER  "./raylib/"
#include "nob.h"

int
main(int argc, char** argv) {
	NOB_GO_REBUILD_URSELF(argc, argv);
	if (!mkdir_if_not_exists(BUILD_FOLDER)) return 1;

	Cmd cmd = {0};
	cmd_append(&cmd, "cc");
	cmd_append(&cmd, "-Wall", "-Wextra", "-Werror", "-Wpedantic");

	cmd_append(&cmd, "-o", BUILD_FOLDER "main", SRC_FOLDER "main.c");

	cmd_append(&cmd, "-I" LIBS_FOLDER);
	cmd_append(&cmd, "-L" LIBS_FOLDER);
	cmd_append(&cmd, "-lraylib", "-lm");

	if (!cmd_run(&cmd)) return 1;

	if (argc > 1) {
		if (!strcmp(argv[1], "run")) {
			cmd_append(&cmd, BUILD_FOLDER "main");
			if (!cmd_run(&cmd)) return 1;
		} else {
			return 1;
		}
	}

	return 0;
}
