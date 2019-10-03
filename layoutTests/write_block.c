#include "helper.h"

const char retn[] = "\n";
extern pthread_mutex_t mutex_item_done;
extern pthread_mutex_t mutex_write_att;
extern pthread_mutex_t mutex_write_con;
extern int pic_size;
extern FILE *pfile, *cfile;
extern char* item_done;
extern int** attractor;
extern int** convergence;
extern int header_len;
extern int wrt_thds;
extern char charColor[121];
extern char charGreyColor[400];

void*
write_block(void* restrict arg)
{
  size_t offset = ((size_t*)arg)[0];
  free(arg);
  struct timespec sleep_timespec;
  sleep_timespec.tv_sec = 0;
  sleep_timespec.tv_nsec = 100; // sleep 100 microseconds.
  int attra_row_loc[pic_size];
  char item_done_loc[pic_size];

  for (size_t ix = offset; ix < pic_size;) {
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
    char attra_char_colors[12 * pic_size];

    for (; ix < pic_size && item_done_loc[ix]; ix += wrt_thds) {
      memcpy(attra_row_loc, attractor[ix], pic_size * sizeof(int));
      int curr_offset = header_len + 12 * pic_size * ix;

      // write convergence.ppm

      for (size_t jx = 0; jx < pic_size; jx++) {
        int color_type = attra_row_loc[jx];
        for (size_t iz = 0; iz < 12; iz++) {
          attra_char_colors[12 * jx + iz] = charColor[12 * color_type + iz];
        }

        // printf("%d %d %d\n",
        //        output_colors[3 * jx],
        //        output_colors[3 * jx + 1],
        //        output_colors[3 * jx + 2]);
      }
      attra_char_colors[12 * pic_size] = '\n';
      // printf("%s\n", attra_char_colors);
      // int index = 0;
      // for (size_t jx = 0; jx < 3 * pic_size; jx++) {
      //  index += sprintf(&attra_char_colors[index], "%03d ",
      //  output_colors[jx]);
      //  // printf("str len: %ld\n", strlen(attra_char_colors));
      //}

      pthread_mutex_lock(&mutex_write_att);
      fseek(pfile, curr_offset, SEEK_SET);
      fwrite(attra_char_colors, 12 * pic_size, 1, pfile);
      // fseek(pfile, -1, SEEK_CUR);
      // fwrite(retn, 1, 1, pfile);
      fflush(pfile);
      // free(attractor[ix]);
      pthread_mutex_unlock(&mutex_write_att);

      // write convergence.ppm
      memcpy(attra_row_loc, convergence[ix], pic_size * sizeof(int));

      for (size_t jx = 0; jx < pic_size; jx++) {
        int color_type = attra_row_loc[jx] - 1;
        for (size_t iz = 0; iz < 12; iz++) {
          attra_char_colors[12 * jx + iz] = charGreyColor[4 * color_type + iz % 4];
        }
        // printf("%d %d %d\n",
        //        output_colors[3 * jx],
        //        output_colors[3 * jx + 1],
        //        output_colors[3 * jx + 2]);
      }

      // int index = 0;
      // for (size_t jx = 0; jx < 3 * pic_size; jx++) {
      //  index += sprintf(&attra_char_colors[index], "%03d ",
      //  output_colors[jx]);
      // printf("str len: %ld\n", strlen(attra_char_colors));
      //}
      attra_char_colors[12 * pic_size] = '\n';
      //printf("%s\n", attra_char_colors);

      pthread_mutex_lock(&mutex_write_con);
      fseek(cfile, curr_offset, SEEK_SET);
      fwrite(attra_char_colors, 12 * pic_size, 1, cfile);
      // fseek(cfile, -1, SEEK_CUR);
      // fwrite(retn, 1, 1, cfile);
      fflush(cfile);
      // free(convergence[ix]);
      pthread_mutex_unlock(&mutex_write_con);
    }
  }
  return (NULL);
}
