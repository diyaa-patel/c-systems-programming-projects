/**
    @file input.c 
    @author Diya Patel 
    this class formats the data input given and creates a map of column types.
  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "input.h"

/** Define format options for string type */
#define STRING_FORMAT "string"

/** Define format options for int type */
#define INTEGER_FORMAT "integer"

/** Define format options for float type, output to 2 fractional digits */
#define FLOAT2_FORMAT "float2"

/** Define format options for float type, output to 5 fractional digits */
#define FLOAT5_FORMAT "float5"

/** Define format options for date type */
#define DATE_FORMAT "date"

/** format for float 2 */ 
#define FLOAT_FORMAT2 2

/** format for float 5 */ 
#define FLOAT_FORMAT5 3

/** format for date */ 
#define DATE_NUM 4 

/** Max length for string */
#define FIELD_MAX 30

/** Maximum number of columns */
#define COLUMN_MAX 25

/** File not containing at least formats and headings */
#define FILE_ERROR 100

/** Error in format type */
#define FORMAT_ERROR 101

/** Error in heading (too long, too few, too many */
#define HEADING_ERROR 102

/** if there is a error with the heading limit */ 
#define LIMIT_ERROR -2


/**
    the function is given an empty sptring and reads into that string a format type, heading, or string 
    data type.
    @param str the string 
    @return -2 if the string exceeds FIELD_MAX
  */
int readString(char str[FIELD_MAX + 1]){
    int ch; 
    int x = 0; 
    //while loop as a delimiter 
    while((ch = getchar()) != EOF && ch != ',' && ch !='\n') {
        if (x < FIELD_MAX) {
            str[x] = ch; 
            x++; 
        } else {
            return LIMIT_ERROR;
        }
    }
    str[x] = '\0'; // null termination
    return ch; 
}

/**
    function is given an empty integer array and fills the array with values that mp to the format type of each 
    data item in each row. If the dataset contains more than 25 columns, the program should 
    terminate with an exit status of 101.
    @param map the map that will be filled with the data
  */
int createMap(int map[COLUMN_MAX]) {
    // counter for amount of columns 
    int col = 0; 

    // the current string of chars 
    char current[FIELD_MAX + 1]; 
    // the data being read */ 
    int data; 
    data = readString(current);

    // if there is no data throws file error
    if(data == EOF ) {
        exit(FILE_ERROR); 
    }

    //fills the array with values to the format type	
    while(data != EOF && data != '\n'){
        if(col >= COLUMN_MAX){
            exit(FORMAT_ERROR); 
        }

        if(strcmp(current, STRING_FORMAT) == 0){
            map[col] = 0; 
        } else if (strcmp(current, INTEGER_FORMAT) == 0){
            map[col] = 1; 
        } else if (strcmp(current, FLOAT2_FORMAT) == 0){
            map[col] = FLOAT_FORMAT2; 
        } else if (strcmp(current, FLOAT5_FORMAT) == 0){
            map[col] = FLOAT_FORMAT5; 
        } else if(strcmp(current, DATE_FORMAT) == 0){
            map[col] = DATE_NUM; 
        } else {
            exit(FORMAT_ERROR); 
        }

        col++; 
        // the data being read 
        data = readString(current); 
    } 

    if(col == 0) {
        exit(FILE_ERROR); 
    }

    return col;
}