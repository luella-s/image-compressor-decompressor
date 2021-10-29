#include "helper.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    float to clamp,
    lower bound,
    upper bound.
 * Purpose: clamps values to 'lower' and 'upper' respectively
 * Returns: clamped value
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float clamp(float n, float lower, float higher)
{
    if (n < lower) {
        return lower;
    } else if (n > higher) {
        return higher;
    }
    return n;
}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arguments:
    column index,
    row index,
    UArray2 to check bounds of.
 * Purpose: Checks if 'col' and 'row' are within the bounds of 
    width and height of 'arr'.
 * Returns: nothing.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void check_bounds(int col, int row, UArray2_T arr)
{
    assert(row < UArray2_height(arr));
    assert(col < UArray2_width(arr));
}