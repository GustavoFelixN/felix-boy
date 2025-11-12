#include "cpu.h"

int main(int argc, char **argv) {
    CPU cpu = CPU();

    cpu.regs.a = 0x00;
    cpu.regs.setHL(0x03);

    cpu.memory[0x00] = 0x2A;
    cpu.memory[0x03] = 0xF0;

    cpu.regs.showRegisters();
    cpu.showMemory();

    cpu.runTillNOP();
}
