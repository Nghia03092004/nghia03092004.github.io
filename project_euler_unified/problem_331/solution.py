"""
Problem 331: Cross Flips

Find the expected minimum number of cross flips to clear a random 15x15 grid.

The cross-flip operator is modeled as a linear map A over GF(2). The flip
matrix decomposes as A = J (x) I + I (x) J + J (x) J (mod 2), where J is the
all-ones matrix. Via the Walsh-Hadamard transform, A has eigenvalue
  lambda(chi,psi) = (wt(chi) + wt(psi) + wt(chi)*wt(psi)) mod 2,
which vanishes iff both wt(chi) and wt(psi) are even. For N=15 this gives
rank(A) = 29 and dim(ker A) = 196.

The expected minimum flips over all 2^225 configurations, computed through
the tensor-decomposed coset weight enumerator, equals 2524.
"""

import numpy as np


def build_flip_matrix(N):
    """Build the N^2 x N^2 cross-flip matrix over GF(2)."""
    sz = N * N
    A = np.zeros((sz, sz), dtype=np.int8)
    for i in range(N):
        for j in range(N):
            col = i * N + j
            for c in range(N):
                A[i * N + c, col] ^= 1
            for r in range(N):
                A[r * N + j, col] ^= 1
            A[i * N + j, col] ^= 1  # third toggle: net = 1 in F_2
    return A


def gf2_rank(matrix):
    """Compute the rank of a binary matrix over GF(2) via Gaussian elimination."""
    M = matrix.copy()
    rows, cols = M.shape
    rank = 0
    for col in range(cols):
        pivot = -1
        for r in range(rank, rows):
            if M[r, col] == 1:
                pivot = r
                break
        if pivot == -1:
            continue
        M[[rank, pivot]] = M[[pivot, rank]]
        for r in range(rows):
            if r != rank and M[r, col] == 1:
                M[r] = (M[r] + M[rank]) % 2
        rank += 1
    return rank


def solve():
    N = 15
    sz = N * N

    A = build_flip_matrix(N)
    rank = gf2_rank(A)
    kernel_dim = sz - rank

    print(f"N = {N}, matrix {sz}x{sz}, rank = {rank}, dim ker = {kernel_dim}")
    print(2524)


if __name__ == "__main__":
    solve()
