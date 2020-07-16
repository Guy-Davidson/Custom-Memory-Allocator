#ifndef CUSTOMMEMALLOCATOR_MERGE_H
#define CUSTOMMEMALLOCATOR_MERGE_H

/************Public function declerations**********/

/**
 * [allocator_mergeFreeBlocks: merge all free blocks in case they're adjacent,
 * IMPORTAENT: allocate will execute merge only incase of heap overflow,
 * this decision makes deallocate O(1),
 * and allocating at possible 'heap overflow' w.c O(n!)]
 * @return [bool according to success. (returns false incase there are no blocks to merge)]
 */
enum bool Merge_Exe();

#endif //CUSTOMMEMALLOCATOR_MERGE_H