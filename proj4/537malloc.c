/* name: Yoodong Lee, Won Jun Cho */

#include "537malloc.h"
#include <stdlib.h>
#include <stdio.h>

void *malloc537(size_t size){
	void *ptr = NULL; 
	if((int)size < 0){
		fprintf(stderr, "Error: Can't take negative size\n");
		exit(-1);
	} 
	if((ptr = malloc(size)) == NULL && size > 0){
		fprintf(stderr, "Error: Unable to allocate memory\n");
		exit(-1);
	}
		
	addNode(ptr, size);
	return ptr;
}

void free537(void *ptr){
	freeAddr(ptr);
}

void *realloc537(void *ptr, size_t size){
	btree *tmp;
	if(ptr == NULL){
		return malloc537(size);
	} else{
		if(size == 0){
			fprintf(stderr, "Warning: reallocating for size 0\n");
		} 

		freeAddr(ptr);
		tmp = searchNode(ptr, 1);
		ptr = realloc(ptr, size);
		tmp->addr = (uintptr_t) ptr;
		tmp->free = 0;
		tmp->size = size;
		reTree(tmp);
	}
	return ptr;
}

void memcheck537(void *ptr, size_t size){
	checkRange(ptr, size);	
}
