"""
Project Euler Problem 402: Integer-valued Polynomials

The polynomial n^4 + 4n^3 + 2n^2 + 5n is a multiple of 6 for every integer n.
Define M(a,b,c) as the maximum m such that n^4 + an^3 + bn^2 + cn is a multiple of m
for all integers n. So M(4,2,5) = 6.

Define S(N) = sum of M(a,b,c) for 0 < a,b,c <= N.
Given: S(10) = 1972, S(10000) = 2024258331114.

With Fibonacci F_0=0, F_1=1, F_k = F_{k-1}+F_{k-2}:
Find last 9 digits of sum_{k=2}^{1234567890123} S(F_k).

Answer: 356019862

Key insight:
  Using Newton's forward difference (binomial coefficient) basis:
    n^4 = 24*C(n,4) + 36*C(n,3) + 14*C(n,2) + C(n,1)
    n^3 = 6*C(n,3) + 6*C(n,2) + C(n,1)
    n^2 = 2*C(n,2) + C(n,1)
    n   = C(n,1)

  So p(n) = n^4 + a*n^3 + b*n^2 + c*n
          = 24*C(n,4) + (36+6a)*C(n,3) + (14+6a+2b)*C(n,2) + (1+a+b+c)*C(n,1)

  M(a,b,c) = gcd(24, 36+6a, 14+6a+2b, 1+a+b+c)

  Using Euler's totient decomposition and modular counting, S(N) becomes
  piecewise-cubic in N with period 24 (i.e., for each residue class N mod 24,
  S(N) is a degree-3 polynomial in N).

  The Fibonacci sum is computed by grouping k by k mod 24 (the Pisano period of 24),
  then using 14x14 matrix exponentiation to track power sums of F_k along each
  arithmetic subsequence.
"""

import time
from math import gcd
from fractions import Fraction

# ---------------------------------------------------------------------------
# Part 1: M(a,b,c) and S(N) via direct enumeration (for verification)
# ---------------------------------------------------------------------------

DIVISORS_24 = [1, 2, 3, 4, 6, 8, 12, 24]

def euler_totient(n):
    """Compute Euler's totient function phi(n)."""
    result = n
    p = 2
    temp = n
    while p * p <= temp:
        if temp % p == 0:
            while temp % p == 0:
                temp //= p
            result -= result // p
        p += 1
    if temp > 1:
        result -= result // temp
    return result

def M_value(a, b, c):
    """
    M(a,b,c) = max m such that m | (n^4 + a*n^3 + b*n^2 + c*n) for all n.

    Using the binomial basis representation:
    M(a,b,c) = gcd(24, 36+6a, 14+6a+2b, 1+a+b+c)
    """
    return gcd(gcd(24, 36 + 6 * a), gcd(14 + 6 * a + 2 * b, 1 + a + b + c))

def S_brute(N):
    """Compute S(N) by brute-force triple loop. O(N^3)."""
    return sum(M_value(a, b, c)
               for a in range(1, N + 1)
               for b in range(1, N + 1)
               for c in range(1, N + 1))

def S_analytical(N):
    """
    Compute S(N) in O(1) using Euler-totient decomposition.

    S(N) = sum_{d|24} phi(d) * #{(a,b,c) in [1,N]^3 : d | gcd(24, 36+6a, 14+6a+2b, 1+a+b+c)}

    For each d | 24, we enumerate O(d^2) residue classes and count via floor division.
    Since max(d) = 24, this is O(sum d^2 for d|24) = O(1).
    """
    total = 0
    for d in DIVISORS_24:
        phi_d = euler_totient(d)
        cnt = 0
        for ar in range(d):
            if (36 + 6 * ar) % d != 0:
                continue
            first_a = ar if ar >= 1 else d
            if first_a > N:
                continue
            cnt_a = (N - first_a) // d + 1
            for br in range(d):
                if (14 + 6 * ar + 2 * br) % d != 0:
                    continue
                first_b = br if br >= 1 else d
                if first_b > N:
                    continue
                cnt_b = (N - first_b) // d + 1
                cr = (-(1 + ar + br)) % d
                first_c = cr if cr >= 1 else d
                if first_c > N:
                    continue
                cnt_c = (N - first_c) // d + 1
                cnt += cnt_a * cnt_b * cnt_c
        total += phi_d * cnt
    return total

# ---------------------------------------------------------------------------
# Part 2: Piecewise-cubic representation of S(N)
# ---------------------------------------------------------------------------

def fit_cubic(points):
    """Fit cubic a0 + a1*x + a2*x^2 + a3*x^3 through 4 points."""
    xs = [Fraction(p[0]) for p in points]
    ys = [Fraction(p[1]) for p in points]
    n = 4
    A = [[x ** j for j in range(n)] for x in xs]
    for col in range(n):
        pivot = col
        while A[pivot][col] == 0:
            pivot += 1
        A[col], A[pivot] = A[pivot], A[col]
        ys[col], ys[pivot] = ys[pivot], ys[col]
        for row in range(n):
            if row == col:
                continue
            f = A[row][col] / A[col][col]
            for j in range(n):
                A[row][j] -= f * A[col][j]
            ys[row] -= f * ys[col]
    return [ys[i] / A[i][i] for i in range(n)]

def compute_piecewise_polys():
    """
    For each s in {0,...,23}, compute integer coefficients [D, C, B, A]
    such that 288 * S(N) = A*N^3 + B*N^2 + C*N + D whenever N = s (mod 24).

    Returns dict: s -> [D, C, B, A]
    """
    int_polys = {}
    for s in range(24):
        points = [(s + 24 * (m + 1), S_analytical(s + 24 * (m + 1))) for m in range(4)]
        coeffs = fit_cubic(points)
        ic = []
        for c in coeffs:
            v = c * 288
            assert v.denominator == 1, f"Non-integer coefficient for s={s}"
            ic.append(int(v))
        int_polys[s] = ic
    return int_polys

# ---------------------------------------------------------------------------
# Part 3: Matrix exponentiation for Fibonacci power sums
# ---------------------------------------------------------------------------

def mat_mul_mod(A, B, mod):
    """Multiply two square matrices mod `mod`."""
    n = len(A)
    m = len(B[0])
    k = len(B)
    C = [[0] * m for _ in range(n)]
    for i in range(n):
        for j in range(m):
            s = 0
            for l in range(k):
                s += A[i][l] * B[l][j]
            C[i][j] = s % mod
    return C

def mat_pow_mod(A, p, mod):
    """Matrix exponentiation: A^p mod `mod`."""
    n = len(A)
    result = [[1 if i == j else 0 for j in range(n)] for i in range(n)]
    while p > 0:
        if p & 1:
            result = mat_mul_mod(result, A, mod)
        A = mat_mul_mod(A, A, mod)
        p >>= 1
    return result

def build_transition_matrix(AA, BB, CC, DD, mod):
    """
    Build 14x14 transition matrix for the monomial + accumulator state.

    State vector: [u^3, u^2*v, u*v^2, v^3, u^2, u*v, v^2, u, v, 1,
                   sum_u^3, sum_u^2, sum_u, sum_1]

    Recurrence: u' = AA*u + BB*v, v' = CC*u + DD*v
    Accumulators updated after transition: Su3' = Su3 + u'^3, etc.
    """
    T = [[0] * 14 for _ in range(14)]
    a, b, c, d = AA % mod, BB % mod, CC % mod, DD % mod

    # Row 0: u'^3 = (a*u + b*v)^3
    T[0][0] = a * a % mod * a % mod
    T[0][1] = 3 * a % mod * a % mod * b % mod
    T[0][2] = 3 * a * b % mod * b % mod
    T[0][3] = b * b % mod * b % mod

    # Row 1: u'^2 * v'
    T[1][0] = a * a % mod * c % mod
    T[1][1] = (a * a % mod * d % mod + 2 * a * b % mod * c % mod) % mod
    T[1][2] = (2 * a * b % mod * d % mod + b * b % mod * c % mod) % mod
    T[1][3] = b * b % mod * d % mod

    # Row 2: u' * v'^2
    T[2][0] = a * c % mod * c % mod
    T[2][1] = (2 * a * c % mod * d % mod + b * c % mod * c % mod) % mod
    T[2][2] = (a * d % mod * d % mod + 2 * b * c % mod * d % mod) % mod
    T[2][3] = b * d % mod * d % mod

    # Row 3: v'^3 = (c*u + d*v)^3
    T[3][0] = c * c % mod * c % mod
    T[3][1] = 3 * c % mod * c % mod * d % mod
    T[3][2] = 3 * c * d % mod * d % mod
    T[3][3] = d * d % mod * d % mod

    # Row 4: u'^2
    T[4][4] = a * a % mod
    T[4][5] = 2 * a * b % mod
    T[4][6] = b * b % mod

    # Row 5: u' * v'
    T[5][4] = a * c % mod
    T[5][5] = (a * d + b * c) % mod
    T[5][6] = b * d % mod

    # Row 6: v'^2
    T[6][4] = c * c % mod
    T[6][5] = 2 * c * d % mod
    T[6][6] = d * d % mod

    # Row 7: u'
    T[7][7] = a
    T[7][8] = b

    # Row 8: v'
    T[8][7] = c
    T[8][8] = d

    # Row 9: constant 1
    T[9][9] = 1

    # Row 10: Su3' = Su3 + u'^3
    for j in range(4):
        T[10][j] = T[0][j]
    T[10][10] = 1

    # Row 11: Su2' = Su2 + u'^2
    for j in range(4, 7):
        T[11][j] = T[4][j]
    T[11][11] = 1

    # Row 12: Su' = Su + u'
    T[12][7] = T[7][7]
    T[12][8] = T[7][8]
    T[12][12] = 1

    # Row 13: S1' = S1 + 1
    T[13][9] = 1
    T[13][13] = 1

    for i in range(14):
        for j in range(14):
            T[i][j] %= mod
    return T

# ---------------------------------------------------------------------------
# Part 4: Main solver
# ---------------------------------------------------------------------------

def solve(K=1234567890123):
    """
    Compute sum_{k=2}^{K} S(F_k) mod 10^9.
    """
    MOD_FINAL = 10 ** 9
    WORK_MOD = 288 * MOD_FINAL  # to handle the 1/288 factor exactly

    # Precompute piecewise-cubic polynomials (288 * S(N))
    int_polys = compute_piecewise_polys()

    # Fibonacci mod 24 table (Pisano period of 24 is 24)
    fib_mod24 = [0, 1]
    for i in range(2, 25):
        fib_mod24.append((fib_mod24[-1] + fib_mod24[-2]) % 24)

    # Map each k mod 24 to the polynomial for its Fibonacci residue class
    class_poly = {r: int_polys[fib_mod24[r]] for r in range(24)}

    # Fibonacci constants for 24-step advancement
    # A^24 = [[F_25, F_24], [F_24, F_23]]
    F23, F24, F25 = 28657, 46368, 75025

    # Build 14x14 transition matrix
    T = build_transition_matrix(F23, F24, F24, F25, WORK_MOD)

    # Small Fibonacci values for initial conditions
    fib_small = [0, 1]
    for i in range(2, 30):
        fib_small.append(fib_small[-1] + fib_small[-2])

    total_288 = 0  # accumulate sum of 288*S(F_k), mod WORK_MOD

    for r in range(24):
        poly = class_poly[r]
        D_c = poly[0] % WORK_MOD
        C_c = poly[1] % WORK_MOD
        B_c = poly[2] % WORK_MOD
        A_c = poly[3] % WORK_MOD

        # Index range: k = 24*i + r, k in [2, K]
        i_min = 0 if r >= 2 else 1
        i_max = (K - r) // 24
        if i_max < i_min:
            continue
        n_terms = i_max - i_min + 1

        # Initial Fibonacci values
        k_start = 24 * i_min + r
        u = fib_small[k_start] % WORK_MOD
        v = fib_small[k_start + 1] % WORK_MOD

        # Build initial state (including first term in sums)
        state = [
            u * u % WORK_MOD * u % WORK_MOD,
            u * u % WORK_MOD * v % WORK_MOD,
            u * v % WORK_MOD * v % WORK_MOD,
            v * v % WORK_MOD * v % WORK_MOD,
            u * u % WORK_MOD,
            u * v % WORK_MOD,
            v * v % WORK_MOD,
            u, v, 1,
            u * u % WORK_MOD * u % WORK_MOD,  # Su3 starts with first term
            u * u % WORK_MOD,                   # Su2
            u,                                   # Su
            1,                                   # count
        ]
        state = [x % WORK_MOD for x in state]

        if n_terms > 1:
            T_pow = mat_pow_mod(T, n_terms - 1, WORK_MOD)
            new_state = [0] * 14
            for i in range(14):
                s = 0
                for j in range(14):
                    s += T_pow[i][j] * state[j]
                new_state[i] = s % WORK_MOD
            state = new_state

        Su3, Su2, Su1, S1 = state[10], state[11], state[12], state[13]
        contrib = (A_c * Su3 + B_c * Su2 + C_c * Su1 + D_c * S1) % WORK_MOD
        total_288 = (total_288 + contrib) % WORK_MOD

    return total_288 // 288

# ---------------------------------------------------------------------------
# Part 5: Verification and output
# ---------------------------------------------------------------------------

def verify():
    """Run verification checks."""
    print("=" * 60)
    print("VERIFICATION")
    print("=" * 60)

    # Check M(a,b,c)
    assert M_value(4, 2, 5) == 6, "M(4,2,5) should be 6"
    print("[PASS] M(4,2,5) = 6")

    # Check S(10) and S(10000)
    assert S_analytical(10) == 1972, "S(10) should be 1972"
    print("[PASS] S(10) = 1972")

    assert S_analytical(10000) == 2024258331114, "S(10000) should be 2024258331114"
    print("[PASS] S(10000) = 2024258331114")

    # Brute-force cross-check for S(10)
    assert S_brute(10) == 1972, "S_brute(10) should be 1972"
    print("[PASS] S_brute(10) = 1972 (brute-force confirmed)")

    # Verify polynomial representation for all N in [0, 100]
    int_polys = compute_piecewise_polys()
    for N in range(101):
        s = N % 24
        poly = int_polys[s]
        computed = poly[0] + poly[1] * N + poly[2] * N ** 2 + poly[3] * N ** 3
        actual = 288 * S_analytical(N)
        assert computed == actual, f"Polynomial mismatch at N={N}"
    print("[PASS] Polynomial representation verified for N=0..100")

    # Cross-validate with brute-force Fibonacci sum for K=20
    MOD = 10 ** 9
    fib = [0, 1]
    for i in range(2, 21):
        fib.append(fib[-1] + fib[-2])
    bf_sum = sum(S_analytical(fib[k]) for k in range(2, 21)) % MOD
    matrix_sum = solve(K=20)
    assert matrix_sum == bf_sum, f"K=20 mismatch: matrix={matrix_sum}, brute={bf_sum}"
    print(f"[PASS] K=20 cross-validation: {bf_sum}")

    print()

def generate_visualization():
    """Generate visualization and save as visualization.png."""
