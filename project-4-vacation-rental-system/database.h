/**
    @file database.h 
    @author Diya Patel 
    This file is the h file for database.c
*/
// Header for database
#define DATABASE_H

#include <stdbool.h>

// Max length of the house name
#define NAME_LENGTH 30

// Initial capacity of array
#define INITIAL_CAPACITY 5

// Max length for customer email
#define EMAIL_LENGTH 30

// global email variable
extern char *customer_email; 

// typedef struct for the date info
typedef struct 
{
    int month; 
    int day; 
    bool isReserved; 
    char customerEmail[EMAIL_LENGTH];

} Date;

// typedef struct for the house info 
typedef struct
{
    int id; 
    int maxGuests;
    int cost; 
    char name[NAME_LENGTH + 1];
    // arrya to pointer of date structs
    Date **dates;
    //count for the dates
    int count;
    //capacity of the data array
    int capacity;
} House;

// typedef struct for the rental database
typedef struct
{
   House **houses;
   //count of houses
   int count;
   // capacity of the house array
   int capacity;
} Database;

typedef struct {
    House *house;
    int month;
    int day;
} HouseDate;

/**
    This function dynamically allocates storage for the Database, initializes its fields (to store a resizeable array) and returns 
    a pointer to the new Database

    @return a pointer to the new Database
*/
Database *makeDatabase();

/**
    This function frees the memory used to store the given Database, including freeing space for all the Houses and their Date arrays, 
    freeing the resizeable array of pointers and freeing space for the Database struct itself.

    @param *database the given database to free space from	
*/
void freeDatabase(Database *database); 

/**
    This function reads all the houses from a house file with the given name. It makes an instance of the House struct for each one and
    stores a pointer to that house in the resizeable array in database.

    @param *filename the house file to read from
    @param *database where to store the pointer to the house
*/
void readHouses(char const *filename, Database *database);

/**
    This function sorts the houses in the given database. It uses the qsort() function together with the function pointer
    parameter to order the houses. The function pointer is described in the "Sorting Houses" section below.

    @param *database the database to sort houses in
    @param *va a pointer to the inputs 
    @param *vb a pointer to another inputs
*/
void sortHouses(Database *database, int (* compare) (void const *va, void const *vb));

/**
    This function prints all or some of the houses. It uses the function pointer parameter together with the integers, val1 and val2,
    which are passed to the function, to decide which houses to print. This function will be used for the list, list names, and search 
    date commands. The function pointer is described in "Listing Houses" section below.

    @param *database the database to print houses from 
    @param val1 the value of the house 
    @param val2 another value of a house 
*/
void listHouses(Database *database, bool (*test)( House const *house, int val1, int val2 ), int val1, int val2);

/**
    This function returns true if the month and day represent a Saturday date between 6/7 and 11/29, 2025. Otherwose, it returns false.

    @param month the month of the date 
    @param day the day of the date

    @return true if it is a valid date for saturday
*/
bool isValidSaturday(int month, int day);

/**
    This function compares the two house IDs

    @param *a pointer to one house 
    @param *b a pointer to the second house

    @return 0 if they are equal
*/
int compareById(const void *a, const void *b);

/**
    This function compares two houses names

    @param *a pointer to one house 
    @param *b a pointer to the second house

    @return 0 if they are equal
*/
int compareByName(const void *a, const void *b);

/**
    This function compares two houses guests

    @param *a pointer to one house 
    @param *b a pointer to the second house

    @return 0 if they are equal
*/
int compareByGuests(const void *a, const void *b);

/**
    This function always returns true 
    @param *house the pointer to house 
    @param val1 an unused parameter 
    @param val2 an unused parameter

    @return true, always returns true
*/
bool alwaysTrue(House const *house, int val1, int val2);

/**
    This function checks if a house an any avaliable dates 

    @param *house pointer to house 
    @param val1 unused parameter
    @param val2 an unused parameter 

    @return true if the house has available dates
*/
bool isAvailable(House const *house, int val1, int val2);

/**
    This function checks if a house if available on the given date

    @param *house pointer to house struct 
    @param month the month to check 
    @param day the day to check

    @return true if the house is available
*/
bool isAvailableOnDate(House const *house, int month, int day);

/**
    This function checks if a house is reserved 

    @param *house pointer to house struct
    @param unused unused parameter
    @param val2 unused parameter
    @return true if the house is reserved by the given customer
*/
bool isReservedByCustomer(House const *house, int unused, int val2);

/**
    This function returns true if able to reserve house

    @param *database a pointer to the database 
    @param id the house id 
    @param month the month of the reservation 
    @param day the day of the reserivation 
    @param email the customers email
    @return true is the house is reserved successfully 
*/ 
bool reserveHouse(Database *database, int id, int month, int day, char *email);

/**
    This function will return true if it was able to cancel the reservation

    @param *database a pointer to the database
    @param id the house id 
    @param month the month of the reservation 
    @param day the day of the reserivation 
    @return true is the house is cancelled successfully 
*/
bool cancelReservation(Database *database, int id, int month, int day);