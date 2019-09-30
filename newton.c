#include <stdio.h> // printing to screan
#include <string.h>
#include <stdlib.h> // memory allocation
#include <pthread.h>
#include "newtonlib.h" // Newton routines.

int n_threads, pic_size, exponent, block_size;

int main(int argc, char *argv[])
{
  char *ptr, *ptr2; // pointers to parse input data.
  size_t ix, jx, tx, ib, ie; // loop indices.

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
  block_size = pic_size / n_threads; // NOTE: integer division.

  void *newton_routine; // pointer to the specific newton function;
  if ( exponent == 1 ) newton_routine = newton1;
  else if ( exponent == 2) newton_routine = newton2;
  else if ( exponent == 3) newton_routine = newton3;
  else {
    printf("Exponent > 3 is not supported yet. \n");
    exit(0);
  }
  
//initialize attractor and convergence arrays before writing
  //these are just dummy arrays now, filled with random roots now, they should be initialized for each compute thread
  int** attractor = (int**) malloc(sizeof(int*) * pic_size);
  int* atrEntries = (int*) malloc(sizeof(int) * pic_size * pic_size);
  for ( ix = 0, jx = 0; ix < pic_size; ++ix, jx+=pic_size ) {
    attractor[ix] = atrEntries + jx;
  }
  int** convergence = (int**) malloc(sizeof(int*) * pic_size);
  int* convEntries = (int*) malloc(sizeof(int) * pic_size * pic_size);
  for ( ix = 0, jx = 0; ix < pic_size; ++ix, jx+=pic_size ) {
    convergence[ix] = convEntries + jx;
  }

  //////////////////////////////// Create threads //////////////////////////////
  pthread_t thread_ids[n_threads];
//  pthread_mutex_t mutex_newton;
  int result_code; // Returned by pthread_create and pthread_join.

//  pthread_mutex_init( &mutex_newton, NULL);
  for ( tx = 0, ib = 0;  tx < n_threads;  ++tx, ib += block_size ){
    ie = ib + block_size;
    if (tx == n_threads - 1) ie = pic_size; // In case n_threads is not a divisor of pic_size.
    
    int **arg = malloc( 5 * sizeof(int*) ); // 2: attractor, convergence, newton routine, ib, ie.
    arg[0] = attractor[ib];
    arg[1] = convergence[ib];
    arg[2] = newton_routine;
    arg[3] = &ib;
    arg[4] = &ie;
    
    if ( return_code = pthread_create( thread_ids[tx], NULL,
                                       compute_block, (void*)arg ) ){
      printf("Error creating thread: %d\n", return_code);
      exit(1);
    }
  }
  

  //perform newton iterations, attractor and convergence arrays as output
  double re = 0, im = 0;
  for ( ix = 0; ix < pic_size; ++ix ) {
    im = 2 - (4./(double)(pic_size-1))*(ix);
    for ( jx = 0; jx < pic_size; ++jx ) {
      re = -2 + (4./(double)(pic_size-1))*(jx);
      if ( exponent == 1 ) {
	newton1(re, im, &attractor[ix][jx], &convergence[ix][jx]);
      } else if ( exponent == 2 ) {
	newton2(re, im, &attractor[ix][jx], &convergence[ix][jx]);
      } else if ( exponent == 3 ) {
	newton3(re, im, &attractor[ix][jx], &convergence[ix][jx]);
	//printf("real = %f, imaginary = %f \n", re, im);
	//printf("attractor: %d \n", attractor[ix][jx]);
	//printf("convergence: %d \n", convergence[ix][jx]);
      } else {
	printf("Not implemented.\n");
	exit(1);
      }
    }
  }

  //create colormap arrays - hardcoded for max 10 colors
  const int atrColorMap[30] = { 158,1,66, 216,62,79, 244,109,67, 253,174,97, 254,224,139, 230,245,152, 171,221,164, 102,194,165, 50,136,189, 94,79,162  };
  FILE *atrfile;
  atrfile = fopen("attractor.ppm","wb");
  fprintf(atrfile, "P3\n");
  fprintf(atrfile, "%d %d \n", pic_size, pic_size);
  fprintf(atrfile,"255\n");
  unsigned short index;
  char output[12];
  for ( ix = 0; ix < pic_size; ++ix ) {
    for ( jx = 0; jx < pic_size; ++jx ) {
      index = 3*attractor[ix][jx];
      sprintf(output, "%.3d %.3d %.3d", atrColorMap[index], atrColorMap[index+1], atrColorMap[index+2]);
      output[11] = ' '; //replace the null by space
      fwrite(output, sizeof(char), sizeof(output), atrfile);
    }
  }
  fclose(atrfile);

  FILE *convfile;
  convfile = fopen("convergence.ppm","wb");
  fprintf(convfile, "P3\n");
  fprintf(convfile, "%d %d \n", pic_size, pic_size);
  fprintf(convfile,"255\n");
  unsigned short greyDegree;
  for ( ix = 0; ix < pic_size; ++ix ) {
    for ( jx = 0; jx < pic_size; ++jx ) {
      greyDegree = 255 * convergence[ix][jx] / (10*exponent);
      sprintf(output, "%.3d %.3d %.3d", greyDegree, greyDegree, greyDegree);
      output[11] = ' ';
      fwrite(output, sizeof(char), sizeof(output), convfile);
    }
  }
  fclose(convfile);

  free(attractor);
  free(atrEntries);
  free(convergence);
  free(convEntries);

  return(0);
}
