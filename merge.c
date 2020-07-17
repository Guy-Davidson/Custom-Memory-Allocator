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

/*****************Private Function Declerations*******************/

/**
 * [merge_isAdjacent check of two blocks are adjacent on the heap]
 * @param  block1 
 * @param  block2 
 * @return [bool]
 */
static enum bool merge_isAdjacent(Block block1, Block block2);

/**
 * [merge_mergeTwoBlocks merges two free blocks into a bigger free block.
 * updates free-block link list, and heap allocated size.]
 * @param  block1 
 * @param  block2 
 * @return [bool according to succes]
 */
static enum bool merge_mergeTwoBlocks(Block block1, Block block2);
 
/**
 * [merge_getPriorBlock return the block who comes first in the free block list.]
 * @param  block1 
 * @param  block2 
 * @return        [prior block]
 */
static Block merge_getPriorBlock(Block block1, Block block2);

/*******************Function Implementations********************/

/**
 * [allocator_mergeFreeBlocks: merge all free blocks in case they're adjacent,
 * IMPORTAENT: allocate will execute merge only incase of heap overflow,
 * this decision makes deallocate O(1),
 * and allocating at possible 'heap overflow' w.c O(n!)]
 * @return [bool according to success. (returns false incase there are no blocks to merge)]
 */
enum bool Merge_Exe()
{
	enum bool changedHeap = false;
	enum bool isMerged = false; 

	LOG_INFO("searching free list to merge blocks...");	

	while (!isMerged)
	{		
		LOOP:

		isMerged = true;
		Block currentBlock = NULL;		

		currentBlock = Heap_GetFreeListHead();

		if (!currentBlock)
		{
			LOG_DEBUG("Merge_Exe failed");
			return false;
		}

		while (currentBlock)
		{
			Block listIterator = Heap_GetFreeListHead();

			while (listIterator)				
			{												
				if (merge_isAdjacent(currentBlock, listIterator))
				{
					merge_mergeTwoBlocks(currentBlock, listIterator);
					changedHeap = true;					
					goto LOOP;
				}				

				listIterator = listIterator->next;
			}

			currentBlock = currentBlock->next;
		}
	}

	LOG_INFO("finishd searching free list succesfuly!");

	return changedHeap;
}

/**
 * [merge_isAdjacent check of two blocks are adjacent on the heap]
 * @param  block1 
 * @param  block2 
 * @return [bool]
 */
enum bool merge_isAdjacent(Block block1, Block block2)
{			
	enum bool right = ((uint8_t*)block1 + sizeof(Block_st) + block1->size) == (uint8_t*)block2;

	enum bool left = ((uint8_t*)block2 + sizeof(Block_st) + block2->size) == (uint8_t*)block1;	

	return right || left;
}

/**
 * [merge_mergeTwoBlocks merges two free blocks into a bigger free block.
 * updates free-block link list, and heap allocated size.]
 * @param  block1 
 * @param  block2 
 * @return [bool according to succes]
 */
enum bool merge_mergeTwoBlocks(Block block1, Block block2)
{
	if ((!block1) || (!block2))
	{
		LOG_DEBUG("mergeTwoBlocks failed");
		return false;
	}

	LOG_INFO("merging two blocks...");

	//add min and max

	Block leftBlock = NULL;
	Block rightBlock = NULL;
	Block firstBlock = NULL;
	Block secondBlock = NULL;

	if (block1 < block2)
	{
		leftBlock = block1;
		rightBlock = block2;
	}
	else
	{
		leftBlock = block2;
		rightBlock = block1;
	}

	if (block1 == merge_getPriorBlock(block1, block2))
	{		
		firstBlock = block1;
		secondBlock = block2;
	}
	else
	{
		firstBlock = block2;
		secondBlock = block1;
	}	

	Block_st tempLink1 = *firstBlock;
	Block_st tempLink2 = *secondBlock;		

	uint32_t mergedBlockSize = leftBlock->size + rightBlock->size + sizeof(Block_st);
	
	if (!Block_NewBlock('f', mergedBlockSize, (uint8_t*)leftBlock))
	{
		LOG_DEBUG("mergeTwoBlocks failed");
		return false;
	}

	if (firstBlock == Heap_GetFreeListHead() || secondBlock == Heap_GetFreeListHead()) 
	{		
		Heap_UpdateHeadFree(leftBlock);
	}		

	//update list linking 
	Block mergedBlock = leftBlock;

	//case:merge to first. not linked to each other.
	if ((firstBlock == mergedBlock) && ((&tempLink1)->next != secondBlock))
	{				
		if (!Heap_FreeListDelete(&tempLink2))
		{
			LOG_DEBUG("mergeTwoBlocks failed");
			return false;
		}
		if (!Heap_FreeListReplace(&tempLink1, mergedBlock))
		{
			LOG_DEBUG("mergeTwoBlocks failed");
			return false;	
		}
	}			
	//case:merge to second. not linked to each other.
	else if ((secondBlock == mergedBlock) && (((&tempLink2)->prev != firstBlock)))
	{
		if (!Heap_FreeListDelete(&tempLink1))
		{
			LOG_DEBUG("mergeTwoBlocks failed");
			return false;
		}
		if (!Heap_FreeListReplace(&tempLink2, mergedBlock))
		{
			LOG_DEBUG("mergeTwoBlocks failed");
			return false;	
		}
	}
	//case: linked to each other.
	else
	{
		mergedBlock->prev = tempLink1.prev;
			
		if (tempLink1.prev != NULL)
		{
			(tempLink1.prev)->next = mergedBlock;
		}
		
		mergedBlock->next = tempLink2.next;

		if (tempLink2.next != NULL) 
		{
			(tempLink2.next)->prev = mergedBlock;
		}
	}
		
	if (!Heap_UpdateSize(sizeof(Block_st), 's'))
	{
		LOG_DEBUG("mergeTwoBlocks failed");
		return false;	
	}

	LOG_INFO("merged two free blocks succesfuly!");
	//THIS IS A DEBUG STATMENTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTttftftftftftfTTTTTTTTTTTTTTTTTTTTTTTT
	Heap_PrintHeap();
	return true;
}

/**
 * [merge_getPriorBlock return the block who comes first in the free block list.]
 * @param  block1 
 * @param  block2 
 * @return        [prior block]
 */
Block merge_getPriorBlock(Block block1, Block block2)
{
	Block listIterator = NULL;

	listIterator = Heap_GetFreeListHead();

	if ((!listIterator) || (!block1) || (!block2))
	{
		LOG_DEBUG("getPriorBlock failed");
		return NULL;
	}

	while (listIterator)
	{
		if (listIterator == block1)
		{
			//LOG_INFO("PriorBlock returns block1!");
			return block1;
		}

		if (listIterator == block2)
		{
			//LOG_INFO("PriorBlock returns block2!");
			return block2;
		}

		listIterator = listIterator->next;
	}
	
	LOG_DEBUG("getPriorBlock failed");
	return NULL;
	
}

/***************Debuging Functions Implementations****************/

/*******************End of File**********************/