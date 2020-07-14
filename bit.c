/*Standsrd Libraries*/
#include <stdio.h>

/**
 * Header Files Declarations
 * definitions.h includes all program defines.
 * log.h includes all program error, info and debug outputs.
 * customAllocator.h is a module for allocating memory on the global stack.
 */
#include "definitions.h"
#include "log.h"
#include "block.h"
#include "allocator.h"
#include "bit.h"

/**Function Implementations**/
/**
 * [Bit_Insert32BitsIn4Bytes: 'cast' uint32 to uint8 by moving bits to the matching place.]
 * @param dest   [an array of 4 uint8]
 * @param source [uint 32 to copy from.]
 */
void Bit_Insert32BitsIn4Bytes(uint8_t* dest, const uint32_t source)
{		
	dest[3] = (0xFF & source);		
	dest[2] = (0xFF & (source >> BYTE));
	dest[1] = (0xFF & (source >> (2 * BYTE)));	
	dest[0] = (0xFF & (source >> (3 * BYTE)));

	return;
}

/**
 * [Bit_HeapIndexTo32Bit: takes 32 bits from heap and cast them into uint32 var.]
 * @param  index [base indexfor 32 bits.]
 * @return       [a uint32 var with matching bits]
 */
uint32_t Bit_IndexTo32Bit(uint8_t* index)
{
	uint32_t res = 0;
	Bit_Insert4BytesIn32Bits(&res, index);

	return res;
}

/**
 * [Bit_Insert32BitsIn4Bytes: 'cast' 4 uint8 to uint32 by moving bits to the matching place.]
 * @param dest   [uint32 to hold value]
 * @param source [an array of 4 uint8 to copy from.]
 */
void Bit_Insert4BytesIn32Bits(uint32_t* dest, const uint8_t* source)
{
	*dest = (((uint32_t)(source[0]) << (3 * BYTE)) |\
		    ((uint32_t)(source[1]) << (2 * BYTE)) |\
		    ((source[2]) << BYTE) |\
		    (source[3])) ;

	return;
}

