#ifndef PPMDIFF_INCLUDED
#define PPMDIFF_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assert.h"
#include "pnm.h"

static FILE *open_or_abort(char *filename, char *mode);
float calculate_E(int width, int height, Pnm_ppm img_1, Pnm_ppm img_2, 
                    A2Methods_T methods);

#endif