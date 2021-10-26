#include <stdio.h>
#include <assert.h>
#include "bitpack.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

static uint64_t leftshift(uint64_t num,  unsigned shift)
{
    assert(shift <= 64);
    if (shift == 64) {
        return 0;
    }
    return (num << shift);
}

/**/
static uint64_t logical_rightshift(uint64_t num,  unsigned shift)
{
    assert(shift <= 64);
    if (shift == 64) {
        return 0;
    }
    return (num >> shift);
}

static int64_t arith_rightshift(int64_t num,  unsigned shift)
{
    assert(shift <= 64);
    if (shift == 64) {
        /* If num is negative, then shifting by 64 should return -1 */
        if (num < 0) {
            return ~0;
        }
        /* If num is non-negative, then 1's dont propagate and return 0 */
        return 0;
    }
    return (num >> shift);
}

static update(uint64_t word, unsigned width, unsigned lsb, uint64_t value)
{
    //clear field
        //mask of 0s at field, 1s everywhere else
        //mask or word
    uint64_t 

    //put new value
        //turn value into binary
        //shift value into field, 0 everywhere else
        //
}

//TODO: given float width, it truncates?
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert(width <= 64);
    // if (width <= 0) {
    //     return false;
    // }
    uint64_t limit = ~0;
    if (width == 64) {
        if (n <= limit) {
            return true;
        }
        else {
            return false;
        }
    }
    limit = ((uint64_t)1 << width) - 1;
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

/*
 * Returns a field in a bitpack given word, width of field and least significant
 * bit of field. 
 * Creates mask using shifts then retrieving field. Logical right-shift of res
 * needed since we need to flush field to the right before returning.
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    /* width and height must be non-negative since they are unsigned */
    assert(width <= 64);
    assert((lsb + width) <= 64);

    /* Creating mask */
    uint64_t mask = ~0;
    mask = leftshift(mask, (64 - width - lsb));
    mask = logical_rightshift(mask, 64 - width);
    mask = leftshift(mask, lsb);

    /* Getting field */
    uint64_t res = (mask & word);
    res = logical_rightshift(res, lsb); 
    return res;
}


int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    /* width and height must be non-negative since they are unsigned */
    assert(width <= 64);
    assert((lsb + width) <= 64);
    
    /* Creating mask */
    uint64_t mask = ~0;
    mask = leftshift(mask, (64 - width - lsb));
    mask = logical_rightshift(mask, 64 - width);
    mask = leftshift(mask, lsb);

    /* Getting field */
    int64_t res = (mask & word);
    res = leftshift(res, (64 - width - lsb));
    res = arith_rightshift(res, (64 - width));
    return res;
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)
{
    assert(width <= 64);
    assert((lsb + width) <= 64);

    if (Bitpack_fitsu(value, width) == false) {
        RAISE(Bitpack_Overflow);
    }
    
    //clear field
        //mask of 0s at field, 1s everywhere else
        //mask or word

    //put new value
        //turn value into binary
        //shift value into field, 0 everywhere else
        //
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value)
{
    assert(width <= 64);
    assert((lsb + width) <= 64);

    if (Bitpack_fitss(value, width) == false) {
        RAISE(Bitpack_Overflow);
    }
}

extern Except_T Bitpack_Overflow;