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

//heap is not initialized similerly to run-time memory heap.
uint8_t heap[HEAP_SIZE];

extern enum bool g_isInit;

/*****************Private Function Declerations*******************/

/*******************Function Implementations********************/

/**
 * [Heap_UpdateSize change allocated memory size]
 * @param  size [size to add or decrease]
 * @param  mode ['a' for add and 's' for subtract]
 * @return      [bool according to succes]
 */
enum bool Heap_UpdateSize(uint32_t size, uint8_t mode)
{
	HeapHeader currentHH = NULL;

	currentHH = (HeapHeader) heap;

	if ((!currentHH) || ((mode != 'a') && (mode != 's')))
	{
		LOG_ERROR("UpdateSize failed");
		return false;
	}

	if (mode == 'a') 
	{
		currentHH->allocMemSize += size;
	}
	else
	{
		currentHH->allocMemSize -= size;	
	}

	return true;
}

/**
 * [Heap_UpdateHeadFree change headFree]
 * @return [bool according to succes]
 */
enum bool Heap_UpdateHeadFree(Block block)
{
	HeapHeader currentHH = NULL;

	currentHH = (HeapHeader) heap;

	if (!currentHH)
	{
		LOG_DEBUG("UpdateHeadFree failed");
		return false;
	}

	currentHH->headFree = block;
	return true;
}

/**
 * [Heap_getFreeListHead]
 * @return [free list head]
 */
Block Heap_GetFreeListHead()
{
	HeapHeader currentHH = NULL;

	currentHH = (HeapHeader) heap;

	if (!currentHH)
	{
		LOG_DEBUG("GetFreeListHead failed");
		return NULL;
	}

	return currentHH->headFree;
}

/**
 * [Heap_FreeListInsertFront]
 * @param  block [block to insert]
 * @return       [bool according to succes]
 */
enum bool Heap_FreeListInsertFront(Block block)
{
	HeapHeader currentHH = NULL;

	currentHH = (HeapHeader) heap;

	if ((currentHH == NULL) || (block == NULL))
	{
		LOG_DEBUG("FreeListInsertFront failed");
		return false;
	}

	block->next = currentHH->headFree;
	(currentHH->headFree)->prev = block;
	currentHH->headFree = block;

	return true;
}

/**
 * [Heap_FreeListReplaceMiddle change a Block in the free block list to another]
 * @param  outBlock [block to take off the list]
 * @param  inBlock [block to in the list]
 * @return       [bool according to success]
 */
enum bool Heap_FreeListReplace(Block outBlock, Block inBlock)
{
	//case: outBlock is in middle
	if ((inBlock->next != NULL) && (inBlock->prev != NULL))
	{
		(outBlock->prev)->next = inBlock;
		inBlock->prev = outBlock->prev;

		(outBlock->next)->prev = inBlock;
		inBlock->next = outBlock->next;
	}
	//case: outBlock is in front
	else if (outBlock->next != NULL)
	{
		(outBlock->next)->prev = inBlock;
		inBlock->next = outBlock->next;	
	}
	//case: outBlock is last
	else if (outBlock->prev != NULL)
	{
		(outBlock->prev)->next = inBlock;
		inBlock->prev = outBlock->prev;		
	}
	else
	{
		if (!Heap_UpdateHeadFree(inBlock))
		{
			LOG_DEBUG("FreeListReplace failed");
			return false;
		}
	}

	return true;
}

/**
 * [Heap_FreeListDelete]
 * @param  block [block to be deleted]
 * @return       [bool according to succes]
 */
enum bool Heap_FreeListDelete(Block block)
{
	//case: block is in middle
	if ((block->next != NULL) && (block->prev != NULL))
	{
		(block->prev)->next = block->next;
		(block->next)->prev = block->prev;
	}	
	//case: outBlock is last
	else if (block->prev != NULL)
	{		
		(block->prev)->next = NULL;
	}
	else
	{
		LOG_DEBUG("FreeListReplace failed");
		return false;
	}

	return true;
}

/***************Debuging Functions Implementations****************/

#ifdef DEBUG

/**
 * [Allocator_PrintHeapHeader: prints the heap header
 	to get info about heap allocated memory size and head of free list.]
 */
void Heap_PrintHeapHeader()
{
	if (!g_isInit) 
	{
		LOG_ERROR("Heap uninitialized");
		return;
	}

	HeapHeader currentHH = (HeapHeader) heap;

	CUSTOMMEMALLOCATOR_PRINT("Heap Header -> {allocated memory size: %d, headFree: %lu}\n\n",\
							 currentHH->allocMemSize, (uint8_t*)currentHH->headFree - heap);		

	return;
}

/**
 * [Allocator_PrintHeap: prints the entire heap block by block.]
 */
void Heap_PrintHeap()
{
	Heap_PrintHeapHeader();

	uint8_t* currentBlock = NULL;

	currentBlock = heap + sizeof(HeapHeader_st);		
			 
	while (currentBlock - heap < HEAP_SIZE)
	{
		Block_PrintBlock((Block)currentBlock);		
		currentBlock = currentBlock + sizeof(Block_st) + ((Block)currentBlock)->size;
	}

	printf("\n");
	
	return; 
}

/**
 * [Allocator_PrintFreeBlockList]
 */
void Heap_PrintFreeBlockList()
{
	HeapHeader currentHH = (HeapHeader) heap;
	Block currentFreeBlock = (Block) currentHH->headFree;

	while (currentFreeBlock != NULL)
	{
		Block_PrintBlock(currentFreeBlock);
		currentFreeBlock = currentFreeBlock->next;
	}

	return;
}

#endif 

/*******************End of File**********************/