#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/* Create element (alloc + set data) function.
 *   - data to add:
 *       data *Data;
 *   - data size:
 *       int Size;
 * RETURNS:
 *   (list *) added element.
 */
static list * CreateElement( data *Data )
{
  /* Allocate memoty for newlist element */
  list *L = malloc(sizeof(list));

  /* No memory */
  if (L == NULL)
    return NULL;

  /* Set data to cell */
  strcpy(L->Data.Word, Data->Word);
  L->Data.Key = Data->Key;

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
static list * AddToListBegin( list *L, data *Data )
{
  /* Initialize element */
  list *el = CreateElement(Data);

  if (el == NULL)
    return NULL;

  el->Next = L;
  return el;
} /* End of 'AddToListBegin' function */

/* Add by alphabet function.
 * ARGUMENTS:
 *   - list to add:
 *       list *L;
 *   - barrier:
 *       list *B;
 *   - data to add:
 *       data *Data;
 * RETURNS:
 *   (list *) new begin ptr.
 */
static list * AddByAlphabet( list *L, list *B, data *Data )
{
  list *ptr = L, *prev = L;

  /* Fill barrier */
  strcpy(B->Data.Word, Data->Word);
  B->Data.Key = Data->Key;

  /* Searching word */
  while (strcmp(ptr->Data.Word, B->Data.Word) < 0)
  {
    prev = ptr;
    ptr = ptr->Next;
  }

  /* Add data - we reached barrier */
  if (ptr->Next == NULL || strcmp(ptr->Data.Word, B->Data.Word) > 0)
  {
    list *newel = AddToListBegin(ptr, Data);

    /* If added before first element, redirect L */
    if (L == B)
      L = newel;
    else
      prev->Next = newel;
  }
  else /* Summarize keys */
    ptr->Data.Key += Data->Key;

  return L;
} /* End of 'AddByAlphabet' function */

/* Read list function.
 * ARGUMENTS:
 *   - file name:
 *       const char *Name;
 *   - barrier (functiob fills it):
 *       list **B;
 * RETURNS:
 *   (list *) list.
 */
list * Read( const char *Name, list **B )
{
  FILE *F = fopen(Name, "rt");
  data d = { 0 };
  list
    *L = { NULL },
    *b = AddToListBegin(L, &d);
  L = b;

  /* Fill barrier */
  *B = b;

  /* Empty file */
  if (F == NULL)
  {
    printf("File doesn't exist\n");
    return NULL;
  }

  while (!feof(F))
  {
    fscanf(F, "%s%i", &d.Word, &d.Key);
    L = AddByAlphabet(L, b, &d);
  }

  if (!ferror(F))
    return L;

  FreeList(L);
  return NULL;
} /* End of 'Read' function */

/* Print list function.
 * ARGUMENTS:
 *   - list to print:
 *       list *L;
 * RETURNS: None.
 */
void Print( list *L )
{
  while (L->Next !=  NULL)
  {
    printf("%s: %i\n", L->Data.Word, L->Data.Key);
    L = L->Next;
  }
} /* End of 'Print' function */

/* Free list function.
 * ARGUMENTS:
 *   - list:
 *       list *L;
 * RETURNS: None.
 */
void FreeList( list *L )
{
  list *p = L, *f;

  while (p != NULL)
  {
    /* Separate ptr is going to brcime invalid */
    f = p;
    /* Redirect ptr */
    p = p->Next;
    /* Invalidaize free ptr */
    free(f);
  }
} /* End of 'FreeList' function */

/* Check word existence function.
 * ARGUMENTS:
 *   - list:
 *       list *L;
 *   - barrier:
 *       list *B;
 *   - word:
 *       const char *Word;
 * RETURNS:
 *   (int) true if exists, 0 otherwise.
 */
int CheckWord( list *L, list *B, const char *Word )
{
  strcpy(B->Data.Word, Word);
  while (strcmp(L->Data.Word, Word) < 0)
    L = L->Next;

  /* Return true if found word and it is not a barrier */
  return strcmp(L->Data.Word, Word) == 0 && L->Next != NULL;
} /* End of 'CheckWord' function */

/* Output word function.
  * ARGUMENTS:
 *   - list:
 *       list *L;
 *   - barrier:
 *       list *B;
 *   - word:
 *       const char *Word;
 * RETURNS: None.
 */
void OutputByWord( list *L, list *B, const char *Word )
{
  strcpy(B->Data.Word, Word);
  while (strcmp(L->Data.Word, Word) < 0)
    L = L->Next;

  /* Print if found a word and it is not a barrier */
  if (strcmp(L->Data.Word, Word) == 0 && L->Next != NULL)
    printf("Element is found: %s, %i\n", L->Data.Word, L->Data.Key);
  else
    printf("Element is not found!\n");
} /* End of 'Output' function */

/* Output by number function.
 * ARGUMENTS:
 *   - list:
 *       list *L;
 *   - number:
 *       int N;
 * RETURNS: None.
 */
void OutputByNumber( list *L, int N )
{
  /* While didn't reach barrier, not the last element */
  while (L->Next != NULL)
  {
    if (L->Data.Key == N)
      printf("%s ", L->Data.Word);
    L = L->Next;
  }

  printf("\n");
} /* End of 'OutputByNumber' function */
