/**
    @file overflow.c 
    @author Diya Patel dapatel8
    This program provides functions for detecting overflow in the four basic arithmetic operations. 
 */

#include "overflow.h"
#include <limits.h>

bool checkAdd(long a, long b) 
{
    // Sum of a + b
    long sum = a + b; 

    // Check to see if there is an overflow 
    if((a >= 0 && b >= 0 && sum < 0) || (a < 0 && b < 0 && sum >= 0)){
        return false;
    }

    return true; 
}

bool checkSub(long a, long b)
{
    // Difference of a and b 
    long diff = a - b; 

    // If a negative value - non negative != negative 
    if( a < 0 && b >= 0 && diff >= 0){
        return false; 
    }

    // If non negative - negative != non negative 
    if(a >= 0 && b < 0 && diff < 0){
        return false; 
    }

    return true; 
}

bool checkMul(long a, long b)
{
    // Checking when a and b are both positive 
    if( a > 0 && b > 0) {
        if(a > (LONG_MAX / b)) {
            return false; 
        }
    }
    // Both a and b are negative 
    else if(a < 0 && b < 0) {
        if(a > (LONG_MAX / b)){
            return false; 
        }
    }
    // a is negative and b is positive 
    else if( a < 0 && b > 0){
        if(a < (LONG_MIN / b)){
            return false;
        }
    }
    // a is positive and b is negative 
    else if(a > 0 && b < 0) {
        if(b < (LONG_MIN / a)){
            return false;
        }
    }

    return true;
}

bool checkDiv(long a, long b)
{
    // Handles special case for overflow --> causes value larger than LONG_MAX
    if(a == LONG_MIN && b == -1){
        return false; 
    }

    return true; 
}