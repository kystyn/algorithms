#include <stdlib.h>
#include "planner.h"

/* Memory leak checker */
#ifdef _DEBUG
# include <crtdbg.h>
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#endif /* _DEBUG */

int main( int argc, char *argv[] ) {

  project p;
#ifdef _DEBUG
  SetDbgMemHooks();
#endif
  malloc(1);

  p = scan("task1.in");
  formPlan(&p);
  outputResult("bigtest.out", &p);

  freeProject(&p);

  return 0;
}