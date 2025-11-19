#include "load.h"
#include "../cpu.h"

namespace instr { 
    void LD_r_r(CPU& cpu, uint8_t opcode) { 
        uint8_t dest = (opcode >> 3) & 0b111;
        uint8_t src = opcode & 0b111;
        cpu.LD_r_r(static_cast<Reg8>(dest), static_cast<Reg8>(src)); 
    }
    void LD_r_n(CPU& cpu, uint8_t opcode) {
        uint8_t dest = (opcode >> 3) & 0b111;
        cpu.LD_r_n(static_cast<Reg8>(dest)); 
    }
    void LD_a_mem(CPU& cpu, uint8_t opcode) { 
        if(opcode == 0x0A)
            cpu.LD_a_mem(REG_BC); 
        else 
            cpu.LD_a_mem(REG_DE); 
    }
    void LD_mem_a(CPU& cpu, uint8_t opcode) {
        if(opcode == 0x02)
            cpu.LD_mem_a(REG_BC); 
        else 
            cpu.LD_mem_a(REG_DE); 

    }
    void LD_a_nn(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LD_a_nn(); }
    void LD_nn_a(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LD_nn_a(); }
    void LDH_a_c(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LDH_a_c(); }
    void LDH_c_a(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LDH_c_a(); }
    void LDH_a_n(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LDH_a_n(); }
    void LDH_n_a(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LDH_n_a(); }
    void LD_a_hl_decrement(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LD_a_hl_decrement(); }
    void LD_hl_a_decrement(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LD_hl_a_decrement(); }
    void LD_a_hl_increment(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LD_a_hl_increment(); }
    void LD_hl_a_increment(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LD_hl_a_increment(); }
    void LD_rr_nn(CPU& cpu, uint8_t opcode) { 
        uint8_t dest = (opcode >> 4) & 0x03;
        cpu.LD_rr_nn(static_cast<Reg16>(dest)); 
    }
    void LD_nn_sp(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LD_nn_sp(); }
    void LD_sp_hl(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LD_sp_hl(); }
    void PUSH(CPU& cpu, uint8_t opcode) {
        uint8_t src  = (opcode >> 4) & 0x03;
        cpu.PUSH(static_cast<Reg16>(src)); 
    }
    void POP(CPU& cpu, uint8_t opcode) { 
        uint8_t dest  = (opcode >> 4) & 0x03;
        cpu.POP(static_cast<Reg16>(dest)); 
    }
    void LD_hl_sp_e(CPU& cpu, uint8_t opcode) {(void) opcode; cpu.LD_hl_sp_e(); } 
}
