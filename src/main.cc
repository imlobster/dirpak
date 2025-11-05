#include <stdio.h>

#include "pack.h"

int main(int argc, char** argv) {
	if(argc <= 1) { fprintf(stderr, "dirpak: usage:\n\t'dirpak input/'\n\t'dirpak input/ output.txt'\n"); return 1; }
	char* filename = nullptr;
	if(argc == 3) { filename = argv[2]; }
	else if(argc >= 4) { fprintf(stdout, "dirpak: only first two arguments will be used\n"); }

	return pack(argv[1], filename);
}
