#include <conio.h>
#include "turtle.h"

#define MAX_LEN 30

/* Memory leak checker */
#ifdef _DEBUG
# include <crtdbg.h>
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#endif /* _DEBUG */


/* Main program function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (int) exit code.
 */
int main( void ) {
  char fname[MAX_LEN] = "turtle.in";
  int i;
  map m;
  route r;
  SetDbgMemHooks();

  printf("Input file name: ");
  //scanf("%s", fname);

  m = loadMap(fname);

  if (m.paths == NULL) {
    printf("Error: no file or no memory\n");
  }

  r = evalShortestRoute(&m);
  printf("The shortest route length is %i\nRoute is: ", r.time);

  for (i = 0; i < m.w + m.h; i++)
    printf("(%i, %i) ", r.route[i].x, r.route[i].y);
  printf("\n");
  _getch();

  destructMap(&m);
  free(r.route);

  return 0;
} /* End of 'main' function */