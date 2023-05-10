#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef PRECISION real;

inline int idx(const int i, const int j, const int ny) {return j + i*ny;}

real* make_array(int nx, int ny) {
  return malloc(nx*ny*sizeof(real));
}

void run_jacobi(real *p, const real* b, const real dx, const real dy, const int nx, const int ny, const int max_iterations) {
  const real D = 2.0*(dx*dx + dy*dy);
  const real D_x = dy*dy/D;
  const real D_y = dx*dx/D;
  const real B = -(dx*dx*dy*dy)/D;

  real* p_new = make_array(nx, ny);
  for(int iter = 0; iter<max_iterations; ++iter) {
    for(int i=1; i<nx-1; ++i) {
      for(int j=1; j<ny-1; ++j) {
        p_new[idx(i,j,ny)] = D_x*(p[idx(i+1,j,ny)] + p[idx(i-1,j,ny)]) + D_y*(p[idx(i,j+1,ny)] + p[idx(i,j-1,ny)]) + B*b[idx(i,j,ny)];
      }
    }
    real* temp = p_new;
    p_new = p;
    p = temp;
  }
  free(p_new);
}

int main(int argc, char* argv[]) {
  const int nx = 128;
  const int ny = 128;
  const int max_iterations = 1<<16;

  real* p = make_array(nx, ny);
  real* b = make_array(nx, ny);
  real* p_soln = make_array(nx, ny);

  real dx = 1.0/(nx-1);
  real dy = 1.0/(ny-1);

  for(int i=0; i<nx; ++i) {
    for(int j=0; j<ny; ++j) {
      real x = i*dx;
      real y = j*dx;

      b[idx(i,j,ny)] = sin(M_PI*x)*sin(M_PI*y);
      p_soln[idx(i,j,ny)] = -sin(M_PI*x)*sin(M_PI*y)/(2.0*M_PI*M_PI);
      p[idx(i,j,ny)] = 0.0;
    }
  }

  clock_t start = clock();
  run_jacobi(p, b, dx, dy, nx, ny, max_iterations);
  clock_t diff = clock() - start;

  int msec = diff * 1000 / CLOCKS_PER_SEC;

  real av_error = 0.0;
  for(int i=1; i<nx-1; ++i) {
    for(int j=1; j<ny-1; ++j) {
      av_error += fabs(p[idx(i,j,ny)] - p_soln[idx(i,j,ny)]);
    }
  }
  av_error /= (nx*ny);

  printf("%s, c, %d, %d, %d, %d, %e\n", argv[0], nx, ny, max_iterations, msec, av_error);

  free(p);
  free(b);
  free(p_soln);

  (void)argc; // Disable compiler warnings for argc
  return 0;
}
