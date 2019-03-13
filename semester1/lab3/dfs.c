#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "dfs.h"

#define MAX_S 1000

list
  *Visited = NULL;

int IsVisited( int Key ) {
  list *q = Visited;

  while (q != NULL) {
    if (q->Data.Key == Key)
      return 1;
    q = q->Next;
  }
  return 0;
}

data SearchElementByKey( list *Graph, int Key ) {
  data d = {NULL, 0};
  while (Graph != NULL) {
    if (Graph->Data.Key == Key)
      return Graph->Data;
    Graph = Graph->Next;
  }

  return d;
}

/* Search path function. 
 * ARGUMENTS:
 *   - graph list:
 *       list *G;
 *   - keys to search from and to:
 *       int StartKey, EndKey;
 * RETURNS:
 *   exit_code.
 */
exit_code Search( list *G, int StartKey, int EndKey ) {
  list *stack = NULL;
  data d, parent;

  d = SearchElementByKey(G, StartKey);
  stack = AddToListBegin(stack, &d);

  while (stack != NULL) {
    list *child;
    if (stack->Data.Key == EndKey) {
      FreeList(stack, 0);
      return PATH_EXIST;
    }

    stack = DeleteFromListBegin(stack, &parent);
    if (!IsVisited(parent.Key)) {
      Visited = AddToListBegin(Visited, &parent);

      child = parent.Children;
      while (child != NULL) {
        d = SearchElementByKey(G, child->Data.Key);
        if (!IsVisited(d.Key))
          stack = AddToListBegin(stack, &d);
        child = child->Next;
      }
    }
  }

  return PATH_NOT_EXIST;
} /* End of 'Search' function */

/* Load graph function.
 * 
 */
list * LoadGraph( char *FileName ) {
  FILE *F = fopen(FileName, "rt");
  list *G = NULL;
  int n;

  if (F == NULL)
    return NULL;

  while (fscanf(F, "%i", &n) == 1) {
    char str[MAX_S];
  char *p = str;
    data d = {NULL, n};

    G = AddToListBegin(G, &d);

    fgets(str, MAX_S, F);
  while (*p == ' ')
    p++;
    while (sscanf(p, "%i", &n) == 1) {
    int strcnt = 0;
      data d = {NULL, n};
    G->Data.Children = AddToListBegin(G->Data.Children, &d);
    while (n != 0) {
      n = n / 10;
      strcnt++;
    }
    p += strcnt;
    while (*p == ' ')
      p++;
    }
  }
  fclose(F);

  return G;
} /* */

exit_code DepthFirstSearch( list *G, int Key1, int Key2 ) {
  list *p = G;
  int res;

  while (p != NULL && p->Data.Key != Key2) {
    p = p->Next;
  }

  if (p == NULL)
    return KEY2_NOT_EXIST;

  p = G;
  while (p != NULL && p->Data.Key != Key1) {
    p = p->Next;
  }

  if (p == NULL)
    return KEY1_NOT_EXIST;

  res = Search(G, Key1, Key2);

  FreeList(Visited, 0);
  Visited = NULL;

  return res;
} /* End of 'DepthFirstSearch' function */