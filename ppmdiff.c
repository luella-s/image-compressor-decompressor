#include "ppmdiff.h"
#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include <math.h>

int main(int argc, char *argv[])    //TO DO: make more modular
{
    FILE *fp_1 = NULL;
    FILE *fp_2 = NULL;
    if (argc == 3) {
        /* first file is read from stdin */
        if (strcmp(argv[1], "-") == 0) {
            if (strcmp(argv[2], "-")) {
                fprintf(stderr, "Only one file can be from stdin\n");
                exit(EXIT_FAILURE);
            }
            fp_1 = stdin;
            fp_2 = open_or_abort(argv[2], "r");
        } else if (strcmp(argv[2], "-") == 0) {
            /* second file is read from stdin */
            fp_1 = open_or_abort(argv[1], "r");
            fp_2 = stdin;
        } else {
            /* no files from stdin */
            fp_1 = open_or_abort(argv[1], "r");
            fp_2 = open_or_abort(argv[2], "r");
        }
    } else {
      /* Exit if there are too many arguments */
        fprintf(stderr, "Invalid command line args\n");
        exit(EXIT_FAILURE);
    }

    /* create A2Methods suite */
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods != NULL);

    /* pnm to read image */
    Pnm_ppm img_1 = Pnm_ppmread(fp_1, methods);
    assert(img_1 != NULL);
    int img_1_width = methods->width(img_1->pixels);
    int img_1_height = methods->height(img_1->pixels);

    Pnm_ppm img_2 = Pnm_ppmread(fp_2, methods);
    assert(img_2 != NULL);
    int img_2_width = methods->width(img_2->pixels);
    int img_2_height = methods->height(img_2->pixels);

    //DEBUG
    // printf("img 1: width %d, height %d\n", img_1_width, img_1_height);
    // printf("img 2: width %d, height %d\n", img_2_width, img_2_height);

    /* Check if width and height of both images differ by at most 1.
     * Else, ppmdiff should print an error message to standard error 
     * and should print the number 1.0 to standard output.
    */
    if (abs(img_1_height - img_2_height) > 1 || 
        abs(img_1_width - img_2_width) > 1) {
        fprintf(stderr, "width and height of both images differ by at most 1");
        fprintf(stdout, "%1.0f", 1.0);
        exit(EXIT_FAILURE);
        // printf("%d", 1);
    }

    /* calculate E */
    float E = -1;
    int width = 0;
    int height = 0;

    // set_size(img_1, img_2, &width, &height);

    //TO DO: can one image have a smaller width, but a bigger height?

    if (img_1_width <= img_2_width) {
        width = img_1_width;
    } else {
        width = img_2_width;
    }
    if (img_1_height <= img_2_height) {
        height = img_1_height;
    } else {
        height = img_2_height;
    }

    E = calculate_E(width, height, img_1, img_2, methods);

    /* output E */
    printf("%1.4f\n", E);

    /* free dynamically allocated memory */
    fclose(fp_1);
    fclose(fp_2);
    Pnm_ppmfree(&img_1);
    Pnm_ppmfree(&img_2);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
   file name obtained throught the command line,
   mode to use for fopen.
 * Purpose: open a file stream to a file obtained through the command line
    and check if it's successfully opened. if not, exit the program.
 * Error cases:
    If the file isn't successfully opened, we exit with CRE.
 * Returns: a pointer to the successfully opened file stream.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static FILE *open_or_abort(char *filename, char *mode)
{
    /*
     * Open a file stream to the given file. Then, check if the file is
     * successfully opened. If not, exit the program.
     */

    FILE *fp = fopen(filename, mode);
    assert(fp != NULL);

    return fp;
}

float calculate_E(int width, int height, Pnm_ppm img_1, Pnm_ppm img_2, 
    A2Methods_T methods)
{
    float red_t = 0;
    float green_t = 0;
    float blue_t = 0;
    Pnm_rgb pixel;
    float red_1 = 0;
    float green_1 = 0;
    float blue_1 = 0;
    float red_2 = 0;
    float green_2 = 0;
    float blue_2 = 0;
    float denom_1 = (float) img_1->denominator;
    float denom_2 = (float) img_2->denominator;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            pixel = methods->at(img_1->pixels, i, j);
            red_1 = (float) pixel->red / denom_1;
            green_1 = (float) pixel->green / denom_1;
            blue_1 = (float) pixel->blue / denom_1;
            
            pixel = methods->at(img_2->pixels, i, j);
            red_2 = (float) pixel->red / denom_2;
            green_2 = (float) pixel->green / denom_2;
            blue_2 = (float) pixel->blue / denom_2;

            red_t += ((red_1 - red_2) * (red_1 - red_2));
            green_t += ((green_1 - green_2) * (green_1 - green_2));
            blue_t += ((blue_1 - blue_2) * (blue_1 - blue_2));
        }
    }

    float numerator = (red_t + green_t + blue_t);
    float res = sqrt(numerator / (3 * width * height));
    return res;
}