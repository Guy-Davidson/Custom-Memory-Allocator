#ifndef CUSTOMMEMALLOCATOR_ALLOCATOR_H
#define CUSTOMMEMALLOCATOR_ALLOCATOR_H

/************Public function declerations**********/

/**
 * [Allocator_Allocate: allocated bytes cells in heap]
 * @param  bytes [number of bytes to allocate]
 * @return       [buffer index upon success and NILL upon failure.]
 */
uint8_t* Allocator_Allocate(uint32_t bytes);

/***************Debuging functions declertions****************/

#ifdef DEBUG

/**
 * [Allocator_PrintHeapHeader: prints the heap header
 	to get info about heap size and head of free list.]
 */
void Allocator_PrintHeapHeader();

/**
 * [Allocator_PrintHeap: prints the entire heap block by block.]
 */
void Allocator_PrintHeap();

#endif		


#endif //CUSTOMMEMALLOCATOR_ALLOCATOR_H