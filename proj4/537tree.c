/* name: Yoodong Lee, Won Jun Cho */

#include "537tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

btree *root;
int bal = 0;
//int a = 0; //for debug

btree *createNode(uintptr_t addr, size_t size){
	btree *node = malloc(sizeof(btree));

	node->addr = addr;
    node->size = size;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->free = 0;
    node->realloc = 0;

	return node;
}

/*
 * Add node to the root node
 */
void addNode(void* ptr, size_t size){
	uintptr_t addr = (uintptr_t) ptr;
	
	btree *node =createNode(addr, size); 
	btree *tmp = NULL;
	btree *unbal = NULL;
	
	if(root == NULL){
		root = node;
	}else{
		if(tmp == NULL){
			tmp = root;
		}
		while(1){
			if(tmp->addr > node->addr){
				if(tmp->left != NULL){
					tmp = tmp->left;
				} else{
					tmp->left = node;
					node->parent = tmp;
					break;
				}
			} else if(tmp->addr < node->addr){
				if(tmp->right != NULL){
					tmp = tmp->right;
				} else{
					tmp->right = node;
					node->parent = tmp;
					break;
				}
			}
		}
	}
	unbal = getUnbalNode(node);

	if(unbal != NULL){
		if(bal > 1 && addr < unbal->left->addr){
			rightRotate(unbal);
		}
		else if(bal < -1 && addr > unbal->right->addr){
			leftRotate(unbal);
		}
		else if(bal > 1 && addr > unbal->left->addr){
			unbal->left = leftRotate(unbal->left);
			rightRotate(unbal);
		}
		else if(bal < -1 && addr < unbal->right->addr){
			unbal->right = rightRotate(unbal->right);
			rightRotate(unbal);
		}	
	}
	//printAddr(root);
}

btree *getUnbalNode(btree *node){
	while(node != NULL){
		bal = getBalance(node);
		if(bal > 1 || bal < -1){
			return node;
		}
		else{
			node = node->parent;
		}
	}
	return NULL;
}

int getBalance(btree *node){
	if(node == NULL){
		return 0;
	}
	return getHeight(node->left) - getHeight(node->right); // should be 1 or -1 to be balanced
}

int getHeight(btree *node){
	int left, right;
	if(node == NULL){
		return 0;
	}
	
	left = getHeight(node->left);
	right = getHeight(node->right);
	if(left > right){
		return left + 1;
	} else{
		return right + 1;
	}
}

btree *searchNode(void* addr, size_t size){
	btree* tmp;
	uintptr_t ptr = (uintptr_t) addr;

	if(size <= 0){
		return NULL;
	}
	tmp = root;
	while(tmp != NULL){
		if(ptr == tmp->addr && size <= tmp->size){
			return tmp;
		}
		
		if(ptr < tmp->addr){
			tmp = tmp->left;
		} else if(ptr > tmp->addr){
			if(ptr + size < tmp->addr + tmp->size){
				return tmp;
			}else{
				tmp = tmp->right;
			}
		}
	}
	if(tmp != NULL){
		return tmp;
	}else{
		fprintf(stderr, "Error: Can't find address\n");
		exit(-1);
	}
}


/*
 * Print information of node
 *
 * */
void printAddr(btree *node){
	printf("Parent Node addr: %p \n", &(node->parent->addr));
	printf("Node addr: %p with size %ld\n", &(node->addr), node->size);
	if(node->right){
		printf("\\ ");
		printAddr(node->right);
	}
	if(node->left){
		printf("/ ");
		printAddr(node->left);
	}
}


/*
 * Free the address of the node
 *
 * */
void freeAddr(void* addr){
	btree* node = searchNode(addr, 1);
	if(node != NULL){
		if(node->addr == (uintptr_t)addr && !node->free){
			node->free = 1;
			return;
		}
		if(node->addr != (uintptr_t)addr){
			fprintf(stderr, "Error: Not the first byte of the range of memory\n");
			exit(-1);
		}
		if(node->free){
			fprintf(stderr, "Error: Attempt to double free\n");
			exit(-1);
		}
	}
	free(addr);
}

int checkRange(void* addr, size_t size){
	uintptr_t ptr = (uintptr_t)addr;
	btree *tmp;
	if(size <= 0){
		return 0;
	}
	tmp = root;
	while(tmp != NULL){
		if(tmp->addr == ptr){
			if(tmp->size >= size){
				return 1;
			} else{
				fprintf(stderr, "Error: Memory out of range\n");
				exit(-1);
			}
		} else if(tmp->addr < ptr){
			//if ptr in between tmp's addr
			if(tmp->addr + tmp->size > ptr){
				if(tmp->addr + tmp->size < ptr + size){
					fprintf(stderr, "Error: Memory out of range\n");
					exit(-1);
				}else{
					return 1;
				}
			}else{
				tmp = tmp->right;
			}
		} else{
			tmp = tmp->left;
		}
	}	
	if(tmp == NULL){
		fprintf(stderr, "Error: Memory out of range\n");
		exit(-1);
	}
	return 0;
}

void reTree(btree* node){
	if(root->addr == node->addr){
		return;
	}
	btree *tmp;
	tmp = root;

	//Make parent's left/right NULL
	node->realloc = 1;

	if(node->left != NULL){
		reTree(node->left);
	}

	if(node->right != NULL){
		reTree(node->right);
	}

	while(1){
		if(tmp->addr > node->addr){
			if(tmp->left != NULL && !tmp->left->realloc){
				tmp = tmp->left;
			}else{
				tmp->left = node;
				node->parent = tmp;
				node->realloc = 0;
				break;
			}
		}else if(tmp->addr < node->addr){
			if(tmp->right != NULL && !tmp->right->realloc){
				tmp = tmp->right;
			} else{
				tmp->right = node;
				node->parent = tmp;
				node->realloc = 0;
				break;
			}
		}
	}	
}	

btree* rightRotate(btree* node){
	btree *x = node->left;
	btree *tmp; 
	//btree *nptr = NULL;
	if(root == node){
		root = x;
	}

	if(node->parent->right == node){
		node->parent->right = x;
	}else{
		node->parent->left = x;
	}
	if(x->right){
		tmp = x->right;
		x->parent = node->parent;
		x->right = node;
		node->parent = x;
		node->left = tmp;
		tmp->parent = node;
	} else{
		if(node == root){
			root = x;
		}
		x->parent = node->parent;
		x->right = node;
		if(node->parent){
			node->parent->left = x;
		}
		node->parent = x;
		node->left = NULL;
	}
	

	return x;
}
btree* leftRotate(btree* node){
	btree *x = node->right;
	btree *tmp;
	//btree *nptr = NULL;
	if(node == root){
		root = x; 
	}
	if(node->parent){
		if(node->parent->right == node){
			node->parent->right = x;
		}else{
			node->parent->left = x;
		}
	}
	if(x->left){
		tmp = x->left;
		x->parent = node->parent;
		x->left = node;
		node->parent = x;
		node->right = tmp;
		tmp->parent = node;
	} else{
		// child node doesn't have left
		//tmp = x->right;
		x->parent = node->parent;
		x->left = node;
		node->parent = x;
		node->right = NULL;
	}

	return x;
}
