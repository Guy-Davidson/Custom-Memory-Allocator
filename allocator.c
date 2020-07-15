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

//heap is not initialized similerly to run-time memory heap.
uint8_t heap[HEAP_SIZE];

enum bool g_isInit = false;

/*****************Private Function Declerations*******************/

/**
 * [allocator_initialize: init the heap]
 */
static void allocator_initialize();

/**
 * [allocator_isAllocateValid: validate that allocate is valid by
 * checking that bytes is positive, and there is enough room to allocate in the heap]
 * @param  bytes [number of bytes to allocate.]
 * @return       [bool according to success.]
 */
static enum bool allocator_isAllocateValid(uint32_t bytes);

/**
 * [allocator_isDeallocateValid: check for a valid previously allocated memory block]
 * @param  HeapCell [Pointer to heap index to be deallocated]
 * @return          [true for a valid memory block and false other wise]
 */
static enum bool allocator_isDeallocateValid(uint8_t* HeapCell);



/*******************Function Implementations********************/

void allocator_initialize()
{	
	LOG_INFO("Initializing Heap...");
	
	HeapHeader newHeapHeader = NULL;

	newHeapHeader = (HeapHeader) heap;

	if (newHeapHeader == NULL)
	{
		LOG_ERROR("Initialization failed");
		return;		
	}

	newHeapHeader->allocMemSize = sizeof(HeapHeader_st) + sizeof(Block_st);

	newHeapHeader->headFree = NULL;	

	if (!Block_NewBlock('f', HEAP_SIZE - newHeapHeader->allocMemSize, heap + sizeof(HeapHeader_st)))
	{
		LOG_ERROR("Initialization failed");
		return;
	}

	newHeapHeader->headFree = (Block) (heap + sizeof(HeapHeader_st));

	LOG_INFO("Initialized succesfuly!");

	g_isInit = true;
	
	return;
}

/**
 * [Allocator_Allocate: allocated bytes cells in the heap]
 * @param  bytes [number of bytes to allocate]
 * @return       [buffer pointer upon success and NULL upon failure.]
 */
uint8_t* Allocator_Allocate(uint32_t bytes)			
{	
	if (!g_isInit) allocator_initialize();

	if (!allocator_isAllocateValid(bytes)) 
	{
			LOG_ERROR("Allocate failed");
			return NULL;
	}

	LOG_INFO("Starting to allocate...");

	HeapHeader currentHH = (HeapHeader) heap;	
	Block freeToAlloc = NULL;
	
	//find where to allocate new block
	freeToAlloc	= currentHH->headFree;
	//iterate to find larage enough space.				
	
	uint32_t localAvailableSize = freeToAlloc->size;	

	//creat new allocated block.			
	if (!Block_NewBlock('a', bytes, (uint8_t*)freeToAlloc))
	{
		LOG_ERROR("Allocate failed");
		return NULL;
	}

	uint32_t newBlockSize = sizeof(Block_st) + freeToAlloc->size;
	
	//update free block for being smaller.		
	if (!Block_NewBlock('f', localAvailableSize - newBlockSize, ((uint8_t*)freeToAlloc) + newBlockSize))
	{
		LOG_ERROR("Allocate failed");
		return NULL;
	}
	
	//update heap size.
	currentHH->allocMemSize += newBlockSize;	
	
	//update head free
	currentHH->headFree = (Block)((uint8_t*)freeToAlloc + newBlockSize);
		
	LOG_INFO("Allocation was succesful!");

	return (uint8_t*)freeToAlloc;
}

/**
 * [allocator_isAllocateValid: validate that allocate is valid by
 * checking that bytes is positive, and that there is room to allocate in the heap.]
 * @param  bytes [number of bytes to allocate.]
 * @return       [bool according to success.]
 */
enum bool allocator_isAllocateValid(uint32_t bytes)
{
	if ((int32_t)bytes <= 0)
	{
		//implementaion decision reguarding allocate(0).
		LOG_ERROR("bytes must be positive");
		return false;
	}

	HeapHeader currentHH = (HeapHeader) heap;

	if (currentHH->allocMemSize + bytes > HEAP_SIZE)
	{
		LOG_ERROR("heap overflow");
		return false;
	}

	return true;
}

/**
 * [Allocator_Deallocate: deallocate a memory block.
 * IMPORTANT: deallocate makes some validation checks (desribed in isDeallocateValid),
 * But the user can free a non previously allocated block by misuse.
 * This implementation decision is inherited from the function free(),
 * as the cost of checking each deallocate is O(n), while currently it's O(1)]
 * @param HeapCell [Pointer to heap cell to be deallocated.]
 */
void Allocator_Deallocate(uint8_t* HeapCell)
{		
	if ((!g_isInit) || (!allocator_isDeallocateValid(HeapCell)))
	{
			LOG_ERROR("Deallocate failed");			
			return;
	}

	LOG_INFO("Starting to deallocate...");	

	LOG_INFO("Deallocating was succesful!");

	return;
}

/**
 * [allocator_isDeallocateValid: check for a valid previously allocated memory block]
 * @param  HeapCell [Pointer to heap index to be deallocated]
 * @return          [true for a valid memory block and false other wise]
 */
enum bool allocator_isDeallocateValid(uint8_t* HeapCell)
{
	uint32_t bufferIndex = HeapCell - heap;	

	if ((bufferIndex <= 0) || (bufferIndex > HEAP_SIZE))
	{
		LOG_DEBUG("Pointer out of bounds");
		return false;
	}
	
	Block freeBlock = (Block) HeapCell;	

	if (freeBlock->mode != 'a') 
	{
		LOG_DEBUG("mode inccorect");
		return false;
	}	

	if (freeBlock->size > MAX_BUFFER_SIZE)
	{
		LOG_DEBUG("block size inccorect");
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
void Allocator_PrintHeapHeader()
{
	if (!g_isInit) allocator_initialize();

	HeapHeader currentHH = (HeapHeader) heap;

	CUSTOMMEMALLOCATOR_PRINT("Heap Header -> {%d, %lu}\n", currentHH->allocMemSize, (uint8_t*)currentHH->headFree - heap);		

	return;
}

/**
 * [Allocator_PrintHeap: prints the entire heap block by block.]
 */
void Allocator_PrintHeap()
{
	Allocator_PrintHeapHeader();

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

#endif 



/*******************End of File**********************/
