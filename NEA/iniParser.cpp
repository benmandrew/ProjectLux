/*
#include <vector>
#include "iniParser.h"

bool readINI(
	const char* path
) {
	FILE* file = fopen(path, "r");
	if (file == nullptr) {
		printf("Impossible to open INI file!\n");
		printf(path);
		printf("\n");
		return false;
	}
	parse(file);
	return true;
}

static void parse(FILE* file) {
	while (true) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)	break;

		// Comment line found (fscanf skips over empty lines so they do not need to be checked for)
		if (strcmp(lineHeader, "#") == 0) {
			continue;
		}
		readLine(lineHeader);
	}
}

static void readLine(char* line) {

}

static void readVariable(char* line) {
	char* name = strrchr(line, ':');
}
*/