#include "helper.h"

int pic_size;
int n_threads;
int exponent;
int wrt_thds;
int header_len;
char charColor[121];
char charGreyColor[400];

int** attractor;
int** convergence;
char* item_done;
FILE *pfile, *cfile;

pthread_mutex_t mutex_item_done;
pthread_mutex_t mutex_compute;
pthread_mutex_t mutex_write_att;
pthread_mutex_t mutex_write_con;

const int atrColorMap[30] = {
  158, 1,   66,  216, 62,  79,  244, 109, 67,  253, 174, 97,  254, 224, 139,
  230, 245, 152, 171, 221, 164, 102, 194, 165, 50,  136, 189, 94,  79,  162
};

int
main(int argc, char* argv[])
{
  //struct timespec ts, tn;
  //timespec_get(&ts, TIME_UTC);

  int greyMap[100];
  for (size_t ix = 0; ix < 100; ix++) {
    greyMap[ix] = 255 * (ix + 1) / 100;
  }

  int index = 0;
  for (size_t ix = 0; ix < 30; ix++) {
    index += sprintf(&charColor[index], "%03d ", atrColorMap[ix]);
  }

  index = 0;
  for (size_t ix = 0; ix < 100; ix++) {
    index += sprintf(&charGreyColor[index], "%03d ", greyMap[ix]);
  }

  char* ptr;
  if (argc == 4) {
    for (size_t ix = 1; ix < argc - 1; ++ix) {
      ptr = strchr(argv[ix], 't');
      if (ptr)
        n_threads = strtol(++ptr, NULL, 10);
      else {
        ptr = strchr(argv[ix], 'l');
        pic_size = strtol(++ptr, NULL, 10);
      }
    }
    exponent = strtol(argv[argc - 1], NULL, 10);
  }

  wrt_thds = 2;
  attractor = (int**)aligned_alloc(64, sizeof(int*) * pic_size);
  convergence = (int**)aligned_alloc(64, sizeof(int*) * pic_size);
  item_done = (char*)calloc(pic_size, sizeof(char));

  char header[64];
  sprintf(header, "P3\n%d %d\n255\n", pic_size, pic_size);
  header_len = strlen(header);
  char filname[64];
  sprintf(filname,"newton_attractors_x%d.ppm",exponent);
  pfile = fopen(filname, "wb");
  fwrite(header, header_len, 1, pfile);

  sprintf(filname,"newton_convergence_x%d.ppm",exponent);
  cfile = fopen(filname, "wb");
  fwrite(header, header_len, 1, cfile);

  pthread_mutex_init(&mutex_item_done, NULL);
  pthread_mutex_init(&mutex_compute, NULL);
  pthread_mutex_init(&mutex_write_att, NULL);
  pthread_mutex_init(&mutex_write_con, NULL);

  pthread_t threads_write[wrt_thds];
  pthread_t threads_compute[n_threads];

  for (size_t tx = 0; tx < n_threads; tx++) {
    size_t* args = malloc(sizeof(size_t));
    args[0] = tx;

    pthread_create(threads_compute + tx, NULL, compute_block, (void*)args);
  }

  for (size_t tx = 0; tx < wrt_thds; tx++) {
    size_t* args = malloc(sizeof(size_t));
    args[0] = tx;
    pthread_create(threads_write + tx, NULL, write_block, (void*)args);
  }

  for (size_t tx = 0; tx < n_threads; tx++) {
    pthread_join(threads_compute[tx], NULL);
  }

  for (size_t tx = 0; tx < wrt_thds; tx++) {
    pthread_join(threads_write[tx], NULL);
  }

  pthread_mutex_destroy(&mutex_item_done);
  pthread_mutex_destroy(&mutex_compute);
  pthread_mutex_destroy(&mutex_write_att);
  pthread_mutex_destroy(&mutex_write_con);

  fclose(pfile);
  fclose(cfile);

  free(attractor);
  free(convergence);
  free(item_done);
/*
  timespec_get(&tn, TIME_UTC);
  double diff = (double)(tn.tv_sec - ts.tv_sec) +
                ((double)(tn.tv_nsec - ts.tv_nsec) / 1000000000L);
  printf("Elapsed time: %1.4f s\n", diff);
*/
  return 0;
}
