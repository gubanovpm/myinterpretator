#pragma once
#include "./common.hpp"
#include "./memory_t.hpp"

namespace myCPU {

class hart_t {
public:
	hart_t();
	~hart_t();

	RegisterValue_t getRegister  (size_t register_number) const;
	RegisterValue_t getPCRegister() const;
	RegisterValue_t getNextPC    () const;
	op_memory_t *getOpMemory     () ;

	void			printAllReg  () const;

	void			setRegister  (size_t register_number, RegisterValue_t value) ;
	void            setPCRegister(RegisterValue_t value);
	void            setNextPC    (RegisterValue_t value);

	void load(std::string file_name);
	bool execute();
private:
	static const size_t register_count_ = 32;

	RegisterValue_t registers_[register_count_] = {};
	RegisterValue_t register_pc_ = 0;
	RegisterValue_t next_pc_ = 0;
	op_memory_t *op_memory_ ;
};

}