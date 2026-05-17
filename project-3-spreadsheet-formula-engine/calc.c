/**
    @file calc.c 
    @author Diya Patel
    This class contains the main method and is responsible for printing the table and writing the table to a file.  
 */

#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/** Number of filenames that must be given on the command line. */
#define REQUIRED_FILENAMES 1

/** Number of filenames that may be given on the command line. */
#define OPTIONAL_FILENAMES 2

/** The max amount of arguments */ 
#define MAX_ARGUMENTS 3

/** The minimum amount of arguments */
#define MIN_ARGUMENTS 2

/** the max amount of char in string */ 
#define STR_LEN 25

/**
    This is the start of the program 

    @param argc the arguments count 
    @param *argv[] the argument containing the input and output file names
    @return EXIT_SUCCESS if the table is printed out 
*/ 
int main(int argc, char *argv[]) {

    // Validating the command arguments 
    if(argc < MIN_ARGUMENTS || argc > MAX_ARGUMENTS){ 
        fprintf(stderr, "usage: calc INPUT-FILE [OUTPUT-FILE]\n"); 
        return EXIT_FAILURE;  
    }

    // Opening the input file 
    FILE *input= fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Can't open file: missing-input-file.txt\n");
        return EXIT_FAILURE;
    }

    int rows; 
    int cols;

    // Reading the table dimensions 
    parseHeader(input, &rows, &cols);

    // Creating a table array 
    long table[rows][cols]; 

    // Parsing through the table 
    parseTable(input, rows, cols, table); 

    // Closing the input file 
    fclose(input);

    // Either prints to terminal or writes to file
    if (argc == MAX_ARGUMENTS) {
        FILE *output = fopen(argv[MIN_ARGUMENTS], "w");
        if (!output) {
            fprintf(stderr, "usage: calc INPUT-FILE [OUTPUT-FILE]\n");
            return EXIT_FAILURE;
        }
        writeTable(output, rows, cols, table);
        fclose(output);
    } else {
        printTable(rows, cols, table);
    }
  
    return EXIT_SUCCESS;
} 

void printTable(int rows, int cols, long table[rows][cols]){
    int colWidth[cols]; 

    // Max width needed for the columns
    for(int c = 0; c < cols; c++){
        colWidth[c] = 0;

        for(int r = 0; r < rows; r++){
            char toString[STR_LEN]; 
            // Converting num to string 
            sprintf(toString, "%ld", table[r][c]); 
            int width = strlen(toString); 

            // Setting the width 
            if(width > colWidth[c]){
                colWidth[c] = width; 
            }
        }
    }

    // Printing the table
    for(int r = 0; r < rows; r++){
        printf("+"); 

        for(int c = 0; c < cols; c++){
            // Printing the seperators 
            for(int i = 0; i < colWidth[c]; i++){
                printf("-"); 
            }
            printf("+"); 
        }
        printf("\n"); 

        printf("|");
        for(int c = 0; c < cols; c++){
            // Printing the value right aligned 
            printf("%*ld|", colWidth[c], table[r][c]);  
        }

        printf("\n"); 
    }

    // Printing the final row seperator
    printf("+"); 
    for(int c = 0; c < cols; c++){
        for(int i = 0; i < colWidth[c]; i++){
            printf("-"); 
        }
        printf("+");  
    }
    printf("\n"); 
}

void writeTable(FILE *fp, int rows, int cols, long table[rows][cols])
{
    // Table dimensions 
    fprintf(fp, "%d %d\n", rows, cols); 

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // The number read from the table
            fprintf(fp, "%ld", table[r][c]); 
            if (c < cols - 1) {
                // Seperator 
                fprintf(fp, ","); 
            }
        }
    fprintf(fp, "\n");

    }
}