#include "bitpack.h"

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert(width <= 64);
    if ((pow(2, width) - 1) <= n) {
        return true;
    }
    return false;
}

bool Bitpack_fitss(int64_t n, unsigned width)
{
    assert(width <= 64);
    if (((pow(2, width) - 1) / 2) < n) {
        return true;
    } else if ((pow(2, width) / 2 * -1) >= n) {
        return true;
    }
    return false;
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    assert((lsb + width) <= 64);
    assert(lsb >= 0);
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{

}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)
{

}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value)
{

}

extern Except_T Bitpack_Overflow;