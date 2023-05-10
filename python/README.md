## Logbook

### V001: Original

Based on the original C version, this uses numpy to leverage its vectorised operations. It's horrifically slow though:

```
v001_original.py    Python  128  128  65536  3771.433566013002   1.0305793014656103e-06
```

### V002: Numba

I can't see any way to improve the pure numpy code so I'll try JITifiying it with Numba. Certainly better but not great:

```
v002_numba.py       Python  128  128  65536  2705.6437120045302  1.0306187939894465e-06
v001_original.py    Python  128  128  65536  3534.1721720033092  1.0305793014656103e-06
```

### V003: Numba with explicit loops

I had a sneaking suspicion the vectorised operations are to blame so I changed them to regular for loops and this resulted in basically the same speed as the fastest Julia or slower C versions:

```
v003_numba_loop.py  Python  128  128  65536  330.15602800878696  1.0306187939894465e-06
v002_numba.py       Python  128  128  65536  2614.3530279950937  1.0306187939894465e-06
v001_original.py    Python  128  128  65536  3472.5894259900087  1.0305793014656103e-06
```

### V004: Fastmath

Trying out Numba's fastmath doesn't do much:

```
v003_numba_loop.py  Python  128  128  65536  338.88488900265656  1.0306187939894465e-06
v004_fastmath.py    Python  128  128  65536  339.2826579947723   1.0306187942102752e-06
v002_numba.py       Python  128  128  65536  2665.1220439962344  1.0306187939894465e-06
v001_original.py    Python  128  128  65536  3527.0844099868555  1.0305793014656103e-06
```
