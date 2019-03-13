#include <stdio.h>
#include <stdlib.h>
#include "dfs.h"

/* Memory leak checker */
#ifdef _DEBUG
# include <crtdbg.h>
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#endif /* _DEBUG */

void OutputResult( exit_code c ) {
  switch (c)
  {
  case PATH_EXIST:
    printf("Path exist\n");
    break;
  case PATH_NOT_EXIST:
    printf("Path does not exist\n");
    break;
  case KEY1_NOT_EXIST:
    printf("Start vertex does not exist\n");
    break;
  case KEY2_NOT_EXIST:
    printf("Finish vertex does not exist\n");
    break;
  default:
    break;
  }
}

int main() {
  list *G;
  exit_code c1, c2, c3, c4, c5, c6, c7;

  SetDbgMemHooks();
  malloc(1);

  G = LoadGraph("graph.in");
  c1 = DepthFirstSearch(G, 3, 4);
  c2 = DepthFirstSearch(G, 3, 8);
  c3 = DepthFirstSearch(G, 2, 11);
  c4 = DepthFirstSearch(G, 4, 19);
  c5 = DepthFirstSearch(G, 15, 11);
  c6 = DepthFirstSearch(G, 100, 11);
  c7 = DepthFirstSearch(G, 15, 101);

  OutputResult(c1);
  OutputResult(c2);
  OutputResult(c3);
  OutputResult(c4);
  OutputResult(c5);
  OutputResult(c6);
  OutputResult(c7);

  FreeList(G, 1);

  return 0;
}