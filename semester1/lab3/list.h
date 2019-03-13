#pragma once

/* List element structure */
typedef struct tag_list {
  struct tag_data {
    struct tag_list *Children;
    int Key;
  } Data;
  struct tag_list *Next;
} list;

typedef struct tag_data data;

/* Data structure */
/*struct tag_data {
  list *L;
  int Key;
};                */

/* Add to begin function.
 * ARGUMENTS:
 *   - list to add:
 *       list *L;
 *   - data to add:
 *       data *Data;
 * RETURNS:
 *   (list *) new begin ptr.
 */
list * AddToListBegin( list *L, data *Data );

/* Delete from begin function.
 * ARGUMENTS:
 *   - list to delete from:
 *       list *L;
 *   - deleted element:
 *       data *Data;
 * RETURNS:
 *   (list *) changed list.
 */
list * DeleteFromListBegin( list *L, data *Data );

/* Free list function.
 * ARGUMENTS:
 *   - list:
 *       list *L;
 * RETURNS: None.
 */
void FreeList( list *L, int intro );