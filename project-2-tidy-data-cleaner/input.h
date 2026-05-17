/**
    @file input.h 
    @author Diya Patel 
    the header file for input.c 
  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/** Maximum length of a format or heading name */
#define FIELD_MAX 30
/** Maximum number of columns permitted */
#define COLUMN_MAX 25

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

/**
    the function is given an empty sptring and reads into that string a format type, heading, or string 
    data type.
    @param str the string 
  */
int readString(char str[FIELD_MAX + 1]); 

/**
    function is given an empty integer array and fills the array with values that mp to the format type of each 
    data item in each row. If the dataset contains more than 25 columns, the program should 
    terminate with an exit status of 101.
    @param map the map that will be filled with the data	
  */
int createMap(int map[COLUMN_MAX]); 