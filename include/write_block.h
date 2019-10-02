#include <stdio.h>

#ifndef _H_WRITE_BLOCK // once-only header
#define _H_WRITE_BLOCK

void *write_block( void *restrict arg );

struct write // Define a struct to pass arguments to compute_block()
{
  int **result1;
  int **result2;
  char *completed_items;
  int *color_map;
  FILE *result1_file;
  FILE *result2_file;
};

#endif
