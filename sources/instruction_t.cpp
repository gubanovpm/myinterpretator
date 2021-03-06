#include "../include/instruction_t.hpp"
#include <iostream>

myCPU::instruction_t::instruction_t(myCPU::EncodedInstr_t encodedInstr, myCPU::RegisterValue_t pc) {
	unsigned frmt_ = getBits(0, 6, encodedInstr);
	switch (frmt_) {
		case OP: {
			unsigned funct3_ = getBits(12, 14, encodedInstr);
			unsigned funct7_ = getBits(25, 31, encodedInstr);
			rd_ID_  = getBits( 7, 11, encodedInstr);
			rs1_ID_ = getBits(15, 19, encodedInstr);
			rs2_ID_ = getBits(20, 24, encodedInstr);

			if (funct7_ == 0b0100000)
				if     (funct3_ == A_S) executor_ = myCPU::executeSUB;
				else if(funct3_ == SR_) executor_ = myCPU::executeSRA;
				else {std::cout << "unknown operation" << std::endl; abort(); }

			else if (funct7_ == 0b0000000)
				switch(funct3_) {
					case A_S : executor_ = myCPU::executeADD  ; break;
					case SLL : executor_ = myCPU::executeSLL  ; break;
					case SLT : executor_ = myCPU::executeSLT  ; break;
					case SLTU: executor_ = myCPU::executeSLTU ; break;
					case XOR : executor_ = myCPU::executeXOR  ; break;
					case SR_ : executor_ = myCPU::executeSRL  ; break;
					case OR  : executor_ = myCPU::executeOR   ; break;
					case AND : executor_ = myCPU::executeAND  ; break;
				}
			else {std::cout << "unknown operation" << std::endl; abort(); }

			break;
		}
		case OP_IMM: {
			imm_    = getBits(20, 31, encodedInstr);
			rs1_ID_ = getBits(15, 19, encodedInstr);
			rd_ID_  = getBits( 7, 11, encodedInstr);
			unsigned funct3_ = getBits(12, 14, encodedInstr);
			unsigned funct7_ = getBits(25, 31, encodedInstr);

			switch(funct3_) {
				case ADDI : executor_ = myCPU::executeADDI  ; break;
				case SLLI : executor_ = myCPU::executeSLLI  ; break;
				case SLTI : executor_ = myCPU::executeSLTI  ; break;
				case SLTIU: executor_ = myCPU::executeSLTIU ; break;
				case XORI : executor_ = myCPU::executeXORI  ; break;
				case SR_I : { 
					if        (funct7_ == 0b0100000) executor_ = myCPU::executeSRAI ;
					else if   (funct7_ == 0b0000000) executor_ = myCPU::executeSRLI ;
					     else {std::cout << "unknown operation" << std::endl; abort(); }
					break; }
				case ORI  : executor_ = myCPU::executeORI   ; break;
				case ANDI : executor_ = myCPU::executeANDI  ; break;
				
				default: {std::cout << "unknown operation" << std::endl; abort(); }
			}

			break;
		}
		case LOAD: {
			imm_    = getBits(20, 31, encodedInstr);
			rs1_ID_ = getBits(15, 19, encodedInstr);
			rd_ID_  = getBits( 7, 11, encodedInstr);
			unsigned funct3_ = getBits(12, 14, encodedInstr);

			switch (funct3_) {
				case LB  : executor_ = myCPU::executeLB  ; break;
				case LH  : executor_ = myCPU::executeLH  ; break;
				case LW  : executor_ = myCPU::executeLW  ; break;
				case LBU : executor_ = myCPU::executeLBU ; break;
				case LHU : executor_ = myCPU::executeLHU ; break;

				default: {std::cout << "unknown operation" << std::endl; abort(); }
			}

			break;
		}
		case SAVE: {
			imm_    = (getBits(25, 31, encodedInstr) << 4) | (getBits( 7, 11, encodedInstr));
			rs1_ID_ = getBits(15, 19, encodedInstr);
			rs2_ID_ = getBits(20, 24, encodedInstr);
			unsigned funct3_ = getBits(12, 14, encodedInstr);

			switch (funct3_) {
				case SB : executor_ = myCPU::executeSB ; break;
				case SH : executor_ = myCPU::executeSH ; break;
				case SW : executor_ = myCPU::executeSW ; break;

				default: {std::cout << "unknown operation" << std::endl; abort(); }
			}

			break;
		}
		case BRANCH: {
			imm_    = (getBits(31, 31, encodedInstr) << 12) | (getBits( 7,  7, encodedInstr) << 11) | 
			          (getBits(25, 30, encodedInstr) << 5 ) | (getBits( 8, 11, encodedInstr) << 1 ) ;
			if ((imm_ & (1 << 12)) != 0) imm_ |= 0xfffff000;
			//sprintf("%d + %d\n", imm_, pc);
			imm_ = pc + (int)(imm_);
			rs1_ID_ = getBits(15, 19, encodedInstr);
			rs2_ID_ = getBits(20, 24, encodedInstr);
			unsigned funct3_ = getBits(12, 14, encodedInstr);

			switch (funct3_) {
				case BEQ : executor_ = myCPU::executeBEQ  ; break;
				case BNE : executor_ = myCPU::executeBNE  ; break;
				case BLT : executor_ = myCPU::executeBLT  ; break;
				case BGE : executor_ = myCPU::executeBGE  ; break;
				case BLTU: executor_ = myCPU::executeBLTU ; break;
				case BGEU: executor_ = myCPU::executeBGEU ; break;

				default: {std::cout << "unknown operation" << std::endl; abort(); }
			}
			break;
		}
		case JAL: {
			rd_ID_ = getBits( 7, 11, encodedInstr);
			imm_   = (getBits(31, 31, encodedInstr) << 20) | (getBits(12, 19, encodedInstr) << 12) |
			         (getBits(20, 20, encodedInstr) << 11) | (getBits(21, 30, encodedInstr) << 1 ) ;
			if ((imm_ & (1 << 20)) != 0) imm_ |= 0xfff00000;
			imm_ = pc + int(imm_);
			executor_ = myCPU::executeJAL ;

			break;
		}
		case JALR: {
			imm_    = getBits(20, 31, encodedInstr);
			rs1_ID_ = getBits(15, 19, encodedInstr);
			rd_ID_  = getBits( 7, 11, encodedInstr);
			executor_ = myCPU::executeJALR ; 

			break;
		}
		case AUIPC: {
			imm_   = getBits(12, 31, encodedInstr);
			rd_ID_ = getBits( 7, 11, encodedInstr);
			imm_ += pc;
			executor_ = myCPU::executeAUIPC;

			break;
		}
		case LUI: {
			imm_   = getBits(12, 31, encodedInstr);
			rd_ID_ = getBits( 7, 11, encodedInstr);
			imm_ += pc;
			executor_ = myCPU::executeLUI;

			break;
		}
		default: { std::cout << "unknow format" << std::endl; abort(); }
	}
}

size_t myCPU::instruction_t::getRD_ID() const {
	return rd_ID_;
}
size_t myCPU::instruction_t::getRS1_ID() const {
	return rs1_ID_;
}
size_t myCPU::instruction_t::getRS2_ID() const {
	return rs2_ID_;
}
myCPU::RegisterValue_t myCPU::instruction_t::getIMM() const {
	return imm_;
}

void myCPU::executeSUB  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	int32_t rs1 = hart->getRegister(instruction.getRS1_ID());
	int32_t rs2 = hart->getRegister(instruction.getRS2_ID());
	auto rd_id = instruction.getRD_ID();
	hart->setRegister(rd_id, rs1 - rs2);
}
void myCPU::executeSRA  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	int32_t  rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS1_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 >> rs2);
}
void myCPU::executeADD  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 + rs2);
}
void myCPU::executeSLL  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 << rs2);
}
void myCPU::executeSLT  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	int32_t rs1 = hart->getRegister(instruction.getRS1_ID());
	int32_t rs2 = hart->getRegister(instruction.getRS2_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, (rs1 < rs2) ? 1 : 0);
}
void myCPU::executeSLTU (myCPU::hart_t *hart, const myCPU::instruction_t &instruction){
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, (rs1 < rs2) ? 1 : 0);
}
void myCPU::executeXOR  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 ^ rs2);
}
void myCPU::executeSRL  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 >> rs2);
}
void myCPU::executeOR   (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 | rs2);
}
void myCPU::executeAND  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 & rs2);
}

void myCPU::executeADDI  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM();
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 + imm);
}
void myCPU::executeSLLI  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM() & 0b11111;
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 << imm);
}
void myCPU::executeSLTI  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	int32_t imm   = instruction.getIMM();
	int32_t rs1   = hart->getRegister(instruction.getRS1_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, (rs1 < imm) ? 1 : 0);
}
void myCPU::executeSLTIU (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM();
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, (rs1 < imm) ? 1 : 0);
}
void myCPU::executeXORI  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM();
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 ^ imm);
}
void myCPU::executeSRAI  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM() & 0b11111;
	int32_t rs1   = hart->getRegister(instruction.getRS1_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 >> imm);
}
void myCPU::executeSRLI  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM() & 0b11111;
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 >> imm);
}
void myCPU::executeORI   (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM();
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 | imm);
}
void myCPU::executeANDI  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM();
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, rs1 & imm);
}

void myCPU::executeLB  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM();
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	uint32_t res; hart->getOpMemory()->read(rs1 + imm, 1, res);
	if ((res >> 7) & 1) res |= 0xffffff00;
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, res);
}
void myCPU::executeLH  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM();
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	uint32_t res; hart->getOpMemory()->read(rs1 + imm, 2, res);
	if ((res >> 15) & 1) res |= 0xffff0000;
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, res);
}
void myCPU::executeLW  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM();
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	uint32_t res; hart->getOpMemory()->read(rs1 + imm, 4, res);
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, res);
}
void myCPU::executeLBU (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM();
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	uint32_t res; hart->getOpMemory()->read(rs1 + imm, 1, res);
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, res);
}
void myCPU::executeLHU (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto imm   = instruction.getIMM();
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	uint32_t res; hart->getOpMemory()->read(rs1 + imm, 2, res);
	auto rd_id = instruction.getRD_ID();
	if (rd_id != 0) hart->setRegister(rd_id, res);
}

void myCPU::executeSB (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto imm   = instruction.getIMM();
	hart->getOpMemory()->write(rs1 + imm, 1, rs2);
}
void myCPU::executeSH (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto imm   = instruction.getIMM();
	hart->getOpMemory()->write(rs1 + imm, 2, rs2);
}
void myCPU::executeSW (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto imm   = instruction.getIMM();
	hart->getOpMemory()->write(rs1 + imm, 4, rs2);
}

void myCPU::executeBEQ  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto imm = instruction.getIMM();
	if (rs1 == rs2) hart->setNextPC(imm); 
}
void myCPU::executeBNE  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto imm = instruction.getIMM();
	if (rs1 != rs2) hart->setNextPC(imm);
}
void myCPU::executeBLT  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	int32_t rs1  = hart->getRegister(instruction.getRS1_ID());
	int32_t rs2  = hart->getRegister(instruction.getRS2_ID());
	auto imm = instruction.getIMM();
	//printf("!!!%x\n", imm);
	if (rs1 < rs2) hart->setNextPC(imm);
}
void myCPU::executeBGE  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	int32_t rs1  = hart->getRegister(instruction.getRS1_ID());
	int32_t rs2  = hart->getRegister(instruction.getRS2_ID());
	auto imm = instruction.getIMM();
	if (rs1 >= rs2) hart->setNextPC(imm);
}
void myCPU::executeBLTU (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto imm   = instruction.getIMM();
	if (rs1 < rs2) hart->setNextPC(imm);
}
void myCPU::executeBGEU (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rs1 = hart->getRegister(instruction.getRS1_ID());
	auto rs2 = hart->getRegister(instruction.getRS2_ID());
	auto imm   = instruction.getIMM();
	if (rs1 >= rs2) hart->setNextPC(imm);
}

void myCPU::executeJAL   (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rd_id = instruction.getRD_ID();
	auto imm   = instruction.getIMM();
	if (rd_id != 0) hart->setRegister(rd_id, hart->getPCRegister() + instruction.instruction_size_);
	hart->setNextPC(imm);
}
void myCPU::executeJALR  (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rd_id = instruction.getRD_ID();
	auto imm   = instruction.getIMM();
	if ((imm & (1 << 12)) != 0) imm |= 0xfffff000;
	auto rs1   = hart->getRegister(instruction.getRS1_ID());
	if (rd_id != 0) hart->setRegister(rd_id, hart->getPCRegister() + instruction.instruction_size_);
	hart->setNextPC((int)imm + rs1);
}
void myCPU::executeAUIPC (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rd_id = instruction.getRD_ID();
	auto imm   = instruction.getIMM();
	imm += hart->getPCRegister();
	hart->setRegister(rd_id, imm);
}
void myCPU::executeLUI   (myCPU::hart_t *hart, const myCPU::instruction_t &instruction) {
	auto rd_id = instruction.getRD_ID();
	auto imm   = instruction.getIMM();
	hart->setRegister(rd_id, imm);
}

void myCPU::print_encInstr(std::ostream &stream, const myCPU::EncodedInstr_t &encodedInstr) {
	unsigned frmt_ = getBits(0, 6, encodedInstr);
	//printf("frmt cmnd = %x", encodedInstr);
	size_t rd_ID_, rs1_ID_, rs2_ID_, imm_;
	switch (frmt_) {
		case OP: {
			unsigned funct3_ = getBits(12, 14, encodedInstr);
			unsigned funct7_ = getBits(25, 31, encodedInstr);
			rd_ID_  = getBits( 7, 11, encodedInstr);
			rs1_ID_ = getBits(15, 19, encodedInstr);
			rs2_ID_ = getBits(20, 24, encodedInstr);

			if (funct7_ == 0b0100000)
				if     (funct3_ == A_S) stream << "SUB " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_);
				else if(funct3_ == SR_) stream << "SRA " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_);
				else {std::cout << "unknown operation" << std::endl; abort(); }

			else if (funct7_ == 0b0000000)
				switch(funct3_) {
					
					case A_S : stream << "ADD " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_); break;
					case SLL : stream << "SLL " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_); break;
					case SLT : stream << "SLT " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_); break;
					case SLTU: stream << "SLTU "<< myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_); break;
					case XOR : stream << "XOR " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_); break;
					case SR_ : stream << "SRL " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_); break;
					case OR  : stream << "OR "  << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_); break;
					case AND : stream << "AND " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_); break;
				}
			else {std::cout << "unknown operation" << std::endl; abort(); }

			break;
		}
		case OP_IMM: {
			imm_    = getBits(20, 31, encodedInstr);
			rs1_ID_ = getBits(15, 19, encodedInstr);
			rd_ID_  = getBits( 7, 11, encodedInstr);
			unsigned funct3_ = getBits(12, 14, encodedInstr);
			unsigned funct7_ = getBits(25, 31, encodedInstr);

			switch(funct3_) {
				case ADDI : stream << "ADDI "  << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ; break;
				case SLLI : stream << "SLLI "  << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ; break;
				case SLTI : stream << "SLTI "  << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ; break;
				case SLTIU: stream << "SLTIU " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ; break;
				case XORI : stream << "XORI "  << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ; break;
				case SR_I : { 
					if        (funct7_ == 0b0100000) stream << "SRAI " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ;
					else if   (funct7_ == 0b0000000) stream << "SRLI " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ;
					     else {std::cout << "unknown operation" << std::endl; abort(); }
					break; }
				case ORI  : stream << "ORI "  << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ; break;
				case ANDI : stream << "ANDI " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ; break;
				
				default: {std::cout << "unknown operation" << std::endl; abort(); }
			}

			break;
		}
		case LOAD: {
			imm_    = getBits(20, 31, encodedInstr);
			rs1_ID_ = getBits(15, 19, encodedInstr);
			rd_ID_  = getBits( 7, 11, encodedInstr);
			unsigned funct3_ = getBits(12, 14, encodedInstr);

			switch (funct3_) {
				case LB  : stream << "LB "  << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ; break;
				case LH  : stream << "LH "  << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ; break;
				case LW  : stream << "LW "  << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ; break;
				case LBU : stream << "LBU " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ; break;
				case LHU : stream << "LHU " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_  ; break;

				default: {std::cout << "unknown operation" << std::endl; abort(); }
			}

			break;
		}
		case SAVE: {
			imm_    = (getBits(25, 31, encodedInstr) << 4) | (getBits( 7, 11, encodedInstr));
			rs1_ID_ = getBits(15, 19, encodedInstr);
			rs2_ID_ = getBits(20, 24, encodedInstr);
			unsigned funct3_ = getBits(12, 14, encodedInstr);

			switch (funct3_) {
				case SB : stream << "SB " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_) << ", " << imm_  ; break;
				case SH : stream << "SH " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_) << ", " << imm_  ; break;
				case SW : stream << "SW " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_) << ", " << imm_  ; break;

				default: {std::cout << "unknown operation" << std::endl; abort(); }
			}

			break;
		}
		case BRANCH: {
			imm_    = (getBits(31, 31, encodedInstr) << 12) | (getBits( 7,  7, encodedInstr) << 11) | 
			          (getBits(25, 30, encodedInstr) << 5 ) | (getBits( 8, 11, encodedInstr) << 1 ) ;
			if ((imm_ & (1 << 11)) != 0) {
				imm_ |= 0xfffff000;
			}
			rs1_ID_ = getBits(15, 19, encodedInstr);
			rs2_ID_ = getBits(20, 24, encodedInstr);
			unsigned funct3_ = getBits(12, 14, encodedInstr);

			switch (funct3_) {
				case BEQ : stream << "BEQ " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_) << ", " << (int)imm_ << " + pc" ; break;
				case BNE : stream << "BNE " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_) << ", " << (int)imm_ << " + pc" ; break;
				case BLT : stream << "BLT " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_) << ", " << (int)imm_ << " + pc" ; break;
				case BGE : stream << "BGE " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_) << ", " << (int)imm_ << " + pc" ; break;
				case BLTU: stream << "BLTU " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_) << ", " << (int)imm_ << " + pc" ; break;
				case BGEU: stream << "BGEU " << myCPU::reg_name(rs1_ID_) << ", " << myCPU::reg_name(rs2_ID_) << ", " << (int)imm_ << " + pc" ; break;

				default: {std::cout << "unknown operation" << std::endl; abort(); }
			}
			break;
		}
		case JAL: {
			rd_ID_ = getBits( 7, 11, encodedInstr);
			imm_   = (getBits(31, 31, encodedInstr) << 19) | (getBits(12, 19, encodedInstr) << 11) |
			         (getBits(20, 20, encodedInstr) << 10) | (getBits(21, 30, encodedInstr) << 1 ) ;
			stream << "JAL " << myCPU::reg_name(rd_ID_) << ", " << imm_ << " + pc" ;

			break;
		}
		case JALR: {
			imm_    = getBits(20, 31, encodedInstr);
			rs1_ID_ = getBits(15, 19, encodedInstr);
			rd_ID_  = getBits( 7, 11, encodedInstr);
			stream << "JALR " << myCPU::reg_name(rd_ID_) << ", " << myCPU::reg_name(rs1_ID_) << ", " << imm_ ;//<< " + pc" ;
			break;
		}
		case AUIPC: {
			imm_   = getBits(12, 31, encodedInstr);
			rd_ID_ = getBits( 7, 11, encodedInstr);
			stream << "AUIPC " << myCPU::reg_name(rd_ID_) << ", " << imm_ << " + pc" ;

			break;
		}
		case LUI: {
			imm_   = getBits(12, 31, encodedInstr);
			rd_ID_ = getBits( 7, 11, encodedInstr);
			stream << "LUI " << myCPU::reg_name(rd_ID_) << ", " << imm_ << " + pc" ;

			break;
		}
		default: { std::cout << "unknow format" << std::endl; abort(); }
	}
}
