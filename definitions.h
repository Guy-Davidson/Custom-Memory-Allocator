#ifndef CUSTOMMEMALLOCATOR_DEFINITIONS_H
#define CUSTOMMEMALLOCATOR_DEFINITIONS_H

/*Standsrd Libraries*/
#include <stdint.h>

/**enum and typedef declarations**/
enum bool {false, true};

/**Constants**/
#define NILL -1
#define BYTE 8

/**Memory Size**/
#define GB (1 << 30)

/**Heap Properties**/
#define HEAP_SIZE 100
#define MIN_BUFFER_INDEX 17
#define SIZE_COUNTER 0
#define HEAD_FREE 4
#define HEAP_HEADER_SIZE 8

/**Header Properties**/
#define MODE_OFFSET 9
#define SIZE_OFFSET 8
#define NEXT_OFFSET 4
#define HEADER_SIZE 9

/**Block Properties**/
#define MODE_INDEX (bufferIndex - MODE_OFFSET)
#define SIZE_INDEX (bufferIndex - SIZE_OFFSET)
#define NEXT_INDEX (bufferIndex - NEXT_OFFSET)

/**Implementaion Propertioes**/







#endif //CUSTOMMEMALLOCATOR_DEFINITIONS_H