#include "compress.h"

typedef struct Rgb_float {
    float red;
    float green;
    float blue;
} *Rgb_float;

typedef struct Cvc {
    float y;
    float p_b;
    float p_r;
} *Cvc;

typedef struct Quant {
    float a;
    float b;
    float c;
    float d;
    int ind_pb;
    int ind_pr;
} *Quant;

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

UArray2_T convert_to_scaled_float(Pnm_ppm img)
{
    trim_ppm(img);
    UArray2_T array = UArray2_new(img->width, img->height, sizeof(struct Rgb_float));
    assert(array != NULL);
    img->methods->map_row_major(array, unsigned_to_float, img);
    return array;
}

void unsigned_to_float(int i, int j, A2Methods_UArray2 array2, void *elem, void *cl)
{
    (void) array2;
    Rgb_float rgb_f = malloc(sizeof(*rgb_f));
    Pnm_ppm img = cl;
    Pnm_rgb pixel = img->methods->at(img->pixels, i, j);
    int denom = img->denominator;

    rgb_f->red = (float) pixel->red / denom;
    rgb_f->green = (float) pixel->green / denom;
    rgb_f->blue = (float) pixel->blue / denom;
    fprintf(stderr, "%d %d %d\n", pixel->red, pixel->green, pixel->blue);
    // printf("FLOAT %f %f %f\n", rgb_f->red, rgb_f->green, rgb_f->blue);

    *(Rgb_float)elem = *rgb_f;
    // printf("ELEM %f %f %f\n", ((Rgb_float)elem)->red, ((Rgb_float)elem)->green, ((Rgb_float)elem)->blue);
}

void apply_print(int i, int j, UArray2_T array2, void *elem, void *cl)
{
    (void) i;
    (void) j;
    (void) array2;
    (void) cl;
    Rgb_float pixel = elem;
    printf("%f\n", pixel->red);
    printf("%f\n", pixel->green);
    printf("%f\n", pixel->blue);
}

void print_compressed(UArray2_T arr)
{
    printf("float\n");
    printf("%d %d\n", UArray2_width(arr), UArray2_height(arr));
    UArray2_map_row_major(arr, apply_print, NULL);
}