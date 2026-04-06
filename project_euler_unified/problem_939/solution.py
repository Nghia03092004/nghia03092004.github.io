"""
Problem 939: Goldbach Partition Counting

Compute the sum of G(n) for all even n in [4, N], where G(n) is the number
of ways to write n = p + q with p <= q and both prime (Goldbach partitions).

Key observations:
    - G(4) = 1 (2+2), G(6) = 1 (3+3), G(8) = 1 (3+5), G(10) = 2 (3+7, 5+5)
    - G(n) tends to grow roughly like n / (2 * ln(n)^2) for large n
    - The sum of G(n) counts total Goldbach representations up to N

Results:
    - Sum of G(n) for even n up to 10000 computed below

Methods:
    1. sieve_of_eratosthenes  -- generate prime flags up to N
    2. goldbach_count         -- G(n) for a single even n
    3. sum_goldbach           -- sum of G(n) for even n in [4, N]
    4. goldbach_ratio         -- G(n) vs n/ln(n)^2 heuristic
"""
import numpy as np

def sieve_of_eratosthenes(N):
    """Return boolean list where sieve[i] = True iff i is prime."""
    sieve = [True] * (N + 1)
    sieve[0] = sieve[1] = False
    for i in range(2, int(N**0.5) + 1):
        if sieve[i]:
            for j in range(i * i, N + 1, i):
                sieve[j] = False
    return sieve

def goldbach_count(n, sieve):
    """G(n) = number of pairs (p, q) with p <= q, p+q = n, both prime."""
    count = 0
    for p in range(2, n // 2 + 1):
        if sieve[p] and sieve[n - p]:
            count += 1
    return count

def sum_goldbach(N):
    """Compute sum of G(n) for all even n from 4 to N."""
    sieve = sieve_of_eratosthenes(N)
    total = 0
    g_vals = []
    for n in range(4, N + 1, 2):
        g = goldbach_count(n, sieve)
        g_vals.append(g)
        total += g
    return total, g_vals, sieve

def goldbach_ratio(n, g_n):
    """Ratio G(n) / (n / ln(n)^2) -- Hardy-Littlewood estimate."""
    if n <= 2 or g_n == 0:
        return 0.0
    ln_n = np.log(n)
    return g_n / (n / (ln_n ** 2))

# Verification
sieve_small = sieve_of_eratosthenes(100)
assert goldbach_count(4, sieve_small) == 1    # 2+2
assert goldbach_count(6, sieve_small) == 1    # 3+3
assert goldbach_count(8, sieve_small) == 1    # 3+5
assert goldbach_count(10, sieve_small) == 2   # 3+7, 5+5
assert goldbach_count(20, sieve_small) == 2   # 3+17, 7+13

# Computation
N = 10000
answer, g_vals, sieve = sum_goldbach(N)
print(answer)
