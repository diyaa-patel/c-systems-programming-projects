/** 
    @file IO.h
    @author Diya Patel

    Functions to make it easy to read and write binary files.
*/

#ifndef _IO_H_
#define _IO_H_

#include <stdbool.h>

/** Type used to represent a byte. */
typedef unsigned char byte;

/**
    This function reads the contents of the file with the given filename. 
    
    @param *filename the name of the file to read 
    @param *n the pass-by-refrence parameter which is used to return the length of the file 
    @return a pointer to a dynamically allocated array of bytes with the contents of the file
*/
byte *readFile(char const *filename, int *n);

/**
    This function is for writing bytes in binary. 

    @param *filename the output file will have this name 
    @param *data the data array 
    @param n the amount of bytes
    @return true if the function runs
*/
bool writeFile( char const *filename, byte *data, int n ); 

#endif
