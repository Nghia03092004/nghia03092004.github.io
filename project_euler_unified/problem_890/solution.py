"""
Problem 890: Birthday Paradox Variants
Collision probability: P(n,N) = 1 - prod(1-k/N) for k=0..n-1.
"""

from math import log, exp, comb, factorial, sqrt

def birthday_exact(n, N):
    """Exact collision probability."""
    if n > N:
        return 1.0
    prob_no_collision = 1.0
    for k in range(n):
        prob_no_collision *= (N - k) / N
    return 1.0 - prob_no_collision

def birthday_poisson(n, N):
    """Poisson approximation."""
    lam = n * (n - 1) / (2 * N)
    return 1.0 - exp(-lam)

def threshold_50(N):
    """Find smallest n where P(n,N) >= 0.5."""
    for n in range(1, N + 2):
        if birthday_exact(n, N) >= 0.5:
            return n
    return N + 1

def birthday_kwise(n, N, k):
    """Approximate probability of k-wise collision."""
    lam = comb(n, k) / N**(k-1)
    return 1.0 - exp(-lam)

# --- Verification ---
print("=== Classical Birthday Problem (N=365) ===")
N = 365
print(f"{'n':>4} {'Exact':>10} {'Poisson':>10} {'Diff':>10}")
for n in [5, 10, 20, 23, 30, 40, 50, 57, 70]:
    exact = birthday_exact(n, N)
    poisson = birthday_poisson(n, N)
    print(f"{n:>4} {exact:>10.5f} {poisson:>10.5f} {abs(exact-poisson):>10.5f}")

print(f"\n=== 50% Threshold ===")
for N in [100, 365, 1000, 10000, 100000]:
    n50 = threshold_50(N)
    approx = 1.1774 * sqrt(N)
    print(f"  N={N:>6}: n_50={n50:>4}, 1.177*sqrt(N)={approx:.1f}")

print(f"\n=== Hash Collision ===")
for bits in [32, 64, 128, 256]:
    N = 2**bits
    n50_approx = int(1.1774 * sqrt(N))
    print(f"  {bits}-bit hash: 50% collision at ~2^{bits//2} = {2**(bits//2)}")

print(f"\n=== k-wise Collisions (N=365) ===")
for k in [2, 3, 4]:
    for n in [23, 50, 100, 200]:
        p = birthday_kwise(n, 365, k)
        print(f"  k={k}, n={n}: P ~ {p:.5f}")

answer = birthday_exact(23, 365)
print(f"\nAnswer: P(23, 365) = {answer:.10f}")

# --- 4-Panel Visualization ---
