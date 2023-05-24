# C Logbook

## Major Takeaways

- Use `-O3` and `-march=native`
- Use floats if possible
- Total domain size *when reading data* is important, seems like they should be a power of 2 (including ghost points if using)
- Ghost points do not affect performance
- The compiler can probably automatically inline
- Static arrays are a tiny bit faster, but possibly more hassle than it's worth
- Defining parameters like domain size as constant, global literals impacts performance a surprising amount (~10%)
- You probably don't need to precompute constants
- Allocating and deallocating temporary space inside a function is probably fine for functions that run for long enough

## Versions

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

### V004: double vs float

Two interesting things happen when we move from using double precision as in previous experiments to float:

- We get a significant speedup with most optimisation levels
- The overall error decreases

```
./v003_inline_index_O3_native.x,     128,  128,  65536,  152,  4.362253e-07
./v001_original.x,                   128,  128,  65536,  153,  4.362253e-07
./v003_inline_index_Ofast_native.x,  128,  128,  65536,  155,  4.362251e-07
./v002_no_macro_index.x,             128,  128,  65536,  156,  4.362253e-07
./v003_inline_index.x,               128,  128,  65536,  159,  4.362253e-07
./v003_inline_index_O3.x,            128,  128,  65536,  239,  4.362253e-07
./v003_inline_index_O1.x,            128,  128,  65536,  665,  4.362253e-07
./v003_inline_index_O2.x,            128,  128,  65536,  702,  4.362253e-07
```

Let us continue using doubles for now.

### V005: Not precomputing constants

There is general wisdom in precomputing constants but is the compiler clever enough to precompute them itself? It seems like it:

```
./v005_no_precompute_constants_Ofast_native.x,  128,  128,  65536,  283,  1.030579e-06
./v003_inline_index.x,                          128,  128,  65536,  286,  1.030579e-06
./v005_no_precompute_constants_O3_native.x,     128,  128,  65536,  286,  1.030579e-06
./v002_no_macro_index.x,                        128,  128,  65536,  287,  1.030579e-06
./v001_original.x,                              128,  128,  65536,  289,  1.030579e-06
./v005_no_precompute_constants.x,               128,  128,  65536,  290,  1.030579e-06
./v005_no_precompute_constants_O3.x,            128,  128,  65536,  441,  1.030579e-06
./v005_no_precompute_constants_O1.x,            128,  128,  65536,  759,  1.030579e-06
./v005_no_precompute_constants_O2.x,            128,  128,  65536,  768,  1.030579e-06
```

Let us precompute constants for improved readability.

### V006: Should NX or NY be a power of 2?

We can change NX and NY to be 128+1 each. This doesn't seem to affect the results, the fastest still being 280-290 ms:

```
./v006_domain_size.x,               129,  129,  65536,  281,  1.014649e-06
./v001_original.x,                  128,  128,  65536,  283,  1.030579e-06
./v003_inline_index.x,              128,  128,  65536,  289,  1.030579e-06
./v005_no_precompute_constants.x,   128,  128,  65536,  289,  1.030579e-06
./v006_domain_size_O3_native.x,     129,  129,  65536,  289,  1.014649e-06
./v006_domain_size_Ofast_native.x,  129,  129,  65536,  290,  1.014649e-06
./v002_no_macro_index.x,            128,  128,  65536,  295,  1.030579e-06
./v006_domain_size_O3.x,            129,  129,  65536,  455,  1.014649e-06
./v006_domain_size_O2.x,            129,  129,  65536,  766,  1.014649e-06
./v006_domain_size_O1.x,            129,  129,  65536,  798,  1.014649e-06
```

What about just making the interior a power of 2, i.e. NX = NY = 128+2. Now *this* notably and negatively affects the results:

```
./v002_no_macro_index.x,            128,  128,  65536,  287,  1.030579e-06
./v005_no_precompute_constants.x,   128,  128,  65536,  287,  1.030579e-06
./v001_original.x,                  128,  128,  65536,  289,  1.030579e-06
./v003_inline_index.x,              128,  128,  65536,  291,  1.030579e-06
./v006_domain_size_Ofast_native.x,  130,  130,  65536,  309,  9.990804e-07
./v006_domain_size.x,               130,  130,  65536,  309,  9.990804e-07
./v006_domain_size_O3_native.x,     130,  130,  65536,  316,  9.990804e-07
./v006_domain_size_O3.x,            130,  130,  65536,  469,  9.990804e-07
./v006_domain_size_O1.x,            130,  130,  65536,  799,  9.990804e-07
./v006_domain_size_O2.x,            130,  130,  65536,  809,  9.990804e-07
```

What about NX = NY = 128-2? Again worse:

```
./v002_no_macro_index.x,            128,  128,  65536,  284,  1.030579e-06
./v001_original.x,                  128,  128,  65536,  289,  1.030579e-06
./v005_no_precompute_constants.x,   128,  128,  65536,  289,  1.030579e-06
./v003_inline_index.x,              128,  128,  65536,  292,  1.030579e-06
./v006_domain_size.x,               126,  126,  65536,  304,  1.063573e-06
./v006_domain_size_Ofast_native.x,  126,  126,  65536,  311,  1.063573e-06
./v006_domain_size_O3_native.x,     126,  126,  65536,  315,  1.063573e-06
./v006_domain_size_O3.x,            126,  126,  65536,  443,  1.063573e-06
./v006_domain_size_O1.x,            126,  126,  65536,  681,  1.063573e-06
./v006_domain_size_O2.x,            126,  126,  65536,  693,  1.063573e-06
```

So NX and NY should be powers of 2. What if we half NX and double NY? Possibly better results:

```
./v006_domain_size_Ofast_native.x,  64,   256,  65536,  274,  1.756221e-02
./v006_domain_size.x,               64,   256,  65536,  277,  1.756221e-02
./v003_inline_index.x,              128,  128,  65536,  281,  1.030579e-06
./v002_no_macro_index.x,            128,  128,  65536,  284,  1.030579e-06
./v006_domain_size_O3_native.x,     64,   256,  65536,  285,  1.756221e-02
./v001_original.x,                  128,  128,  65536,  291,  1.030579e-06
./v005_no_precompute_constants.x,   128,  128,  65536,  293,  1.030579e-06
./v006_domain_size_O3.x,            64,   256,  65536,  445,  1.756221e-02
./v006_domain_size_O2.x,            64,   256,  65536,  744,  1.756221e-02
./v006_domain_size_O1.x,            64,   256,  65536,  757,  1.756221e-02
```

While of course the accuracy of the solution suffers, making the domain much wider than it is tall improves the runtime dramatically, down to NX=4, NY = 4096:

```
./v006_domain_size_O3_native.x,     4,    4096,  65536,  160,  1.266204e-02
./v006_domain_size.x,               4,    4096,  65536,  161,  1.266204e-02
./v006_domain_size_Ofast_native.x,  4,    4096,  65536,  162,  1.266204e-02
./v006_domain_size_O3.x,            4,    4096,  65536,  225,  1.266204e-02
./v002_no_macro_index.x,            128,  128,   65536,  285,  1.030579e-06
./v003_inline_index.x,              128,  128,   65536,  285,  1.030579e-06
./v001_original.x,                  128,  128,   65536,  290,  1.030579e-06
./v005_no_precompute_constants.x,   128,  128,   65536,  292,  1.030579e-06
./v006_domain_size_O2.x,            4,    4096,  65536,  372,  1.266204e-02
./v006_domain_size_O1.x,            4,    4096,  65536,  377,  1.266204e-02
```

Notice we are processing the same number of points, just over a wider domain.

Performing the opposite test, the runtime for a large NX but small NY is about the same as the recorded runtime for the square array:

```
./v006_domain_size_O3_native.x,     1024,  16,   65536,  273,  3.775647e-04
./v006_domain_size_Ofast_native.x,  1024,  16,   65536,  276,  3.775647e-04
./v006_domain_size.x,               1024,  16,   65536,  278,  3.775647e-04
./v003_inline_index.x,              128,   128,  65536,  280,  1.030579e-06
./v001_original.x,                  128,   128,  65536,  285,  1.030579e-06
./v005_no_precompute_constants.x,   128,   128,  65536,  286,  1.030579e-06
./v002_no_macro_index.x,            128,   128,  65536,  290,  1.030579e-06
./v006_domain_size_O3.x,            1024,  16,   65536,  395,  3.775647e-04
./v006_domain_size_O1.x,            1024,  16,   65536,  594,  3.775647e-04
./v006_domain_size_O2.x,            1024,  16,   65536,  650,  3.775647e-04
```

This only works up to a certain point when NY=8. In this case, the performance gets *much* worse:

```
./v005_no_precompute_constants.x,   128,   128,  65536,  284,  1.030579e-06
./v002_no_macro_index.x,            128,   128,  65536,  286,  1.030579e-06
./v003_inline_index.x,              128,   128,  65536,  291,  1.030579e-06
./v001_original.x,                  128,   128,  65536,  294,  1.030579e-06
./v006_domain_size_Ofast_native.x,  2048,  8,    65536,  506,  1.116604e-04
./v006_domain_size_O3.x,            2048,  8,    65536,  507,  1.116604e-04
./v006_domain_size.x,               2048,  8,    65536,  512,  1.116604e-04
./v006_domain_size_O3_native.x,     2048,  8,    65536,  519,  1.116604e-04
./v006_domain_size_O1.x,            2048,  8,    65536,  520,  1.116604e-04
./v006_domain_size_O2.x,            2048,  8,    65536,  538,  1.116604e-04
```

Ultimately the moral of the story is to try and structure a heavily anisotropic problem so that the longer side of the domain is the most rapidly moving index in the internal memory layout. And ideally use power of 2 number of grid points per *accessed side*. So if two grid points are never written to but are still read, they still count as a grid point for efficiency's sake.

## V007: Allocating temporary space inside hot function

I often allocate temporary space outside of a performant function like the Jacobi solver and then pass that memory into the function. Turns out allocating this just before the loop starts and freeing the memory after it finishes is totally fine!

```
./v007_malloc_temp_space.x,               128,   128,  65536,  281,   1.030579e-06
./v007_malloc_temp_space_O3_native.x,     128,   128,  65536,  283,   1.030579e-06
./v005_no_precompute_constants.x,         128,   128,  65536,  285,   1.030579e-06
./v003_inline_index.x,                    128,   128,  65536,  287,   1.030579e-06
./v002_no_macro_index.x,                  128,   128,  65536,  288,   1.030579e-06
./v001_original.x,                        128,   128,  65536,  298,   1.030579e-06
./v007_malloc_temp_space_Ofast_native.x,  128,   128,  65536,  312,   1.030579e-06
./v007_malloc_temp_space_O3.x,            128,   128,  65536,  483,   1.030579e-06
./v006_domain_size.x,                     2048,  8,    65536,  507,   1.116604e-04
./v007_malloc_temp_space_O1.x,            128,   128,  65536,  686,   1.030579e-06
./v007_malloc_temp_space_O2.x,            128,   128,  65536,  1016,  1.030579e-06
```

## V008: Can functions add overheads

Intuitively using functions does add some overhead in that the stack must be handled. Realistically, it makes little difference at higher optimisation levels:

```
./v003_inline_index.x,                     128,   128,  65536,  283,  1.030579e-06
./v007_malloc_temp_space.x,                128,   128,  65536,  283,  1.030579e-06
./v008_solver_in_function.x,               128,   128,  65536,  283,  1.030579e-06
./v001_original.x,                         128,   128,  65536,  287,  1.030579e-06
./v008_solver_in_function_O3_native.x,     128,   128,  65536,  287,  1.030579e-06
./v002_no_macro_index.x,                   128,   128,  65536,  289,  1.030579e-06
./v005_no_precompute_constants.x,          128,   128,  65536,  290,  1.030579e-06
./v008_solver_in_function_Ofast_native.x,  128,   128,  65536,  298,  1.030579e-06
./v008_solver_in_function_O3.x,            128,   128,  65536,  433,  1.030579e-06
./v006_domain_size.x,                      2048,  8,    65536,  519,  1.116604e-04
./v008_solver_in_function_O2.x,            128,   128,  65536,  700,  1.030579e-06
./v008_solver_in_function_O1.x,            128,   128,  65536,  920,  1.030579e-06
```

## V009: Do const arguments help with performance?

Not really but they don't hurt:

```
./v005_no_precompute_constants.x,         128,   128,  65536,  285,  1.030579e-06
./v009_const_jacobi_args_O3_native.x,     128,   128,  65536,  287,  1.030579e-06
./v009_const_jacobi_args.x,               128,   128,  65536,  289,  1.030579e-06
./v003_inline_index.x,                    128,   128,  65536,  290,  1.030579e-06
./v001_original.x,                        128,   128,  65536,  292,  1.030579e-06
./v008_solver_in_function.x,              128,   128,  65536,  294,  1.030579e-06
./v009_const_jacobi_args_Ofast_native.x,  128,   128,  65536,  294,  1.030579e-06
./v007_malloc_temp_space.x,               128,   128,  65536,  295,  1.030579e-06
./v002_no_macro_index.x,                  128,   128,  65536,  302,  1.030579e-06
./v009_const_jacobi_args_O3.x,            128,   128,  65536,  444,  1.030579e-06
./v006_domain_size.x,                     2048,  8,    65536,  509,  1.116604e-04
./v009_const_jacobi_args_O2.x,            128,   128,  65536,  715,  1.030579e-06
./v009_const_jacobi_args_O1.x,            128,   128,  65536,  921,  1.030579e-06
```

## V010: Passing a structure instead of arguments

Turns out this can have a significant negative performance impact:

```
./v003_inline_index.x,                   128,   128,  65536,  284,   1.030579e-06
./v008_solver_in_function.x,             128,   128,  65536,  284,   1.030579e-06
./v001_original.x,                       128,   128,  65536,  285,   1.030579e-06
./v007_malloc_temp_space.x,              128,   128,  65536,  285,   1.030579e-06
./v009_const_jacobi_args.x,              128,   128,  65536,  285,   1.030579e-06
./v005_no_precompute_constants.x,        128,   128,  65536,  290,   1.030579e-06
./v002_no_macro_index.x,                 128,   128,  65536,  305,   1.030579e-06
./v010_params_structure_Ofast_native.x,  128,   128,  65536,  321,   1.030579e-06
./v010_params_structure_O3_native.x,     128,   128,  65536,  359,   1.030579e-06
./v010_params_structure.x,               128,   128,  65536,  368,   1.030579e-06
./v010_params_structure_O3.x,            128,   128,  65536,  448,   1.030579e-06
./v006_domain_size.x,                    2048,  8,    65536,  528,   1.116604e-04
./v010_params_structure_O2.x,            128,   128,  65536,  1030,  1.030579e-06
./v010_params_structure_O1.x,            128,   128,  65536,  1072,  1.030579e-06
```

I have a feeling this is because the compiler is unable to properly optimise the parameters like it could if the parameters were simple arguments.

## V011: Passing in only some parameters

Even passing in only the domain size results in a hit:

```
./v001_original.x,                            128,   128,  65536,  283,   1.030579e-06
./v002_no_macro_index.x,                      128,   128,  65536,  286,   1.030579e-06
./v005_no_precompute_constants.x,             128,   128,  65536,  290,   1.030579e-06
./v008_solver_in_function.x,                  128,   128,  65536,  291,   1.030579e-06
./v003_inline_index.x,                        128,   128,  65536,  293,   1.030579e-06
./v007_malloc_temp_space.x,                   128,   128,  65536,  302,   1.030579e-06
./v009_const_jacobi_args.x,                   128,   128,  65536,  303,   1.030579e-06
./v011_half_params_structure_Ofast_native.x,  128,   128,  65536,  314,   1.030579e-06
./v011_half_params_structure_O3_native.x,     128,   128,  65536,  317,   1.030579e-06
./v010_params_structure_Ofast_native.x,       128,   128,  65536,  322,   1.030579e-06
./v010_params_structure.x,                    128,   128,  65536,  326,   1.030579e-06
./v011_half_params_structure.x,               128,   128,  65536,  351,   1.030579e-06
./v010_params_structure_O3_native.x,          128,   128,  65536,  367,   1.030579e-06
./v011_half_params_structure_O3.x,            128,   128,  65536,  443,   1.030579e-06
./v010_params_structure_O3.x,                 128,   128,  65536,  445,   1.030579e-06
./v006_domain_size.x,                         2048,  8,    65536,  503,   1.116604e-04
./v011_half_params_structure_O2.x,            128,   128,  65536,  706,   1.030579e-06
./v011_half_params_structure_O1.x,            128,   128,  65536,  1029,  1.030579e-06
./v010_params_structure_O2.x,                 128,   128,  65536,  1031,  1.030579e-06
./v010_params_structure_O1.x,                 128,   128,  65536,  1051,  1.030579e-06
```

## V012: Pass in parameters by value

It may be that the compiler can do something clever with the parameters if it knows they're passed in by value. NOPE:

```
./v001_original.x,                                128,   128,  65536,  283,   1.030579e-06
./v008_solver_in_function.x,                      128,   128,  65536,  283,   1.030579e-06
./v009_const_jacobi_args.x,                       128,   128,  65536,  290,   1.030579e-06
./v005_no_precompute_constants.x,                 128,   128,  65536,  292,   1.030579e-06
./v002_no_macro_index.x,                          128,   128,  65536,  294,   1.030579e-06
./v003_inline_index.x,                            128,   128,  65536,  297,   1.030579e-06
./v007_malloc_temp_space.x,                       128,   128,  65536,  297,   1.030579e-06
./v011_half_params_structure.x,                   128,   128,  65536,  318,   1.030579e-06
./v012_half_params_pass_by_value_Ofast_native.x,  128,   128,  65536,  331,   1.030579e-06
./v012_half_params_pass_by_value.x,               128,   128,  65536,  342,   1.030579e-06
./v012_half_params_pass_by_value_O3_native.x,     128,   128,  65536,  347,   1.030579e-06
./v010_params_structure.x,                        128,   128,  65536,  372,   1.030579e-06
./v012_half_params_pass_by_value_O3.x,            128,   128,  65536,  444,   1.030579e-06
./v006_domain_size.x,                             2048,  8,    65536,  522,   1.116604e-04
./v012_half_params_pass_by_value_O2.x,            128,   128,  65536,  705,   1.030579e-06
./v012_half_params_pass_by_value_O1.x,            128,   128,  65536,  1040,  1.030579e-06
```

## V013: Precompute inside function and use global parameters

We can move the computation of the relevant parts into the Jacobi solver, and use the global parameters defined previously in the code instead of passing them in. No major change from previous fastest, but better than passing in a structure:

```
./v002_no_macro_index.x,              128,   128,  65536,  283,  1.030579e-06
./v009_const_jacobi_args.x,           128,   128,  65536,  283,  1.030579e-06
./v005_no_precompute_constants.x,     128,   128,  65536,  284,  1.030579e-06
./v013_global_params_O3_native.x,     128,   128,  65536,  285,  1.030579e-06
./v013_global_params.x,               128,   128,  65536,  286,  1.030579e-06
./v008_solver_in_function.x,          128,   128,  65536,  288,  1.030579e-06
./v001_original.x,                    128,   128,  65536,  289,  1.030579e-06
./v007_malloc_temp_space.x,           128,   128,  65536,  289,  1.030579e-06
./v013_global_params_Ofast_native.x,  128,   128,  65536,  291,  1.030579e-06
./v003_inline_index.x,                128,   128,  65536,  292,  1.030579e-06
./v011_half_params_structure.x,       128,   128,  65536,  345,  1.030579e-06
./v012_half_params_pass_by_value.x,   128,   128,  65536,  346,  1.030579e-06
./v010_params_structure.x,            128,   128,  65536,  367,  1.030579e-06
./v013_global_params_O3.x,            128,   128,  65536,  457,  1.030579e-06
./v006_domain_size.x,                 2048,  8,    65536,  515,  1.116604e-04
./v013_global_params_O2.x,            128,   128,  65536,  704,  1.030579e-06
./v013_global_params_O1.x,            128,   128,  65536,  917,  1.030579e-06
```

## V014: Using non-literal domain sizes

Hardcoding domain sizes isn't ideal but is definitely faster than passing it in from an external source such as through command line arguments:

```
./v001_original.x,                          128,   128,  65536,  284,  1.030579e-06
./v009_const_jacobi_args.x,                 128,   128,  65536,  284,  1.030579e-06
./v002_no_macro_index.x,                    128,   128,  65536,  287,  1.030579e-06
./v007_malloc_temp_space.x,                 128,   128,  65536,  287,  1.030579e-06
./v005_no_precompute_constants.x,           128,   128,  65536,  288,  1.030579e-06
./v008_solver_in_function.x,                128,   128,  65536,  289,  1.030579e-06
./v003_inline_index.x,                      128,   128,  65536,  290,  1.030579e-06
./v013_global_params.x,                     128,   128,  65536,  292,  1.030579e-06
./v012_half_params_pass_by_value.x,         128,   128,  65536,  317,  1.030579e-06
./v010_params_structure.x,                  128,   128,  65536,  324,  1.030579e-06
./v014_external_parameters_O3_native.x,     128,   128,  65536,  333,  1.030579e-06
./v014_external_parameters.x,               128,   128,  65536,  338,  1.030579e-06
./v011_half_params_structure.x,             128,   128,  65536,  347,  1.030579e-06
./v014_external_parameters_Ofast_native.x,  128,   128,  65536,  388,  1.030579e-06
./v014_external_parameters_O3.x,            128,   128,  65536,  465,  1.030579e-06
./v006_domain_size.x,                       2048,  8,    65536,  504,  1.116604e-04
./v014_external_parameters_O2.x,            128,   128,  65536,  700,  1.030579e-06
./v014_external_parameters_O1.x,            128,   128,  65536,  923,  1.030579e-06
```

## V015: Using literal (but passed) domain sizes

Actually, the domain sizes can be hardcoded in main but still the performance is notably worse:

```
./v001_original.x,                             128,   128,  65536,  282,   1.030579e-06
./v007_malloc_temp_space.x,                    128,   128,  65536,  282,   1.030579e-06
./v008_solver_in_function.x,                   128,   128,  65536,  282,   1.030579e-06
./v002_no_macro_index.x,                       128,   128,  65536,  287,   1.030579e-06
./v009_const_jacobi_args.x,                    128,   128,  65536,  287,   1.030579e-06
./v013_global_params.x,                        128,   128,  65536,  289,   1.030579e-06
./v003_inline_index.x,                         128,   128,  65536,  293,   1.030579e-06
./v005_no_precompute_constants.x,              128,   128,  65536,  296,   1.030579e-06
./v010_params_structure.x,                     128,   128,  65536,  318,   1.030579e-06
./v015_literal_params_in_main_O3_native.x,     128,   128,  65536,  334,   1.030579e-06
./v014_external_parameters.x,                  128,   128,  65536,  348,   1.030579e-06
./v011_half_params_structure.x,                128,   128,  65536,  349,   1.030579e-06
./v012_half_params_pass_by_value.x,            128,   128,  65536,  350,   1.030579e-06
./v015_literal_params_in_main.x,               128,   128,  65536,  353,   1.030579e-06
./v015_literal_params_in_main_Ofast_native.x,  128,   128,  65536,  365,   1.030579e-06
./v015_literal_params_in_main_O3.x,            128,   128,  65536,  459,   1.030579e-06
./v006_domain_size.x,                          2048,  8,    65536,  517,   1.116604e-04
./v015_literal_params_in_main_O2.x,            128,   128,  65536,  705,   1.030579e-06
./v015_literal_params_in_main_O1.x,            128,   128,  65536,  1128,  1.030579e-06
```

## V016: Using const global literals for all parameters

We regain our original performance by making all parameters const literals:

```
./v016_literal_passed_global_params_O3_native.x,     128,   128,  65536,  283,  1.030579e-06
./v007_malloc_temp_space.x,                          128,   128,  65536,  286,  1.030579e-06
./v016_literal_passed_global_params.x,               128,   128,  65536,  287,  1.030579e-06
./v001_original.x,                                   128,   128,  65536,  288,  1.030579e-06
./v003_inline_index.x,                               128,   128,  65536,  289,  1.030579e-06
./v005_no_precompute_constants.x,                    128,   128,  65536,  289,  1.030579e-06
./v002_no_macro_index.x,                             128,   128,  65536,  290,  1.030579e-06
./v008_solver_in_function.x,                         128,   128,  65536,  291,  1.030579e-06
./v009_const_jacobi_args.x,                          128,   128,  65536,  291,  1.030579e-06
./v013_global_params.x,                              128,   128,  65536,  291,  1.030579e-06
./v016_literal_passed_global_params_Ofast_native.x,  128,   128,  65536,  293,  1.030579e-06
./v014_external_parameters.x,                        128,   128,  65536,  336,  1.030579e-06
./v012_half_params_pass_by_value.x,                  128,   128,  65536,  349,  1.030579e-06
./v010_params_structure.x,                           128,   128,  65536,  353,  1.030579e-06
./v011_half_params_structure.x,                      128,   128,  65536,  354,  1.030579e-06
./v015_literal_params_in_main.x,                     128,   128,  65536,  374,  1.030579e-06
./v016_literal_passed_global_params_O3.x,            128,   128,  65536,  441,  1.030579e-06
./v006_domain_size.x,                                2048,  8,    65536,  506,  1.116604e-04
./v016_literal_passed_global_params_O2.x,            128,   128,  65536,  703,  1.030579e-06
./v016_literal_passed_global_params_O1.x,            128,   128,  65536,  823,  1.030579e-06
```

The moral here is that hard-coding literal, global values really does improve performance. This is unfortunate.

## V017: Using stack-defined arrays

I'm curious to see if arrays allocated on the stack are any faster than heap arrays, perhaps due to the indexing calculation being handled by the compiler instead of being hand-written. I can avoid a `memcpy` by manually running the loop twice, once to place the result into a temporary space, and again to place a second iteration's result into the original array. `MAX_ITERATIONS` must then be divisible by 2. It's no faster and no slower than malloc'd arrays. It is, however, slightly less accurate than previous version for some reason:

```
./v001_original.x,                    128,   128,  65536,  285,  1.030579e-06
./v013_global_params.x,               128,   128,  65536,  289,  1.030579e-06
./v017_static_arrays.x,               128,   128,  65536,  289,  6.275444e-04
./v005_no_precompute_constants.x,     128,   128,  65536,  290,  1.030579e-06
./v008_solver_in_function.x,          128,   128,  65536,  290,  1.030579e-06
./v003_inline_index.x,                128,   128,  65536,  292,  1.030579e-06
./v017_static_arrays_Ofast_native.x,  128,   128,  65536,  295,  6.528929e-04
./v017_static_arrays_O3_native.x,     128,   128,  65536,  296,  6.137697e-04
./v009_const_jacobi_args.x,           128,   128,  65536,  298,  1.030579e-06
./v002_no_macro_index.x,              128,   128,  65536,  310,  1.030579e-06
./v011_half_params_structure.x,       128,   128,  65536,  341,  1.030579e-06
./v010_params_structure.x,            128,   128,  65536,  346,  1.030579e-06
./v014_external_parameters.x,         128,   128,  65536,  346,  1.030579e-06
./v012_half_params_pass_by_value.x,   128,   128,  65536,  357,  1.030579e-06
./v015_literal_params_in_main.x,      128,   128,  65536,  373,  1.030579e-06
./v007_malloc_temp_space.x,           128,   128,  65536,  442,  1.030579e-06
./v017_static_arrays_O3.x,            128,   128,  65536,  487,  6.137501e-04
./v006_domain_size.x,                 2048,  8,    65536,  517,  1.116604e-04
./v017_static_arrays_O2.x,            128,   128,  65536,  583,  6.487007e-04
./v017_static_arrays_O1.x,            128,   128,  65536,  708,  6.489012e-04
```

## V018: Swapping arrays with memcpy

Nevermind that the solution is wrong. `memcpy` is too slow:

```
./v005_no_precompute_constants.x,            128,   128,  65536,  282,   1.030579e-06
./v001_original.x,                           128,   128,  65536,  283,   1.030579e-06
./v007_malloc_temp_space.x,                  128,   128,  65536,  284,   1.030579e-06
./v003_inline_index.x,                       128,   128,  65536,  286,   1.030579e-06
./v008_solver_in_function.x,                 128,   128,  65536,  287,   1.030579e-06
./v002_no_macro_index.x,                     128,   128,  65536,  288,   1.030579e-06
./v009_const_jacobi_args.x,                  128,   128,  65536,  289,   1.030579e-06
./v013_global_params.x,                      128,   128,  65536,  291,   1.030579e-06
./v017_static_arrays.x,                      128,   128,  65536,  298,   6.275444e-04
./v011_half_params_structure.x,              128,   128,  65536,  311,   1.030579e-06
./v010_params_structure.x,                   128,   128,  65536,  324,   1.030579e-06
./v014_external_parameters.x,                128,   128,  65536,  357,   1.030579e-06
./v015_literal_params_in_main.x,             128,   128,  65536,  365,   1.030579e-06
./v018_static_arrays_memcpy.x,               128,   128,  65536,  411,   1.291139e-03
./v012_half_params_pass_by_value.x,          128,   128,  65536,  429,   1.030579e-06
./v018_static_arrays_memcpy_Ofast_native.x,  128,   128,  65536,  444,   1.069235e-03
./v018_static_arrays_memcpy_O3_native.x,     128,   128,  65536,  463,   1.283895e-03
./v006_domain_size.x,                        2048,  8,    65536,  505,   1.116604e-04
./v018_static_arrays_memcpy_O3.x,            128,   128,  65536,  587,   1.288286e-03
./v018_static_arrays_memcpy_O2.x,            128,   128,  65536,  697,   1.291099e-03
./v018_static_arrays_memcpy_O1.x,            128,   128,  65536,  1187,  1.292303e-03
```

## V019: Using the Gauss-Seidel method to avoid a temporary array

The Gauss-Seidel method overwrites the original array during one iteration, so there is no need for a second temporary array. This avoids either a swap, memcpy or partially manually unrolling the loop. The results are not positive however:

```
./v013_global_params.x,                            128,   128,  65536,  285,   1.030579e-06
./v017_static_arrays.x,                            128,   128,  65536,  287,   6.275444e-04
./v003_inline_index.x,                             128,   128,  65536,  288,   1.030579e-06
./v007_malloc_temp_space.x,                        128,   128,  65536,  288,   1.030579e-06
./v008_solver_in_function.x,                       128,   128,  65536,  288,   1.030579e-06
./v009_const_jacobi_args.x,                        128,   128,  65536,  290,   1.030579e-06
./v001_original.x,                                 128,   128,  65536,  293,   1.030579e-06
./v005_no_precompute_constants.x,                  128,   128,  65536,  295,   1.030579e-06
./v002_no_macro_index.x,                           128,   128,  65536,  299,   1.030579e-06
./v014_external_parameters.x,                      128,   128,  65536,  333,   1.030579e-06
./v012_half_params_pass_by_value.x,                128,   128,  65536,  342,   1.030579e-06
./v011_half_params_structure.x,                    128,   128,  65536,  346,   1.030579e-06
./v018_static_arrays_memcpy.x,                     128,   128,  65536,  418,   1.291139e-03
./v006_domain_size.x,                              2048,  8,    65536,  512,   1.116604e-04
./v010_params_structure.x,                         128,   128,  65536,  557,   1.030579e-06
./v015_literal_params_in_main.x,                   128,   128,  65536,  581,   1.030579e-06
./v019_static_arrays_gauss_seidel_Ofast_native.x,  128,   128,  65536,  2083,  1.030619e-06
./v019_static_arrays_gauss_seidel_O2.x,            128,   128,  65536,  2570,  1.030619e-06
./v019_static_arrays_gauss_seidel_O3.x,            128,   128,  65536,  2571,  1.030619e-06
./v019_static_arrays_gauss_seidel.x,               128,   128,  65536,  2983,  1.030619e-06
./v019_static_arrays_gauss_seidel_O3_native.x,     128,   128,  65536,  2985,  1.030619e-06
./v019_static_arrays_gauss_seidel_O1.x,            128,   128,  65536,  4478,  1.030619e-06```
```

## V020: The effect of ghost points

Adding ghost points around the domain is a standard practice when implementing von Neumann, periodic and, sometimes, Dirichlet boundary condtions. The effect this has on, for example, the performance of the index function is minimal when literals are used:

```
./v002_no_macro_index.x,                128,   128,  65536,  280,   1.030579e-06
./v008_solver_in_function.x,            128,   128,  65536,  282,   1.030579e-06
./v009_const_jacobi_args.x,             128,   128,  65536,  283,   1.030579e-06
./v016_literal_passed_global_params.x,  128,   128,  65536,  284,   1.030579e-06
./v003_inline_index.x,                  128,   128,  65536,  285,   1.030579e-06
./v007_malloc_temp_space.x,             128,   128,  65536,  285,   1.030579e-06
./v020_ghost_points_O3_native.x,        126,   126,  65536,  285,   1.063556e-06
./v020_ghost_points.x,                  126,   126,  65536,  285,   1.063556e-06
./v020_ghost_points_Ofast_native.x,     126,   126,  65536,  289,   1.063556e-06
./v005_no_precompute_constants.x,       128,   128,  65536,  291,   1.030579e-06
./v001_original.x,                      128,   128,  65536,  293,   1.030579e-06
./v017_static_arrays.x,                 128,   128,  65536,  296,   6.137697e-04
./v014_external_parameters.x,           128,   128,  65536,  338,   1.030579e-06
./v015_literal_params_in_main.x,        128,   128,  65536,  342,   1.030579e-06
./v011_half_params_structure.x,         128,   128,  65536,  351,   1.030579e-06
./v012_half_params_pass_by_value.x,     128,   128,  65536,  353,   1.030579e-06
./v013_global_params.x,                 128,   128,  65536,  354,   1.030579e-06
./v010_params_structure.x,              128,   128,  65536,  367,   1.030579e-06
./v018_static_arrays_memcpy.x,          128,   128,  65536,  418,   1.291139e-03
./v020_ghost_points_O3.x,               126,   126,  65536,  444,   1.063556e-06
./v006_domain_size.x,                   2048,  8,    65536,  506,   1.116604e-04
./v020_ghost_points_O1.x,               126,   126,  65536,  678,   1.063556e-06
./v020_ghost_points_O2.x,               126,   126,  65536,  713,   1.063556e-06
./v019_static_arrays_gauss_seidel.x,    128,   128,  65536,  2988,  1.030619e-06
```
