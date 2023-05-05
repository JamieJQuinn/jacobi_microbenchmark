Idea: Create a number of microbenchmarks testing assumptions about scientific software

## Microbenchmarks

- Effect of packaging data + params in a single structure
- C vs C++ vs julia vs Python vs Numba vs Zig vs Cupy vs CUDA
- Effect of noalias
- Effect of using methods over functions
- Effect of hardcoding variables
- Effect of passing vs global variables for e.g. numerical parameters
- Effect of const ref vs value for small parameter passing
- Effect of calculating index for every data point vs using inbuilt 2D arrays
  - Effect of ghost points in index calc
  - Effect of incorrect data order
  - Effect of inlining
  - Inline index fn vs #define
- Effect of Dirichlet bcs, von Neumann bcs and periodic bcs
- Effect of precalculating variables
- Effect of putting calculations in functions
- Effect of const ref vs just ref on parameter passing
- Allocating temp space outside or inside a function
- Effect of 2D domain shape, e.g. same number of elements, different arrangements. Can is disrupt the cache?
- Jacobi vs Gauss Seidel (i.e. do we write to new array or rewrite old array)
- double vs float on CPUs
- Effect of -ffast-math
- Effect of -O 0,1,2,3 & fast math
- Effect of -march=native

## Test problem

Simple Jacobi solver for the Poisson eqn.

Equation $\nabla^2 u = -\sin(\pi x) \sin(\pi y)$ with the boundary set to zero should give solution $u = \sin (\pi x) \sin(\pi y)/(2\pi^2)$.

Discretising over a rectangular grid with grid spacing $\Delta x$ and $\Delta y$ gives

$\frac{p_{i-1,j} - 2p_{i,j} + p_{i+1,j}}{\Delta x^2} + \frac{p_{i,j-1} - 2p_{i,j} + p_{i,j+1}}{\Delta y^2} = b_{i,j}$,
where $b_{i,j} = \sin(\pi x_i) \sin(\pi y_j)$.

Then, rearranging for $p_{i,j}$ gives our iterative equation:

$p_{i,j} = D_x (p_{i+1,j} + p_{i-1,j}) + D_y (p_{i,j+1} + p_{i,j-1}) + B b_{i,j},$
where $D_x = \frac{\Delta y^2}{D}$, $D_y = \frac{\Delta x^2}{D}$, $B = -\frac{\Delta x^2 \Delta y^2}{D}$ and $D = 2(\Delta x^2 + \Delta y^2)$.

## Method

The code should be written initially in C in such a way that it is both reasonably readable and reasonably performant. To find a baseline for the more extensive tests, the code will be modified into a new version and the performance measured. If it is better than the current reference version, it will replace the reference.

## Logbook

### V001: Compiler optimisations

The same reference code was used to test 5 levels of compiler optimisations and `-march`. These tests can be optionally built with `make vary_flags` and found as executables `<name>_<optimisations>.x`.

On my machine[^my_machine] the results of a single run were:

```
./v001_original_O3_native.x,     128,  128,  65536,  283,   1.030579e-06
./v001_original_Ofast_native.x,  128,  128,  65536,  289,   1.030579e-06
./v001_original.x,               128,  128,  65536,  290,   1.030579e-06
./v001_original_O3.x,            128,  128,  65536,  440,   1.030579e-06
./v001_original_O1.x,            128,  128,  65536,  758,   1.030579e-06
./v001_original_O2.x,            128,  128,  65536,  759,   1.030579e-06
./v001_original_O0.x,            128,  128,  65536,  3620,  1.030579e-06
```

### V002: Removing macro index

The initial code was written with the index calculation as a macro. Moving this to a regular function results in a no slower time with optimisations. Notably, however, when *no* optimisations are enabled, the function is far slower. I suspect it may be inlining the function automatically due to its length.

```
./v002_no_macro_index.x,               128,  128,  65536,  280,    1.030579e-06
./v001_original_O3_native.x,           128,  128,  65536,  284,    1.030579e-06
./v002_no_macro_index_O3_native.x,     128,  128,  65536,  286,    1.030579e-06
./v001_original_Ofast_native.x,        128,  128,  65536,  289,    1.030579e-06
./v001_original.x,                     128,  128,  65536,  291,    1.030579e-06
./v002_no_macro_index_Ofast_native.x,  128,  128,  65536,  291,    1.030579e-06
./v002_no_macro_index_O3.x,            128,  128,  65536,  434,    1.030579e-06
./v001_original_O3.x,                  128,  128,  65536,  444,    1.030579e-06
./v002_no_macro_index_O2.x,            128,  128,  65536,  756,    1.030579e-06
./v002_no_macro_index_O1.x,            128,  128,  65536,  767,    1.030579e-06
./v001_original_O2.x,                  128,  128,  65536,  770,    1.030579e-06
./v001_original_O1.x,                  128,  128,  65536,  772,    1.030579e-06
./v001_original_O0.x,                  128,  128,  65536,  3605,   1.030579e-06
./v002_no_macro_index_O0.x,            128,  128,  65536,  10509,  1.030579e-06
```

### V003: Inline index function

Inlining the index function does little when optimisations are enabled but, interestingly, the linker cannot find the inlined function when using `-O0`. This is particular to C and not an issue in C++. Removing earlier compiler flag variations the current data is:

```
./v003_inline_index_Ofast_native.x,  128,  128,  65536,  284,  1.030579e-06
./v001_original.x,                   128,  128,  65536,  285,  1.030579e-06
./v003_inline_index_O3_native.x,     128,  128,  65536,  286,  1.030579e-06
./v002_no_macro_index.x,             128,  128,  65536,  287,  1.030579e-06
./v003_inline_index.x,               128,  128,  65536,  315,  1.030579e-06
./v003_inline_index_O3.x,            128,  128,  65536,  441,  1.030579e-06
./v003_inline_index_O2.x,            128,  128,  65536,  774,  1.030579e-06
./v003_inline_index_O1.x,            128,  128,  65536,  784,  1.030579e-06
```
