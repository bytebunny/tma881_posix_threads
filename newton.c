#include <stdio.h> // printing to screan
#include <string.h>
#include <stdlib.h> // memory allocation
#include <pthread.h>
#include "newtonlib.h" // Newton routines.
#include "compute_block.h"
#include "write_block.h"

int n_threads, pic_size, exponent;
pthread_mutex_t item_done_mutex;


int main(int argc, char *argv[])
{
  char *ptr; // pointer to parse input data.
  size_t ix, tx; // loop indices.

  if ( argc == 4 ) {
    for ( ix = 1; ix < argc - 1; ++ix ){ // starts with 1 because 0 is the program name.
      ptr = strchr( argv[ix], 't');
      if (ptr) n_threads = strtol(++ptr, NULL, 10);
      else{
	ptr = strchr( argv[ix], 'l');
	pic_size = strtol(++ptr, NULL, 10);
      }
    }
    exponent = strtol(argv[argc - 1], NULL, 10);
  } else {
    printf("Missing arguments! Correct syntax is: newton -t#numberOfThreads# -l#numberOfLines# #degreeOfPolynomial# \n");
    exit(0);
  }
  
  ///////////////////// Write header of PPM files /////////////////////
  FILE *atrfile;
  char filename[25];
  sprintf(filename, "newton_attractors_x%d.ppm", exponent);
  atrfile = fopen(filename,"wb");
  fprintf(atrfile, "P3\n");
  fprintf(atrfile, "%d %d \n", pic_size, pic_size);
  fprintf(atrfile,"255\n");

  FILE *convfile;
  char filename2[26];
  sprintf(filename2, "newton_convergence_x%d.ppm", exponent);
  convfile = fopen(filename2,"wb");
  fprintf(convfile, "P3\n");
  fprintf(convfile, "%d %d \n", pic_size, pic_size);
  fprintf(convfile,"255\n");

  //initialize attractor and convergence arrays before writing
  //these are just dummy arrays now, filled with random roots now, they should be initialized for each compute thread
  int** attractor = (int**) malloc(sizeof(int*) * pic_size);
  int** convergence = (int**) malloc(sizeof(int*) * pic_size);
  char* item_done = (char*) calloc(pic_size, sizeof(char));

  //////////////////////////////// Create threads //////////////////////////////
  pthread_t compute_thread_array[n_threads];
  pthread_t write_thread;
  int result_code; // Returned by pthread_create and pthread_join.

  pthread_mutex_init( &item_done_mutex, NULL );
  for ( tx = 0;  tx < n_threads;  ++tx ){
    struct compute *arg = (struct compute *)malloc( sizeof(struct compute) );
    arg->thread_number = tx;
    arg->result1 = attractor;
    arg->result2 = convergence;
    arg->completed_items = item_done;
    
    result_code = pthread_create( &compute_thread_array[tx], NULL,
                                  compute_block, (void*)arg );
    if ( result_code ){
      printf("Error creating thread: %d\n", result_code);
      exit(1);
    }
  }
  struct write *arg_write = (struct write *)malloc( sizeof(struct write) );
  arg_write->result1 = attractor;
  arg_write->result2 = convergence;
  arg_write->completed_items = item_done;
  arg_write->result1_file = atrfile;
  arg_write->result2_file = convfile;
  pthread_create( &write_thread, NULL, write_block, (void*)arg_write );

  //////////////////////////////// Destroy threads /////////////////////////////
  for (tx=0; tx < n_threads; ++tx) {
    result_code = pthread_join( compute_thread_array[tx], NULL );
    if ( result_code ) {
      printf("Error joining thread: %d\n", result_code);
      exit(1);
    }
  }
  pthread_join( write_thread, NULL );

  pthread_mutex_destroy(&item_done_mutex);

  fclose(atrfile);
  fclose(convfile);

  free(attractor);
  free(convergence);
  free(item_done);

  return(0);
}
