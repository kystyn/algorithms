/* Description of the 'array' structure file.
 * Damaskinskiy K.
 */

#pragma once

typedef unsigned int uint;

/* Array structure definition */
typedef struct tag_array
{
  unsigned char *List;               /* Data array */
  unsigned char *CurrentPtr;         /* List[Numblock] */
  uint CurrentListSize;      /* Maximal count of blocks */
  uint NumBlock;             /* Current number of blocks */
  uint BlockSize;            /* Block size */
} array; /* End of 'array' structure */

/* Init array function.
 * ARGUMENTS:
 *   - array:
 *       array *L;
 *   - block size:
 *       int BlockSize;
 * RETURNS: None.
 */
void InitArray( array *L, uint BlockSize, uint startSize );

/* Add to array function.
 * ARGUMENTS:
 *   - array:
 *       array *L;
 *   - data block:
 *       const void *Block;
 *   - block size:
 *       int Size;
 * RETURNS: None.
 */
void AddToArray( array *L, const void *Block );

/* Get by idx function
 * ARGUMENTS:
 *   - array:
 *       array *L;
 *   - index:
 *       int Idx;
 * RETURNS:
 *   (void *) element.
 */
void * GetByIdx( array *L, uint Idx );

/* Reverse function.
 * ARGUMENTS:
 *   - array:
 *       array *L;
 * RETURNS: None.
 */
void Reverse( array *L );

/* Free array function.
 * ARGUMENTS:
 *  - array to free:
 *       array *L;
 * RETURNS: None.
 */
void FreeArray( array *L );
