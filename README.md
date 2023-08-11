## Test problem

The algorithm is a (relatively) simple Jacobi solver for the Poisson equation,

$$\nabla^2 u = -\sin(\pi x) \sin(\pi y),$$

over the domain $(x, y) \in [0,1]\times[0,1]$ with $u(x,y) = 0$ on each boundary. This has analytical solution,

$$u_{analytical}(x,y) = \sin (\pi x) \sin(\pi y)/(2\pi^2).$$

Discretising over a regular, rectangular grid with constant grid spacing $\Delta x$ and $\Delta y$ gives

$$\frac{p_{i-1,j} - 2p_{i,j} + p_{i+1,j}}{\Delta x^2} + \frac{p_{i,j-1} - 2p_{i,j} + p_{i,j+1}}{\Delta y^2} = b_{i,j},$$

where $b_{i,j} = \sin(\pi x_i) \sin(\pi y_j),$ $x_i = i\Delta x$ and $y_j = j\Delta y$. Rearranging for $p_{i,j}$ gives the iterative equation suitable for the Jacobi method:

$$p_{i,j} = D_x (p_{i+1,j} + p_{i-1,j}) + D_y (p_{i,j+1} + p_{i,j-1}) + B b_{i,j},$$

where $D_x = \frac{\Delta y^2}{D}$, $D_y = \frac{\Delta x^2}{D}$, $B = -\frac{\Delta x^2 \Delta y^2}{D}$ and $D = 2(\Delta x^2 + \Delta y^2)$.

## What is the point?

I want to be able to answer the general questions:

- What performance do I give up by using a higher level language than C?
- How easy is it to optimise modern languages like Python, Julia and Zig?
- What cost do "zero-cost" abstractions really have in C++?

General questions about scientific programs:

- Should I compile my constants into the code or pass them as data?
- Is it worth testing using just 32-bit floats?
- Is it worth experimenting with `-ffast-math`?

Along with questions specific to grid-based codes:

- What effect does using ghost points have on performance?
- Should I use static or dynamic arrays?
- Are built-in array structures just as performant as raw c-like arrays?

## Targeted Languages

- [x] C
- [x] C++
- [ ] C++ OpenMP
- [ ] C++ OpenACC
- [ ] C++ CUDA
- [x] Python
- [ ] CUDA Python
- [ ] DaCe Python
- [x] Julia
- [ ] Julia ParallelStencils.jl
- [ ] Fortran
- [ ] Zig
- [ ] WebGPU
