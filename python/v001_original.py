import numpy as np
import os
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

def main():
    p = np.zeros((NX, NY), dtype=real)
    b = np.zeros((NX, NY), dtype=real)
    p_soln = np.zeros((NX, NY), dtype=real)

    x = np.linspace(0.0, 1.0, num=NX)
    y = np.linspace(0.0, 1.0, num=NY)
    X, Y = np.meshgrid(x, y, indexing='ij')
    b[:,:] = np.sin(np.pi*X)*np.sin(np.pi*Y)
    p_soln[:,:] = -np.sin(np.pi*X)*np.sin(np.pi*Y)/(2.0*np.pi**2)

    start = time.perf_counter()
    p_new = np.zeros_like(p)
    for i in range(MAX_ITERATIONS):
        p_new[1:-1,1:-1] = D_x*(p[2:,1:-1] + p[:-2,1:-1]) + D_y*(p[1:-1,2:] + p[1:-1,:-2]) + B*b[1:-1,1:-1]
        p_new, p = p, p_new
    runtime = (time.perf_counter() - start)*1000

    av_error = np.sum(np.abs(p - p_soln))/(NX*NY)

    print(os.path.basename(__file__), "Python", NX, NY, MAX_ITERATIONS, runtime, av_error)


main()
