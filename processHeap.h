#ifndef CUSTOMMEMALLOCATOR_PROCESSHEAP_H
#define CUSTOMMEMALLOCATOR_PROCESSHEAP_H

/************Public function declerations**********/


/**
 * [ProcessHeap_GetAllocatedMemSize]
 * @return [size of allocated memory in bytes]
 */
uint32_t ProcessHeap_AllocatedMemSize();

/**
 * [ProcessHeap_GetFreeMemSize]
 * @return [size of free memory in bytes]
 */
uint32_t ProcessHeap_FreeMemSize();

/**
 * [ProcessHeap_MaxFreeBlock finds the biggest free block in the heap]
 * @return [biggest free block or null if heap is fully allocated]
 */
Block ProcessHeap_MaxFreeBlock();

/**
 * [ProcessHeap_Reset deallocates the entire heap;]
 */
void ProcessHeap_Reset();

/**
 * [ProcessHeap_SortAllocatedBlocks prints the heap allocated block in a sorted descending order]
 */
void ProcessHeap_SortAllocatedBlocks();


#endif //CUSTOMMEMALLOCATOR_PROCESSHEAP_H