/**
    @file data.h 
    @author Diya Patel 
    This is the .h file for the data.c function 
  */

#include "input.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/**
    This function is given the number of data items in an observation 
    and the map of format values. It reads a complete observation given in a row of data items and 
    compares each with the map of format types.  

    @param len the length of the amount of columns 
    @param map[COLUMN_MAX] the array of data 
  */
bool checkObservation(int len, int map[COLUMN_MAX]); 

/**
    The function is passed a date string and checks that it is in one 
    of the acceptable date formats. It corrects the format 
    of the date string to match the ISO standard format, otherwise terminates
    with an exit status 104 

    @param date[FIELD_MAX + 1] the date string to check 
  */ 
void checkDate(char date[FIELD_MAX + 1]); 

/**
    checks the string to see if it is a correct integer format

    @param str[] the string to check 
 */ 
bool isValidInt(char str[]); 

/**
    checks the string to see if it is a correct float format

    @param str[] the string to check 
 */ 
bool isValidFloat(char str[]); 