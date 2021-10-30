/**************************************************************
 *
 *                     codeword.c
 *
 *     Assignment: COMP40 Homework 4 - arith
 *     Authors:  Luella Sugiman (lsugim01), Yingyang Liang (yliang03)
 *     Date:     10/28/21
 *
 *     Implementation of codeword module - implements functions
 *     that bitpacks a UArray2 containing Quant structs 
 *     (holds scaled_a, scaled_b, scaled_c, scaled_d, index of Pb, index of Pr)
 *     into UArray2 of 32-bit codewords
 *     and one that unpacks it.
 *
 **************************************************************/

#include "codeword.h"

#define WORD_BITS 32
#define A_BITS 6
#define B_BITS 6
#define C_BITS 6
#define D_BITS 6
#define PB_BITS 4
#define PR_BITS 4

/* * * * * * Pack Quant structs to 32-bit codewords * * * * * * * * */

void apply_bitpack(int col, int row, UArray2_T array2, void *elem, void *cl);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArry2 of Quant structs
 * Purpose: Packs Quant structs into 32-bit codewords in row-major
            order.
 * Error cases:
    if the uarray2 passed in is a NULL pointer, we exit with CRE.
 * Returns: UArray2 of 32-bit codewords
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T encode(UArray2_T quant_arr)
{
   assert(quant_arr != NULL);

   /* make UArray2 to hold 32-bit codewords */
   int width = UArray2_width(quant_arr);
   int height = UArray2_height(quant_arr);
   UArray2_T words = UArray2_new(width, height, sizeof(uint32_t));
   assert(words != NULL);
   
   UArray2_map_row_major(quant_arr, apply_bitpack, words);

   return words;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    col, row, array2 and elem refers to UArray2 of Quants
    cl (closure) holds UArray2 of codewords
 * Purpose: Apply function for bitpacking.
 * Error cases:
    It is a CRE if
    the closure holds a NULL pointer, 
    codeword retrieved is not 32-bits
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void apply_bitpack(int col, int row, UArray2_T array2, void *elem, void *cl)
{
   (void) array2;

   UArray2_T codeword_arr = (UArray2_T)cl;
   assert(codeword_arr != NULL);

   uint32_t *codeword = UArray2_at(codeword_arr, col, row);

   Quant block = (Quant)elem;

   /* store values in fields of codeword */
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

/* * * * * * Unpack 32-bit codewords to UArray2 of Quant structs * * * * * */

void apply_bitunpack(int col, int row, UArray2_T array2, void *elem, void *cl);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArry2 of 32-bit codewords
 * Purpose: Upacks 32-bit codewords into Quant structs
 * Error cases:
    if the uarray2 passed in is a NULL pointer, we exit with CRE.
 * Returns: UArray2 of Quant structs
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T decode(UArray2_T codeword_arr)
{
   assert(codeword_arr != NULL);

   /* make UArray2 to hold Quant structs */
   int width = UArray2_width(codeword_arr);
   int height = UArray2_height(codeword_arr);
   UArray2_T quant_arr = UArray2_new(width, height, sizeof(struct Quant));
   assert(quant_arr != NULL);

   UArray2_map_row_major(codeword_arr, apply_bitunpack, quant_arr);

   return quant_arr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    col, row, array2 and elem refers to UArray2 of codewords
    cl (closure) holds UArray2 of Quants
 * Purpose: Apply function for unpacking codewords to Quant structs
 * Error cases:
    It is a CRE if
    the closure holds a NULL pointer or
    elem's size does not match the size of a codeword
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void apply_bitunpack(int col, int row, UArray2_T array2, void *elem, void *cl)
{
   (void) col;
   (void) row;
   (void) array2;

   UArray2_T quant_arr = (UArray2_T)cl;
   assert(quant_arr != NULL);
   
   uint32_t *codeword = (uint32_t *)elem;
   uint64_t word = *codeword;

   /* Retreiving fields in codeword */
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

   /* store retrieved values in Quant struct */
   Quant block = UArray2_at(quant_arr, col, row);
   block->scaled_a = a;
   block->scaled_b = b;
   block->scaled_c = c;
   block->scaled_d = d;
   block->ind_pb = ind_pb;
   block->ind_pr = ind_pr;
}