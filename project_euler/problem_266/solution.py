"""
Problem 266: Pseudo Square Root

Find the largest divisor d of the product P of all primes below 190
such that d <= sqrt(P). Give the last 16 digits of d.

Approach: Meet-in-the-middle on the 42 primes.
Split into two halves, enumerate subset products for each,
then for each product in the second half, binary search the first half
for the largest complementary product that keeps d <= sqrt(P).
"""

from math import log
from bisect import bisect_right

def solve():
    # Sieve primes below 190
    primes = []
    for i in range(2, 190):
        if all(i % j != 0 for j in range(2, int(i**0.5) + 1)):
            primes.append(i)

    n = len(primes)  # 42
    half = n // 2     # 21
    MOD = 10**16

    # log(sqrt(P))
    log_sqrtP = sum(log(p) for p in primes) / 2.0

    # Generate subset products for first half
    sz1 = 1 << half
    groupA = []
    for mask in range(sz1):
        lg = 0.0
        val = 1
        for i in range(half):
            if mask & (1 << i):
                lg += log(primes[i])
                val = (val * primes[i]) % MOD
        groupA.append((lg, val))

    # Sort by log
    groupA.sort()
    sorted_logs = [x[0] for x in groupA]
    sorted_mods = [x[1] for x in groupA]

    best_log = -1.0
    best_mod = 0

    # For each subset of second half
    other = n - half
    sz2 = 1 << other
    for mask in range(sz2):
        lg = 0.0
        val = 1
        for i in range(other):
            if mask & (1 << i):
                lg += log(primes[half + i])
                val = (val * primes[half + i]) % MOD

        target = log_sqrtP - lg
        if target < -1e-15:
            continue

        # Binary search: largest index where sorted_logs[idx] <= target
        idx = bisect_right(sorted_logs, target + 1e-15) - 1
        if idx < 0:
            continue

        # Check a few neighbors for float safety
        for j in range(max(0, idx - 2), min(sz1, idx + 3)):
            if sorted_logs[j] > target + 1e-15:
                continue
            total_log = sorted_logs[j] + lg
            if total_log > best_log + 1e-15:
                best_log = total_log
                best_mod = (sorted_mods[j] * val) % MOD
            elif abs(total_log - best_log) < 1e-12:
                candidate = (sorted_mods[j] * val) % MOD
                if candidate > best_mod:
                    best_mod = candidate

    print(best_mod)

solve()
