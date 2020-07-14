#ifndef CUSTOMMEMALLOCATOR_BLOCK_H
#define CUSTOMMEMALLOCATOR_BLOCK_H

/**Public function declerations**/

/**
 * [Block_newBlock: creates a new block in the heap]
 * @return [bool according to success.]
 */
enum bool Block_newBlock(uint8_t mode, uint32_t size, uint32_t bufferIndex);

/**
 * [Block_switchBlockMode: switches from ]
 * @param  bufferIndex [block to be switched]
 * @return             [bool according to success]
 */
enum bool Block_switchBlockMode(uint32_t bufferIndex);


/***************Debuging functions declerations****************/

#ifdef DEBUG

/**
 * [Block_PrintBlock]
 * @param bufferIndex [the index of a block index]
 */
void Block_PrintBlock(uint8_t bufferIndex);

#endif

#endif //CUSTOMMEMALLOCATOR_BLOCK_H