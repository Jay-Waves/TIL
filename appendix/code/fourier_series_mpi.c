/*
 * S_N(Nx) = a0 + \sum^N_{n-1}(a_n cos(n x) + b_n sin(n x))
 * a_n = 2/T \in^T_0 f(t) cos( \frac{2 \pi n t}{T} ) dt
 * b_n = 2/T \in^T_0 f(t) sin( \frac{2 \pi n t}{T} ) dt
 * mpicc -o ... -lm
 * mpirun -np 4 ./...
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 1000 // 傅里叶级数的项数
#define T 2 * M_PI // 周期
#define NUM_POINTS 1000 // 离散点数

// 被展开的函数 f(t) = t
double f(double t) {
    return t;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank; // current process identifier
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size; // total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int local_N = N / world_size;
    int start = world_rank * local_N;
    int end = start + local_N;

    double local_a0 = 0.0;
    double* local_an = (double*)malloc(local_N * sizeof(double));
    double* local_bn = (double*)malloc(local_N * sizeof(double));

    double delta_t = T / NUM_POINTS;

    for (int n = start; n < end; n++) {
        double a_n = 0.0;
        double b_n = 0.0;
        for (int k = 0; k < NUM_POINTS; k++) {
            double t = k * delta_t;
            a_n += f(t) * cos(2 * M_PI * n * t / T) * delta_t;
            b_n += f(t) * sin(2 * M_PI * n * t / T) * delta_t;
        }
        local_an[n - start] = 2 * a_n / T;
        local_bn[n - start] = 2 * b_n / T;
    }

    if (world_rank == 0) {
        local_a0 = 0.0;
        for (int k = 0; k < NUM_POINTS; k++) {
            double t = k * delta_t;
            local_a0 += f(t) * delta_t;
        }
        local_a0 /= T;
    }

    double global_a0;
    double* global_an = NULL;
    double* global_bn = NULL;

    if (world_rank == 0) {
        global_an = (double*)malloc(N * sizeof(double));
        global_bn = (double*)malloc(N * sizeof(double));
    }

    MPI_Reduce(&local_a0, &global_a0, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Gather(local_an, local_N, MPI_DOUBLE, global_an, local_N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(local_bn, local_N, MPI_DOUBLE, global_bn, local_N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        printf("a0 = %f\n", global_a0);
        for (int n = 0; n < N; n++) {
            printf("a%d = %f, b%d = %f\n", n, global_an[n], n, global_bn[n]);
        }
        free(global_an);
        free(global_bn);
    }

    free(local_an);
    free(local_bn);

    MPI_Finalize();
    return 0;
}
