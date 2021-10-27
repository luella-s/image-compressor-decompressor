#include "print_read.h"

#define WORD_SIZE 32

void apply_print(int col, int row, UArray2_T array2, void *elem, void *cl);

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
    (void) col;
    (void) row;
    (void) array2;
    (void) cl;

    uint32_t *word = (uint32_t *)elem;

    fprintf(stderr, "%u\n", *word);

    for (int i = 0; i < WORD_SIZE / 8; i++) {
        putc(*word, stdout);
    }
}

// void read_binary(FILE *fp)
// {
    
// }