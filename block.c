/*Standsrd Libraries*/
#include <stdint.h>
#include <stdio.h>
/**
 * Header Files Declarations
 * definitions.h includes all program defines.
 * log.h includes all program error, info and debug outputs.
 * block.h is a module for allocating memory on the global stack.
 */
#include "definitions.h"
#include "log.h"
#include "block.h"
#include "allocator.h"
#include "bit.h"

/**This module uses the heap**/
extern uint8_t heap[HEADER_SIZE];

/**struct decleration*
 * mode is either 'a' for allocated and 'f' for free.
 * size is the buffer size.
 * next points to the next block incase it's a mode free block. (no need to link allocated blocks).
 */
typedef struct 
{
	uint8_t mode;
	uint32_t size;
	uint32_t next;
} Block_st;

typedef Block_st* Block;

/*****************Private function declerations*******************/

/**
 * [block_isNewBlockValid checks to validate newBlock agrs.]
 */
enum bool block_isNewBlockValid(uint8_t mode, uint32_t size, uint32_t bufferIndex);


/*******************Function Implementations********************/

enum bool Block_newBlock(uint8_t mode, uint32_t size, uint32_t bufferIndex)
{
	if (!block_isNewBlockValid(mode, size, bufferIndex)) return false;

	LOG_INFO("Creating new Block...");
	
	heap[MODE_INDEX] = mode;	
	Bit_Insert32BitsIn4Bytes(heap + SIZE_INDEX, size);	
	Bit_Insert32BitsIn4Bytes(heap + NEXT_INDEX, NILL);

	#ifdef DEBUG

	Block_PrintBlock(bufferIndex);

	#endif
	
	return true;
}

/**
 * [block_isNewBlockValid checks to validate newBlock agrs.]
 */
enum bool block_isNewBlockValid(uint8_t mode, uint32_t size, uint32_t bufferIndex)
{
	if ((mode != 'a') && (mode != 'f'))
	{
		//ERROR PRINT
		return false;
	}

	if (size <= 0)
	{
		//ERROR PRINT
		return false;
	}

	if (bufferIndex < MIN_BUFFER_INDEX)
	{
		//ERROR PRINT
		return false;
	}

	return true;
}

/**
 * [Block_switchBlockMode: switches from ]
 * @param  bufferIndex [block to be switched]
 * @return             [bool according to success]
 */
enum bool Block_switchBlockMode(uint32_t bufferIndex)
{
	uint8_t currentMode = heap[bufferIndex - MODE_OFFSET];

	if ((currentMode != 'a') && (currentMode != 'f'))
	{
		//ERROR PRINT
		return false;
	}

	
	if (currentMode == 'a')
	{
		//INFO PRINT
		currentMode = 'f';
	}

	else 
	{
		//INFO PRINT
		currentMode = 'a';
	}

	return true;
}


/***************Debuging Functions Implementations****************/

#ifdef DEBUG

/**
 * [Block_PrintBlock]
 * @param bufferIndex [the index of a block index]
 */
void Block_PrintBlock(uint8_t bufferIndex)
{
	uint8_t mode = 0;
	uint32_t size = 0;
	uint32_t next = 0;

	mode = heap[MODE_INDEX];
	Bit_Insert4BytesIn32Bits(&size, heap + SIZE_INDEX);	
	Bit_Insert4BytesIn32Bits(&next, heap + NEXT_INDEX);
	

	printf("[%c, %d, %d, ",mode, size, next);
	
	for (int i = 0; i < size - 1 ; i++)
	{
		printf("-, ");
	}

	printf("-]\n");

	return;
}

#endif

/*************************End of File****************************************/




