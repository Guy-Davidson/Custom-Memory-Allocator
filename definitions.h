#ifndef CUSTOMMEMALLOCATOR_DEFINITIONS_H
#define CUSTOMMEMALLOCATOR_DEFINITIONS_H

/*Standsrd Libraries*/
#include <stdint.h>

/**enum and typedef declarations**/
enum bool {false, true};

/**Memory Size**/
#define GB (1 << 30)

/**Buffer Properties**/
#define BUFFER_UNIT 24

/**Heap Properties**/
#define HEAP_SIZE (20 * BUFFER_UNIT)
#define MAX_BUFFER_SIZE (HEAP_SIZE - sizeof(HeapHeader_st) - sizeof(Block_st))


#endif //CUSTOMMEMALLOCATOR_DEFINITIONS_H