#include "cvc_dct.h"

void apply_dct(int i, int j, UArray2_T array2, void *elem, void *cl);
void apply_inverse_dct(int i, int j, UArray2_T array2, void *elem, 
                            void *cl);
float avg_pb(int col, int row, UArray2_T cvc_arr);
float avg_pr(int col, int row, UArray2_T cvc_arr);

/* * * * * Apply Discrete Cosine Transform & Compute Average Values * * * * */

/* Takes in 2D array of CVC structs, packs 2x2 into a block 
 * and does discrete cosine transfomation (DCT) and computes
 * average Pb Pr. Returns 2D array of Dct_float structs.
 */
UArray2_T dct(UArray2_T cvc_arr)
{
    assert(cvc_arr != NULL);
    
    /* make uarray2 */
    int width = (UArray2_width(cvc_arr) / 2);
    int height = (UArray2_height(cvc_arr) / 2);
    UArray2_T dct_arr = UArray2_new(width, height, sizeof(struct Dct_float));
    
    UArray2_map_row_major(dct_arr, apply_dct, cvc_arr);

    return dct_arr;
}

void apply_dct(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    (void) array2;

    UArray2_T cvc_arr = (UArray2_T)cl;

    //get y1, y2, y3, y4
    float y_1 = (float) ((Cvc) UArray2_at(cvc_arr, col * 2, row * 2))->y;
    float y_2 = (float) ((Cvc) UArray2_at(cvc_arr, col * 2 + 1, row * 2))->y;
    float y_3 = (float) ((Cvc) UArray2_at(cvc_arr, col * 2, row * 2 + 1))->y;
    float y_4 = (float) ((Cvc) UArray2_at(cvc_arr, col * 2 + 1, row * 2 + 1))->y;

    //compute a, b, c, d
    float a = ((y_4 + y_3 + y_2 + y_1) / 4.0);
    clamp(a, 0.0, 1.0);
    float b = ((y_4 + y_3 - y_2 - y_1) / 4.0);
    float c = ((y_4 - y_3 + y_2 - y_1) / 4.0);
    float d = ((y_4 - y_3 - y_2 + y_1) / 4.0);
    clamp(b, -0.3, 0.3);
    clamp(c, -0.3, 0.3);
    clamp(d, -0.3, 0.3);

    Dct_float block = (Dct_float)elem;
    assert(sizeof(*block) == sizeof(struct Dct_float));

    block->a = a;
    block->b = b;
    block->c = c;
    block->d = d;

    //compute and assign average Pb, Pr
    block->avg_pb = avg_pb(col, row, cvc_arr);
    block->avg_pr = avg_pr(col, row, cvc_arr);
    clamp((block->avg_pb), -0.5, 0.5);
    clamp((block->avg_pr), -0.5, 0.5);
}

float avg_pb(int col, int row, UArray2_T cvc_arr)
{
    float pb_1 = ((Cvc)UArray2_at(cvc_arr, col * 2, row * 2))->pb;
    float pb_2 = ((Cvc)UArray2_at(cvc_arr, col * 2 + 1, row * 2))->pb;
    float pb_3 = ((Cvc)UArray2_at(cvc_arr, col * 2, row * 2 + 1))->pb;
    float pb_4 = ((Cvc)UArray2_at(cvc_arr, col * 2 + 1, row * 2 + 1))->pb;
    return ((pb_1 + pb_2 + pb_3 + pb_4) / 4.0);
}

float avg_pr(int col, int row, UArray2_T cvc_arr)
{
    float pr_1 = ((Cvc)UArray2_at(cvc_arr, col * 2, row * 2))->pr;
    float pr_2 = ((Cvc)UArray2_at(cvc_arr, col * 2 + 1, row * 2))->pr;
    float pr_3 = ((Cvc)UArray2_at(cvc_arr, col * 2, row * 2 + 1))->pr;
    float pr_4 = ((Cvc)UArray2_at(cvc_arr, col * 2 + 1, row * 2 + 1))->pr;
    return ((pr_1 + pr_2 + pr_3 + pr_4) / 4.0);
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
            check_bounds(col, row, cvc_arr);
            ((Cvc)UArray2_at(cvc_arr, col, row))->pb = pb;
            ((Cvc)UArray2_at(cvc_arr, col, row))->pr = pr;
        }
    }
}