#include "memorymgr.h"
#include <stdio.h>
#include <stdlib.h>

char *p;

void  initmemory(int size){  // initialize a simulated heap
	int x = size + 12;
	if(x%8 != 0)
		x+=8-(x%8);
	p = (char *)malloc(x);
	*(p+4) = x-8;
	*(p+(x-4)) = 1;
}
void *myalloc(int length){   // allocate a block of length (or more) bytes
	char *block = p+4;
	int temp = *block;
	int inc = 0;
	void *res = NULL;
	int need;
	if(length%8!=0)
		if(length%4==0)
			need = length+4;
		else{
			need = length + (4 - (length % 4)) + 4;
			if(need%8!=0)
				need = need + (8-(need%8));
		}
	else
		need = length + 8;
	while(1){
		if(temp == 1)
			break;
		else if(temp%2!=0)
			inc += (temp-1);
		else{
			if(temp<need)
				inc += temp;
			else if(temp==need){
				*(block+inc) = need + 1;
				res = (void *)(block+inc+4);
				break;
			}else{
				*((block+inc)+need) = *(block+inc)-need;
				*(block+inc) = need + 1;
				res = (void *)(block+inc+4);
				break;
			}
		}
		temp = *(block+inc);	
	}
	return res;
}
void  myfree(void *ptr){     // free an allocated block
	char *d = (char *)ptr;
	*(d-4) = *(d-4) - 1;
}
void  coalesce(){            // go through the heap, coalescing unallocated blocks
	int *temp = (int *)p;
	temp = temp+1;
	int *next = nextBlock(temp);
	while(*next != 1){
		if(!isAllocated(temp) && !isAllocated(next))
			*temp = *temp + *next;
		else
			temp = next;
		next = nextBlock(temp);
	}
}
void  printallocation(){  // print info about the simulated heap, for debugging purposes
	int *block = (int *)p;
	block = block + 1;
	int count = 0;
	while(1){
		if(*block == 1)
			break;
		else if(*block%2 == 0)
			printf("Block %d: size %d, unallocated\n", count, *block);
		else
			printf("Block %d: size %d, allocated\n", count, *block-1);
		count++;
		block = nextBlock(block);
	}
	printf("\n");
}

int  isAllocated(int *ptr){  // is the block at location p allocated?
	if(*ptr % 2 == 0)
		return 0;
	else
		return 1;
}
int *nextBlock(int *ptr){    // return a pointer to the block that follows p
	int next = *ptr;
	int *nextp;
	if(next%2==0)
		nextp = (ptr+(next/4));
	else{
		next = (next-1)/4;
		nextp = (ptr+next);
	}
	return nextp;
}
int *firstBlock(){         // return a pointer to the first block on the heap
	int *ptr = (int *)p;
	ptr = ptr + 1;
	return ptr;
}
int *lastBlock(){          // return a pointer to the sentinel block
	int *ptr = firstBlock();
	int *next = nextBlock(ptr);
	while(*next != 1){
		ptr = next;
		next = nextBlock(ptr);
	}
	return next;
}	
