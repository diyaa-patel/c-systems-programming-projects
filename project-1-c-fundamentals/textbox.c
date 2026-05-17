/**
    @file textbox.c
    @author Diya Patel (dapatel8)
    This program prints astriks around the text. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** Width of each line of text in the box. */
#define LINE_WIDTH 60

/** Symbol used to draw the border around the box. */
#define BORDER '*'

/**
    This function will read and print a single line of text insider the border.
    @return true if it successfully read and print an input line
  */
bool paddedLine()
{
    // amount of characters 
    int count = 0;

    // the current character 
    int current = getchar(); 

    //returns false if current is at the EOF 
    if(current == EOF) {
        return false; 
    }

    //puts the border at the start of the line. 
    putchar(BORDER); 

    //puts the character as long as it doesn't reach the end of the file or a new line.
    while(current != '\n' && current != EOF) {
        // checking to see if it is within the LINE_WIDTH
        if(count < LINE_WIDTH) {
            putchar(current); 
        }
        count++; 
        current = getchar(); 
    }

    //function to add extra spaces to create rectangular border
    while(count < LINE_WIDTH) {
        putchar(' '); 
        count++; 
    }

    //puts the ending border of the line
    putchar(BORDER); 

    //goes to the next line
    putchar('\n'); 

    // if current character is not at end of file return true.  
    if(current != EOF){
        return true; 
    }
    return false; 
}

/**
    This function prints out multiple copies of the given character, followed by a newline.
    @param ch the char given 
    @param count the number of copies to print out. 
  */
void lineOfChars( char ch, int count )
{
    // puts the char the count amount of times. 
    for(int i = 0; i < count; i++) {
        putchar(ch); 
    }
    //prints out a new line. 
    putchar('\n'); 
}

/**
    Program starting point, it will use the other two functions to print the text from standard input with a border around it.
    @return program exit status 
  */
int main()
{
    //prints the top line border 
    lineOfChars(BORDER, LINE_WIDTH + 2); 

    //print the lines inside the border until end of file is reached 
    while(paddedLine()); 

    //print the bottom line border
    lineOfChars(BORDER, LINE_WIDTH + 2); 

    return EXIT_SUCCESS; 
}
