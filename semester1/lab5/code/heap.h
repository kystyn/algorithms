#pragma once

/* Node structure type representation */
typedef struct tagnode {
  int key;
  struct tagnode
    *down, *up;
} node; /* End of 'node' structure */

typedef struct tagheap {
  node
    *root,
    *majorPath;
} heap;

/* Push key into heap function.
 * ARGUMENTS:
 *   - heap:
 *       heap *h;
 *   - key to push:
 *       int key;
 * RETURNS:
 *   (node *) node pointer.
 */
node * pushNode( heap *h, int key );

/* Extract minumum from heap function.
 * ARGUMENTS:
 *   - heap:
 *       heap **h;
 * RETURNS:
 *   (int) minimum.
 */
int extractMin( heap *h );

/* Merge heaps function.
 * ARGUMENTS:
 *   - heaps:
 *       heap *h1, *h2;
 * RETURNS:
 *   (heap) final heap.
 */
heap meld( heap *h1, heap *h2 );

/* Delete node function.
 * ARGUMENTS:
 *   - heap:
 *       heap *h;
 *   - node to delete:
 *       node *toDelete;
 * RETURNS:
 *   (int) key.
 */
int deleteNode( heap *h, node *toDelete );

/* Delete node function.
 * ARGUMENTS:
 *   - heap:
 *       heap *h;
 *   - node to decrease:
 *       node *toDecrease;
 * RETURNS:
 *   (int) true if success.
 */
int decreaseKey( heap *h, node *toDecrease, int newKey );

/* Clear heap function.
 * ARGUMENTS:
 *   - heap:
 *       heap *h;
 * RETURNS: None.
 */
void clearHeap( heap *h );