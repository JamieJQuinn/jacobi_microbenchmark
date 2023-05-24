#include <memory>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>

using std::make_unique;

typedef PRECISION real;

inline int idx(const int i, const int j, const int ny) {return j + i*ny;}

void run_jacobi(std::unique_ptr<real[]>& p, const std::unique_ptr<real[]>& b, const real dx, const real dy, const int nx, const int ny, const int max_iterations) {
  real D = 2.0*(dx*dx + dy*dy);
  real D_x = dy*dy/D;
  real D_y = dx*dx/D;
  real B = -(dx*dx*dy*dy)/D;

  auto p_new = make_unique<real[]>(nx*ny);
  for(int iter = 0; iter<max_iterations; ++iter) {
    for(int i=1; i<nx-1; ++i) {
      for(int j=1; j<ny-1; ++j) {
        p_new[idx(i,j,ny)] = D_x*(p[idx(i+1,j,ny)] + p[idx(i-1,j,ny)]) + D_y*(p[idx(i,j+1,ny)] + p[idx(i,j-1,ny)]) + B*b[idx(i,j,ny)];
      }
    }
    std::swap(p, p_new);
  }
}

int main(int argc, char* argv[]) {
  const int NX = 128;
  const int NY = 128;
  const int MAX_ITERATIONS = 1<<16;

  auto p = make_unique<real[]>(NX*NY);
  auto b = make_unique<real[]>(NX*NY);
  auto p_soln = make_unique<real[]>(NX*NY);

  real dx = 1.0/(NX-1);
  real dy = 1.0/(NY-1);

  for(int i=0; i<NX; ++i) {
    for(int j=0; j<NY; ++j) {
      real x = i*dx;
      real y = j*dx;

      b[idx(i,j,NY)] = sin(M_PI*x)*sin(M_PI*y);
      p_soln[idx(i,j,NY)] = -sin(M_PI*x)*sin(M_PI*y)/(2.0*M_PI*M_PI);
      p[idx(i,j,NY)] = 0.0;
    }
  }

  clock_t start = clock();
  run_jacobi(p, b, dx, dy, NX, NY, MAX_ITERATIONS);
  clock_t diff = clock() - start;

  int msec = diff * 1000 / CLOCKS_PER_SEC;

  real av_error = 0.0;
  for(int i=1; i<NX-1; ++i) {
    for(int j=1; j<NY-1; ++j) {
      av_error += fabs(p[idx(i,j,NY)] - p_soln[idx(i,j,NY)]);
    }
  }
  av_error /= (NX*NY);

  printf("%s, cpp, %d, %d, %d, %d, %e\n", argv[0], NX, NY, MAX_ITERATIONS, msec, av_error);

  (void)argc; // Disable compiler warnings for argc
  return 0;
}
