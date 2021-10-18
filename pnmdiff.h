#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assert.h"
#include "pnm.h"

static FILE *open_or_abort(char *filename, char *mode);
<<<<<<< HEAD
float calculate_E(int width, int height, Pnm_ppm img_1, Pnm_ppm img_2, 
    A2Methods_T methods);
=======
double calculate_E(int width, int height, Pnm_ppm img_1, Pnm_ppm img_2, A2Methods_T methods);
>>>>>>> 7926a726cddc872cffb44b959e312e3d2dc8bed9
