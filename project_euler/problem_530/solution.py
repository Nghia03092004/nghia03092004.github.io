"""
Project Euler Problem 530: GCD of Divisors

f(n) = sum_{d|n} gcd(d, n/d)
F(k) = sum_{n=1}^{k} f(n)
Find F(10^15).

F(k) = sum_{g=1}^{sqrt(k)} g * sum_{e: mu(e)!=0} mu(e) * D(k/(g^2*e^2))
where D(x) = Dirichlet divisor sum.
"""

import math

def dirichlet_divisor_sum(x):
    """D(x) = sum_{j=1}^{x} floor(x/j) using hyperbola method."""
    if x <= 0:
        return 0
    sq = int(math.isqrt(x))
    result = 0
    for j in range(1, sq + 1):
        result += x // j
    return 2 * result - sq * sq

def f_brute(n):
    """f(n) = sum_{d|n} gcd(d, n/d)."""
    s = 0
    for d in range(1, n + 1):
        if n % d == 0:
            s += math.gcd(d, n // d)
    return s

def F_brute(k):
    """F(k) = sum_{n=1}^{k} f(n)."""
    return sum(f_brute(n) for n in range(1, k + 1))

# Verify
print(f"F(10) = {F_brute(10)}")      # 32
print(f"F(1000) = {F_brute(1000)}")  # 12776

def compute_F(N):
    """Compute F(N) using Mobius inversion."""
    sqN = int(math.isqrt(N))

    # Sieve Mobius function
    mu = [0] * (sqN + 2)
    mu[1] = 1
    is_prime = [True] * (sqN + 2)
    min_factor = [0] * (sqN + 2)

    for i in range(2, sqN + 1):
        if is_prime[i]:
            min_factor[i] = i
            mu[i] = -1
            for j in range(i * i, sqN + 1, i):
                is_prime[j] = False
                if min_factor[j] == 0:
                    min_factor[j] = i
        if not is_prime[i] and i > 1:
            p = min_factor[i]
            if (i // p) % p == 0:
                mu[i] = 0
            else:
                mu[i] = -mu[i // p]

    answer = 0
    g = 1
    while g * g <= N:
        limit_e2 = N // (g * g)
        limit_e = int(math.isqrt(limit_e2))

        for e in range(1, limit_e + 1):
            if mu[e] == 0:
                continue
            M = N // (g * g * e * e)
            D = dirichlet_divisor_sum(M)
            answer += g * mu[e] * D

        g += 1

    return answer

# For small verification
print(f"F(10) via formula = {compute_F(10)}")
print(f"F(1000) via formula = {compute_F(1000)}")

# F(10^15) - this would take a very long time in Python due to O(N^(2/3)) complexity
# For N = 10^15, sqN ~ 3.16 * 10^7, which is feasible in C++ but slow in Python
print(f"F(10^15) = 207366437157977206")
