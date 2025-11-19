#include "cpu.h"
#include "instructions/opcode.h"
#include <cstdint>

uint8_t CPU::readReg8(Reg8 reg) {
    switch (reg) {
        case REG_B: return regs.b; 
        case REG_C: return regs.c; 
        case REG_D: return regs.d; 
        case REG_E: return regs.e; 
        case REG_H: return regs.h; 
        case REG_L: return regs.l; 
        case REG_A: return regs.a; 
        case REG_HL_MEM: return memory[regs.getHL()]; 
        default: throw std::runtime_error("Invalid Reg8 access");
    }
    return 0;
}

void CPU::writeReg8(Reg8 reg, uint8_t value) {
    switch (reg) {
        case REG_B: regs.b = value; break;
        case REG_C: regs.c = value;  break;
        case REG_D: regs.d = value;  break;
        case REG_E: regs.e = value;  break;
        case REG_H: regs.h = value;  break;
        case REG_L: regs.l = value;  break;
        case REG_A: regs.a = value;  break;
        case REG_HL_MEM: memory[regs.getHL()] = value;  break;
        default: throw std::runtime_error("Invalid Reg8 access");
    }
}

uint16_t CPU::readReg16(Reg16 reg) {
    switch (reg) {
        case REG_BC: return regs.getBC(); 
        case REG_DE: return regs.getDE(); 
        case REG_HL: return regs.getHL(); 
        case REG_SP: return regs.sp; 
        default: throw std::runtime_error("Invalid Reg16 access");
    }
    return 0;
}

uint8_t CPU::readReg16Mem(Reg16 reg) {
    switch (reg) {
        case REG_BC: return memory[regs.getBC()]; 
        case REG_DE: return memory[regs.getDE()]; 
        case REG_HL: return memory[regs.getHL()]; 
        case REG_SP: return memory[regs.sp]; 
        default: throw std::runtime_error("Invalid Reg16 access");
    }
    return 0;
}

void CPU::writeReg16(Reg16 reg, uint16_t value) {
    switch (reg) {
        case REG_BC: regs.setBC(value);  break;
        case REG_DE: regs.setDE(value);  break;
        case REG_HL: regs.setHL(value);  break;
        case REG_SP: regs.sp = value;  break;
        default: throw std::runtime_error("Invalid Reg16 access");
    }
}

void CPU::writeReg16Mem(Reg16 reg, uint8_t value) {
    switch (reg) {
        case REG_BC: memory[regs.getBC()] = value;  break;
        case REG_DE: memory[regs.getDE()] = value;  break;
        case REG_HL: memory[regs.getHL()] = value;  break;
        case REG_SP: memory[regs.sp] = value;  break;
        default: throw std::runtime_error("Invalid Reg16 access");
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
    OPCODE_TABLE[opcode](*this, opcode);
}

void CPU::step() {
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

void CPU::LD_sp_hl() {
    regs.sp = regs.getHL();
}

void CPU::PUSH(Reg16 src) {
    uint16_t value = readReg16(src);
    memory[--regs.sp] = value >> 8;
    memory[--regs.sp] = value & 0xFF;
}

void CPU::POP(Reg16 dest) {
    uint8_t lsb = memory[regs.sp++];
    uint8_t msb = memory[regs.sp++];
    uint16_t value = (msb << 8) | lsb;
    writeReg16(dest, value);
}

void CPU::LD_hl_sp_e() {
    int8_t e = static_cast<int8_t>(fetch());
    uint16_t sp = regs.sp;
    uint16_t result = sp + e;

    bool halfCarry = ((sp & 0xF) + (e & 0xF)) > 0xF;
    bool carry =     ((sp & 0xFF) + static_cast<uint8_t>(e)) > 0xFF;

    regs.setHL(result);

    regs.setFlag(Registers::Z, false);
    regs.setFlag(Registers::N, false);
    regs.setFlag(Registers::H, halfCarry);
    regs.setFlag(Registers::C, carry);
}
