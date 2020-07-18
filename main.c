/**************************Include Libraries & Headers******************/
#include <stdint.h>
#include <stdio.h>
//program definitions
#include "definitions.h"
//loging error info & debug
#include "log.h"
//struct Block module
#include "block.h"
//Memory allocator module
#include "allocator.h"
//merge adjacent free block module
#include "merge.h"
//heap information module
#include "heap.h"
//analyze heap data
#include "processHeap.h"

/**************************Global Variables**************************/
extern uint8_t heap[HEAP_SIZE];


int main(int argc, char const *argv[])
{					
	

	uint8_t* array[3];

	array[0] = NULL;
	array[0] = Allocator_Allocate(1);

	array[1] = NULL;
	array[1] = Allocator_Allocate(25);	

	array[2] = NULL;
	array[2] = Allocator_Allocate(60);
	
	array[0][1] = 100;

	//Allocator_Deallocate(array[0]);
	//Allocator_Deallocate(array[1]);
	//Allocator_Deallocate(array[2]);

	//Heap_PrintHeap();

	ProcessHeap_SortAllocatedBlocks();

	return 0;
}


