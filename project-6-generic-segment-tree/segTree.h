#include <stdlib.h>
#include <setjmp.h>

/** Incomplete type for a segment tree. */
typedef struct SegTreeStruct SegTree;

/** Constant value for longjmp() to indicate an invalid call to a
    segTree function. */
#define SEGTREE_ERROR 100

/** Incomplete type for a segment tree. */
typedef struct SegTreeStruct SegTree;

/** Constant value for longjmp() to indicate an invalid call to a segTree function. */
#define SEGTREE_ERROR 100

// Function pointer type for comparison
typedef int (*CompFunc)(void const *, void const *);

/** 
   This function dynamically allocates and initializes an instance of a segment tree
   @param vSize the size of each value in bytes
   @param vComp a comparison function 
   @return a pointer that points to the newly created segment tree
 */
SegTree *makeST( size_t vSize, int (*vComp)( void const *, void const * ) );

/**
    This function frees all the memory used to store the given segment tree,, including the memory for the array values and the nodes in the tree
    @param st Pointer to segment tree to free
 */
void freeST(SegTree *st);

/** 
    This function returns the number of values currently stored in the segment tree.
    @param st the segment tree
    @return the number of values currently stored
 */
int sizeST(SegTree *st);

/** 
    This function adds the given value to the segment tree at the next available index
    @param st the segment tree
    @param valPtr pointer to the value that will be added
    @return the index where value was added.
 */
int addST(SegTree *st, void *valPtr);

/**
    This function removes the last value from the segment tree. Although the tree grows when it reaches its capacity, it isn’t expected to reduces its storage when it shrinks
    @param st the segment tree
    @param env jump buffer for errors
 */
void removeST(SegTree *st, jmp_buf *env);

/** 
    This function returns a pointer the start of the segment tree value at index idx
    @param st the segment tree
    @param idx index of value.
    @param env a jump buffer for errors
    @return a pointer to the value.
 */
void *getST(SegTree *st, int idx, jmp_buf *env);

/** 
    This function replaces the segment tree value at the given index with the value pointed to by valPtr
    @param st the segment tree
    @param idx the index to set
    @param valPtr a pointer to new value
    @param env jump buffer for errors
 */
void setST(SegTree *st, int idx, void *valPtr, jmp_buf *env);

/** 
    This function performs a query on the segment tree. It finds and returns the index of the maximum value 
    @param st the segment tree.
    @param i  the start index.
    @param j End index.
    @param env a jump buffer for errors
    @return the index of the maximum value or minimum value
 */
int queryST(SegTree *st, int i, int j, jmp_buf *env);

