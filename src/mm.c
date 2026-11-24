#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define _POSIX_C_SOURCE 200809L

// C = A * B, all N x N, stored flat: element (i,j) is at [i*N + j]
void mm_ijk(const double *A, const double *B, double *C, int N) {
    // TODO: three nested loops: i, then j, then k
    //   C[i][j] += A[i][k] * B[k][j]
    // Hint: write each [row][col] as [row*N + col]

    // i represents row 
    for (int i = 0; i < N; i++){
        // j represents collumn
        for (int j = 0; j < N; j++){
            C[i * N + j] = 0; //init matrix to 0 accumalator

            // 
            for (int k = 0; k < N; k++){
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

// MM using ijk instead Cache-friendly i-k-j loop order
void mm_ikj(const double *A, const double *B, double *C, int N) {
    // TODO: three nested loops: i, then k, then j

    // i represents row 
    for (int i = 0; i < N; i++){
        // k represents collumn of A row of B   
        for (int k = 0; k < N; k++){
            double a_ik = A[i * N + k]; // held fast CPU register  
            for (int j = 0; j < N; j++){
                C[i * N + j] += a_ik * B[k * N + j]; // accumulate into row i of C
            }
        }
    }
}   

// Defines a helper that returns the current monotonic time in seconds
double now_seconds(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9; // Converts the time to seconds:
}

int main(int argc, char **argv) {
    const char *version = (argc > 1) ? argv[1] : "ijk";   // NEW: 1st arg = which version
    int N = (argc > 2) ? atoi(argv[2]) : 512;             // NEW: 2nd arg = size

    double *A = malloc((size_t)N * N * sizeof(double));
    double *B = malloc((size_t)N * N * sizeof(double));
    double *C = calloc((size_t)N * N, sizeof(double));    // NEW: calloc so C starts at 0
    if (!A || !B || !C) { fprintf(stderr, "allocation failed\n"); return 1; }

    srand(42);          
    for (long i = 0; i < (long)N * N; i++) {
        A[i] = (double)rand() / RAND_MAX;
        B[i] = (double)rand() / RAND_MAX;
    }

    double start = now_seconds();
    if (strcmp(version, "ijk") == 0) {                    // NEW: pick the version
        mm_ijk(A, B, C, N);
    } else if (strcmp(version, "ikj") == 0) {
        mm_ikj(A, B, C, N);
    } else {
        fprintf(stderr, "unknown version: %s (use ijk or ikj)\n", version);
        return 1;
    }
    double elapsed = now_seconds() - start;

    double checksum = 0.0;
    for (long i = 0; i < (long)N * N; i++) checksum += C[i];


    printf("%s,%d,%.6f,%.6f\n", version, N, elapsed, checksum);

    free(A); free(B); free(C);
    return 0;
}