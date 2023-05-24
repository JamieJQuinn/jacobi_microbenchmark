# C++ Logbook

## Major Takeaways

- Use std::vector, it's as fast as using raw pointers
- Encapsulating data into an array class shouldn't affect performance
- Like in the C versions, global, constant literals are better than using local variables, either passed in through function parameters or stored in a structure. This obviously impacts the flexibility, i.e. every array must have the same size.

## Versions

### V001: Original code + compiler optimisations

As the original C++ version we take one of the fastest found C codes from the C benchmark, `v013_global_params.c` and change only free/malloc to new/delete. Using similar optimisations as the C benchmark, but also including turning off some (potentially) impactful C++ features:

```
-fno-exceptions -fno-rtti
```

Running each 10 times gives:

```
                                min   mean        std
exe_name                                             
./v001_original_Ofast_native.x  284  292.1   6.607235
./v001_original_O3_native.x     285  292.5   9.823441
./v001_original.x               285  293.1   6.226288
./v001_original_O3.x            434  466.9  82.731762
./v001_original_O2.x            700  708.5   8.396428
./v001_original_O1.x            908  923.0  10.666667
```

The performance is comparable to the C version.

### V002: Using std::swap

Swap pointers in jacobi using the safer std::swap over manual swap. Doesn't affect performance:

```
                                   min   mean        std
exe_name                                                
./v001_original.x                  286  291.8   3.675746
./v002_std_swap.x                  285  294.1   6.367452
```

### V003: Using unique_ptr

Use the safer unique_ptr over raw pointers. Doesn't strongly affect minimum performance:

```
                                   min   mean        std
exe_name                                                
./v001_original.x                  286  292.05   3.619756
./v002_std_swap.x                  285  293.25   5.097729
./v003_unique_ptr.x                285  299.00  10.285401
```

### V004: Using std::vector

Use the even safer std::vector over a pointer at all. Doesn't strongly affect performance:

```
                                   min    mean        std
exe_name                                                 
./v001_original.x                  284  300.54  14.799277
./v004_vector.x                    290  302.04   8.854514
./v003_unique_ptr.x                285  304.29   9.843518
./v002_std_swap.x                  286  304.33  18.084568
```

### V005: Removing global constants

As in the C version it's useful to ask how the performance is affected when the constant literals are defined inside main instead of as globals. This strongly affects performance, increasing the runtime by over 10%:

```
                                   min    mean        std
exe_name                                                 
./v001_original.x                  284  300.54  14.799277
./v004_vector.x                    290  302.04   8.854514
./v003_unique_ptr.x                285  304.29   9.843518
./v002_std_swap.x                  286  304.33  18.084568
./v005_no_globals.x                325  339.01   8.399729
```

### V006: Using a custom array class

In order to encapsulate away some of the annoying details of dealing with raw arrays, like the 2D index function, a custom array class is constructed that makes array indexing slightly nicer. It's really just syntactic sugar but it's much cleaner and potentially less error-prone. In this version we're back to using global literals both in the index function and in the loop. Performance is as good as with a raw vector:

```
                                   min    mean        std
exe_name                                                 
./v001_original.x                  284  300.54  14.799277
./v006_array_class.x               285  301.40   9.951397
./v004_vector.x                    290  302.04   8.854514
./v003_unique_ptr.x                285  304.29   9.843518
./v002_std_swap.x                  286  304.33  18.084568
./v005_no_globals.x                325  339.01   8.399729
```

### V007: NX&NY global in loop, local in idx function

This version tests the impact of using `nx` or `ny` local to the array class, rather than the global `NX` and `NY`, ONLY in the index calculation, i.e. the loop in the Jacobi algorithm still uses the global `NX` and `NY`. This is fairly impactful:

```
                                  min    mean        std
exe_name                                                
./v001_original.x                 284  300.54  14.799277
./v006_array_class.x              285  301.40   9.951397
./v004_vector.x                   290  302.04   8.854514
./v003_unique_ptr.x               285  304.29   9.843518
./v002_std_swap.x                 286  304.33  18.084568
./v007_array_class_global_loop.x  298  312.28  17.187075
./v005_no_globals.x               325  339.01   8.399729
```

### V008: Removal of all globals

As expected, changing all globals to constant literals defined in main strongly impacts performance:

```
                                  min    mean        std
exe_name                                                
./v001_original.x                 284  300.54  14.799277
./v006_array_class.x              285  301.40   9.951397
./v004_vector.x                   290  302.04   8.854514
./v003_unique_ptr.x               285  304.29   9.843518
./v002_std_swap.x                 286  304.33  18.084568
./v007_array_class_global_loop.x  298  312.28  17.187075
./v005_no_globals.x               325  339.01   8.399729
./v008_array_class_no_globals.x   340  362.57  14.998488
```
