/**************************************************************
 *
 *                     dct_quant.c
 *
 *     Assignment: COMP40 Homework 4 - arith
 *     Authors:  Luella Sugiman (lsugim01), Yingyang Liang (yliang03)
 *     Date:     10/28/21
 *
 *     Implementation of dct_quant module - implements functions
 *     that compresses a UArray2 containing Dct structs 
 *     into UArray2 of Quant structs
 *     and decompresses from Quant to Dct.
 *     Quantizes values a, b, c, d, avg_pb and avg_pr.
 *     We lose information in this process but is now able to represent
 *     a, b, c, d, avg_pb, avg_pr in a total of 32 bits.
 *
 **************************************************************/

#include "dct_quant.h"

#define A_BITS 9
#define BCD_BITS 5


/* * * * * * * Quantize to Chroma Indixes & Cosine Coefficients * * * * * * */

void apply_quantize(int col, int row, UArray2_T array2, void *elem, void *cl);
signed quantize_bcd(float n);
uint64_t power_2(int shift);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArray2 of Dct structs.
 * Purpose: Compresses a UArray2 of Dct structs into a UArray2 of Quant structs.
 * Error cases:
    if 'dct_arr' passed in is a NULL pointer, we exit with CRE,
    if dynamic allocation of memory fails, we exit with CRE.
 * Returns: UArray2 of Quant structs.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T quantize(UArray2_T dct_arr)
{
    assert(dct_arr != NULL);
    
    /* make UArray2 to hold Quant structs */
    int width = UArray2_width(dct_arr);
    int height = UArray2_height(dct_arr);
    UArray2_T quant_arr = UArray2_new(width, height, sizeof(struct Quant));
    assert(quant_arr != NULL);
    
    UArray2_map_row_major(quant_arr, apply_quantize, dct_arr);

    return quant_arr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    column index,
    row index,
    UArray2 being mapped through,
    element in array2[col][row],
    closure argument (dct_arr).
 * Purpose: Compresses a Dct struct to a Quant struct by quantizing a, b, 
    c, d, average Pr and average Pb values into given bits sizes.
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void apply_quantize(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    (void) array2;

    Quant block = (Quant)elem;
    UArray2_T dct_arr = (UArray2_T)cl;
    Dct_float dct_elem = (Dct_float)UArray2_at(dct_arr, col, row);

    /* values already clamped in dct to be in expected ranges */
    float avg_pb = dct_elem->avg_pb;
    float avg_pr = dct_elem->avg_pr;
    float a = dct_elem->a;
    float b = dct_elem->b;
    float c = dct_elem->c;
    float d = dct_elem->d;

    /* compute & store indicies of average Pb and Pr */
    unsigned ind_pb = Arith40_index_of_chroma(avg_pb);
    unsigned ind_pr = Arith40_index_of_chroma(avg_pr);
    block->ind_pb = ind_pb;
    block->ind_pr = ind_pr;

    /* quantize & store a, b, c, d */
    int a_choices = (power_2(A_BITS) - 1);
    unsigned scaled_a = round(a * a_choices);
    block->scaled_a = scaled_a;
    assert(block->scaled_a <= (unsigned)a_choices);
    block->scaled_b = quantize_bcd(b);
    block->scaled_c = quantize_bcd(c);
    block->scaled_d = quantize_bcd(d);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    float to quantize.
 * Purpose: quantizes b, c or d value into BCD_BITS bits.
 * Returns: quantized value.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
signed quantize_bcd(float n)
{
    /* copmute range represented by BCD_BITS */
    int bcd_choices = power_2(BCD_BITS) - 1;
    int upper_limit = bcd_choices / 2;
    int lower_limit = bcd_choices / 2 * -1;
    // int scale = 50;
    
    /* quantize value and clamp to range */
    signed res = round(n * bcd_choices);
    res = (signed)clamp(res, lower_limit, upper_limit);
    assert((res <= 15) && (res >= -15));
    return res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    power of 2 exponent.
 * Purpose: computes 2 ^ 'n'.
 * Returns: resulting value.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
uint64_t power_2(int shift)
{
    return ((uint64_t)1 << shift);
}


/* * * * * * * Dequantize to DCT * * * * * * */

void apply_dequant(int col, int row, UArray2_T quant_arr, void *elem, 
    void *cl);
float dequantize_a(unsigned a);
float dequantize_bcd(signed n);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArray2 of Quant structs.
 * Purpose: Compresses a UArray2 of Dct structs into a UArray2 of Quant structs.
 * Error cases:
    if 'quant_arr' passed in is a NULL pointer, we exit with CRE,
    if dynamic allocation of memory fails, we exit with CRE.
 * Returns: UArray2 of Dct structs.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T dequantize(UArray2_T quant_arr)
{
    assert(quant_arr != NULL);

    /* make UArray2 for Dct_float structs */
    int width = UArray2_width(quant_arr);
    int height = UArray2_height(quant_arr);
    UArray2_T dct_arr = UArray2_new(width, height, sizeof(struct Dct_float));
    assert(dct_arr != NULL);

    UArray2_map_row_major(quant_arr, apply_dequant, dct_arr);

    return dct_arr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    column index,
    row index,
    UArray2 being mapped through,
    element in array2[col][row],
    closure argument (dct_arr).
 * Purpose: decompresses a Quant struct to a Dct struct by dequantizing a, b, 
    c, d, index of average Pr and Pb values.
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void apply_dequant(int col, int row, UArray2_T quant_arr, void *elem, 
    void *cl)
{
    (void) quant_arr;

    UArray2_T dct_arr = (UArray2_T)cl;
    Dct_float dct_block = (Dct_float)UArray2_at(dct_arr, col, row);
    Quant block = (Quant)elem;

    unsigned scaled_a = block->scaled_a;
    signed scaled_b = block->scaled_b;
    signed scaled_c = block->scaled_c;
    signed scaled_d = block->scaled_d;

    /* dequantize cosine coefficients */
    float a = dequantize_a(scaled_a);
    float b = dequantize_bcd(scaled_b);
    float c = dequantize_bcd(scaled_c);
    float d = dequantize_bcd(scaled_d);
    dct_block->a= a;
    dct_block->b= b;
    dct_block->c= c;
    dct_block->d= d;

    /* get chroma of indicies */
    float pb = Arith40_chroma_of_index(block->ind_pb);
    float pr = Arith40_chroma_of_index(block->ind_pr);
    dct_block->avg_pb= pb;
    dct_block->avg_pr= pr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    float to quantize.
 * Purpose: quantizes a value into A_BITS bits and clamps result into
    expectes range.
 * Returns: quantized value.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float dequantize_a(unsigned a)
{
    float res = a / (pow(2, (float)A_BITS) - 1);
    res = clamp(res, 0.0, 1.0);
    return res;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    float to quantize.
 * Purpose: quantizes a value into BCD_BITS bits and clamps result into
    expectes range.
 * Returns: quantized value.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float dequantize_bcd(signed n)
{
    float res = n / (pow(2, (float)BCD_BITS) - 1);
    res = clamp(res, -0.3, 0.3);
    return res;
}