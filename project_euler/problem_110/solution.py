"""
Problem 110: Diophantine Reciprocals II

Find the least n such that the number of distinct solutions of
1/x + 1/y = 1/n exceeds 4,000,000.

Same approach as Problem 108 but with threshold 8,000,001 for d(n^2).
Uses log-space comparison to handle large n values.
"""

import math

PRIMES = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71]
LOG_PRIMES = [math.log(p) for p in PRIMES]
TARGET = 8_000_001

best_log_n = float('inf')
best_exponents = []

def search(idx, max_exp, log_n, div_count, exponents):
    global best_log_n, best_exponents

    if div_count >= TARGET:
        if log_n < best_log_n:
            best_log_n = log_n
            best_exponents = exponents[:]
        return

    if idx >= len(PRIMES):
        return

    if log_n >= best_log_n:
        return

    for e in range(1, max_exp + 1):
        new_log = log_n + e * LOG_PRIMES[idx]
        if new_log >= best_log_n:
            break
        exponents.append(e)
        search(idx + 1, e, new_log, div_count * (2 * e + 1), exponents)
        exponents.pop()

search(0, 40, 0.0, 1, [])

# Reconstruct n
n = 1
for i, e in enumerate(best_exponents):
    n *= PRIMES[i] ** e

print(n)

# Verification
if __name__ == "__main__":
    d_n2 = 1
    for e in best_exponents:
        d_n2 *= (2 * e + 1)
    solutions = (d_n2 + 1) // 2

    factors = {PRIMES[i]: best_exponents[i] for i in range(len(best_exponents))}
    print(f"n = {n}")
    print(f"Factorization: {factors}")
    print(f"d(n^2) = {d_n2}")
    print(f"Solutions = {solutions}")
    print(f"Exceeds 4,000,000: {solutions > 4_000_000}")
