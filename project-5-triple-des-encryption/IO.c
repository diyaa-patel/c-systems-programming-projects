/**

   @file IO.c 
   @author Diya Patel 

   This component has the ability to read and write from the file given
*/

#include "IO.h"
#include <stdio.h>
#include <stdlib.h>

byte *readFile( char const *filename, int *n ) 
{
    FILE *fp = fopen(filename, "rb");

    // error if file is missing
    if (!fp) {
        fprintf( stderr, "missing-file.bin: No such file or directory\n" );
        exit( EXIT_FAILURE );
    }

    // Seek to end to determine file size
    fseek(fp, 0, SEEK_END);

    // getting the file size in bytes
    long fileSize = ftell(fp);
    
    //resetting the position
    rewind(fp);

    // if file size is less than zero
    if (fileSize < 0) {
        fclose(fp);
        exit( EXIT_FAILURE );
    }

    // allocating memory to hold the file
    byte *buffer = malloc(fileSize);
    if (!buffer) {
        fclose(fp);
        exit( EXIT_FAILURE );
    }

    // reading file into the buffer
    size_t bytesRead = fread(buffer, 1, fileSize, fp);
    fclose(fp);

    if (bytesRead != fileSize) {
        free(buffer);
        exit( EXIT_FAILURE );
    }

    // making the output param equal the number of bytes read
    *n = (int)fileSize;
    return buffer;
}

bool writeFile( char const *filename, byte *data, int n ) 
{
    // opening the file in write mode for binary
    FILE *fp = fopen(filename, "wb");

    // if there is an error with finding the file
    if (!fp) {
        fprintf( stderr, "missing-file.bin: No such file or directory\n" );
        exit( EXIT_FAILURE );
    }

    // writing the data to the file
    size_t bytesWritten = fwrite(data, 1, n, fp);
    fclose(fp);

    // returning true if all bytes were written
    return bytesWritten == n;
}