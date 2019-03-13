#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef enum tf {
  WEST, SOUTH
} from;

/* Map structure */
typedef struct tagMap {
  int
    w, h; // number of streets, i. e. {number of crossroads - 1}
  int **paths;
} map; /* End of 'map' structure */

typedef struct tagRoute {
  int time;
  struct {int x, y;} *route; //0 - came from west, 1 - from south
} route;

/* Load map function.
 * ARGUMENTS:
 *   - file name:
 *       char *fileName;
 * RETURNS:
 *   (map) .
 */
map loadMap( char *fileName );

/* Evaluate the fastest route time function.
 * ARGUMENTS:
 *   - map:
 *       map *m;
 * RETURNS:
 *   (int) trip minimal time.
 */
route evalShortestRoute( map *m );

/* Destruct map function.
 * ARGUMENTS:
 *   - map:
 *       map *m;
 * RETURNS: None.
 */
void destructMap( map *m );
