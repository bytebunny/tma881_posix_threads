#include <stdlib.h> // free()
#include <pthread.h>
#include <string.h> // memcpy()
#include <time.h> // nanosleep()
#include <stdio.h> // FILE *, fwrite()
#include "write_block.h"

void *write_block( void *restrict arg ){
  int** attractor = ( (struct write*) arg )->result1;
  int** convergence = ( (struct write*) arg )->result2;
  char* item_done = ( (struct write*) arg )->completed_items;
  int* atrColorMap = ( (struct write*) arg )->color_map;
  FILE* atrfile = ( (struct write*) arg )->result1_file;
  FILE* convfile = ( (struct write*) arg )->result2_file;
  free(arg);

  extern int pic_size, exponent;
  extern pthread_mutex_t item_done_mutex;

  char* item_done_loc = (char*) calloc(pic_size, sizeof(char));
  struct timespec sleep_timespec;
  sleep_timespec.tv_sec = 0;
  sleep_timespec.tv_nsec = 100000; // sleep 100 microseconds.

  unsigned short index; // for writing to file.
  char output[12];
  unsigned short greyDegree;
  
  for ( size_t ix = 0; ix < pic_size; ) { // NOTE: emptry increment statement!
    pthread_mutex_lock(&item_done_mutex);
    if ( item_done[ix] != 0 ) // copy data from item_done to item_done_loc:
      memcpy(item_done_loc, item_done, pic_size*sizeof(char));
    pthread_mutex_unlock(&item_done_mutex);

    if ( item_done_loc[ix] == 0 ) {
      nanosleep(&sleep_timespec, NULL);
      continue;
    }
    
    for ( ; ix < pic_size && item_done_loc[ix] != 0; ++ix ) {
      int* attractor_row = attractor[ix];
      int* convergence_row = convergence[ix];
      
      for ( size_t jx = 0; jx < pic_size; ++jx ) {
        // Attractor:
        index = 3 * attractor[ix][jx];
        sprintf(output, "%.3d %.3d %.3d", atrColorMap[index], 
                atrColorMap[index+1], atrColorMap[index+2]);
        output[11] = ' '; //replace the null by space
        fwrite(output, sizeof(char), sizeof(output), atrfile);
        
        // Convergene:
        greyDegree = 255 * convergence[ix][jx] / (10*exponent);
        sprintf(output, "%.3d %.3d %.3d", greyDegree, greyDegree, greyDegree);
        output[11] = ' ';
        fwrite(output, sizeof(char), sizeof(output), convfile);
      }
      free(attractor_row);
      free(convergence_row);
    }
  }
  free(item_done_loc);
  return(NULL);
}
