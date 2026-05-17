/**
    @file data.c 
    @author Diya Patel
    data.c contains two functions to check and validate the data given
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "input.h"
#include "data.h"

/** Error in column length or number of columns */
#define COLUMN_ERROR 103

/** Exit status for a missing or badly formatted date. */
#define DATE_ERROR 104

/** Exit status for unrealistic date */ 
#define DATE_UNREALISTIC 105 

/** Length of nan string */
#define NAN_LENGTH 3

/** Definition of nan string */
#define NAN_STR "nan"

/** Number of digits in the short (2 digit) year. */
#define SHORT_YEAR 2

/** Number of digits in the full (4 digit) year. */
#define FULL_YEAR 4

/** Number of digits in the month. */
#define MONTH_DIGITS 2

/** Number of digits in the day. */
#define DAY_DIGITS 2

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

/** length of date */ 
#define DATE_LENGTH 10

/** number 2 */ 
#define TWO 2 

/** number 3 */ 
#define THREE 3 

/** number 4*/ 
#define FOUR 4

/** number 5 */ 
#define FIVE 5

/** number 6 */ 
#define SIX 6

/** number 7 */ 
#define SEVEN 7

/** number 8 */ 
#define EIGHT 8 

/** number 9 */ 
#define NINE 9

/** number 10 */
#define TEN 10

/** number 1000 */ 
#define THOUSAND 1000

/** number 100 */ 
#define HUNDRED 100


/**
    This function is given the number of data items in an observation 
    and the map of format values. It reads a complete observation given in a row of data items and 
    compares each with the map of format types.  
    @param len the length of the amount of columns 
    @param map[COLUMN_MAX] the array of data 
  */
bool checkObservation(int len, int map[COLUMN_MAX]) {
    /** count */ 
    int count = 0; 
    /** part of the data */ 
    char field[FIELD_MAX + 1]; 
    /** float count */ 
    int floatNum; 

    // goes through map and validates the data and prints it out 
    while(count < len) {
        if(readString(field) == EOF){
            return false; 
        }

        //checks for empty field
        if(strlen(field) == 0 || strcmp(field, "nan") == 0 || strcmp(field, "NaN") == 0 || strcmp(field, "NAN") == 0){
            printf("nan"); 
        } else { 
            switch(map[count]) { 
                case 0: //string field
                    if(strlen(field) > FIELD_MAX){
                        exit(COLUMN_ERROR);
                    }
                    printf("%s", field); 
                    break; 
                case 1: //checks for integer format 
                    if(!isValidInt(field)){
                        exit(COLUMN_ERROR); 
                    }
                    printf("%s", field); 
                    break; 
                case FLOAT_FORMAT2: //checks for float 2 format 
                    while(field[floatNum] != '.'){
                        floatNum++; 
                    }
                    if(floatNum == strlen(field) - 1){
                        exit(COLUMN_ERROR);
                    }
                    floatNum = 0; 
                    printf("%.2f", atof(field)); 
                    break; 
                case FLOAT_FORMAT5: //float format 5 check
                    while(field[floatNum] != '.'){
                        floatNum++; 
                    }
                    if(floatNum == strlen(field) - 1){
                        exit(COLUMN_ERROR);
                    }
                    floatNum = 0; 
                    printf("%.5f", atof(field)); 
                    break; 
                case DATE_NUM: //checks for data format 
                    checkDate(field); 
                    printf("%s", field);
                    break; 
                default: //throws error otherwise 
                    //exit(COLUMN_ERROR); 
                    break; 
            }
        } 

        //incrementing count
        count++; 

        if(count < len){
            printf(","); 
        }
    }

    printf("\n"); 

    //if current is not at end of file returns true
    return true; 
}

/**
    The function is passed a date string and checks that it is in one 
    of the acceptable date formats. It corrects the format 
    of the date string to match the ISO standard format, otherwise terminates
    with an exit status 104 

    @param date[FIELD_MAX + 1] the date string to check 
  */
void checkDate(char date[FIELD_MAX + 1]){
    int day;
    int month; 
    int year; 

    //already in the correct YYYY-MM-DD 
    if(strlen(date) == TEN && date[FOUR] == '-' && date[SEVEN] == '-') {
        return; 
    }

    //when there is a / or - seperation in date 
    if (strlen(date) == DATE_LENGTH && (date[TWO] == '/' || date[TWO] == '-')) {
        char separator1 = date[TWO];  
        char separator2 = date[FIVE]; 

        //seperators should be constant 
        if (separator1 != separator2) {
            exit(DATE_ERROR);
        }

        //converting month to int 
        month = (date[0] - '0') * TEN + (date[1] - '0');
        //converting day to int
        day = (date[THREE] - '0') * TEN + (date[FOUR] - '0'); 
        // Convert 'YYYY' to int
        year = (date[SIX] - '0') * THOUSAND + (date[SEVEN] - '0') * HUNDRED + (date[EIGHT] - '0') * DATE_LENGTH + (date[NINE] - '0'); 
        //puts in correct format if it is MM DD YYYY
        printf("%04d-%02d-%02d", year, month, day);
        return;
    } 

    exit(DATE_ERROR); 
}

/**
    checks the string to see if it is a correct integer format
    @param str[] the string to check 
 */ 
bool isValidInt(char str[]){
    int i = 0; 
    if(str[0] == '-'){
        i++; 
    }

    if(str[i] == '\0'){
        return false; 
    } 

    //goes through char array 
    for (int x = i; str[x] != '\0'; x++) {
        if (str[x] < '0' || str[x] > '9') {
            return false; 
        }
    }
    return true;
}