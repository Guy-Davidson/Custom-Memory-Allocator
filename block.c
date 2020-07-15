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

/*****************Private Function Declerations*******************/

/**
 * [block_isNewBlockValid checks to validate newBlock agrs]
 */
static enum bool block_isNewBlockValid(uint8_t mode, uint32_t size, uint8_t* heapCell);


/**
 * [block_bufferSizePadding: finds and smallest multiple of BUFFER_UNIT that is 
    bigger then new block size, this allows padding of the buffers to blocks of BUFFER_UNIT.
    By doing so, blocks are alligned with sizeof(Block_st), and the
    free-block list get's smaller as the chance for a fit is increased]
 * @param  size [requested new block size]
 * @return      [smallest multiple of BUFFER_UNIT that is bigger then new block size]
 */
static uint32_t block_bufferSizePadding(uint32_t size);


/*******************Function Implementations********************/

/**
 * [Block_NewBlock creates new block in the heap.]
 * @param  mode     [new block mode 'a' or 'f']
 * @param  size     [buffer size]
 * @param  heapCell [Pointer to heap where the block will be stored]
 * @return          [bool according to succes]
 */
enum bool Block_NewBlock(uint8_t mode, uint32_t size, uint8_t* heapCell)
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
	newBlock->size = block_bufferSizePadding(size);	
	newBlock->next = NULL;
		
	#ifdef DEBUG

	Block_PrintBlock(newBlock);

	#endif

	LOG_INFO("Created block succesfuly!");
	
	return true;
}

/**
 * [block_isNewBlockValid checks to validate newBlock agrs]
 */
enum bool block_isNewBlockValid(uint8_t mode, uint32_t size, uint8_t* heapCell)
{
	if ((mode != 'a') && (mode != 'f'))
	{
		LOG_ERROR("mode can be either 'a' or 'f'");
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
 * [block_bufferSizePadding: finds and smallest multiple of BUFFER_UNIT that is 
    bigger then new block size, this allows padding of the buffers to blocks of BUFFER_UNIT.
    By doing so, blocks are alligned with sizeof(Block_st), and the
    free-block list get's smaller as the chance for a fit is increased.]
 * @param  size [requested new block size]
 * @return      [smallest multiple of BUFFER_UNIT that is bigger then new block size]
 */
uint32_t block_bufferSizePadding(uint32_t size)
{
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
	if ((block->mode != 'a') && (block->mode != 'f'))
	{
		LOG_ERROR("Invalid block for switch");
		return false;
	}

	
	if (block->mode == 'a')
	{
		LOG_INFO("switched allocated block to free.");
		block->mode = 'f';
	}

	else 
	{
		LOG_INFO("switched free block to allocated.");
		block->mode = 'a';
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
	if (block->next == NULL)	
	{
		CUSTOMMEMALLOCATOR_PRINT("[%c, %d, (null), ",block->mode, block->size);
	}

	else
	{
		CUSTOMMEMALLOCATOR_PRINT("[%c, %d, %lu, ",block->mode, block->size, (uint8_t*)(block->next) - heap);	
	}		
	
	for (uint32_t i = 0; i < block->size - 1 ; i++)
	{
		CUSTOMMEMALLOCATOR_PRINT("-, ");
	}

	CUSTOMMEMALLOCATOR_PRINT("-]\n\n");

	return;
}

#endif

/*************************End of File****************************************/
