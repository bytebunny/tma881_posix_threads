#include <stdio.h>
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#define IX_M 30
#define N_THREADS 2
#define BLOCK_SIZE (IX_M / 3 / N_THREADS)

int a[IX_M], b[IX_M], sum;
pthread_mutex_t mutex_sum;
FILE *pfile;

void *wrpixel(void *arg);

int main()
{
    struct timespec ts, tn;
    timespec_get(&ts, TIME_UTC);

    int ret;
    size_t ix, tx;
    pthread_t threads[N_THREADS];
    pthread_mutex_init(&mutex_sum, NULL);

    for (ix = 0; ix < IX_M; ix++)
    {
        a[ix] = ix;
    }

    FILE *fp = fopen("file.ppm", "rb");
    if (fp == NULL)
    {
        printf("file dose not exist.\n");
        printf("write new file title.\n");
        FILE *ip = fopen("file.ppm", "wb");
        fprintf(ip, "P3\n");
        fprintf(ip, "1 1\n");
        fprintf(ip, "255\n");
        fflush(ip);
        fclose(ip);
    }
    else
    {
        fclose(fp);
    }

    pfile = fopen("file.ppm", "ab");
    if (pfile != NULL)
    {
        printf("start to write color.\n");
    }

    for (tx = 0, ix = 0; tx < N_THREADS; tx++, ix += BLOCK_SIZE)
    {
        int **arg = malloc(sizeof(int *));
        arg[0] = a + 3 * ix;
        if (ret = pthread_create(threads + tx, NULL, wrpixel, (void *)arg))
        {
            printf("Error creating thread: %d\n", ret);
            exit(1);
        }
    }

    for (tx = 0; tx < N_THREADS; tx++)
    {
        if (ret = pthread_join(threads[tx], NULL))
        {
            printf("Error joining thread: %d\n", ret);
        }
    }

    pthread_mutex_destroy(&mutex_sum);

    timespec_get(&tn, TIME_UTC);
    double diff = (double)(tn.tv_sec - ts.tv_sec) + ((double)(tn.tv_nsec - ts.tv_nsec) / 1000000000L);
    printf("Elapsed time: %1.6f ms\n", diff * 1000);
    return 0;
}

void *wrpixel(void *restrict arg)
{
    int *c_loc = ((int **)arg)[0];
    free(arg);
    int c[3];
    char output[12];
    for (size_t ix = 0; ix < BLOCK_SIZE; ix++)
    {
        c[0] = c_loc[ix * 3 + 0];
        c[1] = c_loc[ix * 3 + 1];
        c[2] = c_loc[ix * 3 + 2];

        sprintf(output, "%d %d %d \n", c[0], c[1], c[2]);

        pthread_mutex_lock(&mutex_sum);
        fwrite(output, sizeof(char), sizeof(output), pfile);
        fflush(pfile);
        pthread_mutex_unlock(&mutex_sum);
    }
    return NULL;
}