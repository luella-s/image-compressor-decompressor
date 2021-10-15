#include "pnmdiff.h"
#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include <math.h>

int main(int argc, char *argv[])
{
    char file_1[1000];
    char file_2[1000];
    if (argc == 3) {
        /* first file is read from stdin */
        if (strcmp(argv[1], "-") == 0) {
            if (strcmp(argv[2], "-")) {
                fprintf(stderr, "Only one file can be from stdin\n");
                exit(EXIT_FAILURE);
            }
            scanf("%s", file_1);
            strcpy(file_2, argv[2]);
        } else if (strcmp(argv[2], "-") == 0) {
            /* second file is read from stdin */
            strcpy(file_1, argv[1]);
            scanf("%s", file_2);
        } else {
            /* no files from stdin */
            strcpy(file_1, argv[1]);
            strcpy(file_2, argv[2]);
        }
    } else {
      /* Exit if there are too many arguments */
        fprintf(stderr, "Invalid command line args\n");
        exit(EXIT_FAILURE);
    }

    /* create A2Methods suite */
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);

    /* open file pointers */
    FILE *fp_1 = open_or_abort(file_1, "r");
    FILE *fp_2 = open_or_abort(file_2, "r");

    /* pnm to read image */
    Pnm_ppm img_1 = Pnm_ppmread(fp_1, methods);
    int img_1_width = methods->width(img_1->pixels);
    int img_1_height = methods->height(img_1->pixels);

    Pnm_ppm img_2 = Pnm_ppmread(fp_2, methods);
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
        fprintf(stdout, "%f", 1.0);
        exit(EXIT_FAILURE);
        // printf("%d", 1);
    }

    /* calculate E */
    double E = -1;
    int width, height;

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
    fprintf(stdout, "%0.4f\n", E);

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

double calculate_E(int width, int height, Pnm_ppm smaller, Pnm_ppm larger, 
    A2Methods_T methods)
{
    // printf("in calculate\n");
    double numerator = 0;
    double denominator = 3 * width * height;
    //find smaller image
    //call map function on it
    //cl contains Pnm_ppm or larger image & running total of summation
        //apply function accesses pixel at larger image & adds ... to sum
    //create struct to pass into cl, holding larger & summation total
    
    // struct closure *cl = malloc(sizeof(*cl));
    // cl->larger = larger;
    // cl->summation = 0;
    // smaller->methods->map_default(smaller->pixel, summation, cl);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Pnm_rgb pixel = methods->at(smaller->pixels, i, j);
            unsigned red_1 = pixel->red;
            unsigned green_1 = pixel->green;
            unsigned blue_1 = pixel->blue;

            // printf("2\n");
            
            pixel = methods->at(larger->pixels, i, j);
            unsigned red_2 = pixel->red;
            unsigned green_2 = pixel->green;
            unsigned blue_2 = pixel->blue;

            // printf("3\n");

            numerator += pow((red_1 - red_2), 2) + pow((green_1 - green_2), 2) 
                + pow((blue_1 - blue_2), 2);
        }
    }

    return sqrt(numerator / denominator);
}

// struct closure {
//     Pnm_ppm larger;
//     double summation;
// };

//apply function