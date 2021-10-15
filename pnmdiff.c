#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"

int main(int argc, char *argv[])
{
    char filename1[1000];
    char filename2[1000];
    if (argc == 3) {
        //first file is read from stdin
        if (argv[1] == '-'') {
            scanf("%s", filename1);
            strcpy(filename2, argv[2]);
        } else {
            //second file is read from stdin
            if (argv[2] == '-') {
                strcpy(filename1, argv[1]);
                scanf("%s", filename2);
            }
        } else {
            //no files from stdin
            strcpy(filename1, argv[1]);
            strcpy(filename2, argv[2]);
        }
    } else {
      /* Exit if there are too many arguments */
        fprintf(stderr, "Invalid command line args\n");
        exit(EXIT_FAILURE);
    }
}
