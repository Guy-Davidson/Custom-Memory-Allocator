#ifndef CUSTOMMEMALLOCATOR_DEFINITIONS_H
#define CUSTOMMEMALLOCATOR_DEFINITIONS_H

/*Standsrd Libraries*/
#include <stdint.h>

/**enum declarations**/
enum bool {false, true};

enum status {free, alloc};

enum operation {subtract, add};

/**Buffer Properties**/
#define BUFFER_UNIT 24 

/**Heap Properties**/
//IMPORTANT: implementation dictates HEAP_SIZE to be a mupltiple of BUFFER_UNIT
#define HEAP_SIZE (20 * BUFFER_UNIT)
#define MAX_BUFFER_SIZE (HEAP_SIZE - sizeof(HeapHeader_st) - sizeof(Block_st))

/**Constants**/
#define FAILED_RETURN -1


#endif //CUSTOMMEMALLOCATOR_DEFINITIONS_H