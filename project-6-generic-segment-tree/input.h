/**
    @file input.h 
    @author Diya Patel 
    This file is the h file for input.c
*/

#include <stdio.h>

/**
    This function reads a single line of input from the given input stream (stdin or a file) and returns it as a string 
    inside a block of dynamically allocated memory. 

    @return buffer the string inside a block of dynamically allocated memory 
*/
char *readLine();