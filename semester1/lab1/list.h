#pragma once

#define WORD_SIZE 100

/* Data structure */
typedef struct tag_data {
  char Word[WORD_SIZE];
  int Key;
} data;

/* List element structure */
typedef struct tag_list {
  data Data;
  struct tag_list *Next;
} list;

/* Read list function.
 * ARGUMENTS:
 *   - file name:
 *       const char *Name;
 *   - barrier (functiob fills it):
 *       list **B;
 * RETURNS:
 *   (list *) list.
 */
list * Read( const char *Name, list **B );

/* Print list function.
 * ARGUMENTS:
 *   - list to print:
 *       list *L;
 * RETURNS: None.
 */
void Print( list *L );

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
int CheckWord( list *L, list *B, const char *Word );

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
void OutputByWord( list *L, list *B, const char *Word );

/* Output by number function.
 * ARGUMENTS:
 *   - list:
 *       list *L;
 *   - number:
 *       int N;
 * RETURNS: None.
 */
void OutputByNumber( list *L, int N );

/* Free list function.
 * ARGUMENTS:
 *   - list:
 *       list *L;
 * RETURNS: None.
 */
void FreeList( list *L );
