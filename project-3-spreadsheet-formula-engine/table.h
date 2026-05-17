/**
    @file table.h 
    @author Diya Patel 
    This is the header file for table.c
 */

// Prototypes in this header need FILE.
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>


/**
    This function pases the table header from the given input file and stores the size of the table 
    on the rows and the cols parameters. If there is an error in the header, it prints an error message and exits. 
	
    @param *fp pointer to the file fp 
    @param *rows a pointer to the amount of rows there are 
    @param *cols a pointer to the amount of columns there are
*/
void parseHeader(FILE *fp, int *rows, int *cols);

/**
    This function reads the contents of the entire table from the given input stream. 

    @param *fp pointer to the file fp 
    @param rows the amount of rows there are 
    @param cols the amount of columns there are 
    @param table the table filled with the data 
*/ 
void parseTable(FILE *fp, int rows, int cols, long table[rows][cols]);

/**
    This function is used to print the table to the terminal after parsing the table. 

    @param rows the rows in the table 
    @param cols the columns in the table 
    @param table the table to print out after parsing
*/
void printTable(int rows, int cols, long table[rows][cols]); 

/**
    This function sends the table to a given file 

    @param *fp the file to write the table to 
    @param rows the amount of rows 
    @param cols the amount of columns 
    @param table the table to write to the files
*/
void writeTable(FILE *fp, int rows, int cols, long table[rows][cols]); 