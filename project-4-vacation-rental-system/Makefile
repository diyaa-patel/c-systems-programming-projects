# Compiler 
CC = gcc 
CFLAGS = -Wall -std=c99 -g 

# Building the program 
rental: rental.o database.o input.o 

# Building rental.o 
rental.o: rental.c database.h input.h 

# Building database.o 
database.o: database.c database.h input.h 

# Building input.o 
input.o: input.c input.h 

# cleaning the .o files and executable 
clean: 
	rm -f rental.o database.o input.o rental