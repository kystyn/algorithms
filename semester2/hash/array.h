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
  uint NumBlock;             /* Current number of visible blocks */
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

void DeleteFromArray( array *L, uint Idx );

/* Get by idx function
 * ARGUMENTS:
 *   - array:
 *       array *L;
 *   - index:
 *       int Idx;
 * RETURNS:
 *   (void *) element.
 */
const void * GetByIdx( array *L, uint Idx );

void ChangeByIdx( array *L, uint Idx, const void *Block );

/* Free array function.
 * ARGUMENTS:
 *  - array to free:
 *       array *L;
 * RETURNS: None.
 */
void FreeArray( array *L );
