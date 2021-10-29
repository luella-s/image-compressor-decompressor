/**************************************************************
 *
 *                     bitpack.c
 *
 *     Assignment: COMP40 Homework 4 - arith
 *     Authors:  Luella Sugiman (lsugim01), Yingyang Liang (yliang03)
 *     Date:     10/28/21
 *
 *     Implementation of Bitpack - defines the implementation
 *     of Bitpack functions, which allows manipulation of bits
 *     in a signed and unsigned 64-bit integer.
 *
 **************************************************************/

#include <stdio.h>
#include <assert.h>
#include "bitpack.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    unsigned 64-bit integer to shift,
    unsigned 64-bit integer indicating shift amount.
 * Purpose: left-shifts bits of 'num' by 'shift' amount.
 * Error cases:
    if the 'shift' amount is larger then 64 bits (size of 'num')
    we exit with CRE.  
 * Returns: the resulting unsigned 64-bit integer from the shift.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static uint64_t leftshift(uint64_t num, uint64_t shift)
{
    assert(shift <= 64);
    if (shift == 64) {
        return 0;
    }
    return (num << shift);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    unsigned 64-bit integer to shift,
    unsigned 64-bit integer indicating shift amount.
 * Purpose: logically right-shifts bits of 'num' by 'shift' amount,
    meaning the sign bit does not get carried over.
 * Error cases:
    if the 'shift' amount is larger then 64 bits (size of 'num')
    we exit with CRE.  
 * Returns: the resulting unsigned 64-bit integer from the shift.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static uint64_t logical_rightshift(uint64_t num, uint64_t shift)
{
    assert(shift <= 64);
    if (shift == 64) {
        return 0;
    }
    return (num >> shift);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    signed 64-bit integer to shift,
    unsigned 64-bit integer indicating shift amount.
 * Purpose: arithmetically right-shifts bits of 'num' by 'shift' amount,
    meaning the sign bit does get carried over.
 * Error cases:
    if the 'shift' amount is larger then 64 bits (size of 'num')
    we exit with CRE.  
 * Returns: the resulting signed 64-bit integer from the shift.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static int64_t arith_rightshift(int64_t num, uint64_t shift)
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    unsigned 64-bit integer to update,
    width of bits to modify inside 'word',
    least significant bit of field to update,
    unsigned 64-bit integer to insert into 'word'.
 * Purpose: inserts 'value' into 'width' bits of 'word', starting at
    the bit 'lsb'.
 * Error cases: 
    handled by function calling this one.
 * Returns: the resulting unsigned 64-bit integer from the update. 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static uint64_t update(uint64_t word, unsigned width, unsigned lsb, 
    uint64_t value)
{
    /* clear field of word */
    uint64_t clear_mask_h = ~0;
    clear_mask_h = leftshift(clear_mask_h, (width + lsb));
    uint64_t clear_mask_l = ~0;
    clear_mask_l = logical_rightshift(clear_mask_l, (64 - lsb));
    uint64_t clear_mask = clear_mask_h | clear_mask_l;
    word = (word & clear_mask);
    
    /* put value into cleared field of word */
    value = leftshift(value, (64 - width));
    value = logical_rightshift(value, (64 - width - lsb));
    word = (word | value);

    return word;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    unsigned 64-bit integer to fit,
    width of bits to fit into.
 * Purpose: checks whether 'n' can be represented by 'width' bits.
 * Error cases:
    if 'width' is larger then 64 bits (size of 'num'), we exit with CRE.  
 * Returns: resulting check.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert(width <= 64);
    uint64_t limit = ~0;

    /* Checking if n fits within limit if width is 64*/
    if (width == 64) {
        if (n <= limit) {
            return true;
        } else {
            return false;
        }
    }
    limit = ((uint64_t)1 << width) - 1;
    if (n <= limit) {
        return true;
    }
    return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    signed 64-bit integer to fit,
    width of bits to fit into.
 * Purpose: checks whether 'n' can be represented by 'width' bits.
 * Error cases:
    if 'width' is larger then 64 bits (size of 'num'), we exit with CRE.  
 * Returns: resulting check.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool Bitpack_fitss(int64_t n, unsigned width)
{
    assert(width <= 64);
    if (width <= 0) {
        return false;
    }
    /* Compute upper and lower limit of the word */
    int64_t upper_limit = ((uint64_t)1 << (width - 1)) - 1;
    int64_t lower_limit = (upper_limit * -1) - 1;

    /* Check if value fits in that range */
    if (n <= upper_limit && n >= lower_limit) {
        return true;
    }
    return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    unsigned 64-bit integer to get field from,
    number of bits of the field to get,
    least significant bit of field to get from.
 * Purpose: gets 'width' bits from the 'lsb'-th bit from 'word'.
 * Error cases:
    if 'width' is larger than 64 bits (size of 'num'), we exit with CRE.
    if the most significant bit is above 64 (size of 'num'), we exit with CRE. 
 * Returns: field (interpreted as an unsigned integer) obtained from 'word'.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    unsigned 64-bit integer to get field from,
    number of bits of the field to get,
    least significant bit of field to get from.
 * Purpose: gets 'width' bits from the 'lsb'-th bit from 'word'.
 * Error cases:
    if 'width' is larger than 64 bits (size of 'num'), we exit with CRE.
    if the most significant bit is above 64 (size of 'num'), we exit with CRE. 
 * Returns: field (interpreted as a signed integer) obtained from 'word'.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    unsigned 64-bit integer to update,
    width of bits to modify inside 'word',
    least significant bit of field to update,
    unsigned 64-bit integer to insert into 'word'.
 * Purpose: inserts 'value' into 'width' bits of 'word', starting at
    the bit 'lsb'.
 * Error cases:
    if 'width' is larger than 64 bits (size of 'num'), we exit with CRE.
    if the most significant bit is above 64 (size of 'num'), we exit with CRE. 
 * Returns: resulting 'word'.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
    uint64_t value)
{
    assert(width <= 64);
    assert((lsb + width) <= 64);

    if (Bitpack_fitsu(value, width) == false) {
        RAISE(Bitpack_Overflow);
    }
    
    return update(word, width, lsb, value);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    unsigned 64-bit integer to update,
    width of bits to modify inside 'word',
    least significant bit of field to update,
    signed 64-bit integer to insert into 'word'
 * Purpose: inserts 'value' into 'width' bits of 'word', starting at
    the bit 'lsb'.
 * Error cases:
    if 'width' is larger than 64 bits (size of 'num'), we exit with CRE.
    if the most significant bit is above 64 (size of 'num'), we exit with CRE. 
 * Returns: resulting 'word'.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
    int64_t value)
{
    assert(width <= 64);
    assert((lsb + width) <= 64);

    if (Bitpack_fitss(value, width) == false) {
        RAISE(Bitpack_Overflow);
    }
    
    return update(word, width, lsb, value);
}

extern Except_T Bitpack_Overflow;