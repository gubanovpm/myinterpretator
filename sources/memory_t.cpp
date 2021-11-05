#include "../include/memory_t.hpp"

myCPU::op_memory_t::op_memory_t() {
	buffer_ = new unsigned char [size_];
}

myCPU::op_memory_t::~op_memory_t() {
	delete [] buffer_;
}

bool myCPU::op_memory_t::write(Adress_t rd_adr, size_t value_size, RegisterValue_t  value){
	if (rd_adr > 0xffffffff - value_size) return false;

	for (int it = value_size - 1 ; it >= 0; --it, value >>= 8)
		buffer_[rd_adr + it] = value & 0xff;
	return true;
}

bool myCPU::op_memory_t::read(Adress_t rd_adr, size_t value_size, RegisterValue_t  &value){
	if (rd_adr > 0xffffffff - value_size) return false;
	value = 0;
	for (size_t it = 0; it != value_size; ++it) {
		value |= ((buffer_[rd_adr + it] & 0xff) << (it * 8));
	}
	return true;
}