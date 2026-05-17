/**
    @file style.c
    @author Diya Patel (dapatel8)
    This program prints the words in a random order of letters for the whole paragraph.
*/
#include <stdio.h>
#include <stdlib.h>

/** Exit code if style problems are encountered. */ 
#define SEVENTY 72


 /** 
    Takes in a integer x and prints a random lower-case letter for each letter in the word. 
    @param x the amount of letters in the word.
*/
void printWord( int x ) 
{
    // Prints a random lower-case letter x amount of times. 
    for ( int i = 0; i < x; i++ ) {    
        // Print a random lower-case letter.
        printf( "%c", 97 + rand() % 26 );
    }
}

/**
    Prints a line with random letters in each word. 
    @return count the total count of words that were randomized in the line.
*/
int printLine() 
{
    // the amount of numbers. 
    int count = 0;

    // the current position. 
    int	pos = 0; 

    // the amount of spaces. 
    int space = 0;

    // the length of the word. 
    int len = 1 + rand() % 10;

    // Print a line of words up to a limited length.
    while ( pos + len + space < SEVENTY ) {
        if ( space > 0 ) {
        	printf( " " );
        }
        printWord( len );
        pos += len + space;
        len = 1 + rand() % 10;
        space = 1;
        count += 1;
    }
    printf( "\n" );

    return count;
}

/**
    Prints the paragraph given in random lower-case letters for each word. 
    @param n the amount of lines in the paragraph.
    @return total the total amount of randomized lettered words in the paragraph.
*/
int printParagraph( int n ) 
{

    // The total amount of lines in the paragraph.
    int total = 0;

    // Prints the paragraph in random lower-case letters by calling printLine() for every line. 
    for ( int i = 0; i < n; i++ ) {
        total += printLine();
    }

    return total;
}

/**
    Program starting point 
    @return program exit status 
*/
int main() {

    //the amount of words printed in the random order. 
    int w = printParagraph(10);

    printf("Words: %d\n",w);

    return EXIT_SUCCESS;
}