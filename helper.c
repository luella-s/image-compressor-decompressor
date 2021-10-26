#include "helper.h"

float clamp(float n, float lower, float higher)
{
    if (n < lower) {
        return lower;
    } else if (n > higher) {
        return higher;
    }
    return n;
}

void check_bounds(int col, int row, UArray2_T arr)
{
    assert(row < UArray2_height(arr));
    assert(col < UArray2_width(arr));
}