/**************************Include Libraries & Headers******************/
#include <stdint.h>
#include <stdio.h>
//program definitions
#include "definitions.h"
//loging error info & debug
#include "log.h"
//struct Block module
#include "block.h"
//Memory allocator module
#include "allocator.h"
//merge adjacent free block module
#include "merge.h"
//heap information module
#include "heap.h"
//analyze heap data
#include "processHeap.h"

/**************************Global Variables**************************/
extern uint8_t heap[HEAP_SIZE];


int main(int argc, char const *argv[])
{					
	
	// uint8_t* array[3];

	// array[0] = NULL;
	// array[0] = Allocator_Allocate(1);

	// array[1] = NULL;
	// array[1] = Allocator_Allocate(25);	

	// array[2] = NULL;
	// array[2] = Allocator_Allocate(60);


	// Allocator_Deallocate(array[1]);
	

	// array[0][0] = 'M';
	// array[0][1] = 'i';
	// array[0][2] = 'c';
	// array[0][3] = 'r';
	// array[0][4] = 'o';
	// array[0][5] = 's';
	// array[0][6] = 'o';
	// array[0][7] = 'f';
	// array[0][8] = 't';

	// Heap_PrintHeap();
	

	// Allocator_Deallocate(array[0]);
	
	// Allocator_Deallocate(array[2]);	

	// ProcessHeap_SortAllocatedBlocks();


	// return 0;


	 //example of a small program that uses memmory allocation:


	// int n;
	// uint8_t i, *ptr, sum = 0;

 //    printf("Enter number of elements: ");
 //    scanf("%d", &n);

 //    ptr = (uint8_t*) Allocator_Allocate(n * sizeof(uint8_t));
 
 //    // if memory cannot be allocated
 //    if(ptr == NULL)                     
 //    {
 //        printf("Error! memory not allocated.");
 //        return 1;
 //    }
    
 //    for(i = 0; i < (uint8_t) n; ++i)
 //    {        
 //        ptr[i] = i + 1;
 //        sum += *(ptr + i);
 //    }

 //    printf("Sum = %d \n", sum);

    // Heap_PrintHeap();
  
    // deallocating the memory
    // Allocator_Deallocate(ptr);

    // return 0;
}



