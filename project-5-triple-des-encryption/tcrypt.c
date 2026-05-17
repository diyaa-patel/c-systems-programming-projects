/**
   @file tcrypt.c 
   @author Diya Patel 
   
   This file contains the main method to start the program 
   that uses the command line arguments for the Triple-DES encryption/decryption.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IO.h"
#include "TDES.h"
#define MAX_KEY_LENGTH 24
#define ENCRYPT_ARG 4 
#define DECRYPT_ARG 5 
#define SECOND_ARG 2
#define THIRD_ARG 3 
#define FOURTH_ARG 4  

/**
    This method starts the program and where the command line arguments are read from 
    @param argc the command line arguments 
    @param *argv an array of the arguements
*/
int main(int argc, char *argv[]) 
{
    // setting the default to encryption
    bool encrypt = true;
    char *keyFile = NULL;
    char *inputFile = NULL;
    char *outputFile = NULL;

    // parsing through the arguments for decryption
    if (argc == DECRYPT_ARG && strcmp(argv[1], "-d") == 0) {
        encrypt = false;
        keyFile = argv[SECOND_ARG];
        inputFile = argv[THIRD_ARG];
        outputFile = argv[FOURTH_ARG];
    } // parsing arguments for encryption
    else if (argc == ENCRYPT_ARG) {
        encrypt = true;
        keyFile = argv[1];
        inputFile = argv[SECOND_ARG];
        outputFile = argv[THIRD_ARG];
    } 
    else {
        // if the args don't match the correct format
        fprintf(stderr, "usage: tcrypt [-d] KEY_FILE INPUT_FILE OUTPUT_FILE\n");
        return EXIT_FAILURE;
    }

    // loasing the key into memory 
    int keyLen;
    byte *key = readFile(keyFile, &keyLen);
    if (!key || keyLen != MAX_KEY_LENGTH) {
        fprintf(stderr, "Invalid key length\n");
        return EXIT_FAILURE;
    }

    // loading the input file into memeory
    int inputLen;
    byte *input = readFile(inputFile, &inputLen);
    if (!input) {
        fprintf(stderr, "Could not read input file\n"); 
        free(key);
        return EXIT_FAILURE;
    }

    // Run the encryption or decryption 
    int outputLen;
    byte *output = NULL;
    if (encrypt) {
        output = encryptTDES(input, inputLen, key, keyLen, &outputLen);
    } 
    else {
        output = decryptTDES(input, inputLen, key, keyLen, &outputLen);
    }

    // checking to see output is not empty
    if (!output) {
        fprintf(stderr, "Encryption/Decryption failed\n");
        free(input);
        free(key);
        return EXIT_FAILURE;
    }

    // writing result to the output file
    if (!writeFile(outputFile, output, outputLen)) {
        fprintf(stderr, "Could not write output file\n");
        free(input);
        free(key);
        free(output);
        return EXIT_FAILURE;
    }

    // freeing the memoery
    free(input);
    free(key);
    free(output);
    return EXIT_SUCCESS;
}