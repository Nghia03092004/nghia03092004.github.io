"""
Problem 975: Zeckendorf Representation

Compute the sum of z(n) for n = 1 to 10^6, where z(n) is the number of terms
in the Zeckendorf (greedy Fibonacci) representation of n.

Every positive integer has a unique representation as a sum of non-consecutive
Fibonacci numbers (Zeckendorf's theorem). z(n) counts how many Fibonacci numbers
are used.

Key results:
    - Greedy algorithm: subtract largest Fibonacci number <= n, repeat
    - The representation never uses two consecutive Fibonacci numbers
    - z(n) grows roughly as log_phi(n) on average
    - answer = sum of z(n) for n = 1..10^6

Methods:
    1. build_fibonacci       — generate Fibonacci numbers up to N
    2. zeckendorf_count      — greedy Fibonacci decomposition, return term count
    3. zeckendorf_decompose  — full decomposition returning the Fibonacci terms
    4. average_z_by_range    — mean z(n) over ranges for growth analysis
"""

from collections import Counter

def build_fibonacci(N):
    """Return list of Fibonacci numbers (starting 1, 2, 3, 5, ...) up to N."""
    fibs = [1, 2]
    while fibs[-1] < N:
        fibs.append(fibs[-1] + fibs[-2])
    return fibs

def zeckendorf_count(n, fibs):
    """Number of Fibonacci numbers in Zeckendorf representation of n."""
    c = 0
    for f in reversed(fibs):
        if f <= n:
            n -= f
            c += 1
    return c

def zeckendorf_decompose(n, fibs):
    """Return the list of Fibonacci numbers used in the Zeckendorf representation."""
    terms = []
    for f in reversed(fibs):
        if f <= n:
            n -= f
            terms.append(f)
    return terms

def average_z_by_range(N, fibs, num_bins=50):
    """Compute mean z(n) in equal-sized bins across [1, N]."""
    bin_size = N // num_bins
    bin_centers = []
    bin_means = []
    for i in range(num_bins):
        lo = i * bin_size + 1
        hi = (i + 1) * bin_size
        total = sum(zeckendorf_count(n, fibs) for n in range(lo, hi + 1))
        bin_centers.append((lo + hi) / 2)
        bin_means.append(total / bin_size)
    return bin_centers, bin_means

#  Verification
_fibs = build_fibonacci(100)
# 1 = F(1): z=1; 2 = F(2): z=1; 3 = F(3): z=1; 4 = F(3)+F(1) = 3+1: z=2
# 5 = F(4): z=1; 6 = F(4)+F(1) = 5+1: z=2; 7 = F(4)+F(2) = 5+2: z=2
# 8 = F(5): z=1; 10 = 8+2: z=2; 11 = 8+3: z=2; 12 = 8+3+1: z=3
assert zeckendorf_count(1, _fibs) == 1
assert zeckendorf_count(4, _fibs) == 2
assert zeckendorf_count(8, _fibs) == 1
assert zeckendorf_count(12, _fibs) == 3
assert zeckendorf_decompose(12, _fibs) == [8, 3, 1]

# No consecutive Fibonacci numbers in decomposition
for test_n in [20, 50, 99]:
    terms = zeckendorf_decompose(test_n, _fibs)
    fib_set = set(_fibs)
    for i in range(len(terms) - 1):
        idx_a = _fibs.index(terms[i])
        idx_b = _fibs.index(terms[i + 1])
        assert abs(idx_a - idx_b) >= 2, f"Consecutive Fibs in decomposition of {test_n}"

#  Main computation
N = 10 ** 6
fibs = build_fibonacci(N)
total = sum(zeckendorf_count(n, fibs) for n in range(1, N + 1))
print(total)

#  Visualization — 4-panel figure
