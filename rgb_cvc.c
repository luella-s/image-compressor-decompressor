/**************************************************************
 *
 *                     rgb_cvc.c
 *
 *     Assignment: COMP40 Homework 4 - arith
 *     Authors:  Luella Sugiman (lsugim01), Yingyang Liang (yliang03)
 *     Date:     10/28/21
 *
 *     Implementation of rgb_cvc module - implements functions
 *     that compresses a Pnm_ppm image containing Pnm_rgb floats
 *     into a UArray2 containing Cvc structs (holds Y, Pb, Pr)
 *     and one that decompresses it.
 *
 **************************************************************/

#include "rgb_cvc.h"

#define DENOMINATOR 255

/* * * * Compress Pnm_ppm File to Component-video Color Array * * * */

void trim_ppm(Pnm_ppm img);
void unsigned_to_cvc(int col, int row, A2Methods_UArray2 array2, void *elem, 
    void *cl);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    Pnm_ppm image.
 * Purpose: Compresses a Pnm_ppm image into a UArray2 of Cvc structs
 * Error cases:
    if the image passed in is a NULL pointer, we exit with CRE.
    if dynamic allocation of memory fails, we exit with CRE.
 * Returns: UArray2 of Cvc structs.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UArray2_T convert_to_cvc(Pnm_ppm img)
{
    assert(img != NULL);
    trim_ppm(img);

    UArray2_T array = UArray2_new(img->width, img->height, sizeof(struct Cvc));
    assert(array != NULL);

    img->methods->map_row_major(array, unsigned_to_cvc, img);

    return array;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    column index,
    row index,
    UArray2 being mapped through,
    element in array2[col][row],
    closure argument (Pnm_ppm image).
 * Purpose: Compresses a Pnm_rgb pixel into a Cvc pixel by scaling
    RGB values, casting them into floats and converting the values into
    Y, Pb and Pr values that's stored in a Cvc struct.
 * Error cases:
    if the image passed in is a NULL pointer, we exit with CRE.
    if dynamic allocation of memory fails, we exit with CRE.
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void unsigned_to_cvc(int col, int row, A2Methods_UArray2 array2, void *elem, 
    void *cl)
{
    (void) array2;

    Cvc cvc = malloc(sizeof(*cvc));
    assert(cvc != NULL);

    Pnm_ppm img = cl;
    Pnm_rgb pixel = img->methods->at(img->pixels, col, row);
    float denom = (float) img->denominator;

    /* convert unscaled unisgned RGB values into scaled floats */
    float r = (float)pixel->red / (float)denom;
    float g = (float)pixel->green / (float)denom;
    float b = (float)pixel->blue / (float)denom;
    
    /* calculate Y, Pb and Pr */
    double y = ((0.299 * r) + (0.587 * g) + (0.114 * b));
    double pb = ((-0.168736 * r) - (0.331264 * g) + (0.5 * b));
    double pr = ((0.5 * r) - (0.418688 * g) - (0.081312 * b));
    cvc->y = (float)y;
    cvc->pb = (float)pb;
    cvc->pr = (float)pr;

    /* store struct in the UArray2 */
    *(Cvc)elem = *cvc;
    free(cvc);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    Pnm_ppm image.
 * Purpose: Trims widths or lengths of the image into even values.
 * Returns: Nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void trim_ppm(Pnm_ppm img)
{
    if (img->width % 2 != 0) {
        img->width -= 1;
    }
    if (img->height % 2 != 0) {
        img->height -= 1;
    }
}


/* * * * Decompress Component-video Color Array to Pnm_ppm File * * * */

void apply_cvc_to_unsigned(int col, int row, UArray2_T array2, void *elem, 
    void *cl);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    Pnm_ppm image.
 * Purpose: Decompresses a Uarray2 of Cvc structs into a Pnm_ppm image.
 * Error cases:
    if the UArray2 or A2Methods passed in is a NULL pointer, we exit with CRE.
    if dynamic allocation of memory fails, we exit with CRE.
 * Returns: Pnm_ppm image.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Pnm_ppm cvc_to_ppm(UArray2_T cvc_arr, A2Methods_T methods) 
{
    assert(cvc_arr != NULL);
    assert(methods != NULL);

    Pnm_ppm pixmap = malloc(sizeof(*pixmap));
    assert(pixmap != NULL);

    /* initialize values of Pnm_ppm */
    pixmap->width = UArray2_width(cvc_arr);
    pixmap->height = UArray2_height(cvc_arr);
    pixmap->denominator = DENOMINATOR;
    pixmap->methods = methods;
    int width = UArray2_width(cvc_arr);
    int height = UArray2_height(cvc_arr);
    UArray2_T pixels = UArray2_new(width, height, sizeof(struct Pnm_rgb));

    UArray2_map_row_major(pixels, apply_cvc_to_unsigned, cvc_arr);
    pixmap->pixels = pixels;

    return pixmap;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    column index,
    row index,
    UArray2 being mapped through,
    element in array2[col][row],
    closure argument (cvc_arr).
 * Purpose: Deompresses a Cvc pixel into a Pnm_rgb pixel by converting Cvc 
    values into RGB values and casting them into unsigned integers.
 * Error cases:
    if dynamic allocation of memory fails, we exit with CRE.
 * Returns: Nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void apply_cvc_to_unsigned(int col, int row, UArray2_T array2, void *elem, 
     void *cl)
{
    (void) col;
    (void) row;
    (void) array2;

    UArray2_T cvc_arr = cl;
    Cvc cvc = UArray2_at(cvc_arr, col, row);
    float y = cvc->y;
    float pb = cvc->pb;
    float pr = cvc->pr;
    
    /* convert RGB values using given formulas */
    float r = ((1.0 * y) + (0.0 * pb) + (1.402 * pr));
    float g = ((1.0 * y) - (0.344136 * pb) - (0.714136 * pr));
    float b = ((1.0 * y) + (1.772 * pb) + (0.0 * pr));

    /* clamp rgb values to be in between 0 and 1 */
    r = clamp(r, 0.0, 1.0);
    g = clamp(g, 0.0, 1.0);
    b = clamp(b, 0.0, 1.0);

    Pnm_rgb pixel = malloc(sizeof(*pixel));
    assert(pixel != NULL);

    /* update Pnm_rgb values & store within UArray2 */
    pixel->red = (unsigned)(r * (float)DENOMINATOR);
    pixel->green = (unsigned)(g * (float)DENOMINATOR);
    pixel->blue = (unsigned)(b * (float)DENOMINATOR);
    *(Pnm_rgb)elem = *pixel;
    free(pixel);
}