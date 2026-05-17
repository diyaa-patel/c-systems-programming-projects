# Compiler 
CC = gcc 
CFLAGS = -Wall -std=c99 -g 

# Building the program 
calc: calc.o table.o overflow.o 

# Building calc.o 
calc.o: calc.c table.h overflow.h 

# Building table.o 
table.o: table.c table.h overflow.h 

# Building overflow.o 
overflow.o: overflow.c overflow.h 

#cleaning the .o files and executable 
clean: 
	rm -f calc.o table.o overflow.o calc