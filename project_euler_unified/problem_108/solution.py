"""
Problem 108: Diophantine Reciprocals I

1/x + 1/y = 1/n  =>  (x-n)(y-n) = n^2
Number of solutions = ceil(d(n^2) / 2)
d(n^2) = product of (2*a_i + 1) for prime factorization n = prod(p_i^a_i)
Need d(n^2) >= 2001.

Minimize n by searching over non-increasing exponent sequences
assigned to primes 2, 3, 5, 7, ...
"""

import math

PRIMES = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]

best_n = float('inf')

def search(idx, max_exp, current_n, current_div):
    global best_n

    if current_div >= 2001:
        best_n = min(best_n, current_n)
        return

    if idx >= len(PRIMES):
        return

    if current_n >= best_n:
        return

    pw = 1
    for e in range(1, max_exp + 1):
        pw *= PRIMES[idx]
        if current_n * pw >= best_n:
            break
        search(idx + 1, e, current_n * pw, current_div * (2 * e + 1))

search(0, 30, 1, 1)
print(best_n)

# Verification
if __name__ == "__main__":
    n = best_n
    # Factor n to verify
    temp = n
    factors = {}
    for p in PRIMES:
        while temp % p == 0:
            factors[p] = factors.get(p, 0) + 1
            temp //= p

    d_n2 = 1
    for a in factors.values():
        d_n2 *= (2 * a + 1)

    solutions = (d_n2 + 1) // 2
    print(f"n = {n}, factorization = {factors}")
    print(f"d(n^2) = {d_n2}, solutions = {solutions}")
