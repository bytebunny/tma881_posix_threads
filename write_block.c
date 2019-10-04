#include <stdlib.h> // free()
#include <pthread.h>
#include <string.h> // memcpy()
#include <time.h> // nanosleep()
#include <stdio.h> // FILE *, fwrite()
#include "write_block.h"

#include <stdio.h> // temp

void *write_block( void *restrict arg ){
  int** attractor = ( (struct write*) arg )->result1;
  int** convergence = ( (struct write*) arg )->result2;
  char* item_done = ( (struct write*) arg )->completed_items;
  FILE* atrfile = ( (struct write*) arg )->result1_file;
  FILE* convfile = ( (struct write*) arg )->result2_file;
  free(arg);

  extern int pic_size;
  extern pthread_mutex_t item_done_mutex;

  //create colormap arrays - hardcoded for max 10 colors
  const int atrColorMap[30] = { 158,1,66, 216,62,79, 244,109,67, 253,174,97, 254,224,139, 230,245,152, 171,221,164, 102,194,165, 50,136,189, 94,79,162  };
  char charColor[121];
  char charGreyColor[400];

  int index = 0;
  for (int ix = 0; ix < 100; ix++) {
    index += sprintf(&charGreyColor[index], "%03d ", 255 * (ix + 1) / 100);
  }

  index = 0;
  for (size_t ix = 0; ix < 30; ix++) {
    index += sprintf(&charColor[index], "%03d ", atrColorMap[ix]);
  }

  char* item_done_loc = (char*) calloc(pic_size, sizeof(char));
  struct timespec sleep_timespec;
  sleep_timespec.tv_sec = 0;
  sleep_timespec.tv_nsec = 100000; // sleep 100 microseconds.

//  unsigned short greyDegree;
  char attra_char_colors[12 * pic_size];
  char conv_char_colors[12 * pic_size];

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
        int color_type = attractor_row[jx];
        memcpy(&attra_char_colors[12 * jx], &charColor[12 * color_type], 12*sizeof(char));

        color_type = convergence_row[jx] - 1;
        memcpy(&conv_char_colors[12 * jx], &charGreyColor[12 * color_type], 4*sizeof(char));
        memcpy(&conv_char_colors[12 * jx + 4], &charGreyColor[12 * color_type], 4*sizeof(char));
        memcpy(&conv_char_colors[12 * jx + 8], &charGreyColor[12 * color_type], 4*sizeof(char));
      }
      attra_char_colors[12 * pic_size -1] = '\n';
      conv_char_colors[12 * pic_size -1] = '\n';

      fwrite(attra_char_colors, sizeof(char), 12*pic_size, atrfile);
      fwrite(conv_char_colors, sizeof(char), 12*pic_size, convfile);

      free(attractor_row);
      free(convergence_row);
    }
  }
  free(item_done_loc);
  return(NULL);
}
