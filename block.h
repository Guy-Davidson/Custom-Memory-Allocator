#ifndef CUSTOMMEMALLOCATOR_BLOCK_H
#define CUSTOMMEMALLOCATOR_BLOCK_H

/***********************Struct Decleration****************************
 *
 * mode is either 'a' for allocated or 'f' for free.
 * size is the buffer size. 
 * next points to the next block incase it's a mode free block. (no need to link allocated blocks).
 */
typedef struct block
{
	uint8_t mode;
	uint32_t size;
	struct block* next;
} Block_st;

typedef Block_st* Block;

/**Public function declerations**/

/**
 * [Block_NewBlock creates new block in the heap.]
 * @param  mode     [new block mode 'a' or 'f']
 * @param  size     [buffer size]
 * @param  heapCell [Pointer to heap where the block will be stored]
 * @return          [bool according to succes]
 */
enum bool Block_NewBlock(uint8_t mode, uint32_t size, uint8_t* heapCell);

/**
 * [Block_switchBlockMode: switches block mode]
 * @param  bufferIndex [block to be switched]
 * @return             [bool according to success]
 */
enum bool Block_SwitchBlockMode(Block block);


/***************Debuging functions declerations****************/

#ifdef DEBUG

/**
 * [Block_PrintBlock]
 * @param bufferIndex [block to print]
 */
void Block_PrintBlock(Block block);

#endif

#endif //CUSTOMMEMALLOCATOR_BLOCK_H