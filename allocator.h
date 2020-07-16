#ifndef CUSTOMMEMALLOCATOR_ALLOCATOR_H
#define CUSTOMMEMALLOCATOR_ALLOCATOR_H

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
		
#endif //CUSTOMMEMALLOCATOR_ALLOCATOR_H