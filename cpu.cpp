#include <iostream>
#include <cstdint>

class Registers {
public:
    uint8_t a, f, b, c, d, e, h, l;
    uint16_t sp, pc;

    enum Flag : uint8_t {
        Z = 1 << 7,
        N = 1 << 6,
        H = 1 << 5,
        C = 1 << 4,
    };

    uint16_t getAF() const { return (a << 8) | f; }
    uint16_t getBC() const { return (b << 8) | c; }
    uint16_t getDE() const { return (d << 8) | e; }
    uint16_t getHL() const { return (h << 8) | l; }

    void setAF(uint16_t val) { a = val >> 8; f = val & 0xF0; }
    void setBC(uint16_t val) { b = val >> 8; c = val & 0xFF; }
    void setDE(uint16_t val) { d = val >> 8; e = val & 0xFF; }
    void setHL(uint16_t val) { h = val >> 8; l = val & 0xFF; }

    bool getFlag(Flag flag) const { return (f & flag) != 0; }
    void setFlag(Flag flag, bool value) { value ? f |= flag : f &= ~flag; }
    void clearFlags() { f = 0; }
};

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

class CPU {
public:
    CPU() { initializeInstructions(); }

    Registers regs;
    using Instruction = void (CPU::*)();
    uint8_t memory[65536] = {0};

    uint8_t readReg8(Reg8 reg) {
        switch (reg) {
            case REG_B: return regs.b;
            case REG_C: return regs.c;
            case REG_D: return regs.d;
            case REG_E: return regs.e;
            case REG_H: return regs.h;
            case REG_L: return regs.l;
            case REG_A: return regs.a;
            case REG_HL_MEM: return memory[regs.getHL()];
        }
        return 0;
    }

    void readReg8(Reg8 reg, uint8_t value) {
        switch (reg) {
            case REG_B: regs.b = value; break;
            case REG_C: regs.c = value; break;
            case REG_D: regs.d = value; break;
            case REG_E: regs.e = value; break;
            case REG_H: regs.h = value; break;
            case REG_L: regs.l = value; break;
            case REG_A: regs.a = value; break;
            case REG_HL_MEM: memory[regs.getHL()] = value; break;
        }
    }

    uint8_t fetch() {
        return memory[regs.pc++];
    }

    void execute() {
        uint8_t opcode = fetch();
        Instruction inst = instructions[opcode];
        (this->*inst)();
    }

private:
    Instruction instructions[256];
    void initializeInstructions();

    void NOP();
};

void CPU::initializeInstructions() { instructions[0x00] = &CPU::NOP; }

void CPU::NOP() { std::cout << "NOP OP" << std::endl; }


//--------------------------------------------------------------------------------------------------------------------------//


int main(int argc, char **argv) {
    CPU cpu = CPU();
    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();
    cpu.execute();

    std::cout << cpu.regs.pc << std::endl;
}
