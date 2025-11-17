#pragma once
#include <cstdint>

class CPU;

namespace instr {
    void LD_r_r(CPU& cpu, uint8_t opcode);
    void LD_r_n(CPU& cpu, uint8_t opcode);
    void LD_a_mem(CPU& cpu, uint8_t opcode);
    void LD_mem_a(CPU& cpu, uint8_t opcode);
    void LD_a_nn(CPU& cpu, uint8_t opcode);
    void LD_nn_a(CPU& cpu, uint8_t opcode);
    void LDH_a_c(CPU& cpu, uint8_t opcode);
    void LDH_c_a(CPU& cpu, uint8_t opcode);
    void LDH_a_n(CPU& cpu, uint8_t opcode);
    void LDH_n_a(CPU& cpu, uint8_t opcode);
    void LD_a_hl_decrement(CPU& cpu, uint8_t opcode);
    void LD_hl_a_decrement(CPU& cpu, uint8_t opcode);
    void LD_a_hl_increment(CPU& cpu, uint8_t opcode);
    void LD_hl_a_increment(CPU& cpu, uint8_t opcode);
    void LD_rr_nn(CPU& cpu, uint8_t opcode);
    void LD_nn_rr(CPU& cpu, uint8_t opcode);
    void LD_nn_sp(CPU& cpu, uint8_t opcode);
    void LD_sp_hl(CPU& cpu, uint8_t opcode);
    void PUSH(CPU& cpu, uint8_t opcode);
    void POP(CPU& cpu, uint8_t opcode);
    void LD_hl_sp_e(CPU& cpu, uint8_t opcode);
}
