"""
Problem 237: Tours on a 4 x n Playing Board

T(n) = number of Hamiltonian paths on a 4 x n grid from top-left (0,0) to
bottom-left (3,0), visiting every cell exactly once.

Given T(10) = 2329, find T(10^12) mod 10^8.

The sequence satisfies the linear recurrence:
    T(n) = 2*T(n-1) + 2*T(n-2) - 2*T(n-3) + T(n-4)

with initial values T(1)=1, T(2)=1, T(3)=4, T(4)=8.

This was found by:
1. Computing small values via brute-force backtracking.
2. Finding the minimal recurrence via Gaussian elimination.

We use matrix exponentiation to compute T(10^12) mod 10^8.
"""

MOD = 10**8

def mat_mul(A, B, mod):
    """Multiply two square matrices modulo mod."""
    n = len(A)
    C = [[0]*n for _ in range(n)]
    for i in range(n):
        for k in range(n):
            if A[i][k] == 0:
                continue
            for j in range(n):
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % mod
    return C

def mat_pow(M, p, mod):
    """Compute M^p modulo mod using binary exponentiation."""
    n = len(M)
    R = [[1 if i == j else 0 for j in range(n)] for i in range(n)]
    while p > 0:
        if p & 1:
            R = mat_mul(R, M, mod)
        M = mat_mul(M, M, mod)
        p >>= 1
    return R

# Recurrence: T(n) = 2*T(n-1) + 2*T(n-2) - 2*T(n-3) + T(n-4)
# Initial: T(1)=1, T(2)=1, T(3)=4, T(4)=8

# Verify small values
T = [0, 1, 1, 4, 8]
for i in range(5, 20):
    T.append(2*T[-1] + 2*T[-2] - 2*T[-3] + T[-4])
assert T[10] == 2329, f"T(10) = {T[10]}, expected 2329"

# Companion matrix:
# [T(n)  ]   [2  2 -2  1] [T(n-1)]
# [T(n-1)] = [1  0  0  0] [T(n-2)]
# [T(n-2)]   [0  1  0  0] [T(n-3)]
# [T(n-3)]   [0  0  1  0] [T(n-4)]

M = [
    [2, 2, MOD - 2, 1],
    [1, 0, 0, 0],
    [0, 1, 0, 0],
    [0, 0, 1, 0]
]

N = 10**12

# State at n=4: [T(4), T(3), T(2), T(1)] = [8, 4, 1, 1]
Mk = mat_pow(M, N - 4, MOD)

state = [8, 4, 1, 1]
ans = 0
for j in range(4):
    ans = (ans + Mk[0][j] * state[j]) % MOD

print(ans)
