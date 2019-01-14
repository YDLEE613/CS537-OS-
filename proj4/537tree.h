/* name: Yoodong Lee, Won Jun Cho */

#include <stdint.h>
#include <stdio.h>

typedef struct btree{
	uintptr_t addr;
	struct btree *parent;
	size_t size;
	struct btree *left;
	struct btree *right;
	int free;
	int realloc;
}btree;

void addNode(void*, size_t);
btree *searchNode(void*,size_t);
void printAddr(btree*);
void freeAddr(void*);
int checkRange(void*, size_t);
void reTree(btree*);
btree *createNode(uintptr_t, size_t);
btree *getUnbalNode(btree*);
int getBalance(btree*);
int getHeight(btree*);
btree* rightRotate(btree*);
btree* leftRotate(btree*);
