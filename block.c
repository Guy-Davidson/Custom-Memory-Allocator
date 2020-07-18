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
 * [block_isNewBlockValid checks to validate newBlock agrs]
 */
static enum bool block_isNewBlockValid(enum status mode, uint32_t size, uint8_t* heapCell);


/*******************Function Implementations********************/

/**
 * [Block_NewBlock creates new block in the heap.]
 * @param  mode     [new block mode 'alloc' or 'free']
 * @param  size     [buffer size]
 * @param  heapCell [Pointer to heap where the block will be stored]
 * @return          [bool according to succes]
 */
enum bool Block_NewBlock(enum status mode, uint32_t size, uint8_t* heapCell)
{
	if (!block_isNewBlockValid(mode, size, heapCell))
	{
		LOG_ERROR("newBlock failure.");
		return false;	
	} 

	LOG_INFO("Creating new Block");

	Block newBlock = NULL;

	newBlock = (Block) heapCell;

	if (newBlock == NULL)
	{
		LOG_ERROR("NewBlock failed");
		return false;
	}

	newBlock->mode = mode;	
	newBlock->size = Block_BufferSizePadding(size);	
	newBlock->next = NULL;
	newBlock->prev = NULL;
		
	#ifdef DEBUG

	Block_PrintBlock(newBlock);

	#endif

	LOG_INFO("Created block succesfuly!");
	
	return true;
}

/**
 * [block_isNewBlockValid checks to validate newBlock agrs]
 */
enum bool block_isNewBlockValid(enum status mode, uint32_t size, uint8_t* heapCell)
{
	if ((mode != alloc) && (mode != free))
	{
		LOG_ERROR("mode can be either 'alloc' or 'free'");
		return false;
	}

	if ((size > MAX_BUFFER_SIZE) || ((int32_t)size <= 0))
	{
		LOG_ERROR("size inccorect");
		return false;
	}

	uint32_t blockIndex = heapCell - heap;

	if ((blockIndex < sizeof(HeapHeader_st)) || (blockIndex > HEAP_SIZE))
	{
		LOG_ERROR("Invalid buffer index");
		return false;
	}

	return true;
}

/**
 * [Block_bufferSizePadding: finds and smallest multiple of BUFFER_UNIT that is 
    bigger then new block size, this allows padding of the buffers to blocks of BUFFER_UNIT.
    By doing so, blocks are alligned with sizeof(Block_st), and the
    free-block list get's smaller as the chance for a fit is increased.]
 * @param  size [requested new block size]
 * @return      [smallest multiple of BUFFER_UNIT that is bigger then new block size]
 */
uint32_t Block_BufferSizePadding(uint32_t size)
{
	if ((int32_t)size <= 0)
	{
		LOG_DEBUG("BufferSizePadding failed");
		return FAILED_RETURN;
	}

	uint32_t res = BUFFER_UNIT;	
	
	while (res < size)
	{
		res += BUFFER_UNIT;
	}	

	return res;
}

/**
 * [Block_switchBlockMode: switches block mode ]
 * @param  bufferIndex [block to be switched]
 * @return             [bool according to success]
 */
enum bool Block_SwitchBlockMode(Block block)
{	
	if ((!block) || ((block->mode != alloc) && (block->mode != free)))
	{
		LOG_ERROR("Invalid block for switch");
		return false;
	}

	
	if (block->mode == alloc)
	{
		LOG_INFO("switched allocated block to free.");
		block->mode = free;
	}

	else 
	{
		LOG_INFO("switched free block to allocated.");
		block->mode = alloc;
	}

	return true;
}


/***************Debuging Functions Implementations****************/

#ifdef DEBUG

/**
 * [Block_PrintBlock]
 * @param bufferIndex [block to print]
 */
void Block_PrintBlock(Block block)
{
	if (!block)
	{
		LOG_DEBUG("PrintBlock failed");
		return;
	}

	CUSTOMMEMALLOCATOR_PRINT("{index: %lu, mode: %c, size: %d, ",(uint8_t*)block - heap, Block_printMode(block->mode), block->size);

	if (block->next == NULL)	
	{
		CUSTOMMEMALLOCATOR_PRINT("next: null, ");
	}

	else
	{
		CUSTOMMEMALLOCATOR_PRINT("next: %lu, ", (uint8_t*)(block->next) - heap);	
	}

	if (block->prev == NULL)	
	{
		CUSTOMMEMALLOCATOR_PRINT("prev: null}");
	}
	else
	{
		CUSTOMMEMALLOCATOR_PRINT("prev: %lu}", (uint8_t*)(block->prev) - heap);	
	}	

	CUSTOMMEMALLOCATOR_PRINT("->[");   
	
	for (uint32_t i = 0; i < block->size ; i++)
	{
		if (block->mode == free)
		{
			CUSTOMMEMALLOCATOR_PRINT("-, ");
		}

		else 
		{
			CUSTOMMEMALLOCATOR_PRINT("%c, ", *((uint8_t*)block + sizeof(Block_st) + i));	
		}
	}

	CUSTOMMEMALLOCATOR_PRINT("]\n\n");

	return;
}

/**
 * [Block_printMode enum status pirnt function]
 * @param  mode [the block mode free ot alloc]
 * @return      ['a' for alloc and 'f for free']
 */
char Block_printMode(enum status mode)
{
	if ((mode != alloc) && (mode != free))
	{
		LOG_DEBUG("printMode failed");
		return FAILED_RETURN;
	}
	if (mode == alloc)
	{
		return 'a';
	}
	else
	{
		return 'f';
	}
}

#endif

/*************************End of File****************************************/
