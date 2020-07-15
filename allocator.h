#ifndef CUSTOMMEMALLOCATOR_ALLOCATOR_H
#define CUSTOMMEMALLOCATOR_ALLOCATOR_H

/***********************Struct Decleration****************************
 *
 * allocMemSize counts how much memory (in bytes) was allocated.
 * headFree points to the head of the free-block linked list. 
 */
typedef struct 
{
	uint32_t allocMemSize;
	Block headFree;	
} HeapHeader_st;

typedef HeapHeader_st* HeapHeader;

/************Public function declerations**********/

/**
 * [Allocator_Allocate: allocated bytes cells in the heap]
 * @param  bytes [number of bytes to allocate]
 * @return       [buffer pointer upon success and NULL upon failure.]
 */
uint8_t* Allocator_Allocate(uint32_t bytes);

/**
 * [Allocator_Deallocate: deallocate a memory block.
 * IMPORTANT: deallocate makes some validation checks (desribed in isDeallocateValid),
 * But the user can free a non previously allocated block by misuse.
 * This implementation decision is inherited from the function free(),
 * as the cost of checking each deallocate is O(n), while currently it's O(1)]
 * @param HeapCell [Pointer to heap cell to be deallocated.]
 */
void Allocator_Deallocate(uint8_t* HeapCell);
	
/***************Debuging functions declertions****************/

#ifdef DEBUG

/**
 * [Allocator_PrintHeapHeader: prints the heap header
 	to get info about heap allocated memory size and head of free list.]
 */
void Allocator_PrintHeapHeader();

/**
 * [Allocator_PrintHeap: prints the entire heap block by block.]
 */
void Allocator_PrintHeap();

#endif		


#endif //CUSTOMMEMALLOCATOR_ALLOCATOR_H