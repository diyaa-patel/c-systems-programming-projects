/**
   @file overflow.h 
   @author Diya Patel 
   header file for overflow.c 
 */

// Prototypes in this header need the bool type.
#include <stdbool.h>
#include <limits.h>

/**
   This method checks to see if computing a + b would cause an overflow of the long int type.
   
   @param a the first long to add 
   @param b the second long to add 
   @return false if a + b ccauses an overflow 
*/
bool checkAdd(long a, long b);


/**
   This method checks to see if computing a - b would cause an overflow of the long int type.

   @param a the first long to subtract 
   @param b the second long to subtract 
   @return false if a - b ccauses an overflow 
*/
bool checkSub(long a, long b);

/**
   This method checks to see if computing a * b would cause an overflow of the long int type.

   @param a the first long to multiplication 
   @param b the second long to multiplication 
   @return false if a * b ccauses an overflow 
*/
bool checkMul(long a, long b);


/**
   This method checks to see if computing a / b would cause an overflow of the long int type.

   @param a the first long to division 
   @param b the second long to division 
   @return false if a / b ccauses an overflow 
*/
bool checkDiv(long a, long b);