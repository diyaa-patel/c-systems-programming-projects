/**
    @file tidydata.c
    @author Diya Patel
    Main class for data cleaning which reads the format types and rows from the input and make 
    sure they are proper and printed out correctly.    
  */

#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "data.h"
#include <stdbool.h>

/** Exit status for missing or incorrect format/header rows */
#define MISSING_ERROR 100

/** Maximum number of columns */
#define COLUMN_MAX 25

/** Error in heading (too long, too few, too many */
#define HEADING_ERROR 102

/** Error in format type */
#define FORMAT_ERROR 101

/** if there is a error with the heading limit */ 
#define LIMIT_ERROR -2


/**
    with help from the other componenets, it is responsible for reading from the input file and outputing 
    values to standard output until the end of input. 
  */ 
int main() {
    // the array of data 
    int map[COLUMN_MAX]; 
    // amount of columns in map 
    int columns = createMap(map); 
    //heading string 
    char field[FIELD_MAX + 1]; 

    if(columns <= 0 || columns >= COLUMN_MAX) {
        exit(FORMAT_ERROR);
    }

    for(int i = 0; i < columns; i++){
        int current = readString(field); 

        //checks to see if heading is within 30 characters 
        if(current == LIMIT_ERROR || current == EOF){
            exit(HEADING_ERROR); 
        }

        printf("%s", field); 

        // prints , if there is still columns left unless it hits a new line
        if(i < columns - 1){
            printf(","); 
        }
    }
    printf("\n"); 

    //cleans the data in the map 
    while(checkObservation(columns, map)){
        //keeps processing
    } 
    return 0; 
}
