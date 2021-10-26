#include "decompress.h"
#include "rgb_cvc.h"

// #define DENOMINATOR 255
#define A_BITS 9
#define BCD_BITS 5








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