/**
    @file database.c 
    @author Diya Patel
    This program will contain code for reading and managing the houses read in at startup.
*/

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "input.h"
#include "database.h"

/** email of the customer */
char *customer_email = NULL;

/** the amount of parts for valid house */
#define HOUSE_LINE 3

/** year */ 
#define YEAR 2025 

/** invalid date */ 
#define INVALID_DATE -1

/** month and day counter */
#define MN_DD 2

/** to change the capacity */
#define INC_CAP 2

/** minimum bound for month */
#define MIN_MONTH 6

/** the maximum bound for month */
#define MAX_MONTH 11

/** start day */
#define START_DAY 7 

/** end day */
#define END_DAY 29

/** offset number */
#define OFFSET 14

/** year in months */ 
#define YEAR_MONTHS 12

/** month factor */
#define MONTH_FACTOR 31

/** numeric value of saturday */
#define SATURDAY 6

/** the divisor for leap year */
#define LEAP_YEAR 4

/** century divisor */
#define CENTURY 100

/** exception leap year */
#define EXCEPTION_YEAR 400

/** days in the week */
#define DAYS_IN_WEEK 7

Database *makeDatabase()
{
    // allocating memeory for the database
    Database *base = malloc(sizeof(Database));

    //if memory allocation fails exit 
    if(!base) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // allocating memeory for house pointer array
    base->houses = malloc(INITIAL_CAPACITY * sizeof(House *));
    if (!base->houses) {
        fprintf(stderr, "Memory allocation failed\n");
        //freeing database
        free(base);
        exit(EXIT_FAILURE);
    }

    // initalizing the fields
    base->count = 0;
    base->capacity = INITIAL_CAPACITY;
 
    return base;
}

void freeDatabase(Database *database) 
{
    // if database doesn't exit
    if (!database) {
        exit(EXIT_FAILURE);
    }

    // going through every house
    for (int i = 0; i < database->count; i++) {
        House *house = database->houses[i];
        // freeing each date
        for (int j = 0; j < house->count; j++) {
            free(house->dates[j]);
        }
        // freeing the dates pointer and house
        free(house->dates);
        free(house);
    }

    // free the house pointers and the database
    free(database->houses);
    free(database);
}

void readHouses(const char *filename, Database *database) 
{
    // opening the file
    FILE *fp = fopen(filename, "r");

    // check for valid file
    if (!fp) {
        fprintf(stderr, "Can't open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char *line;
    while ((line = readLine(fp)) != NULL) {
        House *house = malloc(sizeof(House));
        if (!house) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        // reading the house id, guest amount, and cost
        if (sscanf(line, "%d %d %d", &house->id, &house->maxGuests, &house->cost) != HOUSE_LINE) {
            fprintf(stderr, "Invalid house file: %s\n", filename);
            free(house);
            exit(EXIT_FAILURE);
        }

        // making sure cost is positive 
        if (house->cost < 0) {
            fprintf(stderr, "Invalid house file: %s\n", filename);
            free(house);
            free(line);
            fclose(fp);
            exit(EXIT_FAILURE);
        }

        free(line);

        // checking for duplicate id
        for (int i = 0; i < database->count; i++) {
            if (database->houses[i]->id == house->id) {
                fprintf(stderr, "Invalid house file: %s\n", filename);
                free(house);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
        }

        line = readLine(fp);
        if (!line) {
            fprintf(stderr, "Invalid house file: %s\n", filename);
            free(house);
            exit(EXIT_FAILURE);
        }
        // exit if there is a long house name
        if (strlen(line) > NAME_LENGTH && !isdigit(line[0])) {
            fprintf(stderr, "Invalid house file: %s\n", filename);
            free(house);
            free(line);
            fclose(fp);
            exit(EXIT_FAILURE);  
        }

        strncpy(house->name, line, NAME_LENGTH);
        house->name[NAME_LENGTH] = '\0';
        free(line);

        house->count = 0;
        house->capacity = INITIAL_CAPACITY;
        house->dates = malloc(INITIAL_CAPACITY * sizeof(Date *));
        if (!house->dates) {
            fprintf(stderr, "Memory allocation failed for house dates\n");
            free(house);
            exit(EXIT_FAILURE);
        }

        // Read and parse the date line
        line = readLine(fp);
        if (!line) {
            fprintf(stderr, "Invalid house file: %s\n", filename);
            free(house);
            exit(EXIT_FAILURE);
        }

        char *ptr = line;
        int month, day;
        while (*ptr != '\0') {
            while (*ptr == ' ' || *ptr == '\t') ptr++; 

            if (sscanf(ptr, "%d / %d", &month, &day) == MN_DD || sscanf(ptr, "%d/%d", &month, &day) == MN_DD) {

                if (!isValidSaturday(month, day)) {
                } else {
                    Date *date = malloc(sizeof(Date));
                    date->month = month;
                    date->day = day;
                    date->isReserved = false;

                    if (house->count >= house->capacity) {
                        house->capacity *= INC_CAP;
                        house->dates = realloc(house->dates, house->capacity * sizeof(Date *));
                        if (!house->dates) {
                            fprintf(stderr, "Memory allocation failed\n");
                            free(house);
                            exit(EXIT_FAILURE);
                        }
                    }

                    house->dates[house->count++] = date;
                }
            }

            // moves to the next date
            while (*ptr != ' ' && *ptr != '\0') ptr++; 
        }

        free(line);

        //adding the house
        database->houses[database->count++] = house;

        // resize array
        if (database->count >= database->capacity) {
            database->capacity *= INC_CAP;
            database->houses = realloc(database->houses, database->capacity * sizeof(House *));
            if (!database->houses) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(fp);
}

void sortHouses(Database *database, int (*compare)(const void *, const void *)) 
{
    // exit if there is nothing to sort
    if (!database || database->count == 0) {
        exit(EXIT_FAILURE);
    }

    // sorting the array of house poitners
    qsort(database->houses, database->count, sizeof(House *), compare);
}

void listHouses(Database *database, bool (*test)(House const *house, int val1, int val2), int val1, int val2) 
{
    // printing the header
    printf("ID  Name                           Sleeps   Cost");
    if (test == isAvailable || test == isReservedByCustomer) {
        printf("  Date");
    }
    printf("\n");

    for (int i = 0; i < database->count; i++) {
        House *house = database->houses[i];

        // sorting the houses by date
        for (int j = 0; j < house->count - 1; j++) {
            for (int k = j + 1; k < house->count; k++) {
                if ((house->dates[j]->month > house->dates[k]->month) ||
                    (house->dates[j]->month == house->dates[k]->month && house->dates[j]->day > house->dates[k]->day)) {
                    Date *temp = house->dates[j];
                    house->dates[j] = house->dates[k];
                    house->dates[k] = temp;
                }
            }
        }

        if (test == isAvailable) {
            // printing the houses with avaibale dates
            for (int j = 0; j < house->count; j++) {
                if (!house->dates[j]->isReserved) {
                    printf("%-3d %-30s %6d %6d %02d/%02d\n", house->id, house->name,
                           house->maxGuests, house->cost, house->dates[j]->month, house->dates[j]->day);
                }
            }
        } else if (test == isReservedByCustomer) {
            // printing the houses that are reserved
            for (int j = 0; j < house->count; j++) {
                if (house->dates[j]->isReserved && strcmp(house->dates[j]->customerEmail, customer_email) == 0) {
                    printf("%-3d %-30s %6d %6d %02d/%02d\n", house->id, house->name,
                           house->maxGuests, house->cost, house->dates[j]->month, house->dates[j]->day);
                }
            }
        } else {
            // printing the houses with no dates
            if (test == alwaysTrue || test == isAvailableOnDate) {
                if (test(house, val1, val2)) {
                    printf("%-3d %-30s %6d %6d\n", house->id, house->name,
                           house->maxGuests, house->cost);
                }
            }
        }
    }
}

bool isValidSaturday(int month, int day) 
{
    // checking with bounds
    if (month < MIN_MONTH || month > MAX_MONTH) {
        return false;
    }
    if ((month == MIN_MONTH && day < START_DAY) || (month == MAX_MONTH && day > END_DAY)) {
        return false;
    }

    // Zeller's algorithm to see if date in on Sat
    int y = YEAR; 
    int w = y - (OFFSET - month) / YEAR_MONTHS;
    int x = w + w / LEAP_YEAR - w / CENTURY + w / EXCEPTION_YEAR;
    int z = month + YEAR_MONTHS * ((OFFSET - month) / YEAR_MONTHS) - MN_DD;
    int dayOfWeek = (day + x + (MONTH_FACTOR * z) / YEAR_MONTHS) % DAYS_IN_WEEK;

    return dayOfWeek == SATURDAY;
}

int compareById(const void *a, const void *b) 
{
    // comparing the houses by id
    House *houseA = *(House **)a;
    House *houseB = *(House **)b;
    return houseA->id - houseB->id;
}

int compareByName(const void *a, const void *b) 
{
    // comparing the houses using the name
    House *houseA = *(House **)a;
    House *houseB = *(House **)b;
    int nameComparison = strcmp(houseA->name, houseB->name);
    return (nameComparison != 0) ? nameComparison : (houseA->id - houseB->id);
}

int compareByGuests(const void *a, const void *b) 
{
    House *houseA = *(House **)a;
    House *houseB = *(House **)b;

    // compare with max guests
    if (houseA->maxGuests != houseB->maxGuests) {
        return houseA->maxGuests - houseB->maxGuests;
    }
    // compare with id
    return houseA->id - houseB->id;
}

bool alwaysTrue(House const *house, int val1, int val2) 
{
    return true; 
}

bool isAvailableOnDate(House const *house, int month, int day) 
{

    // going through and checking if the house is avaliable
    for (int i = 0; i < house->count; i++) {
        if (house->dates[i]->month == month && house->dates[i]->day == day &&
            !house->dates[i]->isReserved) {
            return true;
        }
    }
    return false;
}

bool isAvailable(House const *house, int val1, int val2)
{
    for (int i = 0; i < house->count; i++) {
        if (!house->dates[i]->isReserved) {
            return true; // If any date is available, return true
        }
    }
    return false; // No available dates found
}

bool isReservedByCustomer(House const *house, int unused, int val2) 
{
    if (!customer_email) {
        return false; // No email to check
    }

    for (int i = 0; i < house->count; i++) {
        if (house->dates[i]->isReserved && strcmp(house->dates[i]->customerEmail, customer_email) == 0) {
            return true;
        }
    }
    return false;
}

bool cancelReservation(Database *database, int id, int month, int day) 
{
    for (int i = 0; i < database->count; i++) {
        House *house = database->houses[i];

        if (house->id == id) {
            for (int j = 0; j < house->count; j++) {
                if (house->dates[j]->month == month && house->dates[j]->day == day &&
                    house->dates[j]->isReserved) {

                    house->dates[j]->isReserved = false;
                    house->dates[j]->customerEmail[0] = '\0'; // Clear email field
                    return true;
                }
            }
        }
    }
    return false;
}

bool reserveHouse(Database *database, int id, int month, int day, char *email) 
{

    for (int i = 0; i < database->count; i++) {
        House *house = database->houses[i];

        if (house->id == id) {
            

            for (int j = 0; j < house->count; j++) {
                if (house->dates[j]->month == month && house->dates[j]->day == day) {
                    if (house->dates[j]->isReserved) {
                        return false;
                    }

                    house->dates[j]->isReserved = true;
                    strncpy(house->dates[j]->customerEmail, email, EMAIL_LENGTH);
                    house->dates[j]->customerEmail[EMAIL_LENGTH - 1] = '\0';


                    return true;
                }
            }
            
            return false;
        }
    }
    
    return false;
}