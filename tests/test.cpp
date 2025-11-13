#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "../registers.h"

TEST_CASE("Registers 8-bit fields and initialization", "[registers]") {
    Registers r{};

    SECTION("All registers start at 0") {
        REQUIRE(r.a == 0);
        REQUIRE(r.b == 0);
        REQUIRE(r.c == 0);
        REQUIRE(r.d == 0);
        REQUIRE(r.e == 0);
        REQUIRE(r.f == 0);
        REQUIRE(r.h == 0);
        REQUIRE(r.l == 0);
        REQUIRE(r.sp == 0);
        REQUIRE(r.pc == 0);
    }
}

TEST_CASE("Registers 16-bit combined getters and setters", "[registers]") {
    Registers r{};

    SECTION("AF register pair") {
        r.setAF(0x12F0);
        REQUIRE(r.getAF() == 0x12F0);
        REQUIRE(r.a == 0x12);
        REQUIRE(r.f == 0xF0);

        // F lower nibble deve ser sempre 0
        r.setAF(0x12FF);
        REQUIRE(r.f == 0xF0);
    }

    SECTION("BC register pair") {
        r.setBC(0x1234);
        REQUIRE(r.getBC() == 0x1234);
        REQUIRE(r.b == 0x12);
        REQUIRE(r.c == 0x34);
    }

    SECTION("DE register pair") {
        r.setDE(0xABCD);
        REQUIRE(r.getDE() == 0xABCD);
        REQUIRE(r.d == 0xAB);
        REQUIRE(r.e == 0xCD);
    }

    SECTION("HL register pair") {
        r.setHL(0x5566);
        REQUIRE(r.getHL() == 0x5566);
        REQUIRE(r.h == 0x55);
        REQUIRE(r.l == 0x66);
    }
}

TEST_CASE("Registers flag operations", "[flags]") {
    Registers r{};
    using Flag = Registers::Flag;

    SECTION("Setting and getting flags") {
        r.setFlag(Flag::Z, true);
        REQUIRE(r.getFlag(Flag::Z));

        r.setFlag(Flag::N, true);
        REQUIRE(r.getFlag(Flag::N));

        r.setFlag(Flag::H, false);
        REQUIRE_FALSE(r.getFlag(Flag::H));
    }

    SECTION("Clearing a flag") {
        r.setFlag(Flag::C, true);
        REQUIRE(r.getFlag(Flag::C));

        r.setFlag(Flag::C, false);
        REQUIRE_FALSE(r.getFlag(Flag::C));
    }

    SECTION("Clear all flags") {
        r.f = 0xFF;
        r.clearFlags();
        REQUIRE(r.f == 0);
    }
}

TEST_CASE("Registers combined behavior", "[integration]") {
    Registers r{};
    using Flag = Registers::Flag;

    r.setBC(0xBEEF);
    r.setDE(0x1234);
    r.setHL(0x0A0B);
    r.setAF(0xCAFE);

    SECTION("Reading all combined registers") {
        REQUIRE(r.getBC() == 0xBEEF);
        REQUIRE(r.getDE() == 0x1234);
        REQUIRE(r.getHL() == 0x0A0B);
        REQUIRE(r.getAF() == 0xCAF0); // F low nibble truncado
    }

    SECTION("Setting flags independently does not alter A") {
        uint8_t prevA = r.a;
        r.setFlag(Flag::Z, true);
        r.setFlag(Flag::C, true);
        REQUIRE(r.a == prevA);
    }
}

TEST_CASE("Registers::showRegisters output sanity", "[io]") {
    Registers r{};
    r.a = 0x12;
    r.f = 0xF0;
    r.b = 0x34;
    r.c = 0x56;
    r.d = 0x78;
    r.e = 0x9A;
    r.h = 0xBC;
    r.l = 0xDE;

    // Apenas para garantir que a função não lança exceções
    REQUIRE_NOTHROW(r.showRegisters());
}
