void mul_cpx(double* a_re, double* a_im,
             double b_re, double b_im,
             double c_re, double c_im)
{
  *a_re = b_re*c_re - b_im*c_im;
  *a_im = b_re*c_im + b_im*c_re;
}



void newton1(double x_im, double x_re,
             int* attractor, int* convergence)
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



void newton2(double x_im, double x_re,
             int* attractor, int* convergence)
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



void newton3(double x_im, double x_re,
             int* attractor, int* convergence)
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
