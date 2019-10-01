#include <stdlib.h> // free()
#include <pthread.h>

void *compute_block( void *restrict arg ){
  size_t offset = ((size_t*)arg)[0];
  void (*newton_routine)() = (void*)( (size_t*) arg )[1];
  int** attractor = (int**)( (size_t*) arg )[2];
  int** convergence = (int**)( (size_t*) arg )[3];
  char* item_done = (char*)( (size_t*) arg )[4];
  free(arg);

  extern int pic_size, n_threads;
  extern pthread_mutex_t item_done_mutex;

  //perform newton iterations, attractor and convergence arrays as output
  double re = 0, im = 0;
  for ( size_t ix = offset; ix < pic_size; ix += n_threads ){
    // the following 2 allocations will be freed in write_block.c:
    int* attractor_row = (int*) malloc(sizeof(int)*pic_size); 
    int* convergence_row = (int*) malloc(sizeof(int)*pic_size);
   
    im = 2 - (4./(double)(pic_size-1))*(ix); // Convert to [-2; 2] interval.
    for ( size_t jx = 0; jx < pic_size; ++jx ) {
      re = -2 + (4./(double)(pic_size-1))*(jx);
      newton_routine(re, im, &attractor_row[jx], &convergence_row[jx]);
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
