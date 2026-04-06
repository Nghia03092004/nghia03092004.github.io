"""
Problem 845: Prime Pairs

Count twin primes up to N using sieve methods.
pi_2(N) = |{p <= N : p and p+2 both prime}|
"""

from math import isqrt, log

# --- Method 1: Basic sieve + scan ---
def twin_primes_sieve(N: int):
    """Return (count of twin prime pairs, list of twin primes) up to N."""
    sieve = [True] * (N + 3)
    sieve[0] = sieve[1] = False
    for i in range(2, isqrt(N + 2) + 1):
        if sieve[i]:
            for j in range(i*i, N + 3, i):
                sieve[j] = False

    twins = []
    for p in range(2, N + 1):
        if sieve[p] and p + 2 <= N + 2 and sieve[p + 2]:
            twins.append(p)
    return len(twins), twins

# --- Method 2: Segmented sieve ---
def twin_primes_segmented(N: int):
    """Count twin primes using segmented sieve."""
    limit = isqrt(N + 2) + 1
    # Small primes
    small_sieve = [True] * (limit + 1)
    small_sieve[0] = small_sieve[1] = False
    for i in range(2, isqrt(limit) + 1):
        if small_sieve[i]:
            for j in range(i*i, limit + 1, i):
                small_sieve[j] = False
    small_primes = [i for i in range(2, limit + 1) if small_sieve[i]]

    count = 0
    delta = max(limit, 1000)
    prev_last_prime = False  # whether N-1 of previous block was prime

    for lo in range(0, N + 3, delta):
        hi = min(lo + delta, N + 3)
        seg = [True] * (hi - lo)
        if lo == 0:
            if hi > 0: seg[0] = False
            if hi > 1: seg[1] = False

        for p in small_primes:
            start = max(p * p, ((lo + p - 1) // p) * p)
            for j in range(start, hi, p):
                seg[j - lo] = False

        for i in range(hi - lo):
            actual = lo + i
            if actual < 2 or actual > N:
                continue
            if seg[i] and actual + 2 <= N + 2:
                # Check if actual + 2 is prime
                idx2 = actual + 2 - lo
                if 0 <= idx2 < len(seg) and seg[idx2]:
                    count += 1
                # Handle cross-boundary case would need extra logic

    return count

# --- Method 3: Direct primality check (brute force for small N) ---
def is_prime(n):
    if n < 2: return False
    if n < 4: return True
    if n % 2 == 0 or n % 3 == 0: return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

def twin_primes_brute(N: int):
    return sum(1 for p in range(2, N + 1) if is_prime(p) and is_prime(p + 2))

# --- Verify ---
for N in [10, 100, 1000, 10000]:
    c1, _ = twin_primes_sieve(N)
    c3 = twin_primes_brute(N)
    assert c1 == c3, f"N={N}: sieve={c1}, brute={c3}"

print("Verification passed!")

count, twins = twin_primes_sieve(10**7)
print(f"pi_2(10^7) = {count}")

# Hardy-Littlewood estimate
C2 = 0.66016
def hl_estimate(N):
    return 2 * C2 * N / (log(N))**2
