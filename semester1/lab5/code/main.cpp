#include <stdlib.h>
#include "heap.h"

/* Memory leak checker */
#ifdef _DEBUG
# include <crtdbg.h>
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#endif /* _DEBUG */


int main( void ) {
  SetDbgMemHooks();

  heap h1 = { NULL };
  node *n;

  malloc(1);

  pushNode(&h1, 1);
  pushNode(&h1, 5);
  pushNode(&h1, 6);
  pushNode(&h1, 7);
  pushNode(&h1, 10);
  pushNode(&h1, 8);
  pushNode(&h1, 9);
  pushNode(&h1, 4);
  n = pushNode(&h1, 3);
  pushNode(&h1, 2);
  pushNode(&h1, 20);
  pushNode(&h1, 18);

  //deleteNode(&h1, n);

  /*
  for (int i = 0; i < 12; i++)
    extractMin(&h1);
  */

  clearHeap(&h1);

  return 0;
} /* End of 'main' function */