#include "decompress.h"

#define DENOMINATOR 255

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

Pnm_ppm convert_to_ppm(UArray2_T arr, A2Methods_T methods) 
{
    Pnm_ppm pixmap = malloc(sizeof(*pixmap));
    pixmap->width = UArray2_width(arr);
    pixmap->height = UArray2_height(arr);
    pixmap->denominator = DENOMINATOR;
    pixmap->methods = methods;

    int width = UArray2_width(arr);
    int height = UArray2_height(arr);
    UArray2_T pixels = UArray2_new(width, height, sizeof(struct Pnm_rgb));

    UArray2_map_row_major(pixels, float_to_unsigned, arr);
    
    // UArray2_map_row_major(pixels, apply_print2, NULL);

    pixmap->pixels = pixels;

    // UArray2_map_row_major(pixels, apply_print2, NULL);

    return pixmap;
}

void apply_print2(int i, int j, UArray2_T array2, void *elem, void *cl)
{
    (void) i;
    (void) j;
    (void) array2;
    (void) cl;
    Pnm_rgb pixel = (Pnm_rgb) elem;
    fprintf(stderr, "%u %u %u\n", pixel->red, pixel->green, pixel->blue);
}

void float_to_unsigned(int i, int j, UArray2_T array2, void *elem, void *cl)
{
    (void) i;
    (void) j;
    (void) array2;

    UArray2_T old = cl;
    Rgb_float curr = UArray2_at(old, i, j);

    Pnm_rgb pixel = malloc(sizeof(*pixel));
    pixel->red = (unsigned) (curr->red * DENOMINATOR);
    pixel->green = (unsigned) (curr->green * DENOMINATOR);
    pixel->blue = (unsigned) (curr->blue * DENOMINATOR);

    // fprintf(stderr, "CURR %f %f %f\n", curr->red, curr->green, curr->blue);
    // fprintf(stderr, "PIXEL %u %u %u\n", pixel->red, pixel->green, pixel->blue);

    *(Pnm_rgb) elem = *pixel;

    // fprintf(stderr, "ELEM %u %u %u\n", ((Pnm_rgb)elem)->red, ((Pnm_rgb)elem)->green, ((Pnm_rgb)elem)->blue);
    // FREE(pixel);
}


/* input already checked to be not NULL */
// Pnm_ppm read_compress(FILE *input)
// {
//     char *type = fgets(input, );
//     UArray2_T arr = UArray2_new(NULL);
    
// }