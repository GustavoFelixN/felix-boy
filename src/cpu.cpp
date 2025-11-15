#include "cpu.h"
#include <cstdint>

uint8_t CPU::readReg8(Reg8 reg) {
    switch (reg) {
        case REG_B: return regs.b; break;
        case REG_C: return regs.c; break;
        case REG_D: return regs.d; break;
        case REG_E: return regs.e; break;
        case REG_H: return regs.h; break;
        case REG_L: return regs.l; break;
        case REG_A: return regs.a; break;
        case REG_HL_MEM: return memory[regs.getHL()]; break;
    }
    return 0;
}

void CPU::writeReg8(Reg8 reg, uint8_t value) {
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

uint16_t CPU::readReg16(Reg16 reg) {
    switch (reg) {
        case REG_BC: return regs.getBC(); break;
        case REG_DE: return regs.getDE(); break;
        case REG_HL: return regs.getHL(); break;
        case REG_SP: return regs.sp; break;
    }
    return 0;
}


uint8_t CPU::readReg16Mem(Reg16 reg) {
    switch (reg) {
        case REG_BC: return memory[regs.getBC()]; break;
        case REG_DE: return memory[regs.getDE()]; break;
        case REG_HL: return memory[regs.getHL()]; break;
        case REG_SP: return memory[regs.sp]; break;
    }
    return 0;
}

void CPU::writeReg16(Reg16 reg, uint16_t value) {
    switch (reg) {
        case REG_BC: regs.setBC(value); break;
        case REG_DE: regs.setDE(value); break;
        case REG_HL: regs.setHL(value); break;
        case REG_SP: regs.sp = value; break;
    }
}

void CPU::writeReg16Mem(Reg16 reg, uint8_t value) {
    switch (reg) {
        case REG_BC: memory[regs.getBC()] = value; break;
        case REG_DE: memory[regs.getDE()] = value; break;
        case REG_HL: memory[regs.getHL()] = value; break;
        case REG_SP: memory[regs.sp] = value; break;
    }
}

uint8_t CPU::fetch() {
    return memory[regs.pc++];
}

uint16_t CPU::fetch16() {
    uint8_t lsb = fetch();
    uint8_t msb = fetch();
    return (msb << 8) | lsb;

}

void CPU::execute(uint8_t opcode) {
    switch(opcode) {
        case 0x00: return NOP();
        case 0x10: return STOP();
        case 0x76: return HALT();
        case 0xCB: return PREFIX();
        case 0xF3: return DI();
        case 0xFB: return EI();
    }

    if(( opcode & 0b11000000 ) == 0b01000000) {
        uint8_t dest = (opcode >> 3) & 0b111;
        uint8_t src = opcode & 0b111;
        LD_r_r(static_cast<Reg8>(dest), static_cast<Reg8>(src));
    }

    else if((opcode & 0b11000111) == 0b00000110) {
        uint8_t dest = (opcode >> 3) & 0b111;
        LD_r_n(static_cast<Reg8>(dest));
    }

    else if(opcode == 0b00001010) { LD_a_mem(REG_BC); }
    else if(opcode == 0b00011010) { LD_a_mem(REG_DE); }
    else if(opcode == 0b00000010) { LD_mem_a(REG_BC); }
    else if(opcode == 0b00010010) { LD_mem_a(REG_DE); }
    else if(opcode == 0b11111010) { LD_a_nn(); }
    else if(opcode == 0b11101010) { LD_nn_a(); }
    else if(opcode == 0b11110010) { LDH_a_c(); }
    else if(opcode == 0b11100010) { LDH_c_a(); }
    else if(opcode == 0b11110000) { LDH_a_n(); }
    else if(opcode == 0b11100000) { LDH_n_a(); }

    else if(opcode == 0b00111010) { LD_a_hl_decrement(); }
    else if(opcode == 0b00110010) { LD_hl_a_decrement(); }
    else if(opcode == 0b00101010) { LD_a_hl_increment(); }
    else if(opcode == 0b00100010) { LD_hl_a_increment(); }

    else if(( opcode  & 0b11001111) == 0b00000001) {
        uint8_t dest = (opcode >> 4) & 0x03;
        LD_rr_nn(static_cast<Reg16>(dest));
    }
    else if(opcode == 0b00001000) { LD_nn_sp(); }

}
void CPU::executeNext() {
    uint8_t opcode = fetch();
    execute(opcode);
}

void CPU::showMemory(uint16_t window) {
    uint16_t min = regs.pc < window ? 0 : regs.pc - window;
    uint16_t max = regs.pc + window > 0xFFFF ? 0xFFFF : regs.pc + window;

    for(int i = min; i < max; i++) {
        std::bitset<8> value(memory[i]);
        if(i == regs.pc) {
            std::cout << "| pc | " << value << " |" << std::endl;
        } else {
            std::cout << "|" 
            << std::showbase << std::internal 
            << std::uppercase << std::hex 
            << std::setw(4) << std::setfill('0')  << i 
            << "| " << value << " |" << std::endl;
        }
    }
    std::cout << std::endl;
}

void CPU::runTillNOP(bool showRegisters, bool showMem) {
    uint8_t opcode = fetch();
    while(opcode) {
        execute(opcode);
        if(showRegisters) regs.showRegisters();
        if(showMem) showMemory();
        opcode = fetch();
    }
}


//--- Load instructions ---//
void CPU::LD_r_r(Reg8 dest, Reg8 src) {
    uint8_t value = readReg8(src);
    writeReg8(dest, value);
}

void CPU::LD_r_n(Reg8 dest) {
    uint8_t imm = fetch();
    writeReg8(dest, imm);
}

void CPU::LD_a_mem(Reg16 src) {
    uint16_t addr = readReg16(src);
    uint8_t value = memory[addr];
    writeReg8(REG_A, value);
}

void CPU::LD_mem_a(Reg16 dest) {
    uint16_t addr = readReg16(dest);
    uint8_t value = readReg8(REG_A);
    memory[addr] = value;
}

void CPU::LD_a_nn() {
    uint16_t addr = fetch16();
    uint8_t value = memory[addr];
    writeReg8(REG_A, value);
}

void CPU::LD_nn_a() {
    uint16_t addr = fetch16();
    uint8_t value = readReg8(REG_A);
    memory[addr] = value;
}

void CPU::LDH_a_c() {
    uint16_t addr = (0xFF00 | readReg8(REG_C));
    uint8_t value = memory[addr];
    writeReg8(REG_A, value);
}

void CPU::LDH_c_a() {
    uint16_t addr = (0xFF00 | readReg8(REG_C));
    uint8_t value = readReg8(REG_A);
    memory[addr] = value;
}

void CPU::LDH_a_n() {
    uint16_t addr = (0xFF00 | fetch());
    uint8_t value = memory[addr];
    writeReg8(REG_A, value);
}

void CPU::LDH_n_a() {
    uint16_t addr = (0xFF00 | fetch());
    uint8_t value = readReg8(REG_A);
    memory[addr] = value;
}

void CPU::LD_a_hl_decrement() {
    uint16_t addr = readReg16(REG_HL);
    uint8_t value = memory[addr];

    // those two operations only taka one cycle
    writeReg8(REG_A, value);
    writeReg16(REG_HL, --addr);
}
void CPU::LD_hl_a_decrement() {
    uint16_t addr = readReg16(REG_HL);
    uint8_t value = readReg8(REG_A);

    // those two operations only taka one cycle
    memory[addr] = value;
    writeReg16(REG_HL, --addr);
}
void CPU::LD_a_hl_increment() {
    uint16_t addr = readReg16(REG_HL);
    uint8_t value = memory[addr];

    // those two operations only taka one cycle
    writeReg8(REG_A, value);
    writeReg16(REG_HL, ++addr);
}
void CPU::LD_hl_a_increment() {
    uint16_t addr = readReg16(REG_HL);
    uint8_t value = readReg8(REG_A);

    // those two operations only taka one cycle
    memory[addr] = value;
    writeReg16(REG_HL, ++addr);
}

void CPU::LD_rr_nn(Reg16 dest) {
    uint16_t value = fetch16();
    writeReg16(dest, value);
}

void CPU::LD_nn_sp() {
    uint16_t addr = fetch16();
    memory[addr++] = regs.sp & 0xFF;
    memory[addr] = ( regs.sp >> 8 );
}

