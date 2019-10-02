#include "helper.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int atrColorMap[30] = {
  158, 1,   66,  216, 62,  79,  244, 109, 67,  253, 174, 97,  254, 224, 139,
  230, 245, 152, 171, 221, 164, 102, 194, 165, 50,  136, 189, 94,  79,  162
};
const char retn[] = "\n";

extern pthread_mutex_t mutex_item_done;
extern pthread_mutex_t mutex_write;
extern int pic_size;
extern FILE *pfile, *cfile;
extern char* item_done;
extern int** attractor;
extern int** convergence;
extern int header_len;
extern int n_threads;

void*
write_block(void* restrict arg)
{
  size_t offset = ((size_t*)arg)[0];
  free(arg);
  struct timespec sleep_timespec;
  sleep_timespec.tv_sec = 0;
  sleep_timespec.tv_nsec = 100000; // sleep 100 microseconds.
  int* attra_row_loc = (int*)malloc(pic_size * sizeof(int));
  char* item_done_loc = (char*)calloc(pic_size, sizeof(char));

  for (size_t ix = offset;
       ix < pic_size;) { // NOTE: emptry increment statement!
    pthread_mutex_lock(&mutex_item_done);
    if (item_done[ix]) // copy data from item_done to item_done_loc:
    {
      memcpy(item_done_loc, item_done, pic_size * sizeof(char));
    }
    pthread_mutex_unlock(&mutex_item_done);

    if (item_done_loc[ix] == 0) {
      nanosleep(&sleep_timespec, NULL);
      continue;
    }
    int curr_offset;
    int output_colors[3 * pic_size];
    char attra_char_colors[12 * pic_size];

    for (; ix < pic_size && item_done_loc[ix]; ix += n_threads) {
      memcpy(attra_row_loc, attractor[ix], pic_size * sizeof(int));
      curr_offset = header_len + 12 * pic_size * ix;

      // write convergence.ppm

      for (size_t jx = 0; jx < pic_size; jx++) {
        int color_type = attra_row_loc[jx];
        output_colors[3 * jx] = atrColorMap[3 * color_type];
        output_colors[3 * jx + 1] = atrColorMap[3 * color_type + 1];
        output_colors[3 * jx + 2] = atrColorMap[3 * color_type + 2];
        // printf("%d %d %d\n",
        //        output_colors[3 * jx],
        //        output_colors[3 * jx + 1],
        //        output_colors[3 * jx + 2]);
      }

      int index = 0;
      for (size_t jx = 0; jx < 3 * pic_size; jx++) {
        index += sprintf(&attra_char_colors[index], "%03d ", output_colors[jx]);
        // printf("%s\n", attra_char_colors);
        // printf("str len: %ld\n", strlen(attra_char_colors));
      }

      pthread_mutex_lock(&mutex_write);
      fseek(pfile, curr_offset, SEEK_SET);
      fwrite(attra_char_colors, strlen(attra_char_colors), 1, pfile);
      fseek(pfile, -1, SEEK_CUR);
      fwrite(retn, strlen(retn), 1, pfile);
      fflush(pfile);
      pthread_mutex_unlock(&mutex_write);

      // write convergence.ppm
      memcpy(attra_row_loc, convergence[ix], pic_size * sizeof(int));

      for (size_t jx = 0; jx < pic_size; jx++) {
        int color_type = 255 * attra_row_loc[jx] / 100;
        output_colors[3 * jx] = color_type;
        output_colors[3 * jx + 1] = color_type;
        output_colors[3 * jx + 2] = color_type;
        // printf("%d %d %d\n",
        //        output_colors[3 * jx],
        //        output_colors[3 * jx + 1],
        //        output_colors[3 * jx + 2]);
      }

      index = 0;
      for (size_t jx = 0; jx < 3 * pic_size; jx++) {
        index += sprintf(&attra_char_colors[index], "%03d ", output_colors[jx]);
        // printf("%s\n", attra_char_colors);
        // printf("str len: %ld\n", strlen(attra_char_colors));
      }

      pthread_mutex_lock(&mutex_write);
      fseek(cfile, curr_offset, SEEK_SET);
      fwrite(attra_char_colors, strlen(attra_char_colors), 1, cfile);
      fseek(cfile, -1, SEEK_CUR);
      fwrite(retn, strlen(retn), 1, cfile);
      fflush(cfile);
      pthread_mutex_unlock(&mutex_write);
    }
  }
  free(item_done_loc);
  free(attra_row_loc);
  return (NULL);
}
