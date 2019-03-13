#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "memallocator.h"

/* Useful definitions */
#define ptrSize sizeof(char *)
#define intSize sizeof(int)

typedef struct tagBulk {
  int
    *size,
    *signedSize;
  char *userSpace;
  char
    **prev,
    **next;
} bulkInfo;

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
bulkInfo getBulk( char *bulk ) {
  bulkInfo b;

  b.prev = (char **)bulk;
  b.size = (int *)(bulk + ptrSize);
  b.signedSize = (int *)(bulk + ptrSize + intSize + *b.size);
  b.userSpace = bulk + ptrSize + intSize;
  b.next = (char **)(bulk + ptrSize + 2 * intSize + *b.size);

  return b;
} /* End of 'getBulk' function */

// Init memory system with memory block pMemory.
int meminit(void *pMemory, int size) {
  bulkInfo bi;

  /* Memory bulk */
  memory.bulk = pMemory;
  /* Set size */
  setBulkSize(memory.bulk, size - 2 * (ptrSize + intSize));
  /* Get bulk info */
  bi = getBulk(pMemory);
  /* Memory bulk size */
  memory.bulkSize = size;

  memory.freeBlock = memory.bulk;

  /* Set previous */
  *bi.prev = NULL;
  /* Set size */
  *bi.size = memory.bulkSize - 2 * (ptrSize + intSize);

  /* Set next */
  *bi.next = memory.freeBlock;
  /* Set sgn size */
  *bi.signedSize = *bi.size;

  return pMemory == NULL;
}

// You can implement memory leak checks here
void memdone() {
  char
    *start = memory.freeBlock,
    *ptr = memory.freeBlock;
  int freeSize = 0;

  do {
    bulkInfo bi = getBulk(ptr);

    freeSize += (*bi.signedSize > 0) * *bi.size;
    /* Go to next bulk */
    ptr = *bi.next;
  } while (ptr != start);

  if (freeSize != memory.bulkSize)
    printf("Memory leak detected!\n");
}

// Allocate memory block of size 'size'.
// Returns pointer to memory block is success, 0 otherwise
void *memalloc(int size) {
  int abletoalloc = 0;
  char *prev = memory.freeBlock;
  bulkInfo bi, prevbi;

  /* No free memory */
  if (memory.freeBlock == NULL)
    return NULL;

  /* Redirect free block ptr */
  do {
    bi = getBulk(memory.freeBlock);
    if (*bi.size >= size && *bi.signedSize > 0) {
      abletoalloc = 1;
      break;
    }

    memory.freeBlock = *bi.next;
  } while (memory.freeBlock != prev);

  /* Cannot alloc memory */
  if (!abletoalloc) {
    memory.freeBlock = NULL;
    return NULL;
  }

  // Will be given to user
  prev = memory.freeBlock;

  prevbi = getBulk(prev);

  // Check whether size
  // of free separated block is enough for using it as memory bulk
  if (*prevbi.size - size > memgetblocksize()) {
    // new really free block
    memory.freeBlock += memgetblocksize() + size;

    /* Fill current's new free block size */
    setBulkSize(memory.freeBlock, *prevbi.size - size - memgetblocksize());
    bi = getBulk(memory.freeBlock);
    *bi.signedSize = *bi.size;
    /* Fill current's 'prev' */
    *bi.prev = prev;

    // prepare 'prev' bulk for user
    /* Fill prev's size */
    *prevbi.size = size;
    prevbi = getBulk(prev);
    /* Fill prev's signed size */
    *prevbi.signedSize = -size;
    /* Fill prev's 'next' */
    *prevbi.next = memory.freeBlock;
    /* First malloc call*/
    if (*prevbi.prev == NULL)
      *prevbi.prev = memory.freeBlock;
  }
  else { // give user FULL bulk, nothin is changing
    // loop to find free block
    char *cur = memory.freeBlock;
    bulkInfo fbi = prevbi;

    do {
      memory.freeBlock = *fbi.next;
      fbi = getBulk(memory.freeBlock);

      if (*fbi.signedSize > 0)
        break;

    } while (memory.freeBlock != cur);

    /* No free memory */
    if (memory.freeBlock == cur)
      memory.freeBlock = NULL;

    *prevbi.signedSize *= -1;
  }

  return prevbi.userSpace;
} /* End of 'memalloc' function */

// Free memory previously allocated by memalloc
void memfree(void *p) {
  bulkInfo bi, prevbi;

  if (!((char *)p > memory.bulk && (char *)p < memory.bulk + memory.bulkSize)) {
    printf("Non-existing pointer to free given!\n");
    return;
  }

  bi = getBulk((char *)p - intSize - ptrSize);
  /* If there is the only block*/
  if (*bi.prev == NULL) {
    memory.freeBlock = (char *)p - intSize - ptrSize;
    *bi.signedSize *= -1;
    return;
  }
  prevbi = getBulk((char *)*bi.prev);

  if (*bi.signedSize > 0) {
    printf("Already free pointer fiven!\n");
    return;
  }

  /* Unify of two bulks - defragmentation */
  if (*prevbi.signedSize > 0) {
    bulkInfo nextbi;
    *prevbi.size += 2 * (ptrSize + intSize) + *bi.size;
    prevbi = getBulk(*bi.prev);
    *prevbi.signedSize = *prevbi.size;
    nextbi = getBulk(*bi.next);
    *nextbi.prev = prevbi.userSpace - ptrSize - intSize;

    /* If there wasn't free memory */
    if (memory.freeBlock == NULL)
      memory.freeBlock = *bi.prev;
  }
  else {
    if (memory.freeBlock == NULL)
      memory.freeBlock = (char *)p - intSize - ptrSize;
    *bi.signedSize *= -1;
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
  return 2 * (ptrSize + intSize);
} /* End of 'memgetminimumsize' function */


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
  return 2 * (ptrSize + intSize);
} /* End of 'memgetblocksize' function */

int main() {
  return 0;
}
