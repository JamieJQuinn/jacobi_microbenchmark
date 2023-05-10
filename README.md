## Microbenchmarks

- C vs C++ vs julia vs Python vs Numba vs Zig vs Cupy vs CUDA vs Fortran
- Effect of restrict
- Effect of using methods over functions
- [x] Effect of packaging data + params in a single structure
- [x] Effect of hardcoding variables
- [x] Effect of passing vs global variables for e.g. numerical parameters
- [x] Effect of const ref vs value for small parameter passing
- [x] Effect of calculating index for every data point vs using inbuilt 2D arrays
- [x] Effect of ghost points in index calc
- [x] Effect of inlining
- [x] Effect of Dirichlet bcs, von Neumann bcs and periodic bcs
- [x] Effect of precalculating variables
- [x] Effect of putting calculations in functions
- [x] Effect of const ref vs just ref on parameter passing
- [x] Allocating temp space outside or inside a function
- [x] Effect of 2D domain shape, e.g. same number of elements, different arrangements. Can is disrupt the cache?
- [x] Jacobi vs Gauss Seidel (i.e. do we write to new array or rewrite old array)
- [x] double vs float on CPUs
- [x] Effect of -ffast-math
- [x] Effect of -O 0,1,2,3 & fast math
- [x] Effect of -march=native

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

The code should be written initially in C in such a way that it is both reasonably readable and reasonably performant. Various modifications will be made to the code in different versions. Some modifications are one off for that version, some continue into proceeding versions. 

The code will then be translated into different languages and optimised in each.
