#include <stdlib.h>
#include "list.h"

/* Create element (alloc + set data) function.
 *   - data to add:
 *       data *Data;
 *   - data size:
 *       int Size;
 * RETURNS:
 *   (list *) added element.
 */
static list * CreateElement( data *Data ) {
  /* Allocate memoty for newlist element */
  list *L = malloc(sizeof(list));

  /* No memory */
  if (L == NULL)
    return NULL;

  L->Data = *Data;

  return L;
} /* End of 'CreateElement' function */

/* Add to begin function.
 * ARGUMENTS:
 *   - list to add:
 *       list *L;
 *   - data to add:
 *       data *Data;
 * RETURNS:
 *   (list *) new begin ptr.
 */
list * AddToListBegin( list *L, data *Data ) {
  /* Initialize element */
  list *el = CreateElement(Data);

  if (el == NULL)
    return NULL;

  el->Next = L;
  return el;
} /* End of 'AddToListBegin' function */

/* Delete from begin function.
 * ARGUMENTS:
 *   - list to delete from:
 *       list *L;
 *   - deleted element:
 *       data *Data;
 * RETURNS:
 *   (list *) changed list.
 */
list * DeleteFromListBegin( list *L, data *Data ) {
  list *del = L;

  /* Empty list case */
  if (L == NULL) {
    Data = NULL;
    return L;
  }

  /* Filled list case */
  L = L->Next;

  /* Copy data from the list element */
  *Data = del->Data;

  //while (del->Data.Children != NULL) {
  //  del->Data.Children = DeleteFromListBegin(del->Data.Children, &d);
  //  Data->Children = AddToListBegin(Data->Children, &d);
  //}

  /* Free memory*/
  free(del);

  return L;
} /* End of 'DeleteFromListBegin' function */

/* Free list function.
 * ARGUMENTS:
 *   - list:
 *       list *L;
 * RETURNS: None.
 */
void FreeList( list *L, int intro ) {
  list *p = L, *f;

  while (p != NULL)
  {
    /* Separate ptr is going to brcime invalid */
    f = p;

    if (f->Data.Children != NULL && intro)
      FreeList(f->Data.Children, intro);
    /* Redirect ptr */
    p = p->Next;
    /* Invalidaize free ptr */
    f->Data.Children = NULL;
    free(f);
    f = NULL;
  }
} /* End of 'FreeList' function */