#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>

using std::vector;

typedef PRECISION real;

const int NX = 128;
const int NY = 128;
const int MAX_ITERATIONS = 1<<16;

class Array {
  public:
  Array(int nx_in, int ny_in) :
    nx{nx_in}, ny{ny_in},
    data(nx_in*ny_in)
  {}
  const real& operator()(const int i, const int j) const {return data[idx(i,j)];}
  real& operator()(const int i, const int j) {return data[idx(i,j)];}
  int idx(int i, int j) const {return j + i*ny;}

  int nx;
  int ny;
  private:
    vector<real> data;
};

void run_jacobi(Array& p, const Array& b, const real dx, const real dy) {
  real D = 2.0*(dx*dx + dy*dy);
  real D_x = dy*dy/D;
  real D_y = dx*dx/D;
  real B = -(dx*dx*dy*dy)/D;

  Array p_new(p.nx,p.ny);
  for(int iter = 0; iter<MAX_ITERATIONS; ++iter) {
    for(int i=1; i<NX-1; ++i) {
      for(int j=1; j<NY-1; ++j) {
        p_new(i,j) = D_x*(p(i+1,j) + p(i-1,j)) + D_y*(p(i,j+1) + p(i,j-1)) + B*b(i,j);
      }
    }
    std::swap(p, p_new);
  }
}

int main(int argc, char* argv[]) {
  Array p(NX, NY);
  Array b(NX, NY);
  Array p_soln(NX, NY);

  real dx = 1.0/(NX-1);
  real dy = 1.0/(NY-1);

  for(int i=0; i<NX; ++i) {
    for(int j=0; j<NY; ++j) {
      real x = i*dx;
      real y = j*dx;

      b(i,j) = sin(M_PI*x)*sin(M_PI*y);
      p_soln(i,j) = -sin(M_PI*x)*sin(M_PI*y)/(2.0*M_PI*M_PI);
      p(i,j) = 0.0;
    }
  }

  clock_t start = clock();
  run_jacobi(p, b, dx, dy);
  clock_t diff = clock() - start;

  int msec = diff * 1000 / CLOCKS_PER_SEC;

  real av_error = 0.0;
  for(int i=1; i<NX-1; ++i) {
    for(int j=1; j<NY-1; ++j) {
      av_error += fabs(p(i,j) - p_soln(i,j));
    }
  }
  av_error /= (NX*NY);

  printf("%s, cpp, %d, %d, %d, %d, %e\n", argv[0], NX, NY, MAX_ITERATIONS, msec, av_error);

  (void)argc; // Disable compiler warnings for argc
  return 0;
}
