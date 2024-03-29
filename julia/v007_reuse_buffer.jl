const NX::Int = 128
const NY::Int = 128
const MAX_ITERATIONS::Int = 1<<16

const real = Float64

const DX::real = 1.0/(NX-1)
const DY::real = 1.0/(NY-1)

const D::real = 2.0*(DX*DX + DY*DY);
const D_x::real = DY*DY/D;
const D_y::real = DX*DX/D;
const B::real = -(DX*DX*DY*DY)/D;

function run_jacobi!(p, p_new, b)
    for iter in 1:MAX_ITERATIONS
        for j in 2:NY-1
            @simd for i in 2:NX-1
                @inbounds p_new[i,j] = D_x*(p[i+1,j] + p[i-1,j]) + D_y*(p[i,j+1] + p[i,j-1]) + B*b[i,j];
            end
        end
        p, p_new = p_new, p
    end
end

function main()
    p = zeros(real, NX, NY)
    p_soln = zeros(real, NX, NY)
    b = zeros(real, NX, NY)

    for j in 1:NY
        for i in 1:NX
            x::real = (i-1)*DX
            y::real = (j-1)*DY
            b[i,j] = sin(pi*x)*sin(pi*y)
            p_soln[i,j] = -sin(pi*x)*sin(pi*y)/(2.0*pi^2)
        end
    end

    p_new = zeros(real, NX, NY)
    runtime = @elapsed run_jacobi!(p, p_new, b)

    av_error::real = 0.0
    for j in 2:NY-1
        for i in 2:NX-1
            av_error += abs(p[i,j] - p_soln[i,j])
        end
    end
    av_error /= (NX*NY);

    println("$PROGRAM_FILE, Julia, $NX, $NY, $MAX_ITERATIONS, $(runtime*1000), $av_error")
end

main()
