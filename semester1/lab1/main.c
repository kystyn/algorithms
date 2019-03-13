#include <stdio.h>
#include <conio.h>
#include "list.h"

/* Memory leak checker */
#ifdef _DEBUG
# include <crtdbg.h>
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#endif /* _DEBUG */


/* Menu function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void Menu( void )
{
  int run = 1, k;
  list *L = NULL, *b = NULL;
  char fname[WORD_SIZE], word[WORD_SIZE];

  /* Memory check */
  SetDbgMemHooks();

  /* Run loop */
  while (run != 0)
  {
    char c;

    printf("\n0 - exit\n"
           "1 - read list from file\n"
           "2 - print whole list\n"
           "3 - check word existence\n"
           "4 - find key by word\n"
           "5 - find all words by key\n\n");
    c = _getch();
    switch (c)
    {
    case '0':
      run = 0;
      break;
    case '1':
      printf("Input file name: ");
      scanf("%s", &fname);
      L = Read(fname, &b);
      break;
    case '2':
      Print(L);
      break;
    case '3':
      printf("Input word to check: ");
      scanf("%s", &word);
      printf(CheckWord(L, b, word) ? "Word exists\n" : "Word doesn't exist\n");
      break;
    case '4':
      printf("Input word to find its key: ");
      scanf("%s", &word);
      OutputByWord(L, b, word);
      break;
    case '5':
      printf("Input key to find all words associated with it: ");
      scanf("%i", &k);
      OutputByNumber(L, k);
      break;
    }
  }

  /* Free list */
  FreeList(L);
}

/* Maint program function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (int) exit code.
 */
int main( void )
{
  Menu();
} /* End of 'main' function */

