#include <iostream>
#include <fstream>

#define from 1
#define to 2

int main(int argc, char *argv[]) {
	
	FILE *inp = fopen(argv[from], "r");
	FILE *out = fopen(argv[to]  , "w");

	int count = 0, x = 0;
	fscanf(inp, "%d", &count);
	for (int i = 0; i < count; ++i) {
		fscanf(inp, "%x", &x);
		fprintf(out, "%c%c%c%c", (x >> 24) & 0xff, (x >> 16) & 0xff, (x >> 8) & 0xff, x & 0xff);
	}

	fclose(inp);
	fclose(out);
	return 0;
}