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
  char *ptr, *ptr2; // pointers to parse input data.
  size_t ix, tx; // loop indices.

  if ( argc == 4 ) {
    for ( ix = 1; ix < argc - 1; ++ix ){ // starts with 1 because 0 is the program name.
      ptr = strchr( argv[ix], 't');
      if (ptr) n_threads = strtol(++ptr, &ptr2, 10);
      else{
	ptr = strchr( argv[ix], 'l');
	pic_size = strtol(++ptr, &ptr2, 10);
      }
    }
    exponent = strtol(argv[argc - 1], &ptr2, 10);
    printf("t is %d, l is %d, d is %d\n", n_threads, pic_size, exponent);
  } else {
    printf("Missing arguments! Correct syntax is: newton -t#numberOfThreads# -l#numberOfLines# #degreeOfPolynomial# \n");
    exit(0);
  }

  void (*newton_routine)(); // pointer to the specific newton function;
  if ( exponent == 1 ) newton_routine = newton1;
  else if ( exponent == 2) newton_routine = newton2;
  else if ( exponent == 3) newton_routine = newton3;
  else {
    printf("Exponent > 3 is not supported yet. \n");
    exit(1);
  }
  
  ///////////////////// Write header of PPM files /////////////////////
  //create colormap arrays - hardcoded for max 10 colors
  const int atrColorMap[30] = { 158,1,66, 216,62,79, 244,109,67, 253,174,97, 254,224,139, 230,245,152, 171,221,164, 102,194,165, 50,136,189, 94,79,162  };
  FILE *atrfile;
  atrfile = fopen("attractor.ppm","wb");
  fprintf(atrfile, "P3\n");
  fprintf(atrfile, "%d %d \n", pic_size, pic_size);
  fprintf(atrfile,"255\n");

  FILE *convfile;
  convfile = fopen("convergence.ppm","wb");
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
    size_t *arg = malloc( 5 * sizeof(size_t) ); // 5: tx, newton routine, attractor, convergence, item_done.
    arg[0] = tx;
    arg[1] = (size_t)newton_routine;
    arg[2] = (size_t)attractor;
    arg[3] = (size_t)convergence;
    arg[4] = (size_t)item_done;
    
    result_code = pthread_create( &compute_thread_array[tx], NULL,
                                  compute_block, (void*)arg );
    if ( result_code ){
      printf("Error creating thread: %d\n", result_code);
      exit(1);
    }
  }
  int** arg_write = calloc( 6, sizeof(int*) );
  arg_write[0] = (int*)item_done;
  arg_write[1] = (int*)attractor;
  arg_write[2] = (int*)convergence;
  arg_write[3] = atrColorMap;
  arg_write[4] = (int*)atrfile;
  arg_write[5] = (int*)convfile;
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

  fclose(atrfile);
  fclose(convfile);

  free(attractor);
  free(convergence);
  free(item_done);

  return(0);
}
