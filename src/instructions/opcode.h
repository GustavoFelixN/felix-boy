#pragma once
#include <array>
#include <cstdint>

class CPU;

using InstrFn = void (*)(CPU&, uint8_t opcode);
extern std::array<InstrFn, 256> OPCODE_TABLE;

void initOpcodes();
