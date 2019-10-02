#include "helper.h"
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int pic_size;
int n_threads;

int** attractor;
int** convergence;
char* item_done;

pthread_mutex_t mutex_item_done;
pthread_mutex_t mutex_compute;

FILE *pfile, *cfile;
int header_len;

int
main(int argc, char* argv[])
{
  struct timespec ts, tn;
  timespec_get(&ts, TIME_UTC);

  char* ptr;
  int exponent;
  if (argc == 4) {
    for (size_t ix = 1; ix < argc - 1;
         ++ix) { // starts with 1 because 0 is the program name.
      ptr = strchr(argv[ix], 't');
      if (ptr)
        n_threads = strtol(++ptr, NULL, 10);
      else {
        ptr = strchr(argv[ix], 'l');
        pic_size = strtol(++ptr, NULL, 10);
      }
    }
    exponent = strtol(argv[argc - 1], NULL, 10);

    printf("t is %d, l is %d, d is %d\n", n_threads, pic_size, exponent);
  } else {
    printf("Missing arguments! Correct syntax is: newton -t#numberOfThreads# "
           "-l#numberOfLines# #degreeOfPolynomial# \n");
    exit(0);
  }

  attractor = (int**)malloc(sizeof(int*) * pic_size);
  convergence = (int**)malloc(sizeof(int*) * pic_size);
  item_done = (char*)calloc(pic_size, sizeof(char));

  char header[128];
  sprintf(header, "P3\n%d %d\n255\n", pic_size, pic_size);
  header_len = strlen(header);
  //printf("header len: %d\n",header_len);

  pfile = fopen("attractor.ppm", "wb");
  fwrite(header, header_len, 1, pfile);

  cfile = fopen("convergence.ppm", "wb");
  fwrite(header, header_len, 1, cfile);

  pthread_mutex_init(&mutex_item_done, NULL);
  pthread_mutex_init(&mutex_compute, NULL);

  int ret;
  pthread_t threads_write[1];
  pthread_t* threads_compute =
    (pthread_t*)malloc(sizeof(pthread_t) * n_threads);

  for (size_t tx = 0; tx < n_threads; tx++) {
    size_t* args = malloc(2 * sizeof(size_t));
    args[0] = tx;
    args[1] = exponent;
    if ((ret = pthread_create(
           threads_compute + tx, NULL, compute_block, (void*)args))) {
      printf("Error creating thread: %d\n", ret);
      exit(1);
    }
  }

  for (size_t tx = 0; tx < 1; tx++) {
    // char **args = malloc(sizeof(char *));
    // args[0] = item_done;
    if ((ret = pthread_create(threads_write + tx, NULL, write_block, NULL))) {
      printf("Error creating thread: %d\n", ret);
      exit(1);
    }
  }

  for (size_t tx = 0; tx < n_threads; tx++) {
    if ((ret = pthread_join(threads_compute[tx], NULL))) {
      printf("Error joining thread: %d\n", ret);
    }
  }

  for (size_t tx = 0; tx < 1; tx++) {
    if ((ret = pthread_join(threads_write[tx], NULL))) {
      printf("Error joining thread: %d\n", ret);
    }
  }
  pthread_mutex_destroy(&mutex_item_done);
  pthread_mutex_destroy(&mutex_compute);
  fclose(pfile);

  free(attractor);
  free(convergence);
  free(item_done);
  free(threads_compute);

  timespec_get(&tn, TIME_UTC);
  double diff = (double)(tn.tv_sec - ts.tv_sec) +
                ((double)(tn.tv_nsec - ts.tv_nsec) / 1000000000L);
  printf("Elapsed time: %1.6f ms\n", diff * 1000);
  return 0;
}
