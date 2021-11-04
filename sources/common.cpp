#include "../include/common.hpp"
#include <cstdint>

uint32_t getBits(size_t from, size_t to, uint32_t value) {
	if (from > to) std::swap(from, to);
	if (from > 31) return 0;

	myCPU::RegisterValue_t result = 0;
	for (size_t itb = 0, ite = to - from; itb != ite ; ++itb )
		result |= ((value >> (itb + from)) & 1) << itb ;
	return result;
}