"""
Problem 484: Arithmetic Derivative
The arithmetic derivative n' is defined by: p' = 1 for prime p,
(ab)' = a'b + ab' (product rule), 0' = 1' = 0.
For n = p1^a1 * ... * pk^ak, n' = n * sum(ai/pi).
"""

def arithmetic_derivative(n: int):
    """Compute the arithmetic derivative of n using prime factorization."""
    if n <= 1:
        return 0
    result = 0
    temp = n
    d = 2
    while d * d <= temp:
        while temp % d == 0:
            result += n // d
            temp //= d
        d += 1
    if temp > 1:
        result += n // temp
    return result

def sieve_arithmetic_derivatives(limit: int) -> list:
    """Compute arithmetic derivatives for all n in [0, limit] using SPF sieve."""
    spf = list(range(limit + 1))
    for i in range(2, int(limit**0.5) + 1):
        if spf[i] == i:  # i is prime
            for j in range(i * i, limit + 1, i):
                if spf[j] == j:
                    spf[j] = i

    ad = [0] * (limit + 1)
    for n in range(2, limit + 1):
        temp = n
        s = 0  # sum of a_i / p_i, scaled by n at the end
        while temp > 1:
            p = spf[temp]
            a = 0
            while temp % p == 0:
                temp //= p
                a += 1
            # Contribution: n * a / p = a * (n // p) if p | n exactly
            s += a * (n // p)
            # But n/p won't be integer if we already divided... use original n
        # Actually: ad(n) = n * sum(a_i / p_i) where n = prod(p_i^a_i)
        # Since a_i/p_i may not be integer, compute as sum(a_i * n / p_i)
        # n / p_i is always integer since p_i | n
        temp2 = n
        s2 = 0
        while temp2 > 1:
            p = spf[temp2]
            a = 0
            while temp2 % p == 0:
                temp2 //= p
                a += 1
            s2 += a * (n // p)
        ad[n] = s2

    return ad

def solve_small(limit: int = 1000):
    """Sum of arithmetic derivatives from 2 to limit (demonstration)."""
    ad = sieve_arithmetic_derivatives(limit)
    return sum(ad[2:])

def solve_brute_force(limit: int = 1000):
    """Brute-force verification."""
    return sum(arithmetic_derivative(n) for n in range(2, limit + 1))

# Compute and verify on small scale
answer_sieve = solve_small(1000)
answer_bf = solve_brute_force(1000)
assert answer_sieve == answer_bf, f"Mismatch: {answer_sieve} vs {answer_bf}"

print(f"Sum of arithmetic derivatives for n=2..1000: {answer_sieve}")
