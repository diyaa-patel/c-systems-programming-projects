/**
    @file sort.c
    @author Diya Patel 
    implements the sorting of the double values read from an input file.
 */

#include "segTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

/** two arguments */
#define TWO_ARG 2

/** Comparison function to configure the segment tree as a minimizing tree. */

/**
    This function compares the two doubles in order to minimize the tree
    @param a a pointer to the first double
    @param b a poitner to the second double
    @return 0 if the two values are equal
*/
static int compareDoubles(void const *a, void const *b) 
{
    double da = *(double const *)a;
    double db = *(double const *)b;
    return (da > db) ? 1 : (da < db) ? -1 : 0;
}

/**
   This is the entry point for the sorting function
   @param argc the number of command line arguments
   @param argv an array of command line arguments
   @return EXIT_SUCCESS if there are no errors
*/
int main(int argc, char *argv[]) 
{
    // checking for two arguments
    if (argc != TWO_ARG) {
        fprintf(stderr, "Usage: sort INPUT_FILE\n");
        return EXIT_FAILURE;
    }

    // trying to open the input file
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }

    // making a segment tree to store the double values
    SegTree *st = makeST(sizeof(double), compareDoubles);

    // reads all the valid double values
    double val;
    while (fscanf(fp, "%lf", &val) == 1) {
        addST(st, &val);
    }

    // if the file has anything that isnt a double
    if (!feof(fp)) {
        fprintf(stderr, "Invalid input file\n");
        freeST(st);
        fclose(fp);
        return EXIT_FAILURE;
    }

    fclose(fp);

    // copy all the double values from the segment tree into the array
    int size = sizeST(st);
    double *array = malloc(sizeof(double) * size);
    for (int i = 0; i < size; i++) {
        array[i] = *(double *)getST(st, i, NULL);
    }

    // use qsort to sort the values
    qsort(array, size, sizeof(double), compareDoubles);

    // printing the sorted values
    for (int i = 0; i < size; i++) {
        printf("%8.3f\n", array[i]);
    }

    // freeing the array and the tree
    free(array);
    freeST(st);
    return EXIT_SUCCESS;
}
