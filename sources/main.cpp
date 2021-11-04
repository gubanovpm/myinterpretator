#include "../include/common.hpp"
//#include "../include/hart_t.hpp"
//#include "../include/instruction_t.hpp"
#include "../include/memory_t.hpp"


int main() {

	myCPU::op_memory_t operative;
	myCPU::RegisterValue_t res = 0, wr_num = 0xab06f044;
	operative.write(12, 4, wr_num);
	operative.read(12, 4, res);
	printf("%x\n", res);
	std::cout << getBits(2, 19, 15449) << "; res of write-read = " << (res == wr_num) << std::endl;
	return 0;
}