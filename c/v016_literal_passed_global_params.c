#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef PRECISION real;

const int NX = 128;
const int NY = 128;
const int MAX_ITERATIONS = 1<<16;

const real DX = 1.0/(NX-1);
const real DY = 1.0/(NY-1);

const real D = 2.0*(DX*DX + DY*DY);
const real D_x = DY*DY/D;
const real D_y = DX*DX/D;
const real B = -(DX*DX*DY*DY)/D;

inline int idx(const int i, const int j) {return j + i*NY;}

real* make_array(int NX, int NY) {
  return malloc(NX*NY*sizeof(real));
}

void run_jacobi(real *p, const real* b) {
  real* p_new = make_array(NX, NY);
  for(int iter = 0; iter<MAX_ITERATIONS; ++iter) {
    for(int i=1; i<NX-1; ++i) {
      for(int j=1; j<NY-1; ++j) {
        p_new[idx(i,j)] = D_x*(p[idx(i+1,j)] + p[idx(i-1,j)]) + D_y*(p[idx(i,j+1)] + p[idx(i,j-1)]) + B*b[idx(i,j)];
      }
    }
    real* temp = p_new;
    p_new = p;
    p = temp;
  }
  free(p_new);
}

int main(int argc, char* argv[]) {
  real* p = make_array(NX, NY);
  real* b = make_array(NX, NY);
  real* p_soln = make_array(NX, NY);

  for(int i=0; i<NX; ++i) {
    for(int j=0; j<NY; ++j) {
      real x = i*DX;
      real y = j*DY;

      b[idx(i,j)] = sin(M_PI*x)*sin(M_PI*y);
      p_soln[idx(i,j)] = -sin(M_PI*x)*sin(M_PI*y)/(2.0*M_PI*M_PI);
      p[idx(i,j)] = 0.0;
    }
  }

  clock_t start = clock();
  run_jacobi(p, b);
  clock_t diff = clock() - start;

  int msec = diff * 1000 / CLOCKS_PER_SEC;

  real av_error = 0.0;
  for(int i=1; i<NX-1; ++i) {
    for(int j=1; j<NY-1; ++j) {
      av_error += fabs(p[idx(i,j)] - p_soln[idx(i,j)]);
    }
  }
  av_error /= (NX*NY);

  printf("exe name, NX, NY, MAX_ITERATIONS, Runtime, Average Error\n");
  printf("%s, %d, %d, %d, %d, %e\n", argv[0], NX, NY, MAX_ITERATIONS, msec, av_error);

  free(p);
  free(b);
  free(p_soln);

  (void)argc; // Disable compiler warnings for argc
  return 0;
}
