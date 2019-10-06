#ifndef _H_COMP_BLOCK // once-only header
#define _H_COMP_BLOCK

void *compute_block( void *restrict arg );

struct compute // Define a struct to pass arguments to compute_block()
{
  size_t thread_number;
  int** result1;
  int** result2;
  char* completed_items;
};

#endif
