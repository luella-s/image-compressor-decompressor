#include "compress.h"

// UArray2_T convert_to_scaled(Pnm_ppm img)
// {

// }
typedef struct rgb_float {
    float red;
    float green;
    float blue;
} *rgb_float;

UArray2_T convert_to_scaled_float(Pnm_ppm img)
{
    int width = img->width;
    int height = img->height;
    UArray2_T array = UArray2_new(width, height, sizeof(struct rgb_float));
    Pnm_rgb pixel;
    // float red_1, green_1, blue_1, red_2, green_2, blue_2;
    float denom_1 = (float) img->denominator;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            pixel = img->methods->at(img->pixels, i, j);
            ((struct rgb_float*)UArray2_at(array, i, j))->red = (float) pixel->red / denom_1;
            ((struct rgb_float*)UArray2_at(array, i, j))->green = (float) pixel->green / denom_1;
            ((struct rgb_float*)UArray2_at(array, i, j))->blue = (float) pixel->blue / denom_1;
        }
    }
    return array;
}

void apply_print(int i, int j, UArray2_T array2, 
                                      void *elem, void *cl)
{
    (void) i;
    (void) j;
    (void) array2;
    (void) cl;
    rgb_float pixel = elem;
    printf("%f\n", pixel->red);
    printf("%f\n", pixel->green);
    printf("%f\n", pixel->blue);
    
}

void print_compressed(UArray2_T arr)
{
    printf("%d %d", UArray2_width(arr), UArray2_height(arr));
    UArray2_map_row_major(arr, apply_print, NULL);
}