#include "opcode.h"
#include "load.h"
#include "../cpu.h"

std::array<InstrFn, 256> OPCODE_TABLE;

void initOpcodes() {
    for(uint8_t op = 0x06; op <= 0x3E; op += 8)
        OPCODE_TABLE[op] = instr::LD_r_n;

    // LD A,(BC) e A,(DE)
    OPCODE_TABLE[0x0A] = instr::LD_A_mem_rr;
    OPCODE_TABLE[0x1A] = instr::LD_A_mem_rr;

    // (BC)=A e (DE)=A
    OPCODE_TABLE[0x02] = instr::LD_mem_rr_A;
    OPCODE_TABLE[0x12] = instr::LD_mem_rr_A;

    // LD A,(nn) / LD (nn),A
    OPCODE_TABLE[0xFA] = instr::LD_A_nn;
    OPCODE_TABLE[0xEA] = instr::LD_nn_A;

    // LDH
    OPCODE_TABLE[0xF2] = instr::LDH_A_C;
    OPCODE_TABLE[0xE2] = instr::LDH_C_A;
    OPCODE_TABLE[0xF0] = instr::LDH_A_n;
    OPCODE_TABLE[0xE0] = instr::LDH_n_A;

    // HL±
    OPCODE_TABLE[0x3A] = instr::LD_A_HL_dec;
    OPCODE_TABLE[0x32] = instr::LD_HL_A_dec;
    OPCODE_TABLE[0x2A] = instr::LD_A_HL_inc;
    OPCODE_TABLE[0x22] = instr::LD_HL_A_inc;

    // LD rr,nn
    OPCODE_TABLE[0x01] = instr::LD_rr_nn;
    OPCODE_TABLE[0x11] = instr::LD_rr_nn;
    OPCODE_TABLE[0x21] = instr::LD_rr_nn;
    OPCODE_TABLE[0x31] = instr::LD_rr_nn;

    OPCODE_TABLE[0x08] = instr::LD_nn_SP;
    OPCODE_TABLE[0xF9] = instr::LD_SP_HL;

    OPCODE_TABLE[0xF8] = instr::LD_HL_SP_e;
}
