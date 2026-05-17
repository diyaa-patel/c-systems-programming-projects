/**
    @file table.c 
    @author Diya Patel 
    This program will read the contents of the table. It includes code to parse long int values, cell references and formulas that perform add,
    subtract, multiply, and divide operations. 
 */

#include "table.h"
#include "overflow.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

/** Length of the longest supported function name (for the extra credit). */
#define FNAME_MAX 6

/** Base for reading numbers. */
#define BASE 10

/** Max amount of columns */ 
#define MAX_COLUMNS 26

/** Max value of long in digits */
#define MAX_LONG_DIGITS 7

/**
   This function prints an error message for a cell then terminates the program unsuccessfully 
   
   @param r location for the cell 
   @param c the location for the cell 
   @param message the error message to output 
*/
static void cellError(int r, int c, char const message[] ) 
{
    fprintf(stderr, "%c%d : %s\n", 'A' + c, r + 1, message);
    exit(EXIT_FAILURE);
}

/**
    This function tries to read the next long int value from the given input file, which returns true if
    successful and putting the integer value in the pass-by-address result parameter. 

    @param *fp a pointer to the file that we read in from 
    @param *result a pointer to the result
    @return false if there is no next integer, if the next integer is not formatted properly or the next integer is too large
*/
static bool readLong(FILE *fp, long *result)
{
    // The value for checking overflow 
    long value = 0;  
    // The current character in the file 
    int ch; 
    // The sign of the number 
    int sign = 1; 

    // Tracks if we've started reading a number 
    bool started = false;  

    // Skipping the white spaces
    while((ch = fgetc(fp)) != EOF && isspace(ch));

    if (ch == EOF) {
        return false;
    }

    // Handling the sign before if there is one 
    if(ch == '-'){
        // Change sign to negative value 
        sign = -1; 
        ch = fgetc(fp); 
    }
    else if(ch == '+'){
        ch = fgetc(fp); 
    }
    // Check if next character is a digit 
    if(!isdigit(ch)){
        ungetc(ch, fp); 
        return false; 
    }

    do{
        // Find the number 
        long dig = ch - '0'; 

        // Prevent false overflow detection for LONG_MIN
        if (value > (LONG_MAX / BASE) || (value == LONG_MAX / BASE && dig > MAX_LONG_DIGITS)) {
            if (!(sign == -1 && value == LONG_MAX / BASE && dig == (MAX_LONG_DIGITS + 1))) {
                return false;
            }
        }

        value = value * BASE + dig; 
        ch = fgetc(fp); 
        started = true;
    } while(isdigit(ch)); 

    // Ungetting the last character that was read because it is not part of the number
    ungetc(ch, fp); 

    if (!started) {
        return false;
    }

    // Updating the sign if neccessary when it was read as negative 
    *result = value * sign; 

    return true;
}

void parseHeader(FILE *fp, int *rows, int *cols)
{
    long tempRow; 
    long tempCol; 

    // Checking that the values are numbers 
    if(!readLong(fp, &tempRow) || !readLong(fp, &tempCol) || tempRow <= 0 || tempCol <= 0 || tempCol > MAX_COLUMNS){ 
        fprintf(stderr, "Invalid header\n");
        exit(EXIT_FAILURE);
    }

    // Checking for in bound values
    if (tempRow < 1 || tempCol < 1 || tempCol > MAX_COLUMNS ) { 
       fprintf(stderr, "usage: calc INPUT_FILE [OUTPUT-FILE]\n"); 
       exit(EXIT_FAILURE);
    }

    *rows = (int)tempRow;  
    *cols = (int)tempCol;

    // Checking if new line character before EOF 
    int ch = fgetc(fp);
    if(ch != '\n' && ch != EOF){
        fprintf(stderr, "Invalid header format\n");
        exit(EXIT_FAILURE);
    }

}

/**
    This function reads input for the next cell from the given input stream and returns the resulting value. 

    @param *fp a pointer to the file to parse through 
    @param r gives the location to the cell 
    @param rows the amount of rows
    @param cols the amount of columns 
    @param table the table of rows and columns to fill
    @return table after all the formulas are parsed

*/
static long parseFormula(FILE *fp, int r, int c, int rows, int cols, long table[rows][cols])
{
    int ch; 

    // Skipping all the white spaces 
    do{
        ch = fgetc(fp); 
    } while(isspace(ch)); 

    // Numeric case 
    if(isdigit(ch) || ch == '-'){
        ungetc(ch, fp); 
        long value;

        if(!readLong(fp, &value)) { 
            cellError(r, c, "Invalid formula"); 
        }
        return value; 
    }
    // Case for cell reference 
    else if(isupper(ch)){
        int col = ch - 'A'; 
        long row; 
        int next = fgetc(fp); 

        // Checking for space after the colum letter 
        if (isspace(next)) {
            cellError(r, c, "Invalid formula");
        }
    
        ungetc(next, fp); 

        if(!readLong(fp, &row) || row < 1 || row > rows || col < 0 || col >= cols){
            cellError(r, c, "Invalid reference");
        }

       // Checking to see if cell is initialized 
       if (row - 1 > r || (row - 1 == r && col >= c)) { 
           cellError(r, c, "Invalid formula");
        }

       return table[row - 1][col]; 
    }
    //Case for function call  
    else if(islower(ch)){

        char operations[FNAME_MAX + 1]; 
        int i = 0;

        // Store the first letter
        operations[i++] = ch;  

        // Read the rest of the function name
        while (i < FNAME_MAX && islower(ch = fgetc(fp))) {
            operations[i++] = ch;
        }

        // Ensure all characters after function name are '\0'
        while (i < FNAME_MAX + 1) {
            operations[i++] = '\0';
        }
        operations[i] = '\0';

       // Check if the function name is valid
        if (strcmp(operations, "add") != 0 && strcmp(operations, "sub") != 0 && strcmp(operations, "mul") != 0 && strcmp(operations, "div") != 0) {
            cellError(r, c, "Invalid formula");  
        }

        // Skipping the white spaces 
        while(isspace(ch)){
            ch = fgetc(fp); 
        }

        // Making sure there is correct syntax 
        if(ch != '('){ 
            cellError(r, c, "Invalid function");
        }

        do {
            ch = fgetc(fp);
        } while (isspace(ch));

        ungetc(ch, fp); 

        // Parsing the first value 
        long value = parseFormula(fp, r, c, rows, cols, table);

        // Skip spaces before ','
        do {
            ch = fgetc(fp);
        } while (isspace(ch));

        // Checking for , seperator 
        if (ch != ',') {
           cellError(r, c, "Invalid function");
        }

        // Parsing the second value  
        long value2 = parseFormula(fp, r, c, rows, cols, table);

        // Skip spaces before ')'
        do {
            ch = fgetc(fp);
        } while (isspace(ch));

        if (ch != ')') {
           cellError(r, c, "Invalid function");
        }

        // Performing the operation 
        if(strcmp(operations, "add") == 0){
            if(!checkAdd(value, value2)){
                cellError(r,c, "Overflow");
            }
            return value + value2; 
        }
        if(strcmp(operations, "sub") == 0){
            if(!checkSub(value, value2)){
                cellError(r,c, "Overflow");
            } 
            return value - value2; 
        }
        if(strcmp(operations, "mul") == 0){
            if(!checkMul(value, value2)){
                cellError(r,c, "Overflow"); 
            } 
            return value * value2; 
        } 
        if(strcmp(operations, "div") == 0){ 
            if(value2 == 0){
                cellError(r,c, "Divide by zero");  
            }
            if(!checkDiv(value, value2)){
                cellError(r,c, "Overflow"); 
            } 
            return value / value2; 
        } 

        // If the function name doesn't match any of the above 
        cellError(r,c, "Invalid formula"); 
    }


    if(!isdigit(ch) && !islower(ch) && !isupper(ch)){
        cellError(r,c, "Invalid formula"); 
    }

    // If none of the cases match
    cellError(r,c, "Invalid formula"); 
    return 0;
}

void parseTable(FILE *fp, int rows, int cols, long table[rows][cols])
{
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){

            int ch; 
            // Skipping the white spaces 
            while ((ch = fgetc(fp)) != EOF && isspace(ch));
            ungetc(ch, fp); 

            table[r][c] = parseFormula(fp, r, c, rows, cols, table); 

            // Skipping the white spaces
            while ((ch = fgetc(fp)) != EOF && isspace(ch));

            // Cell seperation check 
            if(c < cols - 1){
                if(ch != ','){ 
                    cellError(r, c, "Invalid formula"); 
                }
            }
            else {
                ungetc(ch, fp); 
            }
        }
    }
}