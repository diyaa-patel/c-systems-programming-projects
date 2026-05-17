/** 
    @file TDES.c
    @author Diya Patel
    Implementation of the DES / Triple DES algorithm.
*/

#include "TDES.h"
#include "magic.h"
#include "TDESinternal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_KEY_LENGTH 24
#define MSB_INDEX (BYTE_SIZE - 1)
#define SBOX_INPUT_BITS 6
#define SBOX_OUTPUT_BITS 4
#define SBOX_ROW_BITS 2
#define SBOX_COL_BITS 4
#define SBOX_COL_BIT2 3
#define SBOX_COL_BIT4 5
#define BYTE_SIZE_2 16

int getBit( byte const data[], int idx ) 
{
    idx--;
    //returns the bit at the spefici index from the data aray
    return (data[idx / BYTE_SIZE] >> (MSB_INDEX - (idx % BYTE_SIZE))) & 1;
}

void putBit( byte data[], int idx, int val ) 
{
    idx--;
    if (val) { //sets the bit
        data[idx / BYTE_SIZE] |= (1 << (MSB_INDEX - (idx % BYTE_SIZE)));
    }
    else { //clears the bit
        data[idx / BYTE_SIZE] &= ~(1 << (MSB_INDEX - (idx % BYTE_SIZE)));
    }
}

void permute( byte output[], byte const input[], int const perm[], int n ) {
    // initalizes the output to zero
    memset(output, 0, ROUND_TO_BYTES(n));

    // arraging the bits according to the permutation table. 
    for (int i = 0; i < n; i++) {
        putBit(output, i + 1, getBit(input, perm[i]));
    }
}

/**
    This method performs a circular left shift on a byte array by the given number 

    @param data the data to shift
    @param shifts the amount of shifts to have
*/
static void leftShift(byte data[], int len, int shifts) 
{

    //shifting the data to the left by using a circular shift
    for (int i = 0; i < shifts; i++) {
        int first = getBit(data, 1);
        for (int j = 1; j < len; j++) {
            putBit(data, j, getBit(data, j + 1));
        }
        //wrapping the bit around
        putBit(data, len, first);
    }
}

void generateSubkeys( byte K[ROUND_COUNT][SUBKEY_BYTES], byte const key[BLOCK_BYTES] ) 
{
    byte C[HALF_SUBKEY_BYTES];
    byte D[HALF_SUBKEY_BYTES];
    byte CD[BLOCK_BYTES];

    //permutes the left portion
    permute(C, key, leftSubkeyPerm, HALF_SUBKEY_BITS);
    //permutes the right portion
    permute(D, key, rightSubkeyPerm, HALF_SUBKEY_BITS);

    for (int i = 1; i < ROUND_COUNT; i++) {
        leftShift(C, HALF_SUBKEY_BITS, subkeyShiftSchedule[i]);
        leftShift(D, HALF_SUBKEY_BITS, subkeyShiftSchedule[i]);

        memset(CD, 0, BLOCK_BYTES);
        // combining C and D
        for (int j = 0; j < HALF_SUBKEY_BITS; j++) {
            putBit(CD, j + 1, getBit(C, j + 1));
            putBit(CD, j + HALF_SUBKEY_BITS + 1, getBit(D, j + 1));
        }
        //making the subkey
        permute(K[i], CD, subkeyPerm, SUBKEY_BITS);
    }
}

void sBox( byte output[1], byte const input[SUBKEY_BYTES], int idx ) 
{
    int start = idx * SBOX_INPUT_BITS;
    // calculation for row and column indexes 
    int row = (getBit(input, start + 1) << 1) | getBit(input, start + SBOX_INPUT_BITS);
    int col = (getBit(input, start + SBOX_ROW_BITS) << SBOX_COL_BIT2) | (getBit(input, start + SBOX_COL_BIT2) << SBOX_ROW_BITS)
            | (getBit(input, start + SBOX_COL_BITS) << 1) | getBit(input, start + SBOX_COL_BIT4);

    byte result = sBoxTable[idx][row][col];
    // aligning the result
    output[0] = result << SBOX_OUTPUT_BITS;
}

void fFunction( byte result[HALF_BLOCK_BYTES], byte const R[HALF_BLOCK_BYTES], byte const K[SUBKEY_BYTES] ) 
{
    byte expandedR[SUBKEY_BYTES] = {0};
    // expanding R
    permute(expandedR, R, expandedRSelector, SUBKEY_BITS);

    for (int i = 0; i < SUBKEY_BYTES; i++) {
        expandedR[i] ^= K[i];
    }

    byte sboxResult[HALF_BLOCK_BYTES] = {0};
   
    // sbox substitution
    for (int i = 0; i < SBOX_COUNT; i++) {
        byte tmp[1];
        sBox(tmp, expandedR, i);
        putBit(sboxResult, i * SBOX_OUTPUT_BITS + 1, getBit(tmp, 1));
        putBit(sboxResult, i * SBOX_OUTPUT_BITS + SBOX_ROW_BITS, getBit(tmp, SBOX_ROW_BITS));
        putBit(sboxResult, i * SBOX_OUTPUT_BITS + SBOX_COL_BIT2, getBit(tmp, SBOX_COL_BIT2));
        putBit(sboxResult, i * SBOX_OUTPUT_BITS + SBOX_OUTPUT_BITS, getBit(tmp, SBOX_OUTPUT_BITS));
    }

    permute(result, sboxResult, fFunctionPerm, HALF_BLOCK_BITS);
}

void encryptBlock( byte block[BLOCK_BYTES], byte const K[ROUND_COUNT][SUBKEY_BYTES] ) {
    byte L[HALF_BLOCK_BYTES], R[HALF_BLOCK_BYTES], tempR[HALF_BLOCK_BYTES];

    // initial left half permutation
    permute(L, block, leftInitialPerm, HALF_BLOCK_BITS);
    //initia right half permutation
    permute(R, block, rightInitialPerm, HALF_BLOCK_BITS);

    for (int i = 1; i < ROUND_COUNT; i++) {
        // saving R
        memcpy(tempR, R, HALF_BLOCK_BYTES);
        //using fFunction
        fFunction(R, R, K[i]);
        for (int j = 0; j < HALF_BLOCK_BYTES; j++) {
            R[j] ^= L[j];
        }
        // left become the old right
        memcpy(L, tempR, HALF_BLOCK_BYTES);
    }

    byte finalBlock[BLOCK_BYTES];
    // combining left and right in the opposite order
    for (int i = 0; i < HALF_BLOCK_BITS; i++) {
        putBit(finalBlock, i + 1, getBit(R, i + 1));
        putBit(finalBlock, i + 1 + HALF_BLOCK_BITS, getBit(L, i + 1));
    }

    permute(block, finalBlock, finalPerm, BLOCK_BITS);
}

void decryptBlock( byte block[BLOCK_BYTES], byte const K[ROUND_COUNT][SUBKEY_BYTES] ) {
    byte reverseK[ROUND_COUNT][SUBKEY_BYTES];

    // reversing the subkey order
    for (int i = 1; i < ROUND_COUNT; i++) {
        memcpy(reverseK[i], K[ROUND_COUNT - i], SUBKEY_BYTES);
    }
    // use encryptBlock in reverse to decrypt
    encryptBlock(block, reverseK);
}

byte *encryptTDES( byte input[], int inputLen, byte key[], int keyLen, int *n ) 
{
    //check for valid key length
    if ( keyLen != MAX_KEY_LENGTH ) {
        fprintf( stderr, "Invalid key length\n" );
        exit( EXIT_FAILURE );
    }

    // find the padding length
    int padLength = inputLen + (BLOCK_BYTES - inputLen % BLOCK_BYTES);
    byte *padInput = (byte *)calloc(padLength, sizeof(byte));
    memcpy(padInput, input, inputLen);

    // adding the padding
    byte padVal = padLength - inputLen;
    for (int i = inputLen; i < padLength; i++) {
        padInput[i] = padVal;
    }

    // generating the subkeys for the three keys
    byte subkeys1[ROUND_COUNT][SUBKEY_BYTES];
    byte subkeys2[ROUND_COUNT][SUBKEY_BYTES];
    byte subkeys3[ROUND_COUNT][SUBKEY_BYTES];

    generateSubkeys(subkeys1, key);
    generateSubkeys(subkeys2, key + BYTE_SIZE);
    generateSubkeys(subkeys3, key + BYTE_SIZE_2);

    byte *result = (byte *)malloc(padLength);
    // performing Triple DES encryption for every block
    for (int i = 0; i < padLength; i += BLOCK_BYTES) {
        memcpy(result + i, padInput + i, BLOCK_BYTES);
        encryptBlock(result + i, subkeys1);
        decryptBlock(result + i, subkeys2);
        encryptBlock(result + i, subkeys3);
    }

    free(padInput);
    // setting the output length
    *n = padLength;
    return result;
}

byte *decryptTDES( byte input[], int inputLen, byte key[], int keyLen, int *n ) {

    // checking to see if it is the correct key length
    if ( keyLen != MAX_KEY_LENGTH ) {
        fprintf( stderr, "Invalid key length\n" );
        exit( EXIT_FAILURE );
    }

    // checking that the length is valid
    if ( inputLen == 0 || inputLen % BLOCK_BYTES != 0 ) {
        fprintf(stderr, "Invalid encrypted data length\n");
        exit( EXIT_FAILURE );
    }

    // generating the subkeys for each of the keys
    byte subkeys1[ROUND_COUNT][SUBKEY_BYTES];
    byte subkeys2[ROUND_COUNT][SUBKEY_BYTES];
    byte subkeys3[ROUND_COUNT][SUBKEY_BYTES];

    generateSubkeys(subkeys1, key);
    generateSubkeys(subkeys2, key + BYTE_SIZE);
    generateSubkeys(subkeys3, key + BYTE_SIZE_2);

    byte *result = (byte *)malloc(inputLen);

    // checking for malloc failure
    if ( !result ) {
        fprintf( stderr, "memory allocation failed\n" );
        free(result);
        exit( EXIT_FAILURE );
    }

    // performing Triple DES decryption for every block
   for (int i = 0; i < inputLen; i += BLOCK_BYTES) {
        memcpy(result + i, input + i, BLOCK_BYTES);
        decryptBlock(result + i, subkeys3);
        encryptBlock(result + i, subkeys2);
        decryptBlock(result + i, subkeys1);
    }


    // removing the padding after decryption
    byte padVal = result[inputLen - 1];

    // if there is invalid padding
    if (padVal <= 0 || padVal > BLOCK_BYTES) {
        fprintf( stderr, "Invalid padding\n" );
        free(result);
    }
    // setting the output length
    *n = inputLen - padVal;
    return result;
}

