#include <iostream>
#include <cstdint>
#include <bitset>

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

    void showRegisters() {
        std::bitset<8> ba(a);
        std::bitset<8> bf(f);
        std::bitset<8> bb(b);
        std::bitset<8> bc(c);
        std::bitset<8> bd(d);
        std::bitset<8> be(e);
        std::bitset<8> bh(h);
        std::bitset<8> bl(l);

        std::cout << "|     A    |     F    |     B    |     C    |     D    |     E    |     H    |     L    |" << std::endl;
        std::cout << "| " << ba << " | " << bf << " | " << bb << " | " << bc << " | " << bd << " | " << be << " | " << bh << " | " << bl << " |" << std::endl;
    }
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

    void writeReg8(Reg8 reg, uint8_t value) {
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

    void initializeInstructions() { 
        instructions[0x00] = &CPU::NOP; 
        instructions[0x41] = &CPU::LD_B_C;
    }

    void NOP();
    void LD_r_r(Reg8 dest, Reg8 src);
    void LD_B_C();
};

void CPU::NOP() { std::cout << "NOP OP" << std::endl; }
void CPU::LD_r_r(Reg8 dest, Reg8 src) {
    uint8_t value = readReg8(src);
    writeReg8(dest, value);
}

void CPU::LD_B_C() { LD_r_r(Reg8::REG_B, Reg8::REG_C); }


//--------------------------------------------------------------------------------------------------------------------------//


int main(int argc, char **argv) {
    CPU cpu = CPU();

    cpu.regs.c = 11;

    cpu.regs.showRegisters();
    cpu.memory[0] = 0x41;
    cpu.execute();
    cpu.regs.showRegisters();
}
