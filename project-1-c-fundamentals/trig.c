/**
    @file trig.c
    @author Diya Patel (dapatel8)
    This program print out a table of successively more accurate approximations of the sine of x, the cosine of x
    and the tangent of x. 
*/
#include <stdio.h>
#include <stdlib.h>

/** Approximation of PI.  For now, we're using our own constant for
    this. There's also one provided by the gnu math library, once we
    learn about that. */
#define PI 3.14159265358979323846

/** Threshold for the difference between the current sin/cos and previous sin/cos*/ 
#define THRESHOLD 0.000001

/**
    This function read the angle input from the user and returns it.
    prints "Invalid input" error message and exit the program successfully	
    @return angle the angle the user inputs. 
*/
double getAngle()
{
    //the angle inputted by the user 
    double angle; 

    //checking if the input is a double, and in between -PI and PI 
    //if it doesn't satsify the conditions it is a invalid input 
    if(scanf("%lf", &angle) != 1 || angle < -PI || angle > PI){
        printf("Invalid input\n"); 
        exit(EXIT_FAILURE);
    }
    return angle; 
}

/**
    This function returns the positive difference between a and b.
    @param a one of the double values 
    @param b the other double value 	
    @return difference the difference between the two doubles. 
*/
double difference( double a, double b )
{
    // returns the positive difference between the two numbers. 
    return (a > b) ? (a - b) : (b - a); 
}

/**
    This function prints out the two header lines at the top of the table. 
*/
void tableHeader()
{
    printf("terms |         sin |         cos |         tan \n"); 
    printf("------+-------------+-------------+-------------\n"); 
}

/** 
    This function prints a row of the table and reports the number of terms used for the Taylor series 
    approximation(terms), the current value of sine(s), cosine(c), and tangent(t). 
    @param terms the number of terms 
    @param s the current value of sine 
    @param c the current value of cosine 
    @param t the current value of tangent 
*/
void tableRow( int terms, double s, double c, double t )
{
    //prints the first column of terms in a 5 column field the rest of the values
    //are printed in an 11 character field and rounded to 7 fractional digits.
    printf("%5d | %11.7f | %11.7f | %11.7f\n", terms, s, c, t); 
}

/**
    This function takes in a int for the exponent and a double to the base to raise it to.
    @param exp the exponent 
    @param base the base that will get raised to the given power
    @return result the power of the base to the exponent 	
*/
double power(double base, int exp)
{
    double result = 1.0; 
    for(int i = 1; i <= exp; i++) {
        result *= base; 
    }
    return result; 
}

/**
    This function takes in a number to make the factorial of 
    @param n the number to get the factorial 
    @return result the factorial of n
*/
double factorial(double n)
{
    double result = 1.0; 
    for(int i = n; i >= 2; i--){
        result *= i; 
    }
    return result;
}

/**
    Program starting point, will use the five functions to read user input and to help to 
    write the output table. 
    @return program exit status 
*/
int main()
{
    /** the current angle */ 
    double x = getAngle(); 
    /** sine value at 0 */ 
    double sine = 0.0; 
    /** cosine value at 0 */
    double cosine = 1.0; 
    /** tangent value */ 
    double tangent = 0.0; 
    /** the sine value before the current value */
    double prevSine = 0.0; 
    /** the cosine value before the current value */ 
    double prevCosine = 1.0;
    /** counter */ 
    int n = 1; 

    //prints the table header. 
    tableHeader(); 

    //print 1st row 
    tableRow(n, sine, cosine, tangent); 

    //Taylor series logic starts 
    //keeps repeating until the difference between the two is less than the threshold
    while(1) {

        //only using odd n for sine 
        if(n % 2 == 1) {
            //finding the sine term according to Taylors series = (-1)^n * x^(2n + 1) / (2n + 1)! 
            double sineTerm = (power(-1, (n - 1) / 2) * power(x, n)) / factorial(n);
            // adding to sine 
            sine += sineTerm;
        }

        //using even powers for cosine 
        if(n % 2 == 0) {
            //finding the cosine term according to Taylors series = (-1)^n * x^(2n) / (2n)!
            double cosTerm = (power(-1, n / 2) * power(x, n)) / factorial(n); 
            cosine += cosTerm;
        }

        //finding the tangent by dividing sine/cosine 
        if(cosine != 0.0){
            tangent = sine / cosine; 
        }
        else{
            tangent = 0.0;
        }

        //prints the table row of the current values.
        tableRow(n + 1, sine, cosine, tangent); 

        // checking to see if the difference is within the threshold
        if( n > 1 && difference(prevSine, sine) < THRESHOLD && difference(prevCosine, cosine) < THRESHOLD){
            break; 
        }

        //updating the values to become previous 
        prevSine = sine; 
        prevCosine = cosine; 

        //updating n to go to next term 
        n++; 

    }  

    return EXIT_SUCCESS; 
}
