#include "../include/common.hpp"
#include "../include/hart_t.hpp"
#include "../include/instruction_t.hpp"
//#include "../include/memory_t.hpp"


int main(int argc, char *argv[]) {

	myCPU::hart_t hart {};
	for (int i = 1; i < argc; ++i) {
		hart.load(argv[i]);
		hart.execute();
	}

	return 0;
}