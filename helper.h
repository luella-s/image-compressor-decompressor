#ifndef HELPER_INCLUDED
#define HELPER_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include "stdio.h"
#include "uarray2.h"

void check_bounds(int col, int row, UArray2_T arr);
float clamp(float num, float lower, float upper);

#endif