#include "opcode.h"
#include "misc.h"
#include "load.h"
std::array<InstrFn, 256> OPCODE_TABLE;

void initOpcodes() {
    OPCODE_TABLE[0x00] = instr::NOP;
    OPCODE_TABLE[0x10] = instr::STOP;
    OPCODE_TABLE[0x76] = instr::HALT;
    OPCODE_TABLE[0xCB] = instr::PREFIX;
    OPCODE_TABLE[0xF3] = instr::DI;
    OPCODE_TABLE[0xFB] = instr::EI;

    for(uint8_t op = 0x06; op <= 0x3E; op += 8)
        OPCODE_TABLE[op] = instr::LD_r_n;

    OPCODE_TABLE[0x0A] = instr::LD_a_mem;
    OPCODE_TABLE[0x1A] = instr::LD_a_mem;

    OPCODE_TABLE[0x02] = instr::LD_mem_a;
    OPCODE_TABLE[0x12] = instr::LD_mem_a;

    OPCODE_TABLE[0xFA] = instr::LD_a_nn;
    OPCODE_TABLE[0xEA] = instr::LD_nn_a;

    OPCODE_TABLE[0xF2] = instr::LDH_a_c;
    OPCODE_TABLE[0xE2] = instr::LDH_c_a;
    OPCODE_TABLE[0xF0] = instr::LDH_a_n;
    OPCODE_TABLE[0xE0] = instr::LDH_n_a;

    OPCODE_TABLE[0x3A] = instr::LD_a_hl_decrement;
    OPCODE_TABLE[0x32] = instr::LD_hl_a_decrement;
    OPCODE_TABLE[0x2A] = instr::LD_a_hl_increment;
    OPCODE_TABLE[0x22] = instr::LD_hl_a_increment;

    OPCODE_TABLE[0x01] = instr::LD_rr_nn;
    OPCODE_TABLE[0x11] = instr::LD_rr_nn;
    OPCODE_TABLE[0x21] = instr::LD_rr_nn;
    OPCODE_TABLE[0x31] = instr::LD_rr_nn;

    OPCODE_TABLE[0x08] = instr::LD_nn_sp;
    OPCODE_TABLE[0xF9] = instr::LD_sp_hl;

    OPCODE_TABLE[0xF8] = instr::LD_hl_sp_e;
}
