#include "turtle.h"

#define min(A, B) ((A) <= (B) ? (A) : (B))

int init2DArray( int w, int h, int ***arr ) {
  int i;

  *arr = malloc(h * sizeof(int));

  if (*arr == NULL)
    return 0;

  for (i = 0; i < h; i++) {
    (*arr)[i] = malloc(sizeof(int) * w);

    if ((*arr)[i] == NULL)
      return 0;
  }

  return 1;
} /* End of 'init2DArray' function */

void clear2DArray( int w, int h, int **arr ) {
  int i;

  for (i = 0; i < h; i++)
    free(arr[i]);

  free(arr);
} /* End of 'clear2DArray' function */

/* Load map function.
 * ARGUMENTS:
 *   - file name:
 *       char *fileName;
 * RETURNS:
 *   (map) .
 */
map loadMap( char *fileName ) {
  int i, j;
  map m = { 0 };
  FILE *F = fopen(fileName, "r");

  if (F == NULL)
    return m;

  fscanf(F, "%i%i", &m.h, &m.w);

  if (!init2DArray(m.w + 1, 2 * m.h + 1, &m.paths))
    return m;

  for (i = 2 * m.h; i >= 0; i--)
    for (j = 0; j < m.w + i % 2; j++)
      fscanf(F,"%i", &m.paths[i][j]);

  return m;
} /* End of 'loadMap' function */

/* Evaluate the fastest route time function.
 * ARGUMENTS:
 *   - map:
 *       map *m;
 * RETURNS:
 *   (int) trip minimal time, -1 if fault.
 */
route evalShortestRoute( map *m ) {
  int **times;
  from **cameFrom;
  int x, y, i;
  route r = { 0 };

  if (!init2DArray(m->w + 1, m->h + 1, &times) || !init2DArray(m->w + 1, m->h + 1, &cameFrom) ||
    (r.route = malloc((m->w + m->h) * sizeof(int) * 2)) == NULL)
    return r;

  /* Evaluate times along the lowest row */
  times[0][0] = 0;
  cameFrom[0][0] = WEST; 

  for (x = 1; x <= m->w; x++) {
    times[0][x] = times[0][x - 1] + m->paths[0][x - 1];
    cameFrom[0][x] = WEST;
  }

  /* Evaluate time along the leftist column */
  for (y = 1; y <= m->h; y++) {
    times[y][0] = times[y - 1][0] + m->paths[2 * y - 1][0];
    cameFrom[y][0] = SOUTH;
  }

  for (y = 1; y < m->h + 1; y++)
    for (x = 1; x < m->w + 1; x++) {
      times[y][x] = min(times[y][x - 1] + m->paths[2 * y][x - 1], times[y - 1][x] + m->paths[2 * y - 1][x]);
      cameFrom[y][x] =
        (times[y][x] != times[y][x - 1] + m->paths[2 * y][x - 1]);
    }

  r.time = times[m->h][m->w];

  x = m->w;
  y = m->h;

  for (i = m->h + m->w - 1; i >= 0; i--) {
    int cur = cameFrom[y][x];
    r.route[i].x = x;
    r.route[i].y = y;

    x -= (cur == WEST);
    y -= (cur == SOUTH);
  }

  clear2DArray(m->w + 1, m->h + 1, times);
  clear2DArray(m->w + 1, m->h + 1, cameFrom);

  return r;
} /* End of 'evalShortestRoute' function */

/* Destruct map function.
 * ARGUMENTS:
 *   - map:
 *       map *m;
 * RETURNS: None.
 */
void destructMap( map *m ) {
  clear2DArray(m->w + 1, 2 * m->h + 1, m->paths);
} /* End of 'destructMap' function */
