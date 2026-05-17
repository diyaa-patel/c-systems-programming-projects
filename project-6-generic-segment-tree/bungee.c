/**
    @file bungee.c 
    @author Diya Patel
    This class is used to solve the bungee builder problem
*/

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "segTree.h"
 
/**
    This function compares the two constants and find the minimum in the range.
    @param a a pointer to a int 
    @param b a pointer to a int 
    @return 1 if a is less than b
*/
static int compMin(const void *a, const void *b) {
    int intA = *(int const *)a;
    int intB = *(int const *)b;
    return (intA < intB) ? 1 : (intA > intB) ? -1 : 0;
}

/**
    This is the starting point of bungee.c
    @param argc the inputted arguments 
    @param argv an array of the arguments
    @return 0 if it is able to exit successfully
*/
int main(int argc, char *argv[]) {
    // the number of bridges
    int n;
    
    if (scanf("%d", &n) != 1 || n < 2) {
        // no possible mountains if there are <2 bridges
        printf("0\n");
        return 0;
    }

    // reading in all the heights
    int *height = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &height[i]);
    }

    // building a segment tree
    SegTree *st = makeST(sizeof(int), compMin);
    for (int i = 0; i < n; i++) {
        addST(st, &height[i]);
    }

    jmp_buf env;

    int best = 0;

    // decreasing stack of indices
    int *stack = malloc(sizeof(int) * n);
    int top = 0;

    for (int j = 0; j < n; j++) {
        // handling any previous peaks and they have j as their "next greater to the right"
        while (top > 0 && height[stack[top-1]] <= height[j]) {
            int peak = stack[--top];
            // bridge height will be h[p]
            if (peak + 1 < j) {
                int valley = *(int *)getST(st, 
                    queryST(st, peak+1, j-1, &env), 
                    NULL);
                int drop = height[peak] - valley;
                if (drop > best) best = drop;
            }
        }
        // if theres a peak to the left higher than h[j], that is the "nearest greater to the left" for j
        if (top > 0) {
            int i = stack[top-1];
            // bridge height = h[j], since h[i] > h[j]
            if (i + 1 < j) {
                int valley = *(int *)getST(st,
                    queryST(st, i+1, j-1, &env),
                    NULL);
                int drop = height[j] - valley;
                if (drop > best) best = drop;
            }
        }
        // pushing the index
        stack[top++] = j;
    }

    // freeing the memory
    printf("%d\n", best);
    free(stack);
    freeST(st);
    free(height);

    return EXIT_SUCCESS;
}