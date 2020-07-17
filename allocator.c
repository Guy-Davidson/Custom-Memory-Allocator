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

/**
 * [allocator_getFreeToAllocate iterate on the free block linked list to find the smallest,
 * free block that fits bytes cells.]
 * @param  bytes [requested bytes to allocate.]
 * @return       [pointer to big enough free block or NULL incase there is no such block.]
 */
static Block allocator_getFreeToAllocate(uint32_t bytes);


/**
 * [allocator_allocateInsideFree create two new blocks, allocated and free]
 * @param  freeToAlloc [pointer to free block]
 * @return             [bool according to success]
 */
static enum bool allocator_allocateInsideFree(Block freeToAlloc, uint32_t bytes);

/**
 * [allocator_allocateExapandedBlock creates one expanded allocated block]
 * @param  freeToAlloc [pointer to free block]
 * @return             [bool according to success]
 */
static enum bool allocator_allocateExapandedBlock(Block freeToAlloc, uint32_t bytes);

/**
 * [allocator_allocateInPlace switch a free block to allocated.]
 * @param  freeToAlloc [pointer to free block]
 * @return             [bool according to succss]
 */
static enum bool allocator_allocateInPlace(Block freeToAlloc);



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

	if (Heap_GetAllocMemSize() == HEAP_SIZE)
	{
		LOG_ERROR("Allocate failed: heap overflow");
		return NULL;
	}

	if (!allocator_isAllocateValid(bytes)) 
	{
			LOG_ERROR("Allocate failed");
			return NULL;
	}

	LOG_INFO("Starting to allocate...");
	
	Block freeToAlloc = NULL;
	
	//find where to allocate new block
	freeToAlloc = allocator_getFreeToAllocate(bytes);	

	//case: no room to allocate
	if (!freeToAlloc)
	{		
		if (Merge_Exe())
		{
			//try again
			freeToAlloc = allocator_getFreeToAllocate(bytes);
			if(!freeToAlloc)
			{
				LOG_ERROR("Allocate failed: heap overflow");
				return NULL;
			}			
		}
		else
		{
			LOG_ERROR("Allocate failed: heap overflow");
			return NULL;	
		}				
	}
		
	uint32_t remainingSize = freeToAlloc->size - Block_BufferSizePadding(bytes);
	
	//case: create 'a' block and 'f' block.
	if (remainingSize >= 2 * BUFFER_UNIT) 
	{	
		if (!allocator_allocateInsideFree(freeToAlloc, bytes))
		{
			LOG_ERROR("Allocate failed");
			return NULL;
		}
	}
		
	//case: create exapnded 'a' block. (no room for free).
	else if (remainingSize ==  BUFFER_UNIT) 
	{
		if (!allocator_allocateExapandedBlock(freeToAlloc, bytes))
		{
			LOG_ERROR("Allocate failed");
			return NULL;
		}
	}

	//case: create 'a' block only. (no room for free).
	else 
	{
		if(!allocator_allocateInPlace(freeToAlloc))
		{
			LOG_ERROR("Allocate failed");
			return NULL;
		}
	}	

	if (Heap_GetAllocMemSize() == HEAP_SIZE)
	{		
		Heap_UpdateHeadFree(NULL);
	}
		
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

	return true;
}

/**
 * [allocator_getFreeToAllocate iterate on the free block linked list to find the smallest,
 * free block that fits bytes cells.]
 * @param  bytes [requested bytes to allocate.]
 * @return       [pointer to big enough free block or NULL incase there is no such block.]
 */
Block allocator_getFreeToAllocate(uint32_t bytes)
{
	uint32_t requestedSize = Block_BufferSizePadding(bytes);
	Block freeToAlloc = NULL;
	Block freeListIterator = NULL;

	HeapHeader currentHH = (HeapHeader) heap;	

	freeToAlloc	= currentHH->headFree;
	freeListIterator = freeToAlloc->next;

	LOG_INFO("searching list for a free block large enough to allocate...");

	while (freeListIterator != NULL)
	{
		if ((freeToAlloc->size > requestedSize) && (freeListIterator->size > requestedSize))
		{
			if (freeListIterator->size < freeToAlloc->size)
			{
				freeToAlloc = freeListIterator;
			}			
		}
		else if (freeListIterator->size > requestedSize)
		{
			freeToAlloc = freeListIterator;			
		}
		freeListIterator = freeListIterator->next;
	}

	if (freeToAlloc->size < requestedSize)
	{
		LOG_DEBUG("found no available free block");
		return NULL;
	}

	LOG_INFO("found free block to allocate!");
	return freeToAlloc;
}

/**
 * [allocator_allocateInsideFree create two new blocks, allocated and free]
 * @param  freeToAlloc [pointer to free block]
 * @return             [bool according to success]
 */
enum bool allocator_allocateInsideFree(Block freeToAlloc, uint32_t bytes)
{
	uint32_t localAvailableSize = freeToAlloc->size;
	Block_st tempLinkBlock = *freeToAlloc;	

	//creat new allocated block.			
	if (!Block_NewBlock('a', bytes, (uint8_t*)freeToAlloc))
	{
		LOG_DEBUG("allocateInsideFree failed");
		return false;
	}

	uint32_t newBlockSize = sizeof(Block_st) + freeToAlloc->size;
	Block newFreeBlock = (Block)((uint8_t*)freeToAlloc + newBlockSize);
	
	//update free block for being smaller.		
	if (!Block_NewBlock('f', localAvailableSize - newBlockSize, (uint8_t*)newFreeBlock))
	{
		LOG_DEBUG("allocateInsideFree failed");
		return false;
	}

	if (freeToAlloc == Heap_GetFreeListHead())
	{
		Heap_UpdateHeadFree(newFreeBlock);
	}

	if (!Heap_FreeListReplace(&tempLinkBlock, newFreeBlock))
	{
		LOG_DEBUG("allocateInsideFree failed");
		return false;
	}
	
	if (!Heap_UpdateSize(newBlockSize, 'a'))
	{
		LOG_DEBUG("allocateInsideFree failed");
		return false;	
	}

	return true;
}

/**
 * [allocator_allocateExapandedBlock creates one expanded allocated block]
 * @param  freeToAlloc [pointer to free block]
 * @return             [bool according to success]
 */
enum bool allocator_allocateExapandedBlock(Block freeToAlloc, uint32_t bytes)
{
	uint32_t newBlockBufferSize = Block_BufferSizePadding(bytes) + BUFFER_UNIT;
	Block_st tempLinkBlock = *freeToAlloc;		
	
	//create new allocated block.			
	if (!Block_NewBlock('a', newBlockBufferSize, (uint8_t*)freeToAlloc))
	{
		LOG_DEBUG("allocateExapandedBlock failed");
		return false;
	}

	if (freeToAlloc == Heap_GetFreeListHead())
	{				
		Heap_UpdateHeadFree(tempLinkBlock.next);				
	}

	if (!Heap_FreeListDelete(&tempLinkBlock))
	{
		LOG_DEBUG("allocateExapandedBlock failed");
		return false;
	}

	if (!Heap_UpdateSize(newBlockBufferSize, 'a'))
	{
		LOG_DEBUG("allocateExapandedBlock failed");
		return false;
	}

	return true;
}

/**
 * [allocator_allocateInPlace switch a free block to allocated.]
 * @param  freeToAlloc [pointer to free block]
 * @return             [bool according to succss]
 */
enum bool allocator_allocateInPlace(Block freeToAlloc)
{	
	Block_st tempLinkBlock = *freeToAlloc;	

	if (!Block_SwitchBlockMode(freeToAlloc))
	{
		LOG_DEBUG("allocateInPlace failed");
		return false;
	}

	if (freeToAlloc == Heap_GetFreeListHead())
	{		
		Heap_UpdateHeadFree(tempLinkBlock.next);
	}

	if (!Heap_FreeListDelete(&tempLinkBlock))
	{
		LOG_DEBUG("allocateInPlace failed");
		return false;
	}

	if (!Heap_UpdateSize(freeToAlloc->size, 'a'))
	{
		LOG_DEBUG("allocateInPlace failed");
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
	//check before deallocating
	if ((!g_isInit) || (!allocator_isDeallocateValid(HeapCell)))
	{
			LOG_ERROR("Deallocate failed");			
			return;
	}

	LOG_INFO("Starting to deallocate...");	

	Block freeBlock = (Block) HeapCell;

	//switch freeBlock to free
	Block_SwitchBlockMode(freeBlock);

	Heap_UpdateSize(freeBlock->size, 's');	

	//add freeBlock to free-block linked list
	if(!Heap_FreeListInsertFront(freeBlock))
	{
		LOG_ERROR("Deallocate failed");			
		return;
	}

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

/*******************End of File**********************/
