/*
 * Multiply matrixs using OpenMP 
 * gcc -o matrix_openmp matrix_openmap.c -fopenmp -lm
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000 // 矩阵的维度

void matrix_multiply(double** A, double** B, double** C, int n) {
    int i, j, k;
    #pragma omp parallel for private(i, j, k) shared(A, B, C)
    for(i = 0; i < n; i++){
        for(k = 0; k < n; k++){
            double temp = A[i][k]; // 使用 i-k-j 顺序, 利用缓存
            for(j = 0; j < n; j++){
                C[i][j] += temp * B[k][j];
            }
        }
    }
}

int main() {
    // 动态分配矩阵内存
    double** A = (double**)malloc(N * sizeof(double*));
    double** B = (double**)malloc(N * sizeof(double*));
    double** C = (double**)malloc(N * sizeof(double*));

    for (int i = 0; i < N; i++) {
        A[i] = (double*)malloc(N * sizeof(double));
        B[i] = (double*)malloc(N * sizeof(double));
        C[i] = (double*)malloc(N * sizeof(double));
    }

    // 初始化矩阵 A 和 B
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = drand48();
            B[i][j] = drand48();
        }
    }

    double start_time = omp_get_wtime();
    
    // 并行矩阵乘法
    matrix_multiply(A, B, C, N);
    
    double end_time = omp_get_wtime();
    printf("Matrix multiplication completed in %f seconds.\n", end_time - start_time);

    // 释放矩阵内存
    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
