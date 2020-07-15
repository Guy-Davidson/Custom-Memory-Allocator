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

/**************************Global Variables**************************/
extern uint8_t heap[HEAP_SIZE];


int main(int argc, char const *argv[])
{			
	Allocator_Allocate(1);

	Allocator_Allocate(2);

	Allocator_Allocate(3);

	Allocator_Allocate(3);

	Allocator_Allocate(5);

	Allocator_PrintHeap();

	
	return 0;
}

