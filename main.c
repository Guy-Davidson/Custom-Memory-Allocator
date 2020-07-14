/*Standsrd Libraries*/
#include <stdio.h>

/**
 * Header Files Declarations
 * definitions.h includes all program defines.
 * log.h includes all program error, info and debug outputs.
 * customAllocator.h is a module for allocating memory on the global stack.
 */
#include "definitions.h"
#include "log.h"
#include "block.h"
#include "allocator.h"
#include "bit.h"
extern uint8_t heap[HEAP_SIZE];


int main(int argc, char const *argv[])
{		
	Allocator_Allocate(0);

	Allocator_Allocate(3);

	Allocator_Allocate(7);

	Allocator_PrintHeap();

	return 0;
}

