#pragma once
#include <cstdint>

class CPU;

namespace instr {
    void NOP(CPU& cpu, uint8_t opcode);
    void STOP(CPU& cpu, uint8_t opcode);
    void HALT(CPU& cpu, uint8_t opcode);
    void PREFIX(CPU& cpu, uint8_t opcode);
    void DI(CPU& cpu, uint8_t opcode);
    void EI(CPU& cpu, uint8_t opcode);
}
