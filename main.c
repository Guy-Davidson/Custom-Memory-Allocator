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

/**************************Global Variables**************************/
extern uint8_t heap[HEAP_SIZE];


int main(int argc, char const *argv[])
{			
	uint8_t* ptr1 = NULL;
	uint8_t* ptr2 = NULL;	 
	uint8_t* ptr3 = NULL;
	uint8_t* ptr4 = NULL;
	uint8_t* ptr5 = NULL;	 
	uint8_t* ptr6 = NULL;

	ptr1 = Allocator_Allocate(1);	
	ptr2 = Allocator_Allocate(1);
	ptr3 = Allocator_Allocate(1);
	ptr4 = Allocator_Allocate(1);			
	ptr5 = Allocator_Allocate(1);			
	ptr6 = Allocator_Allocate(1);	
	
	Allocator_Deallocate(ptr6); 
	Allocator_Deallocate(ptr1); 	
	Allocator_Deallocate(ptr2); 	
	Allocator_Deallocate(ptr3); 	
	Allocator_Deallocate(ptr4); 	
	Allocator_Deallocate(ptr5); 	
	
	Allocator_Allocate(430);
	Heap_PrintHeap();	
	//Allocator_Allocate();
	//Heap_PrintHeap();
	


	return 0;
}

