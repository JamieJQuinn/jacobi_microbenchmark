#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef PRECISION real;

const int NX = 128;
const int NY = 128;
const int MAX_ITERATIONS = 1<<16;

inline int idx(int i, int j) {return j + i*NY;}

real* make_array(int nx, int ny) {
  return new real[nx*ny];
}

void run_jacobi(real *p, const real* b, const real dx, const real dy) {
  real D = 2.0*(dx*dx + dy*dy);
  real D_x = dy*dy/D;
  real D_y = dx*dx/D;
  real B = -(dx*dx*dy*dy)/D;

  real* p_new = make_array(NX, NY);
  for(int iter = 0; iter<MAX_ITERATIONS; ++iter) {
    for(int i=1; i<NX-1; ++i) {
      for(int j=1; j<NX-1; ++j) {
        p_new[idx(i,j)] = D_x*(p[idx(i+1,j)] + p[idx(i-1,j)]) + D_y*(p[idx(i,j+1)] + p[idx(i,j-1)]) + B*b[idx(i,j)];
      }
    }
    real* temp = p_new;
    p_new = p;
    p = temp;
  }
  delete[] p_new;
}

int main(int argc, char* argv[]) {
  real* p = make_array(NX, NY);
  real* b = make_array(NX, NY);
  real* p_soln = make_array(NX, NY);

  real dx = 1.0/(NX-1);
  real dy = 1.0/(NY-1);

  for(int i=0; i<NX; ++i) {
    for(int j=0; j<NY; ++j) {
      real x = i*dx;
      real y = j*dx;

      b[idx(i,j)] = sin(M_PI*x)*sin(M_PI*y);
      p_soln[idx(i,j)] = -sin(M_PI*x)*sin(M_PI*y)/(2.0*M_PI*M_PI);
      p[idx(i,j)] = 0.0;
    }
  }

  clock_t start = clock();
  run_jacobi(p, b, dx, dy);
  clock_t diff = clock() - start;

  int msec = diff * 1000 / CLOCKS_PER_SEC;

  real av_error = 0.0;
  for(int i=1; i<NX-1; ++i) {
    for(int j=1; j<NY-1; ++j) {
      av_error += fabs(p[idx(i,j)] - p_soln[idx(i,j)]);
    }
  }
  av_error /= (NX*NY);

  printf("%s, cpp, %d, %d, %d, %d, %e\n", argv[0], NX, NY, MAX_ITERATIONS, msec, av_error);

  delete[] p;
  delete[] b;
  delete[] p_soln;

  (void)argc; // Disable compiler warnings for argc
  return 0;
}
