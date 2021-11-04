#include "../include/hart_t.hpp"
#include <iostream>

myCPU::hart_t::hart_t() {
	op_memory_ = new op_memory_t();
}

myCPU::hart_t::~hart_t() {
	delete op_memory_;
}

myCPU::RegisterValue_t myCPU::hart_t::getRegister  (size_t register_number) const {
	return registers_[register_number];
}
myCPU::RegisterValue_t myCPU::hart_t::getPCRegister() const {
	return register_pc_;
}
myCPU::RegisterValue_t myCPU::hart_t::getNextPC    () const {
	return next_pc_;
}
myCPU::op_memory_t* myCPU::hart_t::getOpMemory()  {
	return op_memory_;
}
void myCPU::hart_t::printAllReg  () const {
	std::cout << "All registers: " << std::endl;
	for (int i = 0; i < register_count_; ++i)
		std::cout << "reg[" << i << "] = " << registers_[i] << " ;" << std::endl ;
	std::cout << "PC value = " << register_pc_ << " ; next PC value = " << next_pc_ << std::endl;
}

void myCPU::hart_t::setRegister (size_t register_number, myCPU::RegisterValue_t value) {
	registers_[register_number] = value;
}
void myCPU::hart_t::setPCRegister (myCPU::RegisterValue_t value) {
	register_pc_ = value;
}
void myCPU::hart_t::setNextPC (myCPU::RegisterValue_t value) {
	next_pc_ = value;
}

bool myCPU::hart_t::execute() {
}