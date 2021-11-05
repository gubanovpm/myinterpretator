#include "../include/hart_t.hpp"
#include "../include/instruction_t.hpp"
#include <fstream>
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
	for (size_t i = 1; i <= register_count_; ++i) {
		std::cout << "reg[" << i - 1 << "] = " << registers_[i - 1] << " ; " ;
		if ((i % 6 == 0 && i != 0) || i == register_count_ ) std::cout << std::endl;
	}
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
	myCPU::RegisterValue_t startPC = register_pc_;
	while (1) {
		myCPU::EncodedInstr_t encdoedInstr;
		myCPU::instruction_t t{};
		if (!op_memory_->read(register_pc_, t.instruction_size_, encdoedInstr)) return false;
		else if (encdoedInstr == 0xffffffff) { register_pc_ = startPC; return true; }
		
		std::cout << register_pc_ << ": " ; print_encInstr(std::cout, encdoedInstr); std::cout << std::endl;
		myCPU::instruction_t instr(encdoedInstr, register_pc_);
		char c;
		bool isNextInstr = false;
		while (!isNextInstr) {
			c = getc(stdin);
			switch (c) {
				case 'r'  : printAllReg(); break;
				case 'n'  : isNextInstr = true; break;
				case 'p'  : size_t num; std::cin >> num ; std::cout << getRegister(num) << std::endl; break;
				case '\n' : break;
				case 'q'  : return false; break;
				default : std::cout << "unknown op, please try again\n" ;  
			}
		}

		next_pc_ = register_pc_ + t.instruction_size_;
		instr.executor_(this, instr);
		register_pc_ = next_pc_;
	}
}

void myCPU::hart_t::load(std::string file_name) {
	myCPU::RegisterValue_t oldPC = register_pc_;
	
	FILE *loaded_file = fopen(file_name.data(), "rb");

	fseek(loaded_file, 0, SEEK_END);
	unsigned sz = ftell(loaded_file);
	fseek(loaded_file, 0, SEEK_SET);

	unsigned char *text = new unsigned char [sz];
	if(!fread(text, sizeof(char), sz, loaded_file)) abort();
	fclose(loaded_file);


	for (size_t i = 0; i < sz / 5; ++i) {
		if (!op_memory_->write(register_pc_, 4, *(uint32_t *)(text + i * 5))) return;
		printf("%x\n", *(uint32_t *)(text + i * 5));
		register_pc_ += 4;
	}

	if (!op_memory_->write(register_pc_, 4, 0xffffffff)) return;
	register_pc_ += 4;

	std::cout << "File \'" << file_name << "\' loaded \n" ;

	delete [] text;

	register_pc_ = oldPC;
}