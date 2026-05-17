/** 
    @file TDES.h
    @author Diya Patel

    Header for the Triple DES Implementation.
*/

#include "TDESinternal.h"
#include "IO.h"
#define EXPANDED_HALF_BLOCK_BITS 48
#define EXPANDED_HALF_BLOCK_BYTES 6


/**
    This function returns 0 or 1 based on the the value of the bit
    at index idx in the given array of bytes
    
    @param data an array of bytes
    @param idx the index in the given array
    @return 0 or 1s
*/
int getBit( byte const data[], int idx );

/**
    This function clears (if val is zero) or sets (if val is one) the bit at index idx of the data array
    
    @param data an array of bytes
    @param idx the index in the given array
    @param val the value to set
*/
void putBit(byte data[], int idx, int val);

/**
    This function performs the permute operation, copying n bits from the given input array 
    to output selected by the first n elements of perm

	@param output the output array
    @param input the input array
    @param perm array which shows which bits to copy
    @param n the amount of bits to permute
*/
void permute( byte output[], byte const input[], int const perm[], int n );

/**
   This function computes 16 subkeys based on the input key and stores each one in an element of the given K array.

    @param K an array to hold the subkeys
    @param key an 8 bytes DES key
*/
void generateSubkeys( byte K[ ROUND_COUNT ][ SUBKEY_BYTES ], byte const key[ BLOCK_BYTES ] );

/**
   This function returns the result of an S-Box calculation in the four high-order bits of output[ 0 ]
    
    @param output the output byte
    @param input an input array of 48 bits
    @param idx the index for sBox
*/
void sBox( byte output[ 1 ], byte const input[ SUBKEY_BYTES ], int idx );

/**
   This function performs a single block encrypt operation on the byte array in block, using the subkeys in the K array.
    
    @param result the result after the fFunction
    @param R 32-bit right half
    @param K 48-bit subkey
*/
void fFunction( byte result[ HALF_BLOCK_BYTES ], byte const R[ HALF_BLOCK_BYTES ], byte const K[ SUBKEY_BYTES ] );

/**
    This function performs a single block encrypt operation on the byte array in block, using the subkeys in the K array.
    
    @param block the block to be encrypted
    @param K array of generated subkeys
*/
void encryptBlock( byte block[ BLOCK_BYTES ], byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] );

/**
    This function performs a single block decrypt operation on the byte array in block, using the subkeys in the K array

    @param block the block to be decrypted
    @param K array of generated subkeys
*/
void decryptBlock( byte block[ BLOCK_BYTES ], byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] );

/**
    Given an input array of inputLen bytes and a key array of keyLen bytes (which must be 24), this function adds padding if needed and encrypts 
    the resulting array using the Triple DES algorithm
	
	@param input an array of inputLen bytes
    @param inputLen the length of the input array 
    @param key the key array 
    @param keyLen the length of the key array 
    @param *n the length of th resulting array
	
	@return a pointer to a dynamically allocated array

    
*/
byte *encryptTDES( byte input[], int inputLen, byte key[], int keyLen, int *n );

/**
    This function is like encryptTDES() but it performs the decrypt operation rather than encrypting.

    @param input an array of inputLen bytes
    @param inputLen the length of the input array 
    @param key the key array 
    @param keyLen the length of the key array 
    @param *n the length of th resulting array
*/
byte *decryptTDES( byte input[], int inputLen, byte key[], int keyLen, int *n );
