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
	enum status mode;
	uint32_t size;
	struct block* next;
	struct block* prev;
} Block_st;

typedef Block_st* Block;

/************Public function declerations**********/

/**
 * [Block_NewBlock creates new block in the heap.]
 * @param  mode     [new block mode 'alloc' or 'free']
 * @param  size     [buffer size]
 * @param  heapCell [Pointer to heap where the block will be stored]
 * @return          [bool according to succes]
 */
enum bool Block_NewBlock(enum status mode, uint32_t size, uint8_t* heapCell);

/**
 * [Block_switchBlockMode: switches block mode]
 * @param  bufferIndex [block to be switched]
 * @return             [bool according to success]
 */
enum bool Block_SwitchBlockMode(Block block);

/**
 * [Block_BufferSizePadding: finds and smallest multiple of BUFFER_UNIT that is 
    bigger then new block size, this allows padding of the buffers to blocks of BUFFER_UNIT.
    By doing so, blocks are alligned with sizeof(Block_st), and the
    free-block list get's smaller as the chance for a fit is increased]
 * @param  size [requested new block size]
 * @return      [smallest multiple of BUFFER_UNIT that is bigger then new block size]
 */
uint32_t Block_BufferSizePadding(uint32_t size);


/***************Debuging functions declerations****************/

#ifdef DEBUG

/**
 * [Block_PrintBlock]
 * @param bufferIndex [block to print]
 */
void Block_PrintBlock(Block block);

/**
 * [Block_printMode enum status pirnt function]
 * @param  mode [the block mode free ot alloc]
 * @return      ['a' for alloc and 'f for free']
 */
char Block_printMode(enum status mode);

#endif

#endif //CUSTOMMEMALLOCATOR_BLOCK_H