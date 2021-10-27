#include "print_read.h"

void print_binary(UArray2_T decode_arr)
{
    assert(decode_arr != NULL);
    unsigned width = UArray2_width(decode_arr) * 2;
    unsigned height = UArray2_height(decode_arr) * 2;
    printf("COMP40 Compressed image format 2\n%u %u\n", width, height);
    UArray2_map_row_major(decode_arr, apply_print, NULL);
}

void apply_print(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    for (int i = 0; i < WORD_SIZE / 4; i++) {
        putc(stdout);
    }
}

void read_binary(FILE *fp)
{
    
}