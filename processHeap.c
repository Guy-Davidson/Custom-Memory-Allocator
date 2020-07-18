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

/*****************Private Function Declerations*******************/

/**
 * [processHeap_blockNext takes a block and returns the bext block in the heap]
 * @return [description]
 */
static Block processHeap_blockNext(Block block);


/*******************Function Implementations********************/

/**
 * [ProcessHeap_GetAllocatedMemSize]
 * @return [size of allocated memory in bytes]
 */
uint32_t ProcessHeap_AllocatedMemSize()
{
	uint32_t res = Heap_GetAllocMemSize();

	if (res > HEAP_SIZE)
	{
		LOG_DEBUG("AllocatedMemSize");
		return FAILED_RETURN;
	}

	return res;
}

/**
 * [ProcessHeap_GetFreeMemSize]
 * @return [size of free memory in bytes]
 */
uint32_t ProcessHeap_FreeMemSize()
{
	uint32_t res = HEAP_SIZE - Heap_GetAllocMemSize();

	if (res < 0)
	{
		LOG_DEBUG("FreeMemSize failed");
		return FAILED_RETURN;
	}

	return res;
}

/**
 * [ProcessHeap_MaxFreeBlock finds the biggest free block in the heap]
 * @return [biggest free block or null if heap is fully allocated]
 */
Block ProcessHeap_MaxFreeBlock()
{

	Block listIterator = NULL;
	Block maxFree = NULL;	

	listIterator  = Heap_GetFreeListHead();
	maxFree = listIterator;

	while (listIterator)
	{
		if ((maxFree != NULL) && (listIterator->size > maxFree->size))
		{
			maxFree = listIterator;
		}		

		listIterator = listIterator->next;
	}

	return maxFree;
}

/**
 * [processHeap_blockNext takes a block and returns the bext block in the heap]
 * @return [description]
 */
Block processHeap_blockNext(Block block)
{
	if (!block)
	{
		LOG_DEBUG("blockNext failed");
		return NULL;
	}

	return (Block) ((uint8_t*)block + sizeof(Block_st) + block->size);
}

/**
 * [ProcessHeap_Reset deallocates the entire heap;]
 */
void ProcessHeap_Reset()
{
	Block blockIterator = NULL;

	blockIterator = (Block) (heap + sizeof(HeapHeader_st));

	while ((uint8_t*)blockIterator - heap < HEAP_SIZE)
	{			

		if (blockIterator->mode == alloc)
		{
			Allocator_Deallocate((uint8_t*)blockIterator + sizeof(Block_st));
		}

		blockIterator = processHeap_blockNext(blockIterator);
	}

	return;
}

/**
 * [ProcessHeap_SortAllocatedBlocks prints the heap allocated block in a sorted descending order]
 */
void ProcessHeap_SortAllocatedBlocks()
{
	uint32_t capacity = HEAP_SIZE / BUFFER_UNIT;

	uint32_t size = 0;

	uint8_t* allocatedArray[HEAP_SIZE / BUFFER_UNIT] = {0};

	Block blockIterator = NULL;

	blockIterator = (Block) (heap + sizeof(HeapHeader_st));

	while ((uint8_t*)blockIterator - heap < HEAP_SIZE)
	{			

		if ((blockIterator->mode == alloc) && (size < capacity))
		{
			allocatedArray[size] = (uint8_t*) blockIterator;
			size++;
		}

		blockIterator = processHeap_blockNext(blockIterator);
	}

	if (!size)
	{
		return;
	}

	//sort array
	for (int i = 0; i < size - 1 ; i++)
	{
		for (int j = 0; j < (size - i - 1) ; j++)
		{
			if (((Block)allocatedArray[j])->size < ((Block)allocatedArray[j + 1])->size)
			{
				uint8_t* temp = allocatedArray[j + 1];
				allocatedArray[j + 1] = allocatedArray[j];
				allocatedArray[j] = temp;
			}
		}
	}

	for (int i = 0; i < size; i++)
	{
		#ifdef DEBUG

		Block_PrintBlock((Block)allocatedArray[i]);

		#else		

		CUSTOMMEMALLOCATOR_PRINT("Allocated memory size: %d\n", ((Block)allocatedArray[i])->size);

		#endif
	}

	return;

}

/*******************End of File**********************/