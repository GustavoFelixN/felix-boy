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
    Registers regs;
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

    void execute(uint8_t opcode) {
        switch(opcode) {
            case 0x00: return NOP();
            case 0x10: return STOP();
            case 0x76: return HALT();
            case 0xCB: return PREFIX();
            case 0xF3: return DI();
            case 0xFB: return EI();
        }

        // Checking immediate before r r for queuing operations in the future
        if ((opcode & 0b11000111) == 0b01000110) {
            uint8_t dest = (opcode >> 3) & 0b111;
            LD_r8_hl(static_cast<Reg8>(dest));
        }

        else if((opcode & 0b11111000) == 0b01110000) {
            uint8_t src = (opcode) & 0b111;
            LD_hl_r8(static_cast<Reg8>(src));
        }

        else if(( opcode & 0b11000000 ) == 0b01000000) {
            uint8_t dest = (opcode >> 3) & 0b111;
            uint8_t src = opcode & 0b111;
            LD_r_r(static_cast<Reg8>(dest), static_cast<Reg8>(src));
        }

        else if(opcode  == 0b00110110) {
            LD_r8_n(REG_HL_MEM);
        }

        else if((opcode & 0b11000111) == 0b00000110) {
            uint8_t dest = (opcode >> 3) & 0b111;
            LD_r8_n(static_cast<Reg8>(dest));
        }

    }
    void executeNext() {
        uint8_t opcode = fetch();
        execute(opcode);
    }

    void showMemory(uint16_t window = 5) {
        uint16_t min = regs.pc < window ? 0 : regs.pc - window;
        uint16_t max = regs.pc + window > 0xFF ? 0xFF : regs.pc + window;

        for(int i = min; i < max; i++) {
            std::bitset<8> value(memory[i]);
            if(i == regs.pc) {
                std::cout << "| pc | " << value << " |" << std::endl;
            } else {
                std::cout << "|    | " << value << " |" << std::endl;
            }
        }
        std::cout << std::endl;
    }

    void runTillNOP(bool showRegisters = true, bool showMem=true) {
        uint8_t opcode = fetch();
        while(opcode) {
            execute(opcode);
            if(showRegisters) regs.showRegisters();
            if(showMem) showMemory();
            opcode = fetch();
        }
    }


private:
    //--- Misc/Control instructions ---//
    void NOP() { std::cout << "NOP OP" << std::endl; }
    void STOP() {  std::cout << "STOP OP" << std::endl;  }
    void HALT() {  std::cout << "HALT OP" << std::endl;  }
    void PREFIX() {  std::cout << "PREFIX OP" << std::endl;  }
    void DI() {  std::cout << "DI OP" << std::endl;  }
    void EI() {  std::cout << "EI OP" << std::endl;  }


    //--- Load instructions ---//
    void LD_r_r(Reg8 dest, Reg8 src) {
        uint8_t value = readReg8(src);
        writeReg8(dest, value);
    }

    void LD_r8_n(Reg8 dest) {
        uint8_t imm = fetch();
        writeReg8(dest, imm);
    }

    void LD_r8_hl(Reg8 dest) {
        uint8_t imm = readReg8(REG_HL_MEM);
        writeReg8(dest, imm);
    }

    void LD_hl_r8(Reg8 src) {
        uint8_t value = readReg8(src);
        writeReg8(REG_HL_MEM, value);
    }

    void LD_hl_n() {
        uint8_t value = fetch();
        writeReg8(REG_HL_MEM, value);
    }

};




//--------------------------------------------------------------------------------------------------------------------------//


int main(int argc, char **argv) {
    CPU cpu = CPU();

    cpu.regs.b = 7;
    cpu.regs.setHL(4);

    cpu.memory[0x00] = 0x36;
    cpu.memory[0x01] = 0xF0;


    cpu.regs.showRegisters();
    cpu.showMemory();

    cpu.runTillNOP();
}
