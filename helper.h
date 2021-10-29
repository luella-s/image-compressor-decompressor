#ifndef HELPER_INCLUDED
#define HELPER_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include "stdio.h"
#include "uarray2.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    column index,
    row index,
    UArray2 to check bounds of.
 * Purpose: Checks if 'col' and 'row' are within the bounds of 
    width and height of 'arr'.
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void check_bounds(int col, int row, UArray2_T arr);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    float to clamp,
    lower bound,
    upper bound.
 * Purpose: clamps values to 'lower' and 'upper' respectively
 * Returns: clamped value
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float clamp(float num, float lower, float upper);

#endif