#include "misc.h"
#include "../cpu.h"

namespace instr {
    void NOP(CPU& cpu, uint8_t opcode) { (void) opcode; cpu.NOP(); }
    void STOP(CPU& cpu, uint8_t opcode) { (void) opcode; cpu.STOP(); }
    void HALT(CPU& cpu, uint8_t opcode) { (void) opcode; cpu.HALT(); }
    void PREFIX(CPU& cpu, uint8_t opcode) { (void) opcode; cpu.PREFIX(); }
    void DI(CPU& cpu, uint8_t opcode) { (void) opcode; cpu.DI(); }
    void EI(CPU& cpu, uint8_t opcode) { (void) opcode; cpu.EI(); }
}
