#include "helper.h"

int pic_size;
int n_threads;

int** attractor;
int** convergence;
char* item_done;
int exponent;

int wrt_thds;

pthread_mutex_t mutex_item_done;
pthread_mutex_t mutex_compute;
pthread_mutex_t mutex_write_att;
pthread_mutex_t mutex_write_con;

FILE *pfile, *cfile;
int header_len;
char charColor[121];

const int atrColorMap[30] = {
  158, 1,   66,  216, 62,  79,  244, 109, 67,  253, 174, 97,  254, 224, 139,
  230, 245, 152, 171, 221, 164, 102, 194, 165, 50,  136, 189, 94,  79,  162
};

void
itoc(int, char*, int*);

int
main(int argc, char* argv[])
{

  int index = 0;
  for (size_t ix = 0; ix < 30; ix++) {
    index += sprintf(&charColor[index], "%03d ", atrColorMap[ix]);
  }
  charColor[120] = '\0';
  printf("%s\n", charColor);
  FILE* colortest = fopen("colortest.text", "wb");
  fwrite(charColor, strlen(charColor), 1, colortest);
  fclose(colortest);

  struct timespec ts, tn;
  timespec_get(&ts, TIME_UTC);

  char* ptr;
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
  wrt_thds = 2;

  attractor = (int**)malloc(sizeof(int*) * pic_size);
  convergence = (int**)malloc(sizeof(int*) * pic_size);
  item_done = (char*)calloc(pic_size, sizeof(char));

  char header[128];
  sprintf(header, "P3\n%d %d\n255\n", pic_size, pic_size);
  header_len = strlen(header);
  // printf("header len: %d\n",header_len);

  pfile = fopen("attractor.ppm", "wb");
  fwrite(header, header_len, 1, pfile);

  cfile = fopen("convergence.ppm", "wb");
  fwrite(header, header_len, 1, cfile);

  pthread_mutex_init(&mutex_item_done, NULL);
  pthread_mutex_init(&mutex_compute, NULL);
  pthread_mutex_init(&mutex_write_att, NULL);
  pthread_mutex_init(&mutex_write_con, NULL);

  int ret;
  pthread_t threads_write[wrt_thds];
  pthread_t threads_compute[n_threads];

  for (size_t tx = 0; tx < n_threads; tx++) {
    size_t* args = malloc(sizeof(size_t));
    args[0] = tx;
    if ((ret = pthread_create(
           threads_compute + tx, NULL, compute_block, (void*)args))) {
      printf("Error creating thread: %d\n", ret);
      exit(1);
    }
  }

  for (size_t tx = 0; tx < wrt_thds; tx++) {
    size_t* args = malloc(sizeof(size_t));
    args[0] = tx;
    if ((ret = pthread_create(
           threads_write + tx, NULL, write_block, (void*)args))) {
      printf("Error creating thread: %d\n", ret);
      exit(1);
    }
  }

  for (size_t tx = 0; tx < n_threads; tx++) {
    if ((ret = pthread_join(threads_compute[tx], NULL))) {
      printf("Error joining thread: %d\n", ret);
    }
  }

  for (size_t tx = 0; tx < wrt_thds; tx++) {
    if ((ret = pthread_join(threads_write[tx], NULL))) {
      printf("Error joining thread: %d\n", ret);
    }
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

  timespec_get(&tn, TIME_UTC);
  double diff = (double)(tn.tv_sec - ts.tv_sec) +
                ((double)(tn.tv_nsec - ts.tv_nsec) / 1000000000L);
  printf("Elapsed time: %1.4f s\n", diff);
  return 0;
}

void
itoc(int n, char* s, int* c)
{
  int ix = 0, jx = 0;
  char tmp[4];
  do {
    tmp[ix++] = n % 10 + 48;
  } while ((n /= 10) > 0);
  tmp[ix] = '\0';
  c[0] = ix;
  do {
    s[jx++] = tmp[--ix];
  } while (ix > 0);
}