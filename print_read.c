#include "print_read.h"

#define WORD_SIZE 32

void apply_print(int col, int row, UArray2_T array2, void *elem, void *cl);
void index_convert(UArray2_T array2, unsigned index, int *col, int *row);

void print_binary(UArray2_T coded_arr)
{
    assert(coded_arr != NULL);
    unsigned width = UArray2_width(coded_arr) * 2;
    unsigned height = UArray2_height(coded_arr) * 2;
    printf("COMP40 Compressed image format 2\n%u %u\n", width, height);
    UArray2_map_row_major(coded_arr, apply_print, NULL);
}

void apply_print(int col, int row, UArray2_T array2, void *elem, void *cl)
{
    (void) col;
    (void) row;
    (void) array2;
    (void) cl;

    uint32_t word = *(uint32_t *)elem;
    // fprintf(stderr, "The bytes for %02X are ", word);
    // fprintf(stderr, "%02X\n", word);
    unsigned lsb;

    for (int i = (WORD_SIZE / 8 - 1); i >= 0; i--) {
        lsb = i * 8;
        uint64_t field = Bitpack_getu(word, 8, lsb);
        // fprintf(stderr, "uint64: %lu", field);
        // fprintf(stderr, "%02lX", field);
        putchar(field);
    }
    // fprintf(stderr, "\n");
}

void index_convert(UArray2_T array2, unsigned index, int *col, int *row)
{	
	assert(array2 != NULL);

	
	int width = UArray2_width(array2);
	int height = UArray2_height(array2);
    int i = index % width;
    int j = index / width;
    // fprintf(stderr, "index: %u, col: %d, row: %d\n", index, i, j);

	/* Assert that index given is within 2D array dimensions */
	assert((i < width) && (i >= 0) && (j < height) && (j >= 0));
    *col = i;
    *row = j;
}

UArray2_T read_binary(FILE *fp)
{
    unsigned height, width;
    int read = fscanf(fp, "COMP40 Compressed image format 2\n%u %u", 
                      &width, &height);
    assert(read == 2);
    int c = getc(fp);
    assert(c == '\n');

    int col = 0;
    int row = 0;
    int index = 0;

    UArray2_T coded_arr = UArray2_new(width, height, sizeof(uint32_t));
    c = getc(fp);
    while (c != EOF) {
        index_convert(coded_arr, index, &col, &row);
        *((uint32_t *)UArray2_at(coded_arr, col, row)) = c;
        index++;
        c = getc(fp);
    }
    return coded_arr;
}