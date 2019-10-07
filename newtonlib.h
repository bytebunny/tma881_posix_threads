#ifndef _H_NEWTON // once-only header
#define _H_NEWTON

#include<math.h>

static inline void mul_cpx(double* a_re, double* a_im,
                           double b_re, double b_im,
                           double c_re, double c_im)
{
  *a_re = b_re*c_re - b_im*c_im;
  *a_im = b_re*c_im + b_im*c_re;
}



static inline void *newton1(double x_re, double x_im,
                            int* attractor, int* convergence)
{
  //Newton routine for polynomial of degree 1
  double r1_re = 1, r1_im = 0;
  int iter=0, capIter=100;

  while (1) {
    if ( (x_re*x_re + x_im*x_im) < 0.000001 || //closer than 0.001 to origin
                ( fabs(x_re) > 10000000000. || fabs(x_im) > 10000000000. ) ) { //real or imaginary part larger than 10^10
      *attractor = 9;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    x_re += -x_re + 1;
    x_im += -x_im;
    ++iter;

    double delta_re = x_re - r1_re;
    double delta_im = x_im - r1_im;

    if ( (delta_re*delta_re + delta_im*delta_im) < 0.000001 ) { //converged to root1
      *attractor = 0;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
  }
  return NULL;
}



static inline void *newton2(double x_re, double x_im,
                            int* attractor, int* convergence)
{
  //Newton routine for polynomial of degree 2
  double r1_re = 1, r1_im = 0, r2_re = -1, r2_im = 0;
  int iter=0, capIter = 100;

  while (1) {
    
    if ( (x_re*x_re + x_im*x_im) < 0.000001 || //closer than 0.001 to origin
                ( fabs(x_re) > 10000000000. || fabs(x_im) > 10000000000. ) ) { //real or imaginary part larger than 10^10
      *attractor = 9;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
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
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta2_re = x_re - r2_re;
    double delta2_im = x_im - r2_im;
    if ( (delta2_re*delta2_re + delta2_im*delta2_im) < 0.000001 ) { //converged to root2
      *attractor = 1;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
  }
  return NULL;
}



static inline void *newton3(double x_re, double x_im,
                            int* attractor, int* convergence)
{
  //Newton routine for polynomial of degree 3
  double r1_re = 1, r1_im = 0, r2_re = -0.5, r2_im = 0.86603, r3_re = -0.5, r3_im = -0.86606;
  int iter=0, capIter = 100;

  while (1) {
    if ( (x_re*x_re + x_im*x_im) < 0.000001 || //closer than 0.001 to origin
                ( fabs(x_re) > 10000000000. || fabs(x_im) > 10000000000. ) ) { //real or imaginary part larger than 10^10
      *attractor = 9;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double a_re=0, a_im=0, c_re=0, c_im=0;
    mul_cpx(&a_re, &a_im, x_re, x_im, x_re, x_im);
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
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double delta2_re = x_re - r2_re;
    double delta2_im = x_im - r2_im;
    if ( (delta2_re*delta2_re + delta2_im*delta2_im) < 0.000001 ) { //converged to root2
      *attractor = 1;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta3_re = x_re - r3_re;
    double delta3_im = x_im - r3_im;
    if ( (delta3_re*delta3_re + delta3_im*delta3_im) < 0.000001 ) { //converged to root3
      *attractor = 2;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
  }
  return NULL;
}

static inline void *newton4(double x_re, double x_im,
                            int* attractor, int* convergence)
{
  //Newton routine for polynomial of degree 4
  double r1_re = 1, r1_im = 0, r2_re = 0, r2_im = 1, r3_re = -1, r3_im = 0, r4_re = 0, r4_im = -1;
  int iter=0, capIter=100;

  while (1) {
    if ( (x_re*x_re + x_im*x_im) < 0.000001 || //closer than 0.001 to origin
                ( fabs(x_re) > 10000000000. || fabs(x_im) > 10000000000. ) ) { //real or imaginary part larger than 10^10
      *attractor = 9;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double a_re=0, a_im=0, b_re=0, b_im=0, c_re=0, c_im=0;
    mul_cpx(&a_re, &a_im, x_re, x_im, x_re, x_im); //x^2
    mul_cpx(&b_re, &b_im, a_re, a_im, a_re, a_im); //x^4
    mul_cpx(&c_re, &c_im, a_re, a_im, x_re, x_im); //x^3
    double num_re = 0, num_im = 0;
    mul_cpx(&num_re, &num_im, b_re - 1, b_im, 4*c_re, -4*c_im);
    double den = 16*c_re*c_re + 16*c_im*c_im;
    x_re -=  num_re / den;
    x_im -= num_im / den;
    ++iter;

    double delta1_re = x_re - r1_re;
    double delta1_im = x_im - r1_im;
    if ( (delta1_re*delta1_re + delta1_im*delta1_im) < 0.000001 ) { //converged to root1
      *attractor = 0;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double delta2_re = x_re - r2_re;
    double delta2_im = x_im - r2_im;
    if ( (delta2_re*delta2_re + delta2_im*delta2_im) < 0.000001 ) { //converged to root2
      *attractor = 1;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta3_re = x_re - r3_re;
    double delta3_im = x_im - r3_im;
    if ( (delta3_re*delta3_re + delta3_im*delta3_im) < 0.000001 ) { //converged to root3
      *attractor = 2;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta4_re = x_re - r4_re;
    double delta4_im = x_im - r4_im;
    if ( (delta4_re*delta4_re + delta4_im*delta4_im) < 0.000001 ) { //converged to root4
      *attractor = 3;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
  }
  return NULL;
}

static inline void *newton5(double x_re, double x_im,
                            int* attractor, int* convergence)
{
  //Newton routine for polynomial of degree 5
  double r1_re = 1, r1_im = 0, r2_re = 0.309017, r2_im = 0.951057, r3_re = -0.809017, r3_im = 0.587785, r4_re = -0.809017, r4_im = -0.587785, r5_re = 0.309017, r5_im = -0.951057;
  int iter=0, capIter=100;

  while (1) {
    if ( (x_re*x_re + x_im*x_im) < 0.000001 || //closer than 0.001 to origin
                ( fabs(x_re) > 10000000000. || fabs(x_im) > 10000000000. ) ) { //real or imaginary part larger than 10^10
      *attractor = 9;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double a_re=0, a_im=0, b_re=0, b_im=0, d_re=0, d_im=0;
    mul_cpx(&a_re, &a_im, x_re, x_im, x_re, x_im); //x^2
    mul_cpx(&b_re, &b_im, a_re, a_im, a_re, a_im); //x^4
    mul_cpx(&d_re, &d_im, b_re, b_im, x_re, x_im); //x^5
    double num_re = 0, num_im = 0;
    mul_cpx(&num_re, &num_im, d_re - 1, d_im, 5*b_re, -5*b_im);
    double den = 25*b_re*b_re + 25*b_im*b_im;
    x_re -=  num_re / den;
    x_im -= num_im / den;
    ++iter;

    double delta1_re = x_re - r1_re;
    double delta1_im = x_im - r1_im;
    if ( (delta1_re*delta1_re + delta1_im*delta1_im) < 0.000001 ) { //converged to root1
      *attractor = 0;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double delta2_re = x_re - r2_re;
    double delta2_im = x_im - r2_im;
    if ( (delta2_re*delta2_re + delta2_im*delta2_im) < 0.000001 ) { //converged to root2
      *attractor = 1;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta3_re = x_re - r3_re;
    double delta3_im = x_im - r3_im;
    if ( (delta3_re*delta3_re + delta3_im*delta3_im) < 0.000001 ) { //converged to root3
      *attractor = 2;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta4_re = x_re - r4_re;
    double delta4_im = x_im - r4_im;
    if ( (delta4_re*delta4_re + delta4_im*delta4_im) < 0.000001 ) { //converged to root4
      *attractor = 3;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta5_re = x_re - r5_re;
    double delta5_im = x_im - r5_im;
    if ( (delta5_re*delta5_re + delta5_im*delta5_im) < 0.000001 ) { //converged to root5
      *attractor = 4;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
  }
  return NULL;
}

static inline void *newton6(double x_re, double x_im,
                            int* attractor, int* convergence)
{
  //Newton routine for polynomial of degree 6
  double r1_re = 1, r1_im = 0, r2_re = 0.5, r2_im = 0.866025, r3_re = -0.5, r3_im = 0.866025, r4_re = -1, r4_im = 0, r5_re = -0.5, r5_im = -0.866025, r6_re = 0.5, r6_im = -0.866025;
  int iter=0, capIter=100;

  while (1) {
    if ( (x_re*x_re + x_im*x_im) < 0.000001 || //closer than 0.001 to origin
                ( fabs(x_re) > 10000000000. || fabs(x_im) > 10000000000. ) ) { //real or imaginary part larger than 10^10
      *attractor = 9;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double a_re=0, a_im=0, b_re=0, b_im=0, c_re = 0, c_im = 0, d_re=0, d_im=0;
    mul_cpx(&a_re, &a_im, x_re, x_im, x_re, x_im); //x^2
    mul_cpx(&b_re, &b_im, a_re, a_im, x_re, x_im); //x^3
    mul_cpx(&c_re, &c_im, b_re, b_im, a_re, a_im); //x^5
    mul_cpx(&d_re, &d_im, b_re, b_im, b_re, b_im); //x^6
    double num_re = 0, num_im = 0;
    mul_cpx(&num_re, &num_im, d_re - 1, d_im, 6*c_re, -6*c_im);
    double den = 36*c_re*c_re + 36*c_im*c_im;
    x_re -=  num_re / den;
    x_im -= num_im / den;
    ++iter;

    double delta1_re = x_re - r1_re;
    double delta1_im = x_im - r1_im;
    if ( (delta1_re*delta1_re + delta1_im*delta1_im) < 0.000001 ) { //converged to root1
      *attractor = 0;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double delta2_re = x_re - r2_re;
    double delta2_im = x_im - r2_im;
    if ( (delta2_re*delta2_re + delta2_im*delta2_im) < 0.000001 ) { //converged to root2
      *attractor = 1;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta3_re = x_re - r3_re;
    double delta3_im = x_im - r3_im;
    if ( (delta3_re*delta3_re + delta3_im*delta3_im) < 0.000001 ) { //converged to root3
      *attractor = 2;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta4_re = x_re - r4_re;
    double delta4_im = x_im - r4_im;
    if ( (delta4_re*delta4_re + delta4_im*delta4_im) < 0.000001 ) { //converged to root4
      *attractor = 3;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta5_re = x_re - r5_re;
    double delta5_im = x_im - r5_im;
    if ( (delta5_re*delta5_re + delta5_im*delta5_im) < 0.000001 ) { //converged to root5
      *attractor = 4;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta6_re = x_re - r6_re;
    double delta6_im = x_im - r6_im;
    if ( (delta6_re*delta6_re + delta6_im*delta6_im) < 0.000001 ) { //converged to root6
      *attractor = 5;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
  }
  return NULL;
}

static inline void *newton7(double x_re, double x_im,
                            int* attractor, int* convergence)
{
  //Newton routine for polynomial of degree 7
  double r1_re = 1, r1_im = 0, r2_re = 0.62349, r2_im = 0.781831, r3_re = -0.222521, r3_im = 0.974928, r4_re = -0.900969, r4_im = 0.433884, r5_re = -0.900969, r5_im = -0.433884, r6_re = -0.222521, r6_im = -0.974928, r7_re = 0.62349, r7_im = -0.781831;
  int iter=0, capIter=100;

  while (1) {
    if ( (x_re*x_re + x_im*x_im) < 0.000001 || //closer than 0.001 to origin
                ( fabs(x_re) > 10000000000. || fabs(x_im) > 10000000000. ) ) { //real or imaginary part larger than 10^10
      *attractor = 9;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double a_re=0, a_im=0, b_re=0, b_im=0, c_re=0, c_im=0, d_re=0, d_im=0;
    mul_cpx(&a_re, &a_im, x_re, x_im, x_re, x_im); //x^2
    mul_cpx(&b_re, &b_im, a_re, a_im, x_re, x_im); //x^3
    mul_cpx(&c_re, &c_im, b_re, b_im, b_re, b_im); //x^6
    mul_cpx(&d_re, &d_im, c_re, c_im, x_re, x_im); //x^7
    double num_re = 0, num_im = 0;
    mul_cpx(&num_re, &num_im, d_re - 1, d_im, 7*c_re, -7*c_im);
    double den = 49*c_re*c_re + 49*c_im*c_im;
    x_re -=  num_re / den;
    x_im -= num_im / den;
    ++iter;

    double delta1_re = x_re - r1_re;
    double delta1_im = x_im - r1_im;
    if ( (delta1_re*delta1_re + delta1_im*delta1_im) < 0.000001 ) { //converged to root1
      *attractor = 0;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double delta2_re = x_re - r2_re;
    double delta2_im = x_im - r2_im;
    if ( (delta2_re*delta2_re + delta2_im*delta2_im) < 0.000001 ) { //converged to root2
      *attractor = 1;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta3_re = x_re - r3_re;
    double delta3_im = x_im - r3_im;
    if ( (delta3_re*delta3_re + delta3_im*delta3_im) < 0.000001 ) { //converged to root3
      *attractor = 2;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta4_re = x_re - r4_re;
    double delta4_im = x_im - r4_im;
    if ( (delta4_re*delta4_re + delta4_im*delta4_im) < 0.000001 ) { //converged to root4
      *attractor = 3;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta5_re = x_re - r5_re;
    double delta5_im = x_im - r5_im;
    if ( (delta5_re*delta5_re + delta5_im*delta5_im) < 0.000001 ) { //converged to root5
      *attractor = 4;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta6_re = x_re - r6_re;
    double delta6_im = x_im - r6_im;
    if ( (delta6_re*delta6_re + delta6_im*delta6_im) < 0.000001 ) { //converged to root6
      *attractor = 5;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta7_re = x_re - r7_re;
    double delta7_im = x_im - r7_im;
    if ( (delta7_re*delta7_re + delta7_im*delta7_im) < 0.000001 ) { //converged to root7
      *attractor = 6;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
  }
  return NULL;
}

static inline void *newton8(double x_re, double x_im,
                            int* attractor, int* convergence)
{
  //Newton routine for polynomial of degree 8
  double r1_re = 1, r1_im = 0, r2_re = 0.707107, r2_im = 0.707107, r3_re = 0, r3_im = 1, r4_re = -0.707107, r4_im = 0.707107, r5_re = -1, r5_im = 0, r6_re = -0.707107, r6_im = -0.707107, r7_re = 0, r7_im = -1, r8_re = 0.707107, r8_im = -0.707107;
  int iter=0, capIter=100;

  while (1) {
    if ( (x_re*x_re + x_im*x_im) < 0.000001 || //closer than 0.001 to origin
                ( fabs(x_re) > 10000000000. || fabs(x_im) > 10000000000. ) ) { //real or imaginary part larger than 10^10
      *attractor = 9;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double a_re=0, a_im=0, b_re=0, b_im=0, c_re=0, c_im=0, d_re=0, d_im=0, e_re=0, e_im=0;
    mul_cpx(&a_re, &a_im, x_re, x_im, x_re, x_im); //x^2
    mul_cpx(&b_re, &b_im, a_re, a_im, a_re, a_im); //x^4
    mul_cpx(&c_re, &c_im, b_re, b_im, b_re, b_im); //x^8
    mul_cpx(&d_re, &d_im, a_re, a_im, x_re, x_im); //x^3
    mul_cpx(&e_re, &e_im, b_re, b_im, d_re, d_im); //x^7
    double num_re = 0, num_im = 0;
    mul_cpx(&num_re, &num_im, c_re - 1, c_im, 8*e_re, -8*e_im);
    double den = 64*e_re*e_re + 64*e_im*e_im;
    x_re -=  num_re / den;
    x_im -= num_im / den;
    ++iter;

    double delta1_re = x_re - r1_re;
    double delta1_im = x_im - r1_im;
    if ( (delta1_re*delta1_re + delta1_im*delta1_im) < 0.000001 ) { //converged to root1
      *attractor = 0;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double delta2_re = x_re - r2_re;
    double delta2_im = x_im - r2_im;
    if ( (delta2_re*delta2_re + delta2_im*delta2_im) < 0.000001 ) { //converged to root2
      *attractor = 1;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta3_re = x_re - r3_re;
    double delta3_im = x_im - r3_im;
    if ( (delta3_re*delta3_re + delta3_im*delta3_im) < 0.000001 ) { //converged to root3
      *attractor = 2;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta4_re = x_re - r4_re;
    double delta4_im = x_im - r4_im;
    if ( (delta4_re*delta4_re + delta4_im*delta4_im) < 0.000001 ) { //converged to root4
      *attractor = 3;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta5_re = x_re - r5_re;
    double delta5_im = x_im - r5_im;
    if ( (delta5_re*delta5_re + delta5_im*delta5_im) < 0.000001 ) { //converged to root5
      *attractor = 4;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta6_re = x_re - r6_re;
    double delta6_im = x_im - r6_im;
    if ( (delta6_re*delta6_re + delta6_im*delta6_im) < 0.000001 ) { //converged to root6
      *attractor = 5;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double delta7_re = x_re - r7_re;
    double delta7_im = x_im - r7_im;
    if ( (delta7_re*delta7_re + delta7_im*delta7_im) < 0.000001 ) { //converged to root7
      *attractor = 6;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double delta8_re = x_re - r8_re;
    double delta8_im = x_im - r8_im;
    if ( (delta8_re*delta8_re + delta8_im*delta8_im) < 0.000001 ) { //converged to root8
      *attractor = 7;
      if ( iter <= capIter ) {
	*convergence = iter;
	} else {
	  *convergence = capIter;
       }
      break;
    }
  }
  return NULL;
}

static inline void *newton9(double x_re, double x_im,
                            int* attractor, int* convergence)
{
  //Newton routine for polynomial of degree 9
  double r1_re = 1, r1_im = 0, r2_re = 0.766044, r2_im = 0.642788, r3_re = 0.173648, r3_im = 0.984808, r4_re = -0.5, r4_im = 0.866025, r5_re = -0.939693, r5_im = 0.34202, r6_re = -0.939693, r6_im = -0.34202, r7_re = -0.5, r7_im = -0.866025, r8_re = 0.173648, r8_im = -0.984808, r9_re = 0.766044, r9_im = -0.642788;
  int iter=0, capIter=100;

  while (1) {
    if ( (x_re*x_re + x_im*x_im) < 0.000001 || //closer than 0.001 to origin
                ( fabs(x_re) > 10000000000. || fabs(x_im) > 10000000000. ) ) { //real or imaginary part larger than 10^10
      *attractor = 9;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double a_re=0, a_im=0, b_re=0, b_im=0, c_re=0, c_im=0, d_re=0, d_im=0;
    mul_cpx(&a_re, &a_im, x_re, x_im, x_re, x_im); //x^2
    mul_cpx(&b_re, &b_im, a_re, a_im, a_re, a_im); //x^4
    mul_cpx(&c_re, &c_im, b_re, b_im, b_re, b_im); //x^8
    mul_cpx(&d_re, &d_im, c_re, c_im, x_re, x_im); //x^9
    double num_re = 0, num_im = 0;
    mul_cpx(&num_re, &num_im, d_re - 1, d_im, 9*c_re, -9*c_im);
    double den = 81*c_re*c_re + 81*c_im*c_im;
    x_re -=  num_re / den;
    x_im -= num_im / den;
    ++iter;

    double delta1_re = x_re - r1_re;
    double delta1_im = x_im - r1_im;
    if ( (delta1_re*delta1_re + delta1_im*delta1_im) < 0.000001 ) { //converged to root1
      *attractor = 0;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double delta2_re = x_re - r2_re;
    double delta2_im = x_im - r2_im;
    if ( (delta2_re*delta2_re + delta2_im*delta2_im) < 0.000001 ) { //converged to root2
      *attractor = 1;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta3_re = x_re - r3_re;
    double delta3_im = x_im - r3_im;
    if ( (delta3_re*delta3_re + delta3_im*delta3_im) < 0.000001 ) { //converged to root3
      *attractor = 2;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta4_re = x_re - r4_re;
    double delta4_im = x_im - r4_im;
    if ( (delta4_re*delta4_re + delta4_im*delta4_im) < 0.000001 ) { //converged to root4
      *attractor = 3;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta5_re = x_re - r5_re;
    double delta5_im = x_im - r5_im;
    if ( (delta5_re*delta5_re + delta5_im*delta5_im) < 0.000001 ) { //converged to root5
      *attractor = 4;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }

    double delta6_re = x_re - r6_re;
    double delta6_im = x_im - r6_im;
    if ( (delta6_re*delta6_re + delta6_im*delta6_im) < 0.000001 ) { //converged to root6
      *attractor = 5;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double delta7_re = x_re - r7_re;
    double delta7_im = x_im - r7_im;
    if ( (delta7_re*delta7_re + delta7_im*delta7_im) < 0.000001 ) { //converged to root7
      *attractor = 6;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double delta8_re = x_re - r8_re;
    double delta8_im = x_im - r8_im;
    if ( (delta8_re*delta8_re + delta8_im*delta8_im) < 0.000001 ) { //converged to root8
      *attractor = 7;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }
    
    double delta9_re = x_re - r9_re;
    double delta9_im = x_im - r9_im;
    if ( (delta9_re*delta9_re + delta9_im*delta9_im) < 0.000001 ) { //converged to root9
      *attractor = 8;
      if ( iter <= capIter ) {
	*convergence = iter;
      } else {
	*convergence = capIter;
      }
      break;
    }    
  }
  return NULL;
}

#endif
