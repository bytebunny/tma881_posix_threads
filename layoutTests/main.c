#include <stdio.h>
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define IX_M 6
#define N_THREADS 3
#define BLOCK_SIZE (IX_M / N_THREADS)

int irow[IX_M], icol[IX_M], sum;
pthread_mutex_t mutex_newton, mutex_write;
FILE *pfile;
int totalColor[IX_M][3];
const char header[] = "P3\n1 1\n255\n";

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
        irow[ix] = ix;
    }

    for (tx = 0, ix = 0; tx < N_THREADS; tx++, ix += BLOCK_SIZE)
    {
        int **arg = malloc(sizeof(int *));
        arg[0] = irow + ix;
        if ((ret = pthread_create(threads_newton + tx, NULL, newton, (void *)arg)))
        {
            printf("Error creating thread: %d\n", ret);
            exit(1);
        }
    }

    for (tx = 0; tx < N_THREADS; tx++)
    {
        if ((ret = pthread_join(threads_newton[tx], NULL)))
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
/*
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
*/
    pfile = fopen("file.ppm", "wb");

    pthread_mutex_init(&mutex_write, NULL);

    fwrite(header,sizeof(header),1,pfile);
    
    for (tx = 0, ix = 0; tx < N_THREADS; tx++, ix += BLOCK_SIZE)
    {
        int **arg = malloc(sizeof(int *));
        arg[0] = irow + ix;
        if ((ret = pthread_create(threads_write + tx, NULL, wrpixel, (void *)arg)))
        {
            printf("Error creating thread: %d\n", ret);
            exit(1);
        }
    }

    for (tx = 0; tx < N_THREADS; tx++)
    {
        if ((ret = pthread_join(threads_write[tx], NULL)))
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
            color[0] = 222;
            color[1] = 222;
            color[2] = 222;
            break;
        case 2:
            color[0] = 333;
            color[1] = 333;
            color[2] = 333;
            break;
        }
        pthread_mutex_lock(&mutex_newton);
        totalColor[line_curr][0] = color[0];
        totalColor[line_curr][1] = color[1];
        totalColor[line_curr][2] = color[2];
        printf("%d: %d %d %d\n", line_curr, totalColor[line_curr][0], totalColor[line_curr][1], totalColor[line_curr][2]);
        pthread_mutex_unlock(&mutex_newton);
    }
    return NULL;
}

void *wrpixel(void *restrict arg)
{
    int iy_loc = *(((int **)arg)[0]);
    free(arg);
    int c[3];
    char output[24];
    char retn[] = "\n";
    int spac = 32;
    int curr_iy;
    int curr_offset;

    for (size_t ix = 0; ix < BLOCK_SIZE; ix+=2)
    {
        curr_iy = iy_loc + ix;
        c[0] = totalColor[curr_iy][0];
        c[1] = totalColor[curr_iy][1];
        c[2] = totalColor[curr_iy][2];
        curr_offset = strlen(header) + 12 * curr_iy;
        sprintf(output, "%03d %03d %03d ", c[0], c[1], c[2]);
        pthread_mutex_lock(&mutex_write);
        fseek(pfile, curr_offset, SEEK_SET);
        fwrite(output, 12, 1, pfile);
        fseek(pfile, -1, SEEK_CUR);
        fwrite(retn, strlen(retn), 1, pfile);
        fflush(pfile);
        pthread_mutex_unlock(&mutex_write);
    }
    
    for (size_t ix = 1; ix < BLOCK_SIZE; ix+=2)
    {
        curr_iy = iy_loc + ix;
        c[0] = totalColor[curr_iy][0];
        c[1] = totalColor[curr_iy][1];
        c[2] = totalColor[curr_iy][2];
        curr_offset = strlen(header) + 12 * curr_iy;
        sprintf(output, "%03d %03d %03d ", c[0], c[1], c[2]);
        pthread_mutex_lock(&mutex_write);
        fseek(pfile, curr_offset, SEEK_SET);
        fwrite(output, 12, 1, pfile);
        fseek(pfile, -1, SEEK_CUR);
        fwrite(retn, strlen(retn), 1, pfile);
        fflush(pfile);
        pthread_mutex_unlock(&mutex_write);
    }
    return NULL;
}