#include "registers.h"

void Registers::showRegisters() const {

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
