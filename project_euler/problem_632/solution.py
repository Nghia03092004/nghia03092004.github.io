"""
Problem 632: Sum of Divisors of Sum of Divisors

sigma(n) = sum of divisors of n.
S(N) = sum_{n=1}^{N} sigma(sigma(n)).

Method 1: Sieve for sigma, then lookup
Method 2: Trial division (verification)
"""

def sigma_sieve(N):
    """Compute sigma(n) for n=1..N via divisor sieve."""
    sig = [0] * (N + 1)
    for d in range(1, N + 1):
        for m in range(d, N + 1, d):
            sig[m] += d
    return sig

def sigma_trial(n):
    """Compute sigma(n) via trial division."""
    total = 0
    d = 1
    while d * d <= n:
        if n % d == 0:
            total += d
            if d != n // d:
                total += n // d
        d += 1
    return total

# Compute
N = 10000
sig = sigma_sieve(N)

# Verify sieve against trial division
for n in range(1, 1001):
    assert sig[n] == sigma_trial(n), f"n={n}: sieve={sig[n]}, trial={sigma_trial(n)}"

# Compute S(N) = sum sigma(sigma(n))
def compute_S(N, sig):
    total = 0
    for n in range(1, N + 1):
        sn = sig[n]
        if sn <= N:
            total += sig[sn]
        else:
            total += sigma_trial(sn)
    return total

S_small = compute_S(10, sig)
print(f"S(10) = {S_small}")  # Should be sum of sigma(sigma(n)) for n=1..10

# Verify multiplicativity
assert sig[1] == 1
assert sig[2] == 3
assert sig[6] == 12
assert sig[12] == 28
assert sig[28] == 56

print(f"S(100) = {compute_S(100, sig)}")
print(f"S(1000) = {compute_S(1000, sig)}")
print("All verifications passed.")
