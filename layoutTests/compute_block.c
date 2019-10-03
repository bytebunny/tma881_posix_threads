#include "helper.h"

extern pthread_mutex_t mutex_item_done;
extern pthread_mutex_t mutex_compute;
void* (*newton_routine)(double, double, int*, int*);
extern int pic_size;
extern int n_threads;
extern int exponent;
extern int** attractor;
extern int** convergence;
extern char* item_done;

void*
compute_block(void* restrict arg)
{
  size_t offset = ((size_t*)arg)[0];
  free(arg);
  switch (exponent) {
    case 1:
      newton_routine = &newton1;
      break;
    case 2:
      newton_routine = &newton2;
      break;
    case 3:
      newton_routine = &newton3;
      break;
    case 4:
      newton_routine = &newton4;
      break;
    case 5:
      newton_routine = &newton5;
      break;
    case 6:
      newton_routine = &newton6;
      break;
    case 7:
      newton_routine = &newton7;
      break;
    case 8:
      newton_routine = &newton8;
      break;
    case 9:
      newton_routine = &newton9;
      break;
  }

  // extern int pic_size, n_threads;
  // extern pthread_mutex_t item_done_mutex;

  // perform newton iterations, attractor and convergence arrays as output
  double re = 0.0, im = 0.0;
  size_t blocksize = 100;
  for (size_t ib = offset; ib < pic_size; ib += blocksize) {
    int ie = ib + blocksize < pic_size ? ib + blocksize : pic_size;

    for (size_t ix = ib; ix < ie; ix += n_threads) {
      // the following 2 allocations will be freed in write_block.c:
      int* attractor_row = (int*)malloc(sizeof(int) * pic_size);
      int* convergence_row = (int*)malloc(sizeof(int) * pic_size);

      im = 2.0 - (4.0 / (double)(pic_size - 1)) *
                   ((double)ix); // Convert to [-2; 2] interval.
      for (size_t jx = 0; jx < pic_size; ++jx) {
        re = -2.0 + (4. / (double)(pic_size - 1)) * ((double)jx);
        newton_routine(re, im, attractor_row + jx, convergence_row + jx);
      }
      // Save results:
      pthread_mutex_lock(&mutex_compute);
      attractor[ix] = attractor_row;
      convergence[ix] = convergence_row;
      pthread_mutex_unlock(&mutex_compute);
      //    printf(
      //      "attractor: %d, convergence: %d\n", attractor[ix][1],
      //      convergence[ix][1]);

      // Mark as ready for writing:
      pthread_mutex_lock(&mutex_item_done);
      item_done[ix] = 1;
      pthread_mutex_unlock(&mutex_item_done);
    }
  }
  return (NULL);
}
