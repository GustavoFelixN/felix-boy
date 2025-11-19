#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "../src/cpu.h"
#include "../src/registers.h"

TEST_CASE("CPU: read/write Reg8", "[cpu]") {
    CPU cpu;
    cpu.regs.b = 0x12;
    cpu.regs.c = 0x34;
    cpu.regs.h = 0x01;
    cpu.regs.l = 0x00;

    REQUIRE(cpu.readReg8(REG_B) == 0x12);
    REQUIRE(cpu.readReg8(REG_C) == 0x34);

    cpu.writeReg8(REG_B, 0xAA);
    REQUIRE(cpu.regs.b == 0xAA);
}

TEST_CASE("CPU: read/write Reg16", "[cpu]") {
    CPU cpu;
    cpu.regs.setBC(0x1234);
    cpu.regs.setHL(0xABCD);
    cpu.regs.sp = 0xFFF0;

    REQUIRE(cpu.readReg16(REG_BC) == 0x1234);
    REQUIRE(cpu.readReg16(REG_HL) == 0xABCD);
    REQUIRE(cpu.readReg16(REG_SP) == 0xFFF0);

    cpu.writeReg16(REG_DE, 0xFACE);
    REQUIRE(cpu.regs.getDE() == 0xFACE);
}

TEST_CASE("CPU: read/write memory through HL", "[cpu]") {
    CPU cpu;
    cpu.regs.setHL(0x2000);
    cpu.memory[0x2000] = 0x55;

    REQUIRE(cpu.readReg8(REG_HL_MEM) == 0x55);

    cpu.writeReg8(REG_HL_MEM, 0x99);
    REQUIRE(cpu.memory[0x2000] == 0x99);
}

TEST_CASE("CPU: fetch and fetch16", "[cpu]") {
    CPU cpu;
    cpu.regs.pc = 0;

    cpu.memory[0] = 0x10;
    cpu.memory[1] = 0x34;
    cpu.memory[2] = 0x12;

    REQUIRE(cpu.fetch() == 0x10);
    REQUIRE(cpu.fetch16() == 0x1234);
    REQUIRE(cpu.regs.pc == 3);
}

TEST_CASE("CPU: LD_r_r", "[cpu][ld]") {
    CPU cpu;
    cpu.regs.b = 0xAA;
    cpu.regs.c = 0x55;

    cpu.LD_r_r(REG_C, REG_B); // C = B
    REQUIRE(cpu.regs.c == 0xAA);
}

TEST_CASE("CPU: LD_r_n", "[cpu][ld]") {
    CPU cpu;
    cpu.regs.pc = 0;
    cpu.memory[0] = 0x42;

    cpu.LD_r_n(REG_A);

    REQUIRE(cpu.regs.a == 0x42);
    REQUIRE(cpu.regs.pc == 1);
}

TEST_CASE("CPU: LD_r_hl", "[cpu][ld]") {
    CPU cpu;
    cpu.regs.setHL(0x2000);
    cpu.memory[0x2000] = 0x77;

    cpu.LD_r_r(REG_A, REG_HL_MEM);

    REQUIRE(cpu.regs.a == 0x77);
}

TEST_CASE("CPU: LD_hl_r", "[cpu][ld]") {
    CPU cpu;
    cpu.regs.setHL(0x3000);
    cpu.regs.d = 0xFE;

    cpu.LD_r_r(REG_HL_MEM, REG_D);
    REQUIRE(cpu.memory[0x3000] == 0xFE);
}

TEST_CASE("CPU: LD_hl_n", "[cpu][ld]") {
    CPU cpu;
    cpu.regs.setHL(0x9000);
    cpu.regs.pc = 0;
    cpu.memory[0] = 0xCC;

    cpu.LD_r_n(REG_HL_MEM);
    REQUIRE(cpu.memory[0x9000] == 0xCC);
}

TEST_CASE("CPU: LD_a_mem / LD_mem_a", "[cpu][ld]") {
    CPU cpu;
    cpu.regs.setBC(0x4000);
    cpu.memory[0x4000] = 0x55;

    cpu.LD_a_mem(REG_BC);
    REQUIRE(cpu.regs.a == 0x55);

    cpu.regs.a = 0x99;
    cpu.LD_mem_a(REG_BC);
    REQUIRE(cpu.memory[0x4000] == 0x99);
}

TEST_CASE("CPU: LD_a_nn / LD_nn_a", "[cpu][ld]") {
    CPU cpu;
    cpu.regs.pc = 0;
    cpu.memory[0] = 0x34;
    cpu.memory[1] = 0x12;
    cpu.memory[0x1234] = 0x77;

    cpu.LD_a_nn();
    REQUIRE(cpu.regs.a == 0x77);

    cpu.regs.pc = 0;
    cpu.memory[0] = 0x34;
    cpu.memory[1] = 0x12;
    cpu.memory[0x1234] = 0x00;
    cpu.regs.a = 0xAB;

    cpu.LD_nn_a();
    REQUIRE(cpu.memory[0x1234] == 0xAB);
}

TEST_CASE("CPU: LDH instructions", "[cpu][ldh]") {
    CPU cpu;

    cpu.regs.c = 0x10;
    cpu.memory[0xFF10] = 0x44;

    cpu.LDH_a_c();
    REQUIRE(cpu.regs.a == 0x44);

    cpu.regs.a = 0x99;
    cpu.LDH_c_a();
    REQUIRE(cpu.memory[0xFF10] == 0x99);

    // LDH a, n
    cpu.regs.pc = 0;
    cpu.memory[0] = 0x20; // offset
    cpu.memory[0xFF20] = 0x66;

    cpu.LDH_a_n();
    REQUIRE(cpu.regs.a == 0x66);

    // LDH n, a
    cpu.regs.pc = 0;
    cpu.regs.a = 0x7A;
    cpu.memory[0] = 0x30;

    cpu.LDH_n_a();
    REQUIRE(cpu.memory[0xFF30] == 0x7A);
}

TEST_CASE("CPU: HL auto-increment/decrement", "[cpu][hlincdec]") {
    CPU cpu;
    cpu.regs.setHL(0x2000);
    cpu.memory[0x2000] = 0xAA;

    cpu.LD_a_hl_increment();
    REQUIRE(cpu.regs.a == 0xAA);
    REQUIRE(cpu.regs.getHL() == 0x2001);

    cpu.regs.setHL(0x3000);
    cpu.regs.a = 0xBB;
    cpu.LD_hl_a_increment();
    REQUIRE(cpu.memory[0x3000] == 0xBB);
    REQUIRE(cpu.regs.getHL() == 0x3001);

    cpu.regs.setHL(0x4000);
    cpu.memory[0x4000] = 0xCC;
    cpu.LD_a_hl_decrement();
    REQUIRE(cpu.regs.a == 0xCC);
    REQUIRE(cpu.regs.getHL() == 0x3FFF);

    cpu.regs.setHL(0x5000);
    cpu.regs.a = 0xDD;
    cpu.LD_hl_a_decrement();
    REQUIRE(cpu.memory[0x5000] == 0xDD);
    REQUIRE(cpu.regs.getHL() == 0x4FFF);
}

TEST_CASE("CPU: LD_rr_nn", "[cpu][ld16]") {
    CPU cpu;
    cpu.regs.pc = 0;
    cpu.memory[0] = 0xCD; // low byte
    cpu.memory[1] = 0xAB; // high byte

    cpu.LD_rr_nn(REG_HL);
    REQUIRE(cpu.regs.getHL() == 0xABCD);
}

TEST_CASE("CPU LD_nn_sp", "[cpu][ld16]") {
    CPU cpu;
    cpu.regs.pc = 0;
    cpu.memory[0] = 0xCD; // low byte
    cpu.memory[1] = 0xAB; // high byte

    cpu.regs.sp = 0x1234;

    cpu.LD_nn_sp();
    REQUIRE(cpu.memory[0xABCD] == 0x34);
    REQUIRE(cpu.memory[0xABCE] == 0x12);

}

TEST_CASE("CPU LD_sp_hl", "[cpu][ld16]") {
    CPU cpu;

    cpu.regs.sp = 0x1234;
    cpu.regs.setHL(0xABCD);

    cpu.LD_sp_hl();
    REQUIRE(cpu.regs.sp == 0xABCD);
}

TEST_CASE("CPU PUSH", "[cpu][ld16]") {
    CPU cpu;

    cpu.regs.sp = 0xFFFF;
    cpu.regs.setBC(0x1234);

    cpu.PUSH(REG_BC);

    REQUIRE(cpu.memory[0xFFFE] == 0x12);
    REQUIRE(cpu.memory[0xFFFD] == 0x34);
}


TEST_CASE("CPU POP", "[cpu][ld16]") {
    CPU cpu;

    cpu.regs.sp = 0xFFFD;
    cpu.memory[0xFFFD] = 0x34;
    cpu.memory[0xFFFE] = 0x12;

    cpu.POP(REG_BC);

    REQUIRE(cpu.regs.getBC() == 0x1234);
}

TEST_CASE("CPU: LD_hl_sp_e", "[cpu][ld][hl][sp]") {
    CPU cpu;

    cpu.regs.sp = 0xFFF8;
    cpu.regs.pc = 0x200;
    cpu.memory[cpu.regs.pc] = 0x08;

    cpu.LD_hl_sp_e();

    REQUIRE(cpu.regs.getHL() == 0x0000);
    REQUIRE(cpu.regs.getFlag(Registers::Z) == false);
    REQUIRE(cpu.regs.getFlag(Registers::N) == false);
    REQUIRE(cpu.regs.getFlag(Registers::H) == true);
    REQUIRE(cpu.regs.getFlag(Registers::C) == true);

    // Caso 2: deslocamento negativo
    cpu.regs.sp = 0x0100;
    cpu.regs.pc = 0x300;
    cpu.memory[cpu.regs.pc] = 0xF0;

    cpu.LD_hl_sp_e();

    REQUIRE(cpu.regs.getHL() == 0x00F0);
    REQUIRE(cpu.regs.getFlag(Registers::Z) == false);
    REQUIRE(cpu.regs.getFlag(Registers::N) == false);
    REQUIRE(cpu.regs.getFlag(Registers::H) == false);
    REQUIRE(cpu.regs.getFlag(Registers::C) == false);

    // Caso sem carry
    cpu.regs.sp = 0x1234;
    cpu.regs.pc = 0x400;
    cpu.memory[cpu.regs.pc] = 0x01;

    cpu.LD_hl_sp_e();

    REQUIRE(cpu.regs.getHL() == 0x1235);
    REQUIRE(cpu.regs.getFlag(Registers::Z) == false);
    REQUIRE(cpu.regs.getFlag(Registers::N) == false);
    REQUIRE(cpu.regs.getFlag(Registers::H) == false);
    REQUIRE(cpu.regs.getFlag(Registers::C) == false);
}
