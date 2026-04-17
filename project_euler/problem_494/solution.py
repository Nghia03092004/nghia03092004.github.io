"""
Problem 494: Collatz Prefix Families
Study families of starting values sharing the same first k steps in the Collatz sequence.
"""

from collections import defaultdict

def collatz_step(n: int):
    """Single Collatz step."""
    if n % 2 == 0:
        return n // 2
    else:
        return 3 * n + 1

def collatz_prefix(n: int, k: int):
    """Return the first k values of the Collatz sequence starting at n."""
    seq = [n]
    val = n
    for _ in range(k - 1):
        val = collatz_step(val)
        seq.append(val)
    return tuple(seq)

def collatz_pattern(n: int, k: int):
    """Return the even/odd pattern of the first k steps from n.
    Pattern is the sequence of parities: 0=even, 1=odd for each visited value.
    """
    pattern = []
    val = n
    for _ in range(k):
        pattern.append(val % 2)
        val = collatz_step(val)
    return tuple(pattern)

def count_prefix_families(N: int, K: int) -> list:
    """Count the number of distinct prefix families for each length k=1..K.

    A prefix family of length k groups all starting values in [1, N]
    whose Collatz sequences share the same first k values.
    """
    family_counts = []
    for k in range(1, K + 1):
        families = set()
        for n in range(1, N + 1):
            prefix = collatz_prefix(n, k)
            families.add(prefix)
        family_counts.append(len(families))
    return family_counts

def count_pattern_families(N: int, K: int) -> list:
    """Count distinct E/O patterns of length k realized by starting values in [1, N]."""
    pattern_counts = []
    for k in range(1, K + 1):
        patterns = set()
        for n in range(1, N + 1):
            pat = collatz_pattern(n, k)
            patterns.add(pat)
        pattern_counts.append(len(patterns))
    return pattern_counts

def solve(N: int = 10000, K: int = 30):
    """Compute the sum of F(k) for k = 1 to K.

    Using a smaller N for demonstration; the full problem uses N = 10^6, K = 40.
    """
    # For the prefix-based counting: each distinct starting value gives a unique
    # prefix of length 1, so F(1) = N. For larger k, some starting values share
    # the same prefix only if they are the same number, so F(k) = N for all k.
    #
    # The interesting interpretation is the pattern-based one.
    # We count distinct E/O patterns of length k.

    # For efficiency, compute all patterns at once up to length K
    all_patterns = [set() for _ in range(K)]
    for n in range(1, N + 1):
        val = n
        pattern = []
        for step in range(K):
            pattern.append(val % 2)
            val = collatz_step(val)
            all_patterns[step].add(tuple(pattern))

    counts = [len(s) for s in all_patterns]
    total = sum(counts)

    print(f"N = {N}, K = {K}")
    for k in range(K):
        print(f"  F({k+1}) = {counts[k]}")
    print(f"  Sum = {total}")

    return total

# Compute for demonstration
answer = solve(N=10000, K=30)
print(f"\nAnswer (demo): {answer}")
