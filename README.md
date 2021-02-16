# Power Memory Allocator 

PMA is a C library that provides the user with a better control and information on the dynamic memory allocations that are being used throughout the program execution.

## Installation

For the full library use the following headers:

```c
//program definitions
#include "definitions.h"
//loging error info & debug
#include "log.h"
//struct Block module
#include "block.h"
//Memory allocator module
#include "allocator.h"
//merge adjacent free blocks module
#include "merge.h"
//heap information module
#include "heap.h"
//analyze heap data
#include "processHeap.h"
```

## Compilation
```bash
//For normal usage:
make clean

//For debuging and info loggins:
make all
```

## Allocate & Free Memory

```c
/**
 * [Allocator_Allocate: allocated bytes cells in the heap]
 * @param  bytes [number of bytes to allocate]
 * @return       [buffer pointer upon success and NULL upon failure.]
 */
uint8_t* Allocator_Allocate(uint32_t bytes);

/**
 * [Allocator_Deallocate: deallocate a memory block.
 * IMPORTANT: deallocate makes some validation checks (desribed in isDeallocateValid),
 * But the user can free a non previously allocated block by missuse.
 * This implementation decision is inherited from the function free(),
 * as the cost of checking each deallocate is O(n), while currently it's O(1)]
 * @param HeapCell [Pointer to heap cell to be deallocated.]
 */
void Allocator_Deallocate(uint8_t* HeapCell);

```

## Process Data

```c
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

```

## Set Heap Size
```c
/**
definitions.h setting:
Choose N to be the maximal number of Buffers allocated. 
Implementation dictates HEAP_SIZE to be a mupltiple of BUFFER_UNIT (24 bits).
**/
#define HEAP_SIZE (N * BUFFER_UNIT)
```


## Contact
If you would like to get in touch with me or comment on anything I've presented in my site, please fill free to send me a Message via one of these links. Thanks!

Gmail: guydav12@gmail.com  
or
[Linkedin](https://www.linkedin.com/in/guy-davidson/)