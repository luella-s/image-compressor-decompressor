#include "rgb_cvc.h"

#define DENOMINATOR 255

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
    
    double y = ((0.299 * r) + (0.587 * g) + (0.114 * b));
    double pb = ((-0.168736 * r) - (0.331264 * g) + (0.5 * b));
    double pr = ((0.5 * r) - (0.418688 * g) - (0.081312 * b));
    cvc->y = (float)y;
    cvc->pb = (float)pb;
    cvc->pr = (float)pr;

    *(Cvc)elem = *cvc;
    free(cvc);
}

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
    r = clamp(r, 0.0, 1.0);
    g = clamp(g, 0.0, 1.0);
    b = clamp(b, 0.0, 1.0);

    Pnm_rgb pixel = malloc(sizeof(*pixel));
    pixel->red = (unsigned)(r * (float)DENOMINATOR);
    pixel->green = (unsigned)(g * (float)DENOMINATOR);
    pixel->blue = (unsigned)(b * (float)DENOMINATOR);
    
    *(Pnm_rgb)elem = *pixel;
    free(pixel);
}