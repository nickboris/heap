#include <stdio.h>
#include <stdlib.h>
#include "gc.h"

void *stackBottom();
void mark(int **p);

void gc(){
	int *p = NULL;
	int **top = (int **)&p;
	int **bot = (int **)stackBottom();
	int *first = (int *)firstBlock();
	int *last = (int *)lastBlock();
	while(top != bot){
		if(*top>first && *top<last){
			int *temp = (int *)firstBlock();
			int *next = (int *)nextBlock(temp);
			while(next<*top){
				temp = next;
				next = (int *)nextBlock(next);
			}	
			if(*temp%8 == 1)
				*temp = *temp + 2;
			mark((int **)temp);			
		}
		top = top + 1;		
	}
	int *scan = first;
	while(scan != last){
		if(*scan%8 == 1)
			*scan = *scan - 1;
		if(*scan%4 == 3)
			*scan = *scan - 2;
		scan = (int *)nextBlock(scan);
	}
	coalesce();
}

//scans for pointers to other blocks in the heap
void mark(int **p){
	int **endBlock = (int **)nextBlock(p);
	int *first = (int *)firstBlock();
	int *last = (int *)lastBlock();
	while(p!=endBlock){
		if(*p>first && *p<last){
			int *temp = (int *)firstBlock();
			int *next = (int *)nextBlock(temp);
			while(next<*p){
				temp = next;
				next = (int *)nextBlock(next);
			}	
			if(*temp%8 == 1)
				*temp = *temp + 2;			
		}
		p = p + 1;
	}	
}

void *stackBottom() {
	unsigned long bottom;
    FILE *statfp = fopen("/proc/self/stat", "r");
    fscanf(statfp,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
           "%*u %*u %*u %*u %*u %*u %*d %*d "
           "%*d %*d %*d %*d %*u %*u %*d "
           "%*u %*u %*u %lu", &bottom);
    fclose(statfp);
	return (void *) bottom;
}
