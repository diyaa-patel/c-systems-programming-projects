# Compiler 
CC = gcc 
CFLAGS = -Wall -std=c99 -g 

# Building the program 
tcrypt: tcrypt.o TDES.o IO.o magic.o 

# Building TDEStest
TDEStest: TDEStest.o TDES.o magic.o 

# compling for the object files
tcrypt.o: tcrypt.c TDES.h

TDEStest.o: TDEStest.c TDES.h TDESinternal.h

TDES.o: TDES.c TDES.h TDESinternal.h IO.h magic.h

IO.o: IO.c IO.h 

magic.o: magic.c magic.h

# cleaning the .o files and executable 
clean: 
	rm -f tcrypt.o TDEStest.o TDES.o IO.o magic.o
	rm -f rental TDEStest