/**
    @file input.c 
    @author Diya Patel
    This program will help with reading input from the house files and it 
    will also be used to read commands from the user.	
*/
#include <stdlib.h>
#include <ctype.h>
#include "input.h"

/** the initial size of the buffer */ 
#define INITIAL_SIZE 32

/** to increase capacity */
#define CHANGE_CAP 2

char *readLine(FILE *fp) 
{
    // Checking if file is null
    //if(fp == NULL) {
    //    return NULL;
    //}

    // Initial capacity of the buffer is initialized
    int capacity = INITIAL_SIZE; 
    int length = 0; 
    char *buffer = malloc(capacity);

    //if(!buffer) {
      //  fprintf(stderr, "Memory allocation failed\n");
       // exit(EXIT_FAILURE);
    //}

    int ch;
    while((ch = fgetc(fp)) != EOF && ch != '\n') {
        buffer[length++] = (char) ch;
        // Resizing the buffer
        if(length >= capacity - 1) {
            capacity *= CHANGE_CAP;
            char *temp = realloc(buffer, capacity);
            //if(!temp) {
               // fprintf(stderr, "Memory allocation failed\n");
               // free(buffer); 
                //exit(EXIT_FAILURE);
            //}
            buffer = temp;
        }
    }

    // If there are no characters in buffer and EOF is reached
    if(length == 0 && ch == EOF) {
        free(buffer);
        return NULL;
    }

    // null terminator added
    buffer[length] = '\0';

    return buffer;
} 