#pragma once
#include <iomanip>
#include "registers.h"

enum Reg8 {
    REG_B = 0,
    REG_C,
    REG_D,
    REG_E,
    REG_H,
    REG_L,
    REG_HL_MEM,
    REG_A
};

enum Reg16 {
    REG_BC = 0,
    REG_DE,
    REG_HL,
    REG_SP,
};

enum Reg16Stk {
    REG_BC_STK = 0,
    REG_DE_STK,
    REG_HL_STK,
    REG_AF_STK,
};

class CPU {
public:
    Registers regs;
    uint8_t memory[65536] = {0};

    uint8_t readReg8(Reg8 reg);
    void writeReg8(Reg8 reg, uint8_t value);

    uint16_t readReg16(Reg16 reg);
    uint8_t readReg16Mem(Reg16 reg);
    void writeReg16(Reg16 reg, uint16_t value);
    void writeReg16Mem(Reg16 reg, uint8_t value);

    uint8_t fetch();
    uint16_t fetch16();

    void execute(uint8_t opcode);
    void executeNext();
    void runTillNOP(bool showRegisters = true, bool showMem=true);

    void showMemory(uint16_t window = 5);


private:
    //--- Misc/Control instructions ---//
    void NOP() { std::cout << "NOP OP" << std::endl; }
    void STOP() {  std::cout << "STOP OP" << std::endl;  }
    void HALT() {  std::cout << "HALT OP" << std::endl;  }
    void PREFIX() {  std::cout << "PREFIX OP" << std::endl;  }
    void DI() {  std::cout << "DI OP" << std::endl;  }
    void EI() {  std::cout << "EI OP" << std::endl;  }


    //--- Load instructions ---//
    void LD_r_r(Reg8 dest, Reg8 src);
    void LD_r_n(Reg8 dest);
    void LD_r_hl(Reg8 dest);
    void LD_hl_r(Reg8 src);
    void LD_hl_n();
    void LD_a_mem(Reg16 src);
    void LD_mem_a(Reg16 dest);
    void LD_a_nn();
    void LD_nn_a();
    void LDH_a_c();
    void LDH_c_a();
    void LDH_a_n();
    void LDH_n_a();
    void LD_a_hl_decrement();
    void LD_hl_a_decrement();
    void LD_a_hl_increment();
    void LD_hl_a_increment();
    void LD_rr_nn(Reg16 dest);
    void LD_nn_rr(Reg16 src);
};
