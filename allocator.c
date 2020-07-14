/*Standsrd Libraries*/
#include <stdint.h>
#include <stdio.h>
/**
 * Header Files Declarations
 * definitions.h includes all program defines.
 * log.h includes all program error, info and debug outputs.
 * block.h is a module for defining the block struct.
 * allocator.h is this module header.
 */
#include "definitions.h"
#include "log.h"
#include "block.h"
#include "allocator.h"
#include "bit.h"


/*****************Global Variables***********************/

//heap is not initialized similerly to run-time memory heap.
uint8_t heap[HEADER_SIZE];

enum bool g_isInit = false;

/************Private function declerations**************/

/**
 * [allocator_initialize: init the heap]
 */
static void allocator_initialize();

/**
 * [allocator_isAllocateValid: validate that allocate is valid by
 * checking that bytes is positive, and that there is room to allocat ein the heap.]
 * @param  bytes [number of bytes to allocate.]
 * @return       [bool according to success.]
 */
static enum bool allocator_isAllocateValid(uint32_t bytes);



/****************Function Implementations*****************/

void allocator_initialize()
{	
	LOG_INFO("Initializing Heap...\n");
	
	Bit_Insert32BitsIn4Bytes(heap + SIZE_COUNTER, HEAP_HEADER_SIZE + HEADER_SIZE);	
	Bit_Insert32BitsIn4Bytes(heap + HEAD_FREE, MIN_BUFFER_INDEX);
	
	Block_newBlock('f',HEAP_SIZE - HEAP_HEADER_SIZE - HEADER_SIZE, MIN_BUFFER_INDEX);

	g_isInit = true;

	LOG_INFO("\nInitialization was succesful!\n");

	return;
}

/**
 * [Allocator_Allocate: allocated bytes cells in heap]
 * @param  bytes [number of bytes to allocate]
 * @return       [buffer index upon success and NILL upon failure.]
 */
uint8_t* Allocator_Allocate(uint32_t bytes)			
{
	//check if possible to start allocate.
	if (!g_isInit) allocator_initialize();

	if (!allocator_isAllocateValid(bytes)) 
	{
			LOG_ERROR("Invalid arguments for allocating block");
			return NULL;
	}
	
	//find where to allocate new block
	uint32_t bufferIndex = 0;	
	uint32_t currentFree = HEAD_FREE; 	
	//iterate to find larage enough space.
	bufferIndex = Bit_IndexTo32Bit(heap + currentFree);
	uint32_t freeBlockSize = Bit_IndexTo32Bit(heap + SIZE_INDEX);
	uint32_t newBlockSize = HEADER_SIZE + bytes;
	
	//creat new allocated block.
	if (!Block_newBlock('a', bytes, Bit_IndexTo32Bit(heap + currentFree)))
	{
		LOG_ERROR("failure to create new block.");
		return NULL;
	}
	
	//update free block for being smaller.
	if (!Block_newBlock('f', freeBlockSize - newBlockSize, Bit_IndexTo32Bit(heap + currentFree) + newBlockSize))
	{
		LOG_ERROR("failure to create new block.");
		return NULL;
	}
	
	//update heap size.
	uint32_t newSize = Bit_IndexTo32Bit(heap + SIZE_COUNTER) + newBlockSize;
	Bit_Insert32BitsIn4Bytes(heap + SIZE_COUNTER, newSize);
	
	//update head free
	Bit_Insert32BitsIn4Bytes(heap + HEAD_FREE, Bit_IndexTo32Bit(heap + currentFree) + newBlockSize);

	return heap;
}

/**
 * [allocator_isAllocateValid: validate that allocate is valid by
 * checking that bytes is positive, and that there is room to allocat ein the heap.]
 * @param  bytes [number of bytes to allocate.]
 * @return       [bool according to success.]
 */
enum bool allocator_isAllocateValid(uint32_t bytes)
{
	if (bytes <= 0) 
	{
		LOG_ERROR("bytes non-positive.");
		return false;
	}

	if (Bit_IndexTo32Bit(heap + SIZE_COUNTER) + HEADER_SIZE + bytes > HEAP_SIZE)
	{
		LOG_ERROR("heap overflow");
		return false;
	}

	return true;
}


/***************Debuging Functions Implementations****************/

#ifdef DEBUG

/**
 * [Allocator_PrintHeapHeader: prints the heap header
 	to get info about heap size and head of free list.]
 */
void Allocator_PrintHeapHeader()
{
	if (!g_isInit) allocator_initialize();

	printf("Heap Header -> {");
	
	printf("%d, ", Bit_IndexTo32Bit(heap + SIZE_COUNTER));
		
	printf("%d}\n", Bit_IndexTo32Bit(heap + HEAD_FREE));

	return;
}

/**
 * [Allocator_PrintHeap: prints the entire heap block by block.]
 */
void Allocator_PrintHeap()
{
	Allocator_PrintHeapHeader();

	uint32_t bufferIndex = MIN_BUFFER_INDEX;

	while (bufferIndex < HEAP_SIZE)
	{
		Block_PrintBlock(bufferIndex);		
		bufferIndex = bufferIndex + Bit_IndexTo32Bit(heap + SIZE_INDEX) + HEADER_SIZE;
	}

	return; 
}

#endif 



/*******************End of File**********************/
