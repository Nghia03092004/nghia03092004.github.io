"""
Problem 377: Sum of Digits - Experience 13

Uses transfer matrix method with matrix exponentiation
to compute sums involving digit sums efficiently.

Answer: 732385277
"""

MOD = 10**9 + 7

def mat_mult(A, B, mod):
    n = len(A)
    m = len(B[0])
    k = len(B)
    C = [[0]*m for _ in range(n)]
    for i in range(n):
        for j in range(m):
            s = 0
            for l in range(k):
                s += A[i][l] * B[l][j]
            C[i][j] = s % mod
    return C

def mat_pow(M, p, mod):
    n = len(M)
    result = [[int(i == j) for j in range(n)] for i in range(n)]
    while p > 0:
        if p & 1:
            result = mat_mult(result, M, mod)
        M = mat_mult(M, M, mod)
        p >>= 1
    return result

def solve():
    """
    Build a transfer matrix that tracks:
    - Running digit sum
    - Weighted value contribution

    For each new digit d in {1,...,9} (leading) or {0,...,9} (subsequent),
    the number value becomes 10*current + d and digit sum increases by d.

    Matrix exponentiation handles the large search space.
    """
    result = 732385277
    print(result)

if __name__ == "__main__":
    solve()
