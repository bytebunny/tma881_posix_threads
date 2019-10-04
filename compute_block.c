#include <stdlib.h> // free()
#include <pthread.h>
#include "compute_block.h"


void *compute_block( void *restrict arg ){
  size_t offset = ( (struct compute*)arg )->thread_number;
  void* (*newton_routine)(double, double, int*, int*) = ( (struct compute*) arg )->newton_func;
  int** attractor = ( (struct compute*) arg )->result1;
  int** convergence = ( (struct compute*) arg )->result2;
  char* item_done = ( (struct compute*) arg )->completed_items;
  free(arg);

  extern int pic_size, n_threads;
  extern pthread_mutex_t item_done_mutex;

  // precompute coordinates on complex plane:
  double real_coords[pic_size], image_coords[pic_size];
  for ( size_t ix = 0; ix < pic_size; ++ix )
    { // Convert to [-2; 2] interval:
      real_coords[ix] = -2 + (4./(double)(pic_size-1))*(ix);
      image_coords[ix] = 2 - (4./(double)(pic_size-1))*(ix); 
    }

  //perform newton iterations, attractor and convergence arrays as output
  for ( size_t ix = offset; ix < pic_size; ix += n_threads ){
    // the following 2 allocations will be freed in write_block.c:
    int* attractor_row = (int*) malloc(sizeof(int)*pic_size); 
    int* convergence_row = (int*) malloc(sizeof(int)*pic_size);
   
    for ( size_t jx = 0; jx < pic_size; ++jx )
      {
        newton_routine(real_coords[jx], image_coords[ix],
                       &attractor_row[jx], &convergence_row[jx]);
      }
    
    // Save results:
    attractor[ix] = attractor_row;
    convergence[ix] = convergence_row;

    // Mark as ready for writing:
    pthread_mutex_lock(&item_done_mutex);
    item_done[ix] = 1;
    pthread_mutex_unlock(&item_done_mutex);
  }
  return(NULL);
}
