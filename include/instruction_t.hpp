#pragma once
#include "./common.hpp"
#include "./hart_t.hpp"

#include <iostream>

enum instr_frmt_t { OP   = 0b0110011, OP_IMM = 0b0010011, LOAD = 0b0000011,
                    SAVE = 0b0100011, BRANCH = 0b1100011, JAL  = 0b1101111, 
					JALR = 0b1100111, AUIPC  = 0b0010111, LUI  = 0b0110111 };

enum op_imm_kind_t : uint8_t { ADDI = 0, SLLI, SLTI, SLTIU, XORI, SR_I, ORI, ANDI };
enum load_kind_t   : uint8_t { LB   = 0, LH  , LW   , LBU , LHU } ;
enum save_kind_t   : uint8_t { SB   = 0, SH  , SW };
enum branch_kind_t : uint8_t { BEQ  = 0, BNE , BLT  , BGE , BLTU, BGEU }; 
enum op_kind_t     : uint8_t { A_S  = 0, SLL , SLT  , SLTU, XOR , SR_  , OR, AND  };

namespace myCPU {

//using instruction_type_t = struct {
//	instr_frmt_t frmt_;
//	union {
//		op_imm_kind_t op_imm;
//		load_kind_t   load;
//		save_kind_t   save;
//		branch_kind_t branch;
//		op_kind_t     op;
//	} u;
//};

class instruction_t {
public:
	static const size_t instruction_size_ = 4;
	instruction_t() = default;
	instruction_t(EncodedInstr_t encodedInstr, RegisterValue_t pc);

	size_t getRD_ID () const;
	size_t getRS1_ID() const;
	size_t getRS2_ID() const;
	RegisterValue_t getIMM () const;
	void (*executor_)(hart_t *, const instruction_t &);

private:
	size_t rd_ID_, rs1_ID_, rs2_ID_;
	RegisterValue_t imm_ = 0;
};

void print_encInstr(std::ostream &stream, const myCPU::EncodedInstr_t &encodedInstr);

void executeSUB  (hart_t *hart, const instruction_t &instruction);
void executeSRA  (hart_t *hart, const instruction_t &instruction);
void executeADD  (hart_t *hart, const instruction_t &instruction);
void executeSLL  (hart_t *hart, const instruction_t &instruction);
void executeSLT  (hart_t *hart, const instruction_t &instruction);
void executeSLTU (hart_t *hart, const instruction_t &instruction);
void executeXOR  (hart_t *hart, const instruction_t &instruction);
void executeSRL  (hart_t *hart, const instruction_t &instruction);
void executeOR   (hart_t *hart, const instruction_t &instruction);
void executeAND  (hart_t *hart, const instruction_t &instruction);

void executeADDI  (hart_t *hart, const instruction_t &instruction);
void executeSLLI  (hart_t *hart, const instruction_t &instruction);
void executeSLTI  (hart_t *hart, const instruction_t &instruction);
void executeSLTIU (hart_t *hart, const instruction_t &instruction);
void executeXORI  (hart_t *hart, const instruction_t &instruction);
void executeSRAI  (hart_t *hart, const instruction_t &instruction);
void executeSRLI  (hart_t *hart, const instruction_t &instruction);
void executeORI   (hart_t *hart, const instruction_t &instruction);
void executeANDI  (hart_t *hart, const instruction_t &instruction);

void executeLB  (hart_t *hart, const instruction_t &instruction);
void executeLH  (hart_t *hart, const instruction_t &instruction);
void executeLW  (hart_t *hart, const instruction_t &instruction);
void executeLBU (hart_t *hart, const instruction_t &instruction);
void executeLHU (hart_t *hart, const instruction_t &instruction);

void executeSB (hart_t *hart, const instruction_t &instruction);
void executeSH (hart_t *hart, const instruction_t &instruction);
void executeSW (hart_t *hart, const instruction_t &instruction);

void executeBEQ  (hart_t *hart, const instruction_t &instruction);
void executeBNE  (hart_t *hart, const instruction_t &instruction);
void executeBLT  (hart_t *hart, const instruction_t &instruction);
void executeBGE  (hart_t *hart, const instruction_t &instruction);
void executeBLTU (hart_t *hart, const instruction_t &instruction);
void executeBGEU (hart_t *hart, const instruction_t &instruction);

void executeJAL   (hart_t *hart, const instruction_t &instruction);
void executeJALR  (hart_t *hart, const instruction_t &instruction);
void executeAUIPC (hart_t *hart, const instruction_t &instruction);
void executeLUI   (hart_t *hart, const instruction_t &instruction);

}