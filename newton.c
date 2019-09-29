#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int n_threads, pic_size, exponent;


void mul_cpx(double* a_re, double* a_im, double b_re, double b_im, double c_re, double c_im)
{
  *a_re = b_re*c_re - b_im*c_im;
  *a_im = b_re*c_im + b_im*c_re;
}

void newton1(double x_im, double x_re, int* attractor, int* convergence)
{
  //Newton routine for polynomial of degree 1
  double r1_re = 1, r1_im = 0;
  int iter=0;

  while (1) {
    x_re += -x_re + 1;
    x_im += -x_im;
    ++iter;

    double delta_re = x_re - r1_re;
    double delta_im = x_im - r1_im;

    if ( (delta_re*delta_re + delta_im*delta_im) < 0.000001 ) { //converged to root1
      *attractor = 0;
      *convergence = iter;
      break;
    } else if ( (x_re*x_re + x_im*x_im) < 0.000001 || //closer than 0.001 to origin
                ( x_re > 10000000000 || x_im > 10000000000 ) ) { //real or imaginary part larger than 10^10
      *attractor = 9;
      *convergence = iter;
      break;
    } 
  }
}

void newton2(double x_im, double x_re, int* attractor, int* convergence)
{
  //Newton routine for polynomial of degree 2
  double r1_re = 1, r1_im = 0, r2_re = -1, r2_im = 0;
  int iter=0;

  while (1) {
    double a_re=0, a_im=0;
    mul_cpx(&a_re, &a_im, x_re, x_im, x_re, x_im);
    double num_re = 0, num_im = 0;
    mul_cpx(&num_re, &num_im, a_re - 1, a_im, 2*x_re, -2*x_im);
    double den = 4*x_re*x_re + 4*x_im*x_im;
    x_re -=  num_re / den;
    x_im -= num_im / den;
    ++iter;

    double delta1_re = x_re - r1_re;
    double delta1_im = x_im - r1_im;
    if ( (delta1_re*delta1_re + delta1_im*delta1_im) < 0.000001 ) { //converged to root1
      *attractor = 0;
      *convergence = iter;
      break;
    }

    double delta2_re = x_re - r2_re;
    double delta2_im = x_im - r2_im;
    if ( (delta2_re*delta2_re + delta2_im*delta2_im) < 0.000001 ) { //converged to root2
      *attractor = 1;
      *convergence = iter;
      break;
    }
    
    if ( (x_re*x_re + x_im*x_im) < 0.000001  || //closer than 0.001 to origin
         ( x_re > 10000000000 || x_im > 10000000000 ) ) { //real or imaginary part larger than 10^10
      *attractor = 9;
      *convergence = iter;
      break;
    } else if ( iter > 50 ) {
      *attractor = 0;
      *convergence = iter;
      break;
    }
  }  
}

void newton3(double x_im, double x_re, int* attractor, int* convergence)
{
  //Newton routine for polynomial of degree 3
  double r1_re = 1, r1_im = 0, r2_re = -0.5, r2_im = 0.86603, r3_re = -0.5, r3_im = -0.86606;
  int iter=0;

  while (1) {
    double a_re=0, a_im=0, b_re=0, b_im=0, c_re=0, c_im=0;
    mul_cpx(&a_re, &a_im, x_re, x_im, x_re, x_im);
    mul_cpx(&b_re, &b_im, a_re, a_im, x_re, x_im);
    mul_cpx(&c_re, &c_im, a_re, a_im, x_re, x_im);
    double num_re = 0, num_im = 0;
    mul_cpx(&num_re, &num_im, c_re - 1, c_im, 3*a_re, -3*a_im);
    double den = 9*a_re*a_re + 9*a_im*a_im;
    x_re -=  num_re / den;
    x_im -= num_im / den;
    ++iter;

    double delta1_re = x_re - r1_re;
    double delta1_im = x_im - r1_im;
    if ( (delta1_re*delta1_re + delta1_im*delta1_im) < 0.000001 ) { //converged to root1
      *attractor = 0;
      *convergence = iter;
      break;
    }
    
    double delta2_re = x_re - r2_re;
    double delta2_im = x_im - r2_im;
    if ( (delta2_re*delta2_re + delta2_im*delta2_im) < 0.000001 ) { //converged to root2
      *attractor = 1;
      *convergence = iter;
      break;
    }

    double delta3_re = x_re - r3_re;
    double delta3_im = x_im - r3_im;
    if ( (delta3_re*delta3_re + delta3_im*delta3_im) < 0.000001 ) { //converged to root3
      *attractor = 2;
      *convergence = iter;
      break;
    }
     
    if ( (x_re*x_re + x_im*x_im) < 0.000001  || //closer than 0.001 to origin
         ( x_re > 10000000000 || x_im > 10000000000 ) ) { //real or imaginary part larger than 10^10
      *attractor = 9;
      *convergence = iter;
      break;
    } else if ( iter > 50 ) {
      *attractor = 0;
      *convergence = iter;
      break;
    }
  }
}


int main(int argc, char *argv[])
{
  char *ptr, *ptr2;

  if ( argc == 4 ) {
    for ( size_t ix = 1; ix < argc - 1; ++ix ){ // starts with 1 because 0 is the program name.
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

  //initialize attractor and convergence arrays before writing
  //these are just dummy arrays now, filled with random roots now, they should be initialized for each compute thread
  int** attractor = (int**) malloc(sizeof(int*) * pic_size);
  int* atrEntries = (int*) malloc(sizeof(int) * pic_size * pic_size);
  for ( size_t ix = 0, jx = 0; ix < pic_size; ++ix, jx+=pic_size ) {
    attractor[ix] = atrEntries + jx;
  }
  int** convergence = (int**) malloc(sizeof(int*) * pic_size);
  int* convEntries = (int*) malloc(sizeof(int) * pic_size * pic_size);
  for ( size_t ix = 0, jx = 0; ix < pic_size; ++ix, jx+=pic_size ) {
    convergence[ix] = convEntries + jx;
  }


  //perform newton iterations, attractor and convergence arrays as output
  for ( size_t ix = 0; ix < pic_size; ++ix ) {
    for ( size_t jx = 0; jx < pic_size; ++jx ) {
      if ( exponent == 1 ) {
	newton1((double)ix-pic_size/2, (double)jx-pic_size/2, &attractor[ix][jx], &convergence[ix][jx]);
      } else if ( exponent == 2 ) {
	newton2((double)ix-pic_size/2, (double)jx-pic_size/2, &attractor[ix][jx], &convergence[ix][jx]);
      } else if ( exponent == 3 ) {
	newton3((double)ix-pic_size/2, (double)jx-pic_size/2, &attractor[ix][jx], &convergence[ix][jx]);
	//printf("real = %d, imaginary = %d \n", (int)ix-pic_size/2, (int)jx-pic_size/2);
	//printf("attractor: %d \n", attractor[ix][jx]);
	//printf("convergence: %d \n", convergence[ix][jx]);
      } else {
	printf("Not implemented.\n");
	exit(1);
      }
    }
  }

  //create colormap arrays - hardcoded for max 10 colors
  const int atrColorMap[30] = { 255,0,0, 0,255,0, 0,0,255, 206,128,84, 179,83,64, 111,54,55, 35,51,66, 75,43,49, 5,21,69, 250,180,140};
  FILE *atrfile;
  atrfile = fopen("attractor.ppm","wb");
  fprintf(atrfile, "P3\n");
  fprintf(atrfile, "%d %d \n", pic_size, pic_size);
  fprintf(atrfile,"255\n");
  char output[13];
  unsigned short index;
  for ( size_t ix = 0; ix < pic_size; ++ix ) {
    for ( size_t jx = 0; jx < pic_size; ++jx ) {
      index = 3*attractor[ix][jx];
      sprintf(output, "%.3d %.3d %.3d ", atrColorMap[index], atrColorMap[index+1], atrColorMap[index+2]);
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
  for ( size_t ix = 0; ix < pic_size; ++ix ) {
    for ( size_t jx = 0; jx < pic_size; ++jx ) {
      greyDegree = 255 * convergence[ix][jx] / (10*exponent);
      sprintf(output, "%.3d %.3d %.3d ", greyDegree, greyDegree, greyDegree);
      //printf("%s\n", output);
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
