#pragma once

#include <iostream>
#include "./common.hpp"

namespace myCPU{

class op_memory_t {
public:
	op_memory_t();
	~op_memory_t();

	bool write(Adress_t rd_adr, size_t value_size, RegisterValue_t  value);
	bool read (Adress_t wr_adr, size_t value_size, RegisterValue_t &value);
private:
	static const uint32_t size_ = 0xffffffff;
	unsigned char *buffer_;
};

class virtual_memory_t {
public:

private:
	
};

}