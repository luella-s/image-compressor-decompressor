#include "decompress.h"

#define DENOMINATOR 255
#define A_BITS 9
#define BCD_BITS 5


/* * * * * * * Component-video Color Space Array to Pnm_ppm File * * * * * * */

Pnm_ppm cvc_to_ppm(UArray2_T arr, A2Methods_T methods) 
{
    Pnm_ppm pixmap = malloc(sizeof(*pixmap));
    pixmap->width = UArray2_width(arr);
    pixmap->height = UArray2_height(arr);
    pixmap->denominator = DENOMINATOR;
    pixmap->methods = methods;

    int width = UArray2_width(arr);
    int height = UArray2_height(arr);
    UArray2_T pixels = UArray2_new(width, height, sizeof(struct Pnm_rgb));

    UArray2_map_row_major(pixels, apply_cvc_to_unsigned, arr);
    pixmap->pixels = pixels;

    return pixmap;
}

void apply_cvc_to_unsigned(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    (void) col;
    (void) row;
    (void) array2;

    UArray2_T old = cl;
    Cvc cvc = UArray2_at(old, col, row);
    float y = cvc->y;
    float pb = cvc->pb;
    float pr = cvc->pr;
    
    float r = ((1.0 * y) + (0.0 * pb) + (1.402 * pr));
    float g = ((1.0 * y) - (0.344136 * pb) - (0.714136 * pr));
    float b = ((1.0 * y) + (1.772 * pb) + (0.0 * pr));

    /* Ensuring rgb values are inbetween 0 and 1 */
    r = clamps(r, 0.0, 1.0);
    g = clamps(g, 0.0, 1.0);
    b = clamps(b, 0.0, 1.0);

    Pnm_rgb pixel = malloc(sizeof(*pixel));
    pixel->red = (unsigned)(r * (float)DENOMINATOR);
    pixel->green = (unsigned)(g * (float)DENOMINATOR);
    pixel->blue = (unsigned)(b * (float)DENOMINATOR);
    
    *(Pnm_rgb)elem = *pixel;
    free(pixel);
}

/* * * * * * * Apply Inverse Discrete Cosine Transform * * * * * * */

UArray2_T inverse_dct(UArray2_T dct_arr)
{
    assert(dct_arr != NULL);
    /* make uarray2 */
    int width = UArray2_width(dct_arr) * 2;
    int height = UArray2_height(dct_arr) * 2;
    UArray2_T cvc_arr = UArray2_new(width, height, sizeof(struct Cvc));

    UArray2_map_row_major(dct_arr, apply_inverse_dct, cvc_arr);

    return cvc_arr;
}

void apply_inverse_dct(int i, int j, UArray2_T array2, void *elem, void *cl)
{
    (void) array2;

    UArray2_T cvc_arr = (UArray2_T)cl;
    Dct_float block = (Dct_float)elem;
    assert(sizeof(*block) == sizeof(struct Dct_float));

    float a = block->a;
    float b = block->b;
    float c = block->c;
    float d = block->d;

    float y_1 = a - b - c + d;
    float y_2 = a - b + c - d;
    float y_3 = a + b - c - d;
    float y_4 = a + b + c + d;

    float pb = block->avg_pb;
    float pr = block->avg_pr;

    ((Cvc)UArray2_at(cvc_arr, i * 2, j * 2))->y = y_1;
    ((Cvc)UArray2_at(cvc_arr, i * 2 + 1, j * 2))->y = y_2;
    ((Cvc)UArray2_at(cvc_arr, i * 2, j * 2 + 1))->y = y_3;
    ((Cvc)UArray2_at(cvc_arr, i * 2 + 1, j * 2 + 1))->y = y_4;

    for (int row = 2 * j; row <= 2 * j + 1; row++) {
        for (int col = 2 * i; col <= 2 * i + 1; col++) {
            check_bound(col, row, cvc_arr);
            ((Cvc)UArray2_at(cvc_arr, col, row))->pb = pb;
            ((Cvc)UArray2_at(cvc_arr, col, row))->pr = pr;
        }
    }
}

void check_bound(int col, int row, UArray2_T cvc_arr)
{
    assert(row < UArray2_height(cvc_arr));
    assert(col < UArray2_width(cvc_arr));
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
    res = clamps(res, 0.0, 1.0);
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
    res = clamps(res, -0.3, 0.3);
    return res;
}

// UArray2_T dequantize_values(UArray2_T quant_arr)
// {
//     assert(quant_arr != NULL);
//     /* make uarray2 */
//     int width = UArray2_width(quant_arr) * 2;
//     int height = UArray2_height(quant_arr) * 2;
//     UArray2_T cvc_arr = UArray2_new(width, height, sizeof(struct Cvc));
    
//     /* 
//      * Module 1:
//      * maps through 2D array of quantized values
//      * compute pb, pr values from
//      * index of chroma values
//      */
//     UArray2_map_row_major(quant_arr, apply_dequant_chroma, cvc_arr);
    
//     /* 
//      * Module 2:
//      * compute a, b, c, d from Y
//      * quantize a, b, c, d
//      */
//      UArray2_map_row_major(quant_arr, apply_dequant_dct, cvc_arr);

//     //  UArray2_map_row_major(quant_arr, print_chroma, NULL);
//     return cvc_arr;
// }

// void apply_dequant_dct(int col, int row, UArray2_T array2, void *elem, void *cl)
// {
//     (void) array2;

//     UArray2_T cvc_arr = (UArray2_T)cl;
//     Quant block = (Quant)elem;
//     assert(sizeof(*block) == sizeof(struct Quant));

//     float a = block->a;
//     float b = block->b;
//     float c = block->c;
//     float d = block->d;

//     float y_1 = a - b - c + d;
//     float y_2 = a - b + c - d;
//     float y_3 = a + b - c - d;
//     float y_4 = a + b + c + d;

//     ((Cvc)UArray2_at(cvc_arr, col * 2, row * 2))->y = y_1;
//     ((Cvc)UArray2_at(cvc_arr, col * 2 + 1, row * 2))->y = y_2;
//     ((Cvc)UArray2_at(cvc_arr, col * 2, row * 2 + 1))->y = y_3;
//     ((Cvc)UArray2_at(cvc_arr, col * 2 + 1, row * 2 + 1))->y = y_4;
// }

// void apply_dequant_chroma(int i, int j, UArray2_T array2, void *elem, void *cl)
// {
//     (void) array2;
//     (void) elem;

//     UArray2_T cvc_arr = (UArray2_T)cl;
//     Quant block = (Quant)elem;
    
//     float pb = Arith40_chroma_of_index(block->ind_pb);
//     float pr = Arith40_chroma_of_index(block->ind_pr);
//     // fprintf(stderr, "%u %u\n", block->ind_pb, block->ind_pr);
//     fprintf(stderr, "%f %f\n", pb, pr);


//     for (int row = 2 * j; row <= 2 * j + 1; row++) {
//         for (int col = 2 * i; col <= 2 * i + 1; col++) {
//             check_bound(col, row, cvc_arr);
//             Cvc pixel = malloc(sizeof(*pixel));
//             pixel->pb = pb;
//             pixel->pr = pr;
//             // fprintf(stderr, "%f %f\n", pixel->p_b, pixel->p_r);
//             *((Cvc)UArray2_at(cvc_arr, col, row)) = *pixel;
//             free(pixel);
//             // fprintf(stderr, "%f %f\n", ((Cvc)UArray2_at(cvc_arr, col, row))->p_b, ((Cvc)UArray2_at(cvc_arr, col, row))->p_r);
//         }
//     }
// }

/* * * * * * * Helper functions * * * * * * */

float clamps(float n, float lower, float higher)
{
    if (n < lower) {
        return lower;
    } else if (n > higher) {
        return higher;
    }
    return n;
}

// void clamp_unsigned(unsigned *num, float upper, float lower)
// {
//     if (*num > upper) {
//         *num = upper;
//     } else if (*num < lower) {
//         *num = lower;
//     }
// }

/* * * * * * * Test functions * * * * * * */

void apply_print_decomp(int i, int j, UArray2_T array2, void *elem, void *cl)
{
    (void) i;
    (void) j;
    (void) array2;
    (void) cl;
    Pnm_rgb pixel = (Pnm_rgb) elem;
    fprintf(stderr, "%u %u %u\n", pixel->red, pixel->green, pixel->blue);
}