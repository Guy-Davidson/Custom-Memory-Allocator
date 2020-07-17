#ifndef CUSTOMMEMALLOCATOR_HEAP_H
#define CUSTOMMEMALLOCATOR_HEAP_H

/***********************Struct Decleration****************************
 *
 * allocMemSize counts how much memory (in bytes) was allocated.
 * headFree points to the head of the free-block linked list. 
 */
typedef struct 
{
	uint32_t allocMemSize;
	Block headFree;	
	Block tailFree;
} HeapHeader_st;

typedef HeapHeader_st* HeapHeader;

/************Public function declerations**********/

/**
 * [Heap_UpdateSize change allocated memory size]
 * @param  size [size to add]
 * @return      [bool according to succes]
 */
enum bool Heap_UpdateSize(uint32_t size, uint8_t mode);

/**
 * [Heap_UpdateHeadFree change headFree]
 * @return [bool according to succes]
 */
enum bool Heap_UpdateHeadFree(Block block);

/**
 * [Heap_getFreeListHead]
 * @return [free list head]
 */
Block Heap_GetFreeListHead();

/**
 * [Heap_getFreeListHead]
 * @return [free list head]
 */
uint32_t Heap_GetAllocMemSize();

/**
 * [Heap_FreeListInsertFront]
 * @param  block [block to insert]
 * @return       [bool according to succes]
 */
enum bool Heap_FreeListInsertFront(Block block);

/**
 * [Heap_FreeListReplaceMiddle change a Block in the free block list to another]
 * @param  outBlock [block to take off the list]
 * @param  inBlock [block to in the list]
 * @return       [bool according to success]
 */
enum bool Heap_FreeListReplace(Block outBlock, Block inBlock);

/**
 * [Heap_FreeListDelete]
 * @param  block [block to be deleted]
 * @return       [bool according to succes]
 */
enum bool Heap_FreeListDelete(Block block);


/***************Debuging functions declertions****************/

#ifdef DEBUG

/**
 * [Allocator_PrintHeapHeader: prints the heap header
 	to get info about heap allocated memory size and head of free list.]
 */
void Heap_PrintHeapHeader();

/**
 * [Allocator_PrintHeap: prints the entire heap block by block.]
 */
void Heap_PrintHeap();

/**
 * [Allocator_PrintFreeBlockList]
 */
void Heap_PrintFreeBlockList();

#endif	



#endif //CUSTOMMEMALLOCATOR_HEAP_H