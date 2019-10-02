#ifndef _H_NEWTON // once-only header
#define _H_NEWTON

void mul_cpx(double* a_re, double* a_im,
             double b_re, double b_im,
             double c_re, double c_im);

void *newton1(double x_im, double x_re,
              int* attractor, int* convergence);

void *newton2(double x_im, double x_re,
              int* attractor, int* convergence);

void *newton3(double x_im, double x_re,
              int* attractor, int* convergence);

void *newton4(double x_im, double x_re,
              int* attractor, int* convergence);

void *newton5(double x_im, double x_re,
              int* attractor, int* convergence);

void *newton6(double x_im, double x_re,
              int* attractor, int* convergence);

void *newton7(double x_im, double x_re,
              int* attractor, int* convergence);

void *newton8(double x_im, double x_re,
              int* attractor, int* convergence);

void *newton9(double x_im, double x_re,
              int* attractor, int* convergence);

#endif
#ifndef _H_COMP_BLOCK // once-only header
#define _H_COMP_BLOCK

void *compute_block( void *restrict arg );

#endif
#ifndef _H_WRITE_BLOCK // once-only header
#define _H_WRITE_BLOCK

void *write_block( void *restrict arg );

#endif
