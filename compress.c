#include "compress.h"

#define A_BITS 9
#define BCD_BITS 5

/* * * * * * * Pnm_ppm File to Component-video Color Space Array * * * * * * */

UArray2_T convert_to_cvc(Pnm_ppm img)
{
    trim_ppm(img);
    UArray2_T array = UArray2_new(img->width, img->height, sizeof(struct Cvc));
    assert(array != NULL);
    img->methods->map_row_major(array, unsigned_to_cvc, img);
    return array;
}

/* If the width and height of the ppm is odd, trim last column or 
 * row to make them even
 */
void trim_ppm(Pnm_ppm img)
{
    if (img->width % 2 != 0) {
        img->width -= 1;
    }
    if (img->height % 2 != 0) {
        img->height -= 1;
    }
}

void unsigned_to_cvc(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl)
{
    (void) array2;
    Cvc cvc = malloc(sizeof(*cvc));
    Pnm_ppm img = cl;
    Pnm_rgb pixel = img->methods->at(img->pixels, i, j);
    float denom = (float) img->denominator;

    float r = (float)pixel->red / (float)denom;
    float g = (float)pixel->green / (float)denom;
    float b = (float)pixel->blue / (float)denom;
    // fprintf(stderr, "%f %f %f\n", r, g, b);
    // clamp(&r, 0, 1);
    // clamp(&g, 0, 1);
    // clamp(&b, 0, 1);
    // fprintf(stderr, "%f %f %f\n", r, g, b);


    double y = ((0.299 * r) + (0.587 * g) + (0.114 * b));
    double pb = ((-0.168736 * r) - (0.331264 * g) + (0.5 * b));
    double pr = ((0.5 * r) - (0.418688 * g) - (0.081312 * b));
    // clamp(&y, 0, 1);
    // clamp(&pb, -0.5, 0.5);
    // clamp(&pr, -0.5, 0.5);
    cvc->y = (float)y;
    cvc->pb = (float)pb;
    cvc->pr = (float)pr;

    *(Cvc)elem = *cvc;
    free(cvc);
}

/* * * * * Apply Discrete Cosine Transform & Compute Average Values * * * * */

/* Takes in 2D array of CVC structs, packs 2x2 into a block 
 * and does discrete cosine transfomation (DCT) and computes
 * average Pb Pr. Returns 2D array of Dct_float structs.
 */
UArray2_T dct(UArray2_T cvc_arr)
{
    assert(cvc_arr != NULL);
    
    /* make uarray2 */
    int width = UArray2_width(cvc_arr) / 2;
    int height = UArray2_height(cvc_arr) / 2;
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


/* * * * * * * Quantize to Chroma Indixes & Cosine Coefficients * * * * * * */

UArray2_T quantize(UArray2_T dct_arr)
{
    assert(dct_arr != NULL);
    
    /* make uarray2 */
    int width = UArray2_width(dct_arr);
    int height = UArray2_height(dct_arr);
    UArray2_T quant_arr = UArray2_new(width, height, sizeof(struct Quant));
    
    /* 
     * Module 1:
     * get index of avg chroma values and quantize a, b, c, d
     */
    UArray2_map_row_major(quant_arr, apply_quantize, dct_arr);
    
    /* 
     * Module 2:
     * quantize a, b, c, d
     */
    // UArray2_map_row_major(quant_arr, quantize_abcd, dct_arr);

    // UArray2_map_row_major(quant_arr, print_chroma, NULL);

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
    int a_choices = (pow(2, (float)A_BITS) - 1);
    block->scaled_a = floor(a * a_choices);
    assert(block->scaled_a <= (unsigned)a_choices);

    /* assertion for ranges in quantize_bcd */
    block->scaled_b = quantize_bcd(b);
    block->scaled_c = quantize_bcd(c);
    block->scaled_d = quantize_bcd(d);
    
    *((Quant)elem) = *block;
    free(block);
}

signed quantize_bcd(unsigned n)
{
    int bcd_choices = pow(2, (float)BCD_BITS) - 1;
    signed res = floor(n * bcd_choices);
    assert(res >= (bcd_choices / 2 * -1) && res < (bcd_choices / 2));
    return res;
}

/* * * * * * * Helper functions * * * * * * */

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