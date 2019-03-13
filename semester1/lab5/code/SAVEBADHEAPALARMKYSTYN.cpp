#include <stdlib.h>
#include "heap.h"


/* Make heap of one element function.
 * ARGUMENTS:
 *   - key:
 *       int key;
 * RETURNS:
 *   (heap) made heap.
 */
heap makeHeap( int key ) {
  heap h;
  h.root = new node();

  if (h.root == NULL)
    return h;

  h.root->key = key;
  h.root->up = h.root;
  h.root->down = h.root;

  h.majorPath = h.root;

  return h;
} /* End of 'makeHeap' function */

/* Find major path function.
 * ARGUMENTS:
 *   - root:
 *       node *r;
 * RETURNS:
 *   (node *) major path start point.
 */
node * findMajorPath( node *r ) {
  while (r->down != r && r->down->up != r)
    r = r->down->up;

  return r;
} /* End of 'findMajorPath' function */

/* Swap two pointers function.
 * ARGUMENTS:
 *   - nodes:
 *       node *h1, *h2;
 * RETURNS: None.
 */
void swap( node **h1, node **h2 ) {
  node *t = *h1;
  *h1 = *h2;
  *h2 = t;
} /* End of 'swap' function */

/* Meld heaps function (got up(h1), up(h2)).
 * ARGUMENTS:
 *   - heaps roots ups:
 *       node *h1, *h2;
 *   - function where it is called drom
 *     (interruption condition depends on this param):
 *       functionType t;
 * RETURNS:
 *   (heap *) final heap.
 */
heap bottomUpMeld( node *h1, node *h2 ) {
  if (h1->key < h2->key)
    swap(&h1, &h2);
  node
    *ptr1 = h1,
    *ptr2 = h2,
    *upptr1 = ptr1,
    *prevh3 = NULL,
    *h3 = h1; // h3 is current top

  heap h;
  h.majorPath = h.root = NULL;

  while (!((h3 == ptr2 && ptr2->up == ptr2) || ((h3 == ptr1 && ptr1->up == ptr1))) && h3 != h3->up) {
    upptr1 = ptr1->up;
    if (ptr1->up->key < ptr2->up->key) {
      // remember element of melded heap
      prevh3 = h3;
      h3 = ptr2;

      // start redirections
      prevh3->up = h3->down; //OK
      if (h3->down == h3) { // h3 has no child 
        prevh3->up = h3;  // prevh3->up->up if h3 has child
        h3->down = prevh3; // make prevh3 a left child of h3
      }
      else                // h3 has a child
        prevh3->up->up = h3;
      h3->down = prevh3; // OK

      ptr2 = ptr2->up;
    }
    ptr1 = upptr1;
    h3 = h3->up;  // h3 is the top!
  }

  // attach leftover elements
  if (h3 == ptr1) {
    h3->up = ptr2;
    if (ptr2->down == ptr2)
      ptr2->down = h3;
    else 
      ptr2->down->up = h3;
  }
  else {
    h3->up = ptr1;
    if (ptr1->down == ptr1)
      ptr1->down = h3;
    else 
      ptr1->down->up = h3;
  }

  // fill major path
  h.majorPath = findMajorPath(h3->up);

  // fill root
  while (h3->up != h3)
    h3 = h3->up;
  h.root = h3;

  return h;
} /* End of 'bottomUpMeld' function */

/* Meld heaps function (got h1, h2).
 * ARGUMENTS:
 *   - heaps roots ups:
 *       heap *h1, *h2;
 * RETURNS:
 *   (heap) final heap.
 */
heap meld( heap *h1, heap *h2 ) {
  if (h1->majorPath == NULL)
    return *h2;
  if (h2->majorPath == NULL)
    return *h1;

  return bottomUpMeld(h1->majorPath, h2->majorPath);
} /* End of 'meld' function */

/* Push key into heap function.
 * ARGUMENTS:
 *   - heap:
 *       heap *h;
 *   - key to push:
 *       int key;
 * RETURNS:
 *   (node *) node pointer.
 */
node * pushNode( heap *h, int key ) {
  heap x = makeHeap(key);
  *h = meld(h, &x);

  return x.root;
} /* End of 'pushNode' function */

/* Extract minumum from heap function.
 * ARGUMENTS:
 *   - heap:
 *       heap **h;
 * RETURNS:
 *   (int) minimum, LONG_MIN if NULL heap.
 */
int extractMin( heap *h ) {
  node *m = h->root;
  node *h1;

  if (h == NULL || h->root == NULL)
    return LONG_MIN;

  int val = m->key;

  // only one child
  if (h->root->down == h->root) {
    h->root = NULL;
    h->majorPath = NULL;
    free(m);
    return val;
  }

  // only left children
  if (h->root->down->up == h->root) {
    h->root = h->root->down;
    h->root->up = h->root;
    h->majorPath = findMajorPath(h->root);
    free(m);
    return val;
  }

  h1 = h->root->down;

  h->root->down->up->up = h->root->down->up;
  h->root->down->up = h->root->down;
  *h = bottomUpMeld(findMajorPath(h1), h->majorPath);

  free(m);

  return val;
} /* End of 'extractMin' function */

/* Delete node function.
 * ARGUMENTS:
 *   - heap:
 *       heap *h;
 *   - node to delete:
 *       node *toDelete;
 * RETURNS:
 *   (int) key.
 */
int deleteNode( heap *h, node *toDelete ) {
  int n = toDelete->key;
  node *d = toDelete;

  // this is the minimum
  if (toDelete->up == toDelete)
    return extractMin(h);
  // up is sibling
  else if (toDelete->up->up->down == toDelete) {
    // this is the leaf
    if (toDelete->down == toDelete)
      toDelete->up->up->down = toDelete->up;
    // this is not the leaf
    else
      toDelete->up->up->down = toDelete->down;
  }
  // up is parent
  else {
     // this is the leaf
    if (toDelete->down == toDelete) {
      // toDelete is sibling
      if (toDelete->up->down->up == toDelete)
        toDelete->up->down->up = toDelete->up;
      // toDelete is only child
      else
        toDelete->up->down = toDelete->up;
    }
    // this is not the leaf
    else {
      // toDelete is sibling
      if (toDelete->up->down->up == toDelete) {
        toDelete->up->down->up = toDelete->down;
        //toDelete = toDelete->down;
        //toDelete->down = toDelete->up;
        //toDelete->up = d->up;
        //toDelete->down = toDelete->down->up;
      }
      // toDelete is only child
      else
        toDelete->up->down = toDelete->down;
    }
  }

  // if we can attach child let's do it
  if (toDelete->down->up == toDelete)
    toDelete->down->up = toDelete->up;

  h->majorPath = findMajorPath(h->root);

  free(d);

  return n;
} /* End of 'deleteNode' function */

/* Delete node function.
 * ARGUMENTS:
 *   - heap:
 *       heap *h;
 *   - node to decrease:
 *       node *toDecrease;
 * RETURNS:
 *   (int) true if success.
 */
int decreaseKey( heap *h, node *toDecrease, int newKey ) {
  if (newKey > toDecrease->key)
    return false;

  deleteNode(h, toDecrease);
  pushNode(h, newKey);

  return true;
} /* End of 'decreaseKey' function */

/* Clear heap function.
 * ARGUMENTS:
 *   - heap:
 *       heap *h;
 * RETURNS: None.
 */
void clearHeap( heap *h ) {
  while (h->root != NULL)
    deleteNode(h, h->majorPath);
} /* End of 'clearHeap' function */
