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
- double vs float on CPUs
- Effect of -O 0,1,2,3 & fast math
- Effect of -march=native
- Allocating temp space outside or inside a function
- Effect of 2D domain shape, e.g. same number of elements, different arrangements. Can is disrupt the cache?
- Jacobi vs Gauss Seidel (i.e. do we write to new array or rewrite old array)

## Test problem

Simple Jacobi solver for the Poisson eqn.

Equation $\nabla^2 u = -\sin(\pi x) \sin(\pi y)$ with the boundary set to zero should give solution $u = \sin (\pi x) \sin(\pi y)/(2\pi^2)$.

Discretising over a rectangular grid with grid spacing $\Delta x$ and $\Delta y$ gives

$\frac{p_{i-1,j} - 2p_{i,j} + p_{i+1,j}}{\Delta x^2} + \frac{p_{i,j-1} - 2p_{i,j} + p_{i,j+1}}{\Delta y^2} = b_{i,j}$,
where $b_{i,j} = \sin(\pi x_i) \sin(\pi y_j)$.

Then, rearranging for $p_{i,j}$ gives our iterative equation:

$p_{i,j} = D_x (p_{i+1,j} + p_{i-1,j}) + D_y (p_{i,j+1} + p_{i,j-1}) + B b_{i,j},$
where $D_x = \frac{\Delta y^2}{D}$, $D_y = \frac{\Delta x^2}{D}$, $B = -\frac{\Delta x^2 \Delta y^2}{D}$ and $D = 2(\Delta x^2 + \Delta y^2)$.
