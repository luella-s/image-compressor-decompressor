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
int index_converter(UArray2_T uarray2, int col, int row);

//Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)

UArray_T encode(UArray2_T quant_arr)
{
    assert(quant_arr != NULL);
    int width = UArray2_width(quant_arr);
    int height = UArray2_height(quant_arr);
    
    UArray2_T words = UArray2_new(width, height, sizeof(uint32_t));
    assert(words != NULL);
    
    UArray2_map_row_major(quant_arr, apply_bitpack, words);
}

void apply_bitpack(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    UArray2_T codeword_arr = (UArray2_T)cl;
    // int index = index_converter(array2, col, row);

    assert(codeword_arr != NULL);
    uint32_t *codeword = UArray2_at(codeword_arr, col, row);
    assert(sizeof(*codeword) == sizeof(uint32_t));

    Quant block = (Quant)elem;
    assert(sizeof(*block) == sizeof(struct Quant));

    uint32_t word = 0;
    int a_lsb = WORD_BITS - A_BITS;
    word = Bitpack_newu(word, a_bits, a_lsb, block->a);
    int b_lsb = a_lsb - B_BITS;
    word = Bitpack_newu(word, b_bits, b_lsb, block->b);
    int c_lsb = b_lsb - C_BITS;
    word = Bitpack_newu(word, c_bits, c_lsb, block->c);
    int d_lsb = c_lsb - D_BITS;
    word = Bitpack_newu(word, d_bits, d_lsb, block->d);
    int pb_lsb = d_lsb - PB_BITS;
    word = Bitpack_newu(word, pb_bits, pb_lsb, block->ind_pb);
    int pr_lsb = pb_lsb - PR_BITS;
    word = Bitpack_newu(word, pr_bits, pr_lsb, block->ind_pr);

    *codeword = word;
}

UArray2_T decode(UArray2_T codeword_arr)
{
    assert(codeword_arr != NULL);

    int width = UArray2_width(codeword_arr);
    int height = UArray3_height(codeword_arr);
    UArray2_T quant_arr = UArray2_new(width, height, sizeof(struct Quant));
    assert(quant_arr != NULL);

    UArray2_map_row_major(codeword_arr, apply_bitunpack, quant_arr);
}

void apply_bitunpack(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    UArray2_T quant_arr = (UArray2_T)cl;
    assert(quant_arr != NULL);
    
    uint32_t codeword = (uint32_t)elem;
    assert(sizeof(*codeword) == sizeof(uint32_t));
}


// int index_converter(UArray2_T uarray2, int col, int row)
// {
// 	assert(uarray2 != NULL);
	
// 	int width = UArray2_width(uarray2);
// 	int height = UArray2_height(uarray2);

// 	/* Assert that index given is within 2D array dimensions */
// 	assert((col < width) && (col >= 0) && (row < height) && (row >= 0));

// 	int index = row * width + col;
// 	return index;
// }