#include "dct_quant.h"

#define A_BITS 9
#define BCD_BITS 5

/* Function declarations */

/* * * * * * * Quantize to Chroma Indixes & Cosine Coefficients * * * * * * */

UArray2_T quantize(UArray2_T dct_arr)
{
    assert(dct_arr != NULL);
    
    /* make uarray2 */
    int width = UArray2_width(dct_arr);
    int height = UArray2_height(dct_arr);
    UArray2_T quant_arr = UArray2_new(width, height, sizeof(struct Quant));
    
    UArray2_map_row_major(quant_arr, apply_quantize, dct_arr);

    return quant_arr;
}

void apply_quantize(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    (void) array2;

    Quant block = malloc(sizeof(*block));
    UArray2_T dct_arr = (UArray2_T)cl;
    Dct_float dct_elem = (Dct_float)UArray2_at(dct_arr, col, row);

    /* Values already clamped in dct to be in expected ranges */
    float avg_pb = dct_elem->avg_pb;
    float avg_pr = dct_elem->avg_pr;
    float a = dct_elem->a;
    float b = dct_elem->b;
    float c = dct_elem->c;
    float d = dct_elem->d;

    /* Computing indicies of average Pb and Pr */
    unsigned ind_pb = Arith40_index_of_chroma(avg_pb);
    unsigned ind_pr = Arith40_index_of_chroma(avg_pr);
    block->ind_pb = ind_pb;
    block->ind_pr = ind_pr;
    // fprintf(stderr, "%f %f\n", avg_pb, avg_pr);
    // fprintf(stderr, "%u %u\n", block->ind_pb, block->ind_pr);
    // fprintf(stderr, "%u %u\n", ind_pb, ind_pr);

    /* Quantizing a, b, c, d */
    int a_choices = (power_2(A_BITS) - 1);
    unsigned scaled_a = round(a * a_choices);
    block->scaled_a = scaled_a;
    assert(block->scaled_a <= (unsigned)a_choices);

    /* assertion for ranges in quantize_bcd */
    block->scaled_b = quantize_bcd(b);
    block->scaled_c = quantize_bcd(c);
    block->scaled_d = quantize_bcd(d);
    
    *((Quant)elem) = *block;
    free(block);
}

signed quantize_bcd(float n)
{
    int bcd_choices = power_2(BCD_BITS) - 1;
    signed res = round(n * bcd_choices);
    clamp(res, (bcd_choices / 2), (bcd_choices / 2 * -1));
    // assert(res >= (bcd_choices / 2 * -1) && res < (bcd_choices / 2));
    return res;
}

uint64_t power_2(int shift)
{
    return ((uint64_t)1 << shift);
}

/* * * * * * * Dequantize to DCT * * * * * * */

UArray2_T dequantize(UArray2_T quant_arr)
{
    assert(quant_arr != NULL);
    /* make uarray2 */
    int width = UArray2_width(quant_arr);
    int height = UArray2_height(quant_arr);
    UArray2_T dct_arr = UArray2_new(width, height, sizeof(struct Dct_float));

    UArray2_map_row_major(quant_arr, apply_dequant, dct_arr);

    return dct_arr;
}

/* 
 * Dequantizing a, b, c, d and get chroma of indices. 
 * Going from Quant to Dct_float.
 */
void apply_dequant(int col, int row, UArray2_T quant_arr, void *elem, 
                    void *cl)
{
    (void) quant_arr;

    UArray2_T dct_arr = (UArray2_T)cl;
    Dct_float dct_block = (Dct_float)UArray2_at(dct_arr, col, row);
    Quant block = (Quant)elem;
    assert(sizeof(*block) == sizeof(struct Quant));

    unsigned scaled_a = block->scaled_a;
    signed scaled_b = block->scaled_b;
    signed scaled_c = block->scaled_c;
    signed scaled_d = block->scaled_d;

    /* Dequantize cosine coefficients */
    float a = dequantize_a(scaled_a);
    float b = dequantize_bcd(scaled_b);
    float c = dequantize_bcd(scaled_c);
    float d = dequantize_bcd(scaled_d);
    dct_block->a= a;
    dct_block->b= b;
    dct_block->c= c;
    dct_block->d= d;

    /* Get chroma of indicies */
    float pb = Arith40_chroma_of_index(block->ind_pb);
    float pr = Arith40_chroma_of_index(block->ind_pr);
    dct_block->avg_pb= pb;
    dct_block->avg_pr= pr;
}

/* 
 * Takes in scaled a and returns float a 
 * Also clamps float a to be between 0 and 1.
 */
float dequantize_a(unsigned a)
{
    float res = a / (pow(2, (float)A_BITS) - 1);
    res = clamp(res, 0.0, 1.0);
    return res;
}

/* 
 * Takes in scaled cosine coefficients b, c or d and returns their
 * float representation. 
 * Also clamps floats to be between -0.3 and 0.3
 */
float dequantize_bcd(signed n)
{
    float res = n / (pow(2, (float)BCD_BITS) - 1);
    res = clamp(res, -0.3, 0.3);
    return res;
}