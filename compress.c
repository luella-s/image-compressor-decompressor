#include "compress.h"
#include "rgb_cvc.h"

#define A_BITS 9
#define BCD_BITS 5

/* * * * * * * Helper functions * * * * * * */



void check_bounds(int col, int row, UArray2_T cvc_arr)
{
    assert(row < UArray2_height(cvc_arr));
    assert(col < UArray2_width(cvc_arr));
}

float clamp(float num, float lower, float upper)
{
    if (num > upper) {
        return upper;
    } else if (num < lower) {
        return lower;
    }
    return num;
}

/* * * * * * * Test functions * * * * * * */

void apply_print(int i, int j, UArray2_T array2, void *elem, void *cl)
{
    (void) i;
    (void) j;
    (void) array2;
    (void) cl;
    (void) elem;
    // Cvc pixel = elem;
    // printf("%f\n", pixel->red);
    // printf("%f\n", pixel->green);
    // printf("%f\n", pixel->blue);
}

void print_compressed(UArray2_T arr)
{
    printf("float\n");
    printf("%d %d\n", UArray2_width(arr), UArray2_height(arr));
    UArray2_map_row_major(arr, apply_print, NULL);
}

void print_chroma(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    (void) col;
    (void) row;
    (void) array2;
    (void) cl;
    Quant block = (Quant)elem;
    unsigned ind_pb = block->ind_pb;
    unsigned ind_pr = block->ind_pr;
    fprintf(stderr, "%u %u\n", ind_pb, ind_pr);
}