#include <stdio.h>
#include <assert.h>
#include "bitpack.h"
//brute force width = 64

Except_T Bitpack_Overflow = { "Overflow packing bits" };

uint64_t leftshift(uint64_t num,  unsigned shift)
{
    assert(shift <= 64);
    if (shift == 64) {
        return 0;
    }
    return (num << shift);
}

uint64_t rightshift(uint64_t num,  unsigned shift)
{
    assert(shift <= 64);
    if (shift == 64) {
        return 0;
    }
    return (num >> shift);
}

uint64_t arith_rightshift(int64_t num,  unsigned shift)
{
    assert(shift <= 64);
    if (shift == 64) {
        if (num <= 0) {
            return 0;
        }
        return -1;
    }
    return (num >> shift);
}

//TODO: given float width, it truncates?
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert(width <= 64);
    if (width <= 0) {
        return false;
    }
    if (width == 64) {
        if (n <= UINT64_MAX) {
            return true;
        }
        else {
            return false;
        }
    }
    uint64_t limit = ((uint64_t)1 << width) - 1;
    fprintf(stderr, "upper_lim: %lu\n", limit);
    if (n <= limit) {
        return true;
    }
    return false;
}

bool Bitpack_fitss(int64_t n, unsigned width)
{
    assert(width <= 64);
    if (width <= 0) {
        return false;
    }
    int64_t upper_limit = ((uint64_t)1 << (width - 1)) - 1;
    int64_t lower_limit = (upper_limit * -1) - 1;
    fprintf(stderr, "upper_lim: %lu lower_lim:%ld\n", upper_limit, lower_limit);
    if (n <= upper_limit && n >= lower_limit) {
        return true;
    }
    return false;
}

// uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
// {
//     assert((lsb + width) <= 64);
//     assert(lsb >= 0);
//     //mask of 1s at spot (0s otherwise)
//     uint64_t mask = ~0;
//     mask = mask >> (64 - width) << lsb;
//     //word & mask
//     return (mask & word);
// }

// int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
// {
//     assert((lsb + width) <= 64);
//     assert(lsb >= 0);
//     //mask of 1s at spot (0s otherwise)
//     //word & mask
// }

// uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)
// {
//     assert((lsb + width) <= 64);
//     assert(lsb >= 0);
//     if (Bitpack_fitss(value, width) == false) {
//         RAISE(Bitpack_Overflow);
//     }
// }

// uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value)
// {
//     assert((lsb + width) <= 64);
//     assert(lsb >= 0);
//     if (Bitpack_fitss(value, width) == false) {
//         RAISE(Bitpack_Overflow);
//     }
// }

extern Except_T Bitpack_Overflow;