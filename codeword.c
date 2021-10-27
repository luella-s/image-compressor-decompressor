#include "codeword.h"
#define WORD_BITS 32
#define A_BITS 9
#define B_BITS 5
#define C_BITS 5
#define D_BITS 5
#define PB_BITS 4
#define PR_BITS 4

/* Mapping functions */
void apply_bitpack(int col, int row, UArray2_T array2, void *elem, void *cl);
void apply_bitunpack(int col, int row, UArray2_T array2, void *elem, void *cl);

/* Helper functions */
// int index_converter(UArray2_T uarray2, int col, int row);

//Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)

UArray2_T encode(UArray2_T quant_arr)
{
    assert(quant_arr != NULL);
    int width = UArray2_width(quant_arr);
    int height = UArray2_height(quant_arr);
    
    UArray2_T words = UArray2_new(width, height, sizeof(uint32_t));
    assert(words != NULL);
    
    UArray2_map_row_major(quant_arr, apply_bitpack, words);

    return words;
}

void apply_bitpack(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    (void) array2;

    UArray2_T codeword_arr = (UArray2_T)cl;
    // int index = index_converter(array2, col, row);

    assert(codeword_arr != NULL);
    uint32_t *codeword = UArray2_at(codeword_arr, col, row);
    assert(sizeof(*codeword) == sizeof(uint32_t));

    Quant block = (Quant)elem;
    assert(sizeof(*block) == sizeof(struct Quant));

    uint64_t word = 0;
    int a_lsb = WORD_BITS - A_BITS;
    word = Bitpack_newu(word, A_BITS, a_lsb, (uint64_t)block->scaled_a);
    int b_lsb = a_lsb - B_BITS;
    word = Bitpack_news(word, B_BITS, b_lsb, (int64_t)block->scaled_b);
    int c_lsb = b_lsb - C_BITS;
    word = Bitpack_news(word, C_BITS, c_lsb, (int64_t)block->scaled_c);
    int d_lsb = c_lsb - D_BITS;
    word = Bitpack_news(word, D_BITS, d_lsb, (int64_t)block->scaled_d);
    int pb_lsb = d_lsb - PB_BITS;
    word = Bitpack_newu(word, PB_BITS, pb_lsb, (uint64_t)block->ind_pb);
    int pr_lsb = pb_lsb - PR_BITS;
    word = Bitpack_newu(word, PR_BITS, pr_lsb, (uint64_t)block->ind_pr);

    *codeword = (uint32_t)word;
}

//big-endian ends with largest address
//getc, putc

UArray2_T decode(UArray2_T codeword_arr)
{
    assert(codeword_arr != NULL);

    int width = UArray2_width(codeword_arr);
    int height = UArray2_height(codeword_arr);
    UArray2_T quant_arr = UArray2_new(width, height, sizeof(struct Quant));
    assert(quant_arr != NULL);

    UArray2_map_row_major(codeword_arr, apply_bitunpack, quant_arr);

    return quant_arr;
}

void apply_bitunpack(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    (void) col;
    (void) row;
    (void) array2;

    UArray2_T quant_arr = (UArray2_T)cl;
    assert(quant_arr != NULL);
    
    uint32_t *codeword = (uint32_t *)elem;
    assert(sizeof(*codeword) == sizeof(uint32_t));
    uint64_t word = *codeword;

    int a_lsb = WORD_BITS - A_BITS;
    unsigned a = Bitpack_getu(word, A_BITS, (uint64_t)a_lsb);

    int b_lsb = a_lsb - B_BITS;
    signed b = Bitpack_gets(word, B_BITS, (int64_t)b_lsb);
    
    int c_lsb = b_lsb - C_BITS;
    signed c = Bitpack_gets(word, C_BITS, (int64_t)c_lsb);

    int d_lsb = c_lsb - D_BITS;
    signed d = Bitpack_gets(word, D_BITS, (int64_t)d_lsb);

    int pb_lsb = d_lsb - PB_BITS;
    unsigned ind_pb = Bitpack_getu(word, PB_BITS, (uint64_t)pb_lsb);
    
    int pr_lsb = pb_lsb - PR_BITS;
    unsigned ind_pr = Bitpack_getu(word, PR_BITS, (uint64_t)pr_lsb);

    Quant block = UArray2_at(quant_arr, col, row);
    block->scaled_a = a;
    block->scaled_b = b;
    block->scaled_c = c;
    block->scaled_d = d;
    block->ind_pb = ind_pb;
    block->ind_pr = ind_pr;
}