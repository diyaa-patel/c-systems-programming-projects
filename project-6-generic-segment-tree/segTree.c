/**
    @file segTree.c 
    @author Diya Patel

    Implements the generic segment tree and has methods for freeing the memeory and addtion, remove, querying 
    and updating the tree.
*/


#include "segTree.h"
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <stdbool.h>

/** Initial capacity of the segment tree. */
#define INITIAL_CAP 4

/** Overhead of for the tree, relative to the capacity. */
#define TREE_OVERHEAD 2

/** the parent node, relative to the node */
#define PARENT(node) ((node) / 2)

/** the left node relative to the node */
#define LEFT(node)   ((node) * 2)

/** the right node relative to the node */
#define RIGHT(node)  ((node) * 2 + 1)

/** divde for the midpoint */
#define MIDPOINT 2

/** double the capcity */
#define CAPACITY 2

/**
    the internal strcture for the segment tree
*/
struct SegTreeStruct 
{
    // pointer to a list of values
    void *vList;
    // a tree array which stores the indices
    int *tree;
    // the number of elements in the tree
    int size;
    // the capacity of the vList
    int capacity;
    // the size in bytes of the stored value
    size_t vSize;
    // a comparator function
    CompFunc vComp;
    // a boolean to check if the tree needs validation
    bool dirty;

};


SegTree *makeST(size_t vSize, CompFunc vComp) {

    SegTree *st = malloc(sizeof(SegTree));
    st->size = 0;
    st->capacity = INITIAL_CAP;
    st->vSize = vSize;
    st->vComp = vComp;
    st->dirty = true;

    // allocating space for the values and the tree indecides.
    st->vList = malloc(vSize * st->capacity);
    st->tree = malloc(sizeof(int) * st->capacity * TREE_OVERHEAD);
    // initalizing the tree with -1 --> invalid
    for (int i = 0; i < st->capacity * TREE_OVERHEAD; i++) {
        st->tree[i] = -1;
    }
    return st;
}

void freeST(SegTree *st) 
{
    // freeing the vLsit
    free(st->vList);
    // freeing the tree
    free(st->tree);
    // freing the segment tree
    free(st);
}

int sizeST(SegTree *st) 
{
    // returning the size of the segment tree
    return st->size;
}

int addST(SegTree *st, void *valPtr) 
{
    // resizng if the capacity is reached
    if (st->size == st->capacity) {
        st->capacity *= CAPACITY;
        // reallocing the value array
        st->vList = realloc(st->vList, st->capacity * st->vSize);
        // reallocating the tree strcure 
        st->tree = realloc(st->tree, sizeof(int) * st->capacity * TREE_OVERHEAD);
        for (int i = 0; i < st->capacity * TREE_OVERHEAD; i++) {
            st->tree[i] = -1;
        }
    }
    // cpying the value into vList
    memcpy((char*)st->vList + st->size * st->vSize, valPtr, st->vSize);
    int idx = st->size;
    st->size++;
    // needs revalidation
    st->dirty = true;
    // returning the index
    return idx;
}

void removeST(SegTree *st, jmp_buf *env) 
{
    // invalid if size is 0
    if (st->size == 0) {
        if (env) longjmp(*env, SEGTREE_ERROR);
    }

    st->size--;
    // needs revalidation
    st->dirty = true;
}

void *getST(SegTree *st, int idx, jmp_buf *env) 
{
    // checking for invalid index
    if (idx < 0 || idx >= st->size) {
        if (env) longjmp(*env, SEGTREE_ERROR);
    }

    return (char*)st->vList + idx * st->vSize;
}

void setST(SegTree *st, int idx, void *valPtr, jmp_buf *env) 
{
    // checking for invalid index
    if (idx < 0 || idx >= st->size) {
        if (env) longjmp(*env, SEGTREE_ERROR);
    }
    // copying the value into the correct place
    memcpy((char*)st->vList + idx * st->vSize, valPtr, st->vSize);
    st->dirty = true;
}

/**
    This recursive function validates the subtree rooted at node. The L and R parameters give the range of sequence values covered by node
    @param st the segment tree 
    @param node the subtree to see if it is valid 
    @param L the left of the node 
    @param R the right of the node
*/
static void validate(SegTree *st, int node, int L, int R) 
{
    if (L > R || node >= st->capacity * TREE_OVERHEAD) { 
        return;
    }
    // leaf node
    if (L == R) {
        if (L < st->size)
            st->tree[node] = L;
        return;
    }

    int mid = (L + R) /MIDPOINT;

    // recursively validating the children
    validate(st, LEFT(node), L, mid);
    validate(st, RIGHT(node), mid + 1, R);

    int leftIdx = st->tree[LEFT(node)];
    int rightIdx = st->tree[RIGHT(node)];

    // determing the best value between the left and right children
    if (leftIdx == -1) {
        st->tree[node] = rightIdx;
    }
    else if (rightIdx == -1) {
        st->tree[node] = leftIdx;
    }
    else {
        void *leftVal = (char *)st->vList + leftIdx * st->vSize;
        void *rightVal = (char *)st->vList + rightIdx * st->vSize;
        st->tree[node] = (st->vComp(leftVal, rightVal) > 0) ? leftIdx : rightIdx;
    }
}

/**
    This recursive function function is used to find the best value in the subtree rooted at node
    @param st the segment tree 
    @param node the node subtree
    @param L the left of the node 
    @param R the right of the node
    @param i a range of values 
    @param j a range of values
*/
static int query(SegTree *st, int node, int L, int R, int i, int j) {
    // checking if it is outside the range
    if (i > R || j < L) {
        return -1;
    }
    // if it is fully contained
    if (i <= L && j >= R) {
        return st->tree[node];
    }

    int mid = (L + R) / MIDPOINT;
    
    // querying the left and right subtrees
    int leftIdx = query(st, LEFT(node), L, mid, i, j);
    int rightIdx = query(st, RIGHT(node), mid + 1, R, i, j);

    // retruning the best index
    if (leftIdx == -1) {
        return rightIdx;
    }
    if (rightIdx == -1) {
        return leftIdx;
    }

    void *leftVal = (char *)st->vList + leftIdx * st->vSize;
    void *rightVal = (char *)st->vList + rightIdx * st->vSize;
    return (st->vComp(leftVal, rightVal) > 0) ? leftIdx : rightIdx;
}

int queryST(SegTree *st, int i, int j, jmp_buf *env) {

    // invalid range check
    if (i < 0 || j >= st->size || i > j) {
        if (env) longjmp(*env, SEGTREE_ERROR);
    }
    if (st->dirty) {
        validate(st, 1, 0, st->size - 1);
        st->dirty = false;
    }
    return query(st, 1, 0, st->size - 1, i, j);
}