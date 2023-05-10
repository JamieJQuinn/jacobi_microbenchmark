from numba import njit
import os
import numpy as np
import time

real = np.float64

NX = 128
NY = 128
MAX_ITERATIONS = 65536

DX = 1.0/(NX-1);
DY = 1.0/(NY-1);

D = 2.0*(DX*DX + DY*DY);
D_x = DY*DY/D;
D_y = DX*DX/D;
B = -(DX*DX*DY*DY)/D;

@njit
def run_jacobi(p, b):
    p_new = np.zeros_like(p)
    for it in range(MAX_ITERATIONS):
        for i in range(1, NX-1):
            for j in range(1, NY-1):
                p_new[i,j] = D_x*(p[i-1,j] + p[i+1,j]) + D_y*(p[i,j-1] + p[i,j+1]) + B*b[i,j]
        p_new, p = p, p_new


def main():
    p = np.zeros((NX, NY), dtype=real)
    b = np.zeros((NX, NY), dtype=real)
    p_soln = np.zeros((NX, NY), dtype=real)

    x = np.linspace(0.0, 1.0, num=NX)
    y = np.linspace(0.0, 1.0, num=NY)
    X, Y = np.meshgrid(x, y, indexing='ij')
    b[:,:] = np.sin(np.pi*X)*np.sin(np.pi*Y)
    p_soln[:,:] = -np.sin(np.pi*X)*np.sin(np.pi*Y)/(2.0*np.pi**2)

    # Run once to let Numba compile
    run_jacobi(p, b)

    start = time.perf_counter()
    run_jacobi(p, b)
    runtime = (time.perf_counter() - start)*1000

    av_error = np.sum(np.abs(p - p_soln))/(NX*NY)

    print(os.path.basename(__file__), "Python", NX, NY, MAX_ITERATIONS, runtime, av_error)

main()
