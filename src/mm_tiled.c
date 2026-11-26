// Cache-blocked (tiled) i-k-j matrix multiply.
// Assumes N is a multiple of T.
void mm_tiled(const double *A, const double *B, double *C, int N, int T) {
    // Outer 3 loops: choose which T×T tile to work on
    for (int ii = 0; ii < N; ii += T) {          // tile row of C (and A)
        for (int kk = 0; kk < N; kk += T) {      // tile column of A / tile row of B
            for (int jj = 0; jj < N; jj += T) {  // tile column of C (and B)

                // Inner 3 loops: normal ikj, but only inside this tile
                for (int i = ii; i < ii + T; i++) {
                    for (int k = kk; k < kk + T; k++) {
                        double a_ik = A[i * N + k];          // stays in a register
                        for (int j = jj; j < jj + T; j++) {
                            C[i * N + j] += a_ik * B[k * N + j];
                        }
                    }
                }

            }
        }
    }
}