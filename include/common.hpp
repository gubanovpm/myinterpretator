#pragma once
#include <cstdint>
#include <iostream>


namespace myCPU {

using RegisterValue_t = uint32_t;
using Adress_t        = uint32_t;
using EncodedInstr_t  = uint32_t;

std::string reg_name(int num);

};

uint32_t getBits(size_t from, size_t to, uint32_t value);