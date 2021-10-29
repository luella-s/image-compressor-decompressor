/**************************************************************
 *
 *                     cvc_dct.c
 *
 *     Assignment: COMP40 Homework 4 - arith
 *     Authors:  Luella Sugiman (lsugim01), Yingyang Liang (yliang03)
 *     Date:     10/28/21
 *
 *     Implementation of cvc_dct module - implements functions
 *     that compresses a Pnm_ppm image containing Pnm_rgb floats
 *     into a UArray2 containing Cvc structs (holds Y, Pb, Pr)
 *     and one that decompresses it.
 *
 **************************************************************/

#include "cvc_dct.h"

/* * * * * Compress From Cvc Struct into Dct Struct * * * * */

void apply_dct(int col, int row, UArray2_T array2, void *elem, void *cl);
float avg_pb(int col, int row, UArray2_T cvc_arr);
float avg_pr(int col, int row, UArray2_T cvc_arr);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArray2 of Cvc structs
 * Purpose: Compresses a UArray2 of Cvc structs into a UArray2 of Dct structs
 * Error cases:
    if 'cvc_arr' passed in is a NULL pointer, we exit with CRE.
    if dynamic allocation of memory fails, we exit with CRE.
 * Returns: UArray2 of Dct structs.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T dct(UArray2_T cvc_arr)
{
    assert(cvc_arr != NULL);
    
    /* make UArray2 of half the width and size (to hold 2x2 blocks of pixels) */
    int width = (UArray2_width(cvc_arr) / 2);
    int height = (UArray2_height(cvc_arr) / 2);
    UArray2_T dct_arr = UArray2_new(width, height, sizeof(struct Dct_float));
    assert(dct_arr != NULL);
    
    UArray2_map_row_major(dct_arr, apply_dct, cvc_arr);

    return dct_arr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    column index,
    row index,
    UArray2 being mapped through,
    element in array2[col][row],
    closure argument (cvc_arr).
 * Purpose: Compresses a Cvc struct to a Dct struct by applying discrete
    cosine transform - calculating a, b, c and d (from Y values) and average 
    Pb and Pr values.
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void apply_dct(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    (void) array2;

    UArray2_T cvc_arr = (UArray2_T)cl;
    assert(cvc_arr != NULL);

    float y_1 = (float) ((Cvc) UArray2_at(cvc_arr, col * 2, row * 2))->y;
    float y_2 = (float) ((Cvc) UArray2_at(cvc_arr, col * 2 + 1, row * 2))->y;
    float y_3 = (float) ((Cvc) UArray2_at(cvc_arr, col * 2, row * 2 + 1))->y;
    float y_4 = (float) ((Cvc) UArray2_at(cvc_arr, col * 2 + 1, row * 2 + 1))->y;

    /* perform discrete cosine transform & clamp into 0 to 1 range */
    float a = ((y_4 + y_3 + y_2 + y_1) / 4.0);
    a = clamp(a, 0.0, 1.0);
    float b = ((y_4 + y_3 - y_2 - y_1) / 4.0);
    float c = ((y_4 - y_3 + y_2 - y_1) / 4.0);
    float d = ((y_4 - y_3 - y_2 + y_1) / 4.0);
    b = clamp(b, -0.3, 0.3);
    c = clamp(c, -0.3, 0.3);
    d = clamp(d, -0.3, 0.3);

    /* store values in Dct struct */
    Dct_float block = (Dct_float)elem;
    assert(block != NULL);
    block->a = a;
    block->b = b;
    block->c = c;
    block->d = d;

    /* compute and store average Pb, Pr */
    block->avg_pb = avg_pb(col, row, cvc_arr);
    block->avg_pr = avg_pr(col, row, cvc_arr);

    /* clamp into -0.5 to 0.5 range */
    block->avg_pb = clamp((block->avg_pb), -0.5, 0.5);
    block->avg_pr = clamp((block->avg_pr), -0.5, 0.5);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    column index,
    row index,
    cvc_arr UArray2.
 * Purpose: computes average Pb of a given block.
 * Returns: average Pb
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float avg_pb(int col, int row, UArray2_T cvc_arr)
{
    float pb_1 = ((Cvc)UArray2_at(cvc_arr, col * 2, row * 2))->pb;
    float pb_2 = ((Cvc)UArray2_at(cvc_arr, col * 2 + 1, row * 2))->pb;
    float pb_3 = ((Cvc)UArray2_at(cvc_arr, col * 2, row * 2 + 1))->pb;
    float pb_4 = ((Cvc)UArray2_at(cvc_arr, col * 2 + 1, row * 2 + 1))->pb;
    return ((pb_1 + pb_2 + pb_3 + pb_4) / 4.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    column index,
    row index,
    cvc_arr UArray2.
 * Purpose: computes average Pr of a given block.
 * Returns: average Pr
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float avg_pr(int col, int row, UArray2_T cvc_arr)
{
    float pr_1 = ((Cvc)UArray2_at(cvc_arr, col * 2, row * 2))->pr;
    float pr_2 = ((Cvc)UArray2_at(cvc_arr, col * 2 + 1, row * 2))->pr;
    float pr_3 = ((Cvc)UArray2_at(cvc_arr, col * 2, row * 2 + 1))->pr;
    float pr_4 = ((Cvc)UArray2_at(cvc_arr, col * 2 + 1, row * 2 + 1))->pr;
    return ((pr_1 + pr_2 + pr_3 + pr_4) / 4.0);
}


/* * * * * * * Decompress from Dct Struct to Cvc Struct * * * * * * */

void apply_inverse_dct(int col, int row, UArray2_T array2, void *elem, void *cl);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    UArray2 of Dct structs
 * Purpose: deompresses a UArray2 of Dct structs into a UArray2 of Cvc structs
 * Error cases:
    if 'dct_arr' passed in is a NULL pointer, we exit with CRE.
    if dynamic allocation of memory fails, we exit with CRE.
 * Returns: UArray2 of Cvc structs.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T inverse_dct(UArray2_T dct_arr)
{
    assert(dct_arr != NULL);

    /* make UArray2 of double width and height to store all 4 
     * pixels of block */
    int width = UArray2_width(dct_arr) * 2;
    int height = UArray2_height(dct_arr) * 2;
    UArray2_T cvc_arr = UArray2_new(width, height, sizeof(struct Cvc));
    assert(cvc_arr != NULL);

    UArray2_map_row_major(dct_arr, apply_inverse_dct, cvc_arr);

    return cvc_arr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    column index,
    row index,
    UArray2 being mapped through,
    element in array2[col][row],
    closure argument (dct_arr).
 * Purpose: deocmpresses a Dct struct to a Cvc struct by applying inverse
    discrete cosine transform - calculating Y, Pb and Pr values from a, b, 
    c, d, average Pb and average Pr.
 * Error cases:
    if 'elem' isn't the size of a Dct_float struct, we exit with CRE.
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void apply_inverse_dct(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    (void) array2;

    UArray2_T cvc_arr = (UArray2_T)cl;
    assert(cvc_arr != NULL);
    Dct_float block = (Dct_float)elem;
    assert(block != NULL);

    float a = block->a;
    float b = block->b;
    float c = block->c;
    float d = block->d;

    /* perform inverse discrete cosine transform */
    float y_1 = a - b - c + d;
    float y_2 = a - b + c - d;
    float y_3 = a + b - c - d;
    float y_4 = a + b + c + d;

    /* store computed values into each pixel in the block */
    float pb = block->avg_pb;
    float pr = block->avg_pr;

    ((Cvc)UArray2_at(cvc_arr, col * 2, row * 2))->y = y_1;
    ((Cvc)UArray2_at(cvc_arr, col * 2 + 1, row * 2))->y = y_2;
    ((Cvc)UArray2_at(cvc_arr, col * 2, row * 2 + 1))->y = y_3;
    ((Cvc)UArray2_at(cvc_arr, col * 2 + 1, row * 2 + 1))->y = y_4;

    for (int r = 2 * row; r <= 2 * row + 1; r++) {
        for (int c = 2 * col; c <= 2 * col + 1; c++) {
            check_bounds(c, r, cvc_arr);
            ((Cvc)UArray2_at(cvc_arr, c, r))->pb = pb;
            ((Cvc)UArray2_at(cvc_arr, c, r))->pr = pr;
        }
    }
}