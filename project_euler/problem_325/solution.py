"""
Problem 325: Stone Game II

S(N) = sum of (a+b) for all losing positions (a,b) with 0 < a < b <= N.
A position (a, a+r) is losing iff r <= floor(a*(sqrt(5)-1)/2).

Uses quadratic floor sum algorithm with Fibonacci rational approximation.
"""
from math import isqrt

MOD = 282475249  # 7^10
inv2 = pow(2, -1, MOD)
inv6 = pow(6, -1, MOD)

# Large Fibonacci numbers for rational approximation of (sqrt(5)-1)/2
fib = [0, 1]
for i in range(100):
    fib.append(fib[-1] + fib[-2])
P, Q = fib[79], fib[80]  # alpha ~ P/Q, exact for a <= 10^16

def S1(n):
    """sum_{x=0}^{n-1} x mod MOD"""
    return n % MOD * ((n - 1) % MOD) % MOD * inv2 % MOD

def S2(n):
    """sum_{x=0}^{n-1} x^2 mod MOD"""
    return n % MOD * ((n - 1) % MOD) % MOD * ((2 * n - 1) % MOD) % MOD * inv6 % MOD

def floor_sum_quad(N, a, b, m):
    """
    Compute mod MOD:
    f0 = sum_{x=0}^{N-1} floor((ax+b)/m)
    f1 = sum_{x=0}^{N-1} x * floor((ax+b)/m)
    f2 = sum_{x=0}^{N-1} floor((ax+b)/m)^2
    """
    if N <= 0:
        return (0, 0, 0)

    if a >= m:
        qa = a // m
        g0, g1, g2 = floor_sum_quad(N, a % m, b, m)
        s1, s2 = S1(N), S2(N)
        qa_m = qa % MOD
        f0 = (qa_m * s1 + g0) % MOD
        f1 = (qa_m * s2 + g1) % MOD
        f2 = (qa_m * qa_m % MOD * s2 + 2 * qa_m * g1 + g2) % MOD
        return (f0 % MOD, f1 % MOD, f2 % MOD)

    if b >= m:
        qb = b // m
        g0, g1, g2 = floor_sum_quad(N, a, b % m, m)
        qb_m, Nm = qb % MOD, N % MOD
        f0 = (qb_m * Nm + g0) % MOD
        f1 = (qb_m * S1(N) + g1) % MOD
        f2 = (qb_m * qb_m % MOD * Nm + 2 * qb_m * g0 + g2) % MOD
        return (f0 % MOD, f1 % MOD, f2 % MOD)

    if a == 0:
        return (0, 0, 0)

    M_val = (a * (N - 1) + b) // m
    if M_val == 0:
        return (0, 0, 0)

    g0, g1, g2 = floor_sum_quad(M_val, m, m - b - 1, a)

    Nm = (N - 1) % MOD
    Mm = M_val % MOD
    T = S1(N)

    f0 = (Nm * Mm - g0) % MOD
    f1 = (Mm * T - inv2 * (g2 + g0)) % MOD
    f2 = (Nm * Mm % MOD * Mm - 2 * g1 - g0) % MOD

    return (f0 % MOD, f1 % MOD, f2 % MOD)

def L(a):
    """L(a) = floor(a * (sqrt(5)-1)/2)"""
    s = isqrt(5 * a * a)
    while (s + 1) * (s + 1) <= 5 * a * a:
        s += 1
    return (s - a) // 2

def compute_S(N):
    """Compute S(N) mod MOD."""
    # Binary search for threshold a*: largest a with L(a) <= N - a
    lo, hi = 1, N - 1
    while lo < hi:
        mid = (lo + hi + 1) // 2
        if L(mid) <= N - mid:
            lo = mid
        else:
            hi = mid - 1
    a_star = lo

    # Range 1: a = 1..a*, R(a) = L(a)
    # Contribution = 2*f1 + (f2 + f0)/2
    f0_1, f1_1, f2_1 = floor_sum_quad(a_star + 1, P, 0, Q)
    range1 = (2 * f1_1 + inv2 * (f2_1 + f0_1)) % MOD

    # Range 2: a = a*+1..N-1, R(a) = N-a
    # Let j = N - a, j = 1..J where J = N - a* - 1
    # sum_{j=1}^{J} [(2N + 1/2)*j - 3/2*j^2]
    J = N - a_star - 1
    if J > 0:
        Jm = J % MOD
        J1m = (J + 1) % MOD
        J2m = (2 * J + 1) % MOD
        Nm = N % MOD
        sum_j = Jm * J1m % MOD * inv2 % MOD
        sum_j2 = Jm * J1m % MOD * J2m % MOD * inv6 % MOD
        range2 = ((2 * Nm + inv2) % MOD * sum_j - 3 * inv2 % MOD * sum_j2) % MOD
    else:
        range2 = 0

    return (range1 + range2) % MOD

def solve():
    N = 10**16
    result = compute_S(N)
    print(result)

if __name__ == "__main__":
    solve()
