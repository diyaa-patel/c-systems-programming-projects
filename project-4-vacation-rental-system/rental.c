/**
    @file rental.c 
    @author Diya Patel
    This program will contain the top level componenet, containing main() and code to parse user commands.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "database.h"
#include "input.h"

/** the minimum number of arguments */
#define NUM_ARGS 2

/** the max length for cmd */ 
#define MAX_LENGTH_CMD 20

/** max length for argument */ 
#define MAX_LENGTH_ARG 30

/** reserve commands */ 
#define RESERVE_CMD 4

/** cancel command */ 
#define CANCEL_CMD 3

/**
    main function that read house files, processes user commands, and cleaning up allocated memory.
    @param argc the commands entered 
    @param *argv[] list of inputs
 
 */
int main(int argc, char *argv[]) 
{
   // checking to see there are at least two arguments.
   if (argc < NUM_ARGS) {
        fprintf(stderr, "usage: rental house-file+\n");
        exit(EXIT_FAILURE);
    }

    // creating the database
    Database *database = makeDatabase();

    // putting the arguments into an array
    for (int i = 1; i < argc; i++) {
        readHouses(argv[i], database);
    }

    char *command;
    // checking for the first command
    bool firstCommand = true;

    // processing the commands and break once finished
    while (1) {
        // reading in the input
        command = readLine(stdin);

        //check for no command
        if (command == NULL) {
            // if EOF is reached
            if (!firstCommand) {
                printf("\ncmd> ");
            }     
            break;
        }

        // for space formatting
        if (!firstCommand) {
            printf("\n"); 
        }
        firstCommand = false;

        // so memeory is not used for no reason
        if (strlen(command) == 0) { 
            free(command);
            continue;
        }

        printf("cmd> %s\n", command);  

        // for parsing
        char cmd[MAX_LENGTH_CMD];
        char arg1[MAX_LENGTH_ARG];

        // getting the command
        if (sscanf(command, "%19s", cmd) != 1) {
            printf("Invalid command\n");
            free(command);
            continue;
        }

        // checking what kind of command it is
        // listing
        if (strcmp(cmd, "list") == 0) {
            int offset = 0;
            if (sscanf(command + strlen(cmd), "%29s%n", arg1, &offset) != 1) {
                sortHouses(database, compareById);
                listHouses(database, alwaysTrue, 0, 0);
            } // listing by name
            else if (strcmp(arg1, "names") == 0) {
                sortHouses(database, compareByName);
                listHouses(database, alwaysTrue, 0, 0);
            } else if (strcmp(arg1, "available") == 0) {
                sortHouses(database, compareById);
                listHouses(database, isAvailable, 0, 0);
            } else {
                printf("Invalid command\n");
            }
        }// reserve command
        else if (strcmp(cmd, "reserve") == 0) {
            int houseId, month, day;
            char email[EMAIL_LENGTH];

            int parsed = sscanf(command + strlen(cmd), "%d %d %d %29s", &houseId, &month, &day, email);

            bool validSaturday = isValidSaturday(month, day);
            bool reservationSuccess = reserveHouse(database, houseId, month, day, email);

            if (parsed != RESERVE_CMD || !validSaturday || !reservationSuccess) {
                printf("Invalid command\n");
            }
        } // cancel command
        else if (strcmp(cmd, "cancel") == 0) {
            int houseId, month, day;

            int parsed = sscanf(command + strlen(cmd), "%d %d %d", &houseId, &month, &day);

            if (parsed != CANCEL_CMD || !cancelReservation(database, houseId, month, day)) {
                printf("Invalid command\n");
            }
        } // search command
        else if (strcmp(cmd, "search") == 0) {
            char type[MAX_LENGTH_CMD];
            int offset = 0;

            int typeParsed = sscanf(command + strlen(cmd), "%19s%n", type, &offset);

            if (typeParsed == 1 && strcmp(type, "customer") == 0) {
                char email[EMAIL_LENGTH];
                int emailParsed = sscanf(command + strlen(cmd) + offset, "%29s", email);

                if (emailParsed == 1) {
                    customer_email = email;
                    sortHouses(database, compareById);
                    listHouses(database, isReservedByCustomer, 0, 0);
                    customer_email = NULL;
                } else {
                    printf("Invalid command\n");
                }
            }
            else if (typeParsed == 1 && strcmp(type, "date") == 0) {
                int month, day;
                if (sscanf(command + strlen(cmd) + offset, "%d %d", &month, &day) == NUM_ARGS && isValidSaturday(month, day)) {
                    sortHouses(database, compareByGuests);
                    listHouses(database, isAvailableOnDate, month, day);
                } else {
                    printf("Invalid command\n");
                }
            }
            else {
                printf("Invalid command\n");
            }
        } // quit command
        else if (strcmp(cmd, "quit") == 0) {
            free(command);
            freeDatabase(database);
            exit(EXIT_SUCCESS);
        }
        else {
            printf("Invalid command\n");
        }
        // freeing the command once done
        free(command);
    }
    // freeing entire database
    freeDatabase(database);
    return EXIT_SUCCESS;
}