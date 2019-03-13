#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "memallocator.h"

/* Useful definitions */
#define ptrSize sizeof(char *)
#define intSize sizeof(int)

typedef struct tagBulk {
  int *size;
  char *userSpace;
  char
    **prev,
    **next;
} bulk;

/* Memory system */
struct {
  char *bulk;
  int bulkSize;
  char *freeBlock;  // first free block (from the [prev | . . . ])
} memory;

/* Set bulk size function.
 * ARGUMENTS:
 *   - bulk:
 *       char *bulk;
 *   - size:
 *       int size;
 * RETURNS: None.
 */
void setBulkSize( char *bulk, int size ) {
  *(int *)(bulk + ptrSize) = size;
} /* End of 'setBulkSuze' function */

/* Get bulk info function.
 * ARGUMENTS:
 *   - bulk:
 *       char *bulk;
 * RETURNS:
 *   (bulkInfo).
 */
bulk getBulk( char *block ) {
  bulk b;

  b.prev = (char **)block;
  b.size = (int *)(block + ptrSize);
  b.userSpace = block + ptrSize + intSize;
  b.next = (char **)(block + ptrSize + intSize + abs(*b.size));

  return b;
} /* End of 'getBulk' function */

// Init memory system with memory block pMemory.
int meminit(void *pMemory, int size) {
  bulk b;

  if (size < memgetminimumsize())
    return 0;

  memory.bulk = pMemory;
  memory.bulkSize = size - memgetblocksize();
  memory.freeBlock = memory.bulk;

  setBulkSize(memory.freeBlock, size - memgetblocksize());
  b = getBulk(memory.freeBlock);
  *b.prev = *b.next = memory.freeBlock;

  return 1;
} /* End of 'meminit' function */

// Allocate memory block of size 'size'.
// Returns pointer to memory block is success, 0 otherwise
void *memalloc(int size) {
  char *prevHead = memory.freeBlock;
  bulk prevHeadB, newHeadB, prevNextB;
  int hasfree = 0;

  if (prevHead == NULL)
    return NULL;

  do {
    prevHeadB = getBulk(prevHead);
    if (*prevHeadB.size >= size) {
      hasfree = 1;
      break;
    }
    prevHead = *prevHeadB.next;
  } while (prevHead != memory.freeBlock);

  if (!hasfree)
    return NULL;

  if (*prevHeadB.size - size - memgetblocksize() > 0) /* Separate */ {
    memory.freeBlock = prevHead + size + memgetblocksize(); // New head
    setBulkSize(memory.freeBlock, *prevHeadB.size - size - memgetblocksize());

    prevHeadB = getBulk(prevHead);
    prevNextB = getBulk(*prevHeadB.next);

    setBulkSize(prevHead, size);
    prevHeadB = getBulk(prevHead);
    *prevHeadB.next = memory.freeBlock; // 1

    newHeadB = getBulk(memory.freeBlock);
    *newHeadB.prev = prevHead; // 2

    *prevNextB.prev = memory.freeBlock; // 3
    *newHeadB.next = prevNextB.userSpace - ptrSize - intSize; //4
  }

  *prevHeadB.size *= -1;

  return prevHeadB.userSpace;
} /* End of 'memalloc' function */

// Free memory previously allocated by memalloc
void memfree(void *p) {
  bulk b = getBulk((char *)p - ptrSize - intSize), prevB = b, nextB = b;
  int newsz;
  int toUnifyLeft = 0, toUnifyRight = 0;
  int tobreak = 0;

  if (*b.size > 0)
    return;

  prevB = getBulk(*b.prev);
  if (*prevB.size > 0 && prevB.userSpace < b.userSpace)
    toUnifyLeft = 1;

  nextB = getBulk(*b.next);
  if (*nextB.size > 0 && nextB.userSpace > b.userSpace)
    toUnifyRight = 1;

  *b.size *= -1;

  // Full circle
  if (nextB.userSpace == prevB.userSpace) {
    meminit(memory.bulk, memory.bulkSize + memgetblocksize());
  }
  else if (toUnifyLeft && !toUnifyRight) {
    newsz = (b.userSpace + *b.size - 1) - prevB.userSpace + 1;
    setBulkSize(prevB.userSpace - intSize - ptrSize, newsz);
    prevB = getBulk(prevB.userSpace - intSize - ptrSize);
    *getBulk(*prevB.next).prev = prevB.userSpace - intSize - ptrSize;
    memory.freeBlock = prevB.userSpace - intSize - ptrSize;
  }
  else if (!toUnifyLeft && toUnifyRight) {
    newsz = (nextB.userSpace + *nextB.size - 1) - b.userSpace + 1;
    setBulkSize(b.userSpace - intSize - ptrSize, newsz);
    b = getBulk(b.userSpace - intSize - ptrSize);
    *getBulk(*b.next).prev = b.userSpace - intSize - ptrSize;
    memory.freeBlock = b.userSpace - intSize - ptrSize;
  }
  else if (toUnifyLeft && toUnifyRight) {
    newsz = (nextB.userSpace + *nextB.size - 1) - prevB.userSpace + 1;
    setBulkSize(prevB.userSpace - intSize - ptrSize, newsz);
    prevB = getBulk(prevB.userSpace - intSize - ptrSize);
    *getBulk(*prevB.next).prev = prevB.userSpace - intSize - ptrSize;
    memory.freeBlock = prevB.userSpace - intSize - ptrSize;
  }
} /* End of 'memfree' function */

// Return minimum size in bytes of the memory pool to allocate 0-bytes block
// use case:
// void *p   = 0;
// void *ptr = malloc(memgetminimumsize() + 1);
// meminit(ptr, memgetminimumsize() + 1)
// p = memalloc(1); // Success!
// memfree(p);
// memdone();
// free(ptr);
int memgetminimumsize() {
  return 2 * ptrSize + intSize;
} /* End of 'memgetblocksize' function */

// Returns size in bytes of additional information per allocation
// use case:
// void *p1 = 0, *p2 = 0;
// int  memsize = memgetminimumsize() + memgetblocksize() + 2;
// void *ptr = malloc(memsize);
// meminit(ptr, memsize())
// p1 = memalloc(1); // Success!
// p2 = memalloc(1); // Success!
// memfree(p2);
// memfree(p1);
// memdone();
// free(ptr);
int memgetblocksize() {
  return 2 * ptrSize + intSize;
} /* End of 'memgetblocksize' function */

void memdone() {
  bulk b = getBulk(memory.freeBlock);
  
  do {

    if (*b.size < 0)
      /* MEMORY LEAK MESSAGE */;

    b = getBulk(*b.next);
  } while (b.userSpace - intSize - ptrSize != memory.freeBlock);
} /* End of 'memdone' function */