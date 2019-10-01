#include<stdlib.h> // free()

void *compute_block( void *restrict arg ){
  int **attractor_loc = ((int***)arg)[0];
  int **convergence_loc = ((int***)arg)[1];
  void (*newton_routine)() = (void*)((int**)arg)[2];
  size_t ib = (size_t)((int**)arg)[3];
  size_t ie = (size_t)((int**)arg)[4];
  free(arg);

  extern int pic_size;

  //perform newton iterations, attractor and convergence arrays as output
  double re = 0, im = 0;
  for ( size_t ix = ib; ix < ie; ++ix ) {
    im = 2 - (4./(double)(pic_size-1))*(ix);
    
    for ( size_t jx = 0; jx < pic_size; ++jx ) {
      re = -2 + (4./(double)(pic_size-1))*(jx);
      newton_routine(re, im, &attractor_loc[ix][jx], &convergence_loc[ix][jx]);
    }
  }
  return NULL;
}
