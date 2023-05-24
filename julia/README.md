# Julia Logbook

## Major takeaways

- Use appropriate looping indices in multidimensional arrays (first index moves fastest)
- Factor everything into functions
- Defining constants as global literals doesn't affect performance
- Bounds-checking of arrays is *incredibly* slow - turn it off after correctness is checked!

### V001: Original version

This version is ridiculously slow:

```
v001_original.jl, 128, 128, 65536, 2702.130189, 1.0305793014655375e-6
```

### V002: Refactoring out Jacobi iteration

Does refactoring Jacobi into its own function and running it once to let the JIT compile it help? yes! But not an awful lot.

```
v001_original.jl,                     Julia,  128,  128,  65536,  2612.1193310000003,  1.0305793014655375e-6
v002_refactor_jacobi_to_function.jl,  Julia,  128,  128,  65536,  2237.3760589999997,  1.0306187939893605e-6
```

### V003: Typing global constants

Let's fix the type of NX, NY and MAX_ITERATIONS. Possibly helps:

```
v001_original.jl,                     Julia,  128,  128,  65536,  2630.959069,         1.0305793014655375e-6
v002_refactor_jacobi_to_function.jl,  Julia,  128,  128,  65536,  2248.6089979999997,  1.0306187939893605e-6
v003_type_global_constants.jl,        Julia,  128,  128,  65536,  2182.61585,          1.0305793014655375e-6
```

### V004: Switching indices

Julia stores arrays as in fortran with the first index moving fastest. All loops are such that the innermost loop is the fastest moving to great effect:

```
v001_original.jl,                     Julia,  128,  128,  65536,  2625.970488,  1.0305793014655375e-6
v002_refactor_jacobi_to_function.jl,  Julia,  128,  128,  65536,  2237.83113,   1.0306187939893605e-6
v003_type_global_constants.jl,        Julia,  128,  128,  65536,  2195.094627,  1.0305793014655375e-6
v004_switch_indices.jl,               Julia,  128,  128,  65536,  1650.767577,  1.0305793014655375e-6
```

### V005: Disabling bounds-checking

Removing bounds checking with `@inbounds` on only the inner loop portion is very productive:

```
v001_original.jl,                     Julia,  128,  128,  65536,  2711.474613,         1.0305793014655375e-6
v002_refactor_jacobi_to_function.jl,  Julia,  128,  128,  65536,  2254.923469,         1.0306187939893605e-6
v003_type_global_constants.jl,        Julia,  128,  128,  65536,  2281.174493,         1.0305793014655375e-6
v004_switch_indices.jl,               Julia,  128,  128,  65536,  1704.7704509999999,  1.0305793014655375e-6
v005_inbounds.jl,                     Julia,  128,  128,  65536,  349.528976,          1.0305793014655375e-6
```

### V006: Enabling SIMD instructions

Doesn't do much in this benchmark:

```
v001_original.jl,                     Julia,  128,  128,  65536,  2705.138939,         1.0305793014655375e-6
v002_refactor_jacobi_to_function.jl,  Julia,  128,  128,  65536,  2315.10274,          1.0306187939893605e-6
v003_type_global_constants.jl,        Julia,  128,  128,  65536,  2327.0211820000004,  1.0305793014655375e-6
v004_switch_indices.jl,               Julia,  128,  128,  65536,  1696.974446,         1.0305793014655375e-6
v005_inbounds.jl,                     Julia,  128,  128,  65536,  355.453746,          1.0305793014655375e-6
v006_simd.jl,                         Julia,  128,  128,  65536,  355.519677,          1.0305793014655375e-6
```
