"""
Project Euler Problem 706: 3-Like Numbers

For a positive integer n, f(n) = number of non-empty substrings divisible by 3.
n is 3-like if f(n) is divisible by 3.
F(d) = count of d-digit 3-like numbers.
Find F(10^5) mod 1_000_000_007.
"""

MOD = 1_000_000_007

def mat_mul(A, B, n):
    C = [[0]*n for _ in range(n)]
    for i in range(n):
        for k in range(n):
            if A[i][k] == 0:
                continue
            for j in range(n):
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD
    return C

def mat_pow(M, p, n):
    result = [[0]*n for _ in range(n)]
    for i in range(n):
        result[i][i] = 1
    while p > 0:
        if p & 1:
            result = mat_mul(result, M, n)
        M = mat_mul(M, M, n)
        p >>= 1
    return result

def solve():
    D = 100000  # 10^5
    N = 81  # 27 states * 3 prefix sums

    def idx(c0, c1, c2, s):
        return ((c0 * 3 + c1) * 3 + c2) * 3 + s

    # Digit counts by residue mod 3: {0,3,6,9}->4, {1,4,7}->3, {2,5,8}->3
    digit_count = [4, 3, 3]

    # Build transition matrix
    T = [[0]*N for _ in range(N)]
    for c0 in range(3):
        for c1 in range(3):
            for c2 in range(3):
                for s in range(3):
                    fr = idx(c0, c1, c2, s)
                    for r in range(3):
                        ns = (s + r) % 3
                        nc = [c0, c1, c2]
                        nc[ns] = (nc[ns] + 1) % 3
                        to = idx(nc[0], nc[1], nc[2], ns)
                        T[to][fr] = (T[to][fr] + digit_count[r]) % MOD

    # Initial state after first digit (1-9, 3 of each residue class)
    state = [0] * N
    for r in range(3):
        c = [1, 0, 0]  # c0=1 for S_0=0
        c[r] = (c[r] + 1) % 3
        i = idx(c[0], c[1], c[2], r)
        state[i] = (state[i] + 3) % MOD

    # Apply transition D-1 times
    if D > 1:
        Tp = mat_pow(T, D - 1, N)
        new_state = [0] * N
        for i in range(N):
            for j in range(N):
                new_state[i] = (new_state[i] + Tp[i][j] * state[j]) % MOD
        state = new_state

    # binom(c, 2) mod 3: c%3=0->0, c%3=1->0, c%3=2->1
    bmod3 = [0, 0, 1]

    answer = 0
    for c0 in range(3):
        for c1 in range(3):
            for c2 in range(3):
                if (bmod3[c0] + bmod3[c1] + bmod3[c2]) % 3 == 0:
                    for s in range(3):
                        answer = (answer + state[idx(c0, c1, c2, s)]) % MOD

    print(answer)

solve()
