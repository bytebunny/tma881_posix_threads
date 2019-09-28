#include <stdio.h>
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#define IX_M 10000
#define N_THREADS 2
#define BLOCK_SIZE (IX_M / N_THREADS)

int a[IX_M], b[IX_M], sum;
pthread_mutex_t mutex_newton, mutex_write;
FILE *pfile;
int totalColor[IX_M][3];

void *newton(void *arg);
void *wrpixel(void *arg);

int main()
{
    struct timespec ts, tn;
    timespec_get(&ts, TIME_UTC);

    int ret;
    size_t ix, tx;
    pthread_t threads_newton[N_THREADS], threads_write[N_THREADS];
    pthread_mutex_init(&mutex_newton, NULL);

    for (ix = 0; ix < IX_M; ix++)
    {
        a[ix] = ix;
    }

    for (tx = 0, ix = 0; tx < N_THREADS; tx++, ix += BLOCK_SIZE)
    {
        int **arg = malloc(sizeof(int *));
        arg[0] = a + ix;
        if (ret = pthread_create(threads_newton + tx, NULL, newton, (void *)arg))
        {
            printf("Error creating thread: %d\n", ret);
            exit(1);
        }
    }

    for (tx = 0; tx < N_THREADS; tx++)
    {
        if (ret = pthread_join(threads_newton[tx], NULL))
        {
            printf("Error joining thread: %d\n", ret);
        }
    }
    pthread_mutex_destroy(&mutex_newton);
/*
    for (size_t ix = 0; ix < IX_M; ix++)
    {
        for (size_t iy = 0; iy < 3; iy++)
        {
            printf("%d ", totalColor[ix][iy]);
        }
        printf("\n");
    }
*/
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

    pthread_mutex_init(&mutex_write, NULL);

    for (tx = 0, ix = 0; tx < N_THREADS; tx++, ix += BLOCK_SIZE)
    {
        int **arg = malloc(sizeof(int *));
        arg[0] = a + ix;
        if (ret = pthread_create(threads_write + tx, NULL, wrpixel, (void *)arg))
        {
            printf("Error creating thread: %d\n", ret);
            exit(1);
        }
    }

    for (tx = 0; tx < N_THREADS; tx++)
    {
        if (ret = pthread_join(threads_write[tx], NULL))
        {
            printf("Error joining thread: %d\n", ret);
        }
    }

    pthread_mutex_destroy(&mutex_write);
    fclose(pfile);
    timespec_get(&tn, TIME_UTC);
    double diff = (double)(tn.tv_sec - ts.tv_sec) + ((double)(tn.tv_nsec - ts.tv_nsec) / 1000000000L);
    printf("Elapsed time: %1.6f ms\n", diff * 1000);
    return 0;
}

void *newton(void *restrict arg)
{
    int line_loc = *(((int **)arg)[0]);
    free(arg);
    int color[3];
    int line_curr;
    for (size_t ix = 0; ix < BLOCK_SIZE; ix++)
    {
        line_curr = line_loc + ix;
        switch (line_curr % 3)
        {
        case 0:
            color[0] = 111;
            color[1] = 111;
            color[2] = 111;
            break;
        case 1:
            color[0] = 123;
            color[1] = 123;
            color[2] = 123;
            break;
        case 2:
            color[0] = 55;
            color[1] = 55;
            color[2] = 55;
            break;
        }
        pthread_mutex_lock(&mutex_newton);
        totalColor[line_curr][0] = color[0];
        totalColor[line_curr][1] = color[1];
        totalColor[line_curr][2] = color[2];
        //printf("%d: %d %d %d\n",line_curr,totalColor[line_curr][0],totalColor[line_curr][1],totalColor[line_curr][2]);
        pthread_mutex_unlock(&mutex_newton);
    }
    return NULL;
}

void *wrpixel(void *restrict arg)
{
    int c_loc = *(((int **)arg)[0]);
    free(arg);
    int c[3];
    char output[12];
    int line_curr;
    for (size_t ix = 0; ix < BLOCK_SIZE; ix++)
    {
        line_curr = c_loc + ix;
        c[0] = totalColor[line_curr][0];
        c[1] = totalColor[line_curr][1];
        c[2] = totalColor[line_curr][2];

        sprintf(output, "%3d %3d %3d\n", c[0], c[1], c[2]);
        pthread_mutex_lock(&mutex_write);
        //printf("%d: %d %d %d\n",line_curr,c[0],c[1],c[2]);
        fwrite(output, sizeof(char), sizeof(output), pfile);
        fflush(pfile);
        pthread_mutex_unlock(&mutex_write);
    }
    return NULL;
}