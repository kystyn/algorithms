// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/* Implementation of the 'array' structure file.
 * Damaskinskiy K.
 */

#include <stdlib.h>
#include <string.h>
#include "array.h"


/* Init array function.
 * ARGUMENTS:
 *   - array:
 *       array *L;
 *   - block size:
 *       int BlockSize;
 * RETURNS: None.
 */
void InitArray( array *L, uint BlockSize, uint startCount )
{
  L->CurrentListSize = startCount;
  L->List = malloc(L->CurrentListSize * BlockSize);
  memset(L->List, 0, L->CurrentListSize * BlockSize);
  if (L->List == NULL)
      return;
  L->CurrentPtr = L->List;
  L->NumBlock = 0;
  L->BlockSize = BlockSize;
} /* End of 'InitList' function */

/* Add to array function.
 * ARGUMENTS:
 *   - array:
 *       array *L;
 *   - data block:
 *       void *Block;
 *   - block size:
 *       int Size;
 * RETURNS: None.
 */
void AddToArray( array *L, const void *Block )
{
  /* Finished memory or fist add */
  /*if (L->NumBlock == 0)
    L->CurrentPtr = L->List = malloc(L->BlockSize * L->CurrentListSize);
  else */if (L->NumBlock == L->CurrentListSize)
  {
    void *b;

    /* Increase memory size twice */
    L->CurrentListSize <<= 1;

    b = malloc(L->BlockSize * L->CurrentListSize);
    memset(b, 0, L->BlockSize * L->CurrentListSize);

    memcpy(b, L->List, L->BlockSize * (L->CurrentListSize >> 1));
    free(L->List);
    L->List = b;
    L->CurrentPtr = L->List + L->NumBlock * L->BlockSize;
  }
  memcpy(L->CurrentPtr, Block, L->BlockSize);
  L->CurrentPtr += L->BlockSize;
  L->NumBlock++;
} /* End of 'AddToList' function */

/// ATTENTION: changes index order
void DeleteFromArray( array *L, uint Idx ) {
    // clear string
    free(*(char **)(L->List + Idx * L->BlockSize));
    memmove(L->List + Idx * L->BlockSize,
      L->List + (Idx + 1) * L->BlockSize,
      (L->NumBlock - 1 - (Idx + 1) + 1) * L->BlockSize);
    *(char **)GetByIdx(L, L->NumBlock - 1) = NULL;
}

/* Free array function.
 * ARGUMENTS:
 *  - array to free:
 *       array *L;
 * RETURNS: None.
 */
void FreeArray( array *L )
{
  free(L->List);
  L->List = 0;
} /* End of 'FreeList' function */

/* Get by idx function
 * ARGUMENTS:
 *   - array:
 *       array *L;
 *   - index:
 *       int Idx;
 * RETURNS:
 *   (void *) element.
 */
void * GetByIdx( array *L, uint Idx )
{
  /* Trying to get too big element */
  if (Idx >= L->CurrentListSize)
    return NULL;
  return L->List + Idx * L->BlockSize;
} /* End of 'GetByIdx' function */

void ChangeByIdx( array *L, uint Idx, const void *Block ) {
    if (Idx >= L->CurrentListSize)
      return;
    memcpy(L->List + Idx * L->BlockSize, Block, L->BlockSize);
}

/* Reverse function.
 * ARGUMENTS:
 *   - array:
 *       array *L;
 * RETURNS: None.
 */
void Reverse( array *L )
{
  unsigned int i;

  for (i = 0; i < L->NumBlock / 2; i++)
  {
    char
      *p1 = GetByIdx(L, i),
      *p2 = GetByIdx(L, L->NumBlock - 1 - i);

    unsigned int j;

    for (j = 0; j < L->BlockSize; j++)
    {
      char t = p1[j];
      p1[j] = p2[j];
      p2[j] = t;
    }
  }
} /* End of 'Reverse' function */
