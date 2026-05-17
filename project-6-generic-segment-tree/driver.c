/**
    @file driver.c 
    @author Diya Patel
    This is the main program for the driver. It creates and maintains a segment tree, and deals with the 
    different commands - add, querying, updatng, and removing, and getting the size of tree.
 */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <setjmp.h>
#include <unistd.h> 

#include "segTree.h"
#include "input.h"

/** the maximum size of a string, for regular credit. */
#define STRING_LIMIT 20

/** the longjump value for an invalid command. */
#define INVALID_COMMAND 101

/** checking for two arguments */
#define TWOARG 2

/** command size */
#define COMMAND_SIZE 16

/** temp size */
#define TEMP_SIZE 1024

char *strdup(const char *s); 


/** 
    This method prints out a usage message and then has a exit failure
*/
static void usage()
{
  fprintf( stderr, "Usage: driver [-term]\n" );
  exit( EXIT_FAILURE );
}

/**
    This method compares the two strings in lexographic order.
    @param a the first string 
    @param b the second string
    @return 0 if the strings are equal
*/
static int compareStrings(void const *a, void const *b) 
{
    char *const *pointera = (char *const *)a;
    char *const *pointerb = (char *const *)b;
    return strcmp(*pointera, *pointerb) < 0 ? 1 : strcmp(*pointera, *pointerb) > 0 ? -1 : 0;
}

/** Handle a single command line and apply it to the segment tree. */

/**
    This method will parse through and exceute a single command on the segment tree
    @param st a pointer to the segment tree
    @param line input string containing the user command
    @param env a longjmp buffer that is used for errors 
    @return true if the quit command was used
*/
bool handleCommand(SegTree *st, char const *line, jmp_buf *env) 
{
    char command[COMMAND_SIZE];
    int idx1; 
    int idx2;
    char temp[TEMP_SIZE];

    // extracting the command
    if (sscanf(line, "%15s", command) != 1) {
      longjmp(*env, INVALID_COMMAND);
    }

    // deals with add command 
    if (strcmp(command, "add") == 0) {
        // checking for string after add command
        if (sscanf(line, "%*s %1023s", temp) != 1) {
            longjmp(*env, INVALID_COMMAND);
        }
        char *str = strdup(temp);
        if (!str) {
          exit(EXIT_FAILURE);
        }
        // adding the address of the string to the segment tree
        addST(st, &str);
    }

    // deals with size command
    else if (strcmp(command, "size") == 0) {
        // no string after size command check
        if (sscanf(line, "%*s %s", temp) == 1) {
            longjmp(*env, INVALID_COMMAND);
        }
        printf("%d\n", sizeST(st));
    }

    // deals with the get command
    else if (strcmp(command, "get") == 0) {
        // checking for integer value after get command
        if (sscanf(line, "%*s %d %s", &idx1, temp) != 1) {
            longjmp(*env, INVALID_COMMAND);
        }
        char **res = (char **)getST(st, idx1, env);
        printf("%s\n", *res);
    }

    // deals with the remove command
    else if (strcmp(command, "remove") == 0) {
        // checking for no arguments after remove command
        if (sscanf(line, "%*s %s", temp) == 1) {
            longjmp(*env, INVALID_COMMAND);
        }
        // gets string of the last index and frees it
        char **last = (char **)getST(st, sizeST(st) - 1, env);
        free(*last);

        // removing the value from the tree
        removeST(st, env);
    }

    // deals with the set command
    else if (strcmp(command, "set") == 0) {
        // checking for addtional int value and string
        if (sscanf(line, "%*s %d %1023s", &idx1, temp) != TWOARG) {
            longjmp(*env, INVALID_COMMAND);
        }
        // freeing the current string
        char **old = (char **)getST(st, idx1, env);
        free(*old);
        // creating and storing a new string
        char *new = strdup(temp);

        if (!new) {
            exit(EXIT_FAILURE);
        }
        setST(st, idx1, &new, env);
    }

    // deals with the queyr command
    else if (strcmp(command, "query") == 0) {
        // checks for two integers
        if (sscanf(line, "%*s %d %d", &idx1, &idx2) != TWOARG) {
            longjmp(*env, INVALID_COMMAND);
        }
        // getting the indeex of the best value 
        int resultIdx = queryST(st, idx1, idx2, env);
        char **res = (char **)getST(st, resultIdx, env);
        printf("%s\n", *res);
    }

    // deals with the quit command
    else if (strcmp(command, "quit") == 0) {
        return true;
    }
    // deals with invalid command
    else {
        longjmp(*env, INVALID_COMMAND);
    }

    return false;
}

/**
   Starting point for the program.
   @param argc number of command-line arguments.
   @param argv array of strings given as command-line arguments.
   @return exit status for the program.
 */
int main( int argc, char *argv[] )
{
    // See if our input is from a terminal.
    bool interactive = isatty( STDIN_FILENO );

    // Parse command-line arguments.
    int apos = 1;
    while ( apos < argc ) {
        if ( strcmp( argv[ apos ], "-term" ) == 0 ) {
            interactive = true;
            apos += 1;
        } 
        else {
            usage();
        }
    }
    // creating the segment tree
    SegTree *st = makeST(sizeof(char *), compareStrings);
    jmp_buf env;

    // processing the commands
    while (true) {
        if (interactive) {
          printf("cmd> ");
          fflush(stdout);
        }

        // reading through a line from stdin
        char *line = readLine(stdin);
        if (!line) {
            break;
        }
        // handling the error
        int code = setjmp(env);
        if (code == 0) {
            // current command
            if (handleCommand(st, line, &env)) {
                free(line);
                break;
            }
        } 
        // segment tree error
        else if (code == SEGTREE_ERROR) {
            if (interactive) {
                printf("Invalid parameters\n");
            }
            else {
                fprintf(stderr, "Invalid parameters: %s\n", line);
                free(line);
                freeST(st);
                return EXIT_FAILURE;
            }
        } 
        // command parse and validaton
        else {
            if (interactive) {
                printf("Invalid command\n");
            } 
            else {
                fprintf(stderr, "Invalid command: %s\n", line);
                free(line);
                freeST(st);
                return EXIT_FAILURE;
            }
        }

        free(line);
    }

    // freeing all the dynamically allocated strings
    for (int i = 0; i < sizeST(st); i++) {
        char **ptr = (char **)getST(st, i, NULL);
        free(*ptr);
    }
    // freeing the segment tree
    freeST(st);

    return EXIT_SUCCESS;
}