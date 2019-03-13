#pragma once

#include "list.h"

typedef enum {
  PATH_EXIST,
  PATH_NOT_EXIST,
  KEY1_NOT_EXIST,
  KEY2_NOT_EXIST
} exit_code;

/* Load graph function.
 * 
 */
list * LoadGraph( char *FileName );

exit_code DepthFirstSearch( list *G, int Key1, int Key2 );

