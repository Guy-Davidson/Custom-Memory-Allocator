#ifndef CUSTOMMEMALLOCATOR_BIT_H
#define CUSTOMMEMALLOCATOR_BIT_H

/**
 * [Block_Insert32BitsIn4Bytes: 'cast' uint32 to uint8 by moving bits to the matching place.]
 * @param dest   [an array of 4 uint8]
 * @param source [uint 32 to copy from.]
 */
void Bit_Insert32BitsIn4Bytes(uint8_t* dest, const uint32_t source);

/**
 * [Block_insert4BytesIn32Bits: 'cast' 4 uint8 to uint32 by moving bits to the matching place.]
 * @param dest   [uint32 to hold value]
 * @param source [an array of 4 uint8 to copy from.]
 */
void Bit_Insert4BytesIn32Bits(uint32_t* dest, const uint8_t* source);


/**
 * [Allocator_HeapIndexTo32Bit: takes 32 bits from heap and cast them into uint32 var.]
 * @param  index [base indexfor 32 bits.]
 * @return       [a uint32 var with matching bits]
 */
uint32_t Bit_IndexTo32Bit(uint8_t* index);


#endif //CUSTOMMEMALLOCATOR_BIT_H