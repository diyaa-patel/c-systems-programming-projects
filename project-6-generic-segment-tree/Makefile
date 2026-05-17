# Compiler 
CC = gcc 
CFLAGS += -Wall -std=c99 -g
LDLIBS += 

# Building the entire program
all: sort segTreeTest driver

# Building sort
sort: sort.o segTree.o

# Building segTreeTest
segTreeTest: segTreeTest.o segTree.o

# Building the driver
driver: driver.o segTree.o input.o

# cleaning the .o files and executable 
clean: 
	rm -f *.o sort segTreeTest driver
	rm -f *.gcda *.gcno *.gcov