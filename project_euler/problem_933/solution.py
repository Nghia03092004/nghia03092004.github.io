"""
Problem 933: Digit Factorial Chains

Define f(n) = sum of factorials of digits of n. Starting from any n,
repeatedly apply f to form a chain until a value repeats. Let L(n) be
the number of distinct values in this chain. Compute sum_{n=1}^{N} L(n).

Key observations:
  - For any n, f(n) <= 9! * digits(n) = 362880 * 7 = 2540160 for n <= 10^7.
  - Once the chain enters a cycle, all cycle members have the same L.
  - Pre-computing chain lengths for all values up to 2540160 allows O(1) lookup.

Methods:
  - digit_fact_sum: Compute f(n).
  - solve_memoized: Full DP approach with cycle detection for large N.
  - chain_length_brute: Simple set-based chain length for small n.

Complexity: O(2540160) precomputation + O(N) for final summation.
"""

from collections import Counter

FACT = [1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880]

# Core: digit factorial sum
def digit_fact_sum(n):
    """Sum of factorials of digits of n."""
    s = 0
    while n > 0:
        s += FACT[n % 10]
        n //= 10
    return s

def chain_length_brute(n):
    """Compute L(n) by following the chain until a repeat."""
    seen = set()
    cur = n
    while cur not in seen:
        seen.add(cur)
        cur = digit_fact_sum(cur)
    return len(seen)

def solve_memoized(N=10**6):
    """Sum of L(n) for n=1..N using memoized chain lengths."""
    LIMIT = 2540161
    chain_len = [0] * LIMIT
    computed = [False] * LIMIT

    for start in range(1, LIMIT):
        if computed[start]:
            continue
        path = []
        visited = {}
        cur = start
        while cur not in visited and cur < LIMIT and not computed[cur]:
            visited[cur] = len(path)
            path.append(cur)
            cur = digit_fact_sum(cur)
        if cur < LIMIT and computed[cur]:
            for i in range(len(path) - 1, -1, -1):
                chain_len[path[i]] = len(path) - i + chain_len[cur]
                computed[path[i]] = True
        elif cur in visited:
            cycle_start = visited[cur]
            cycle_len = len(path) - cycle_start
            for i in range(cycle_start, len(path)):
                chain_len[path[i]] = cycle_len
                computed[path[i]] = True
            for i in range(cycle_start - 1, -1, -1):
                chain_len[path[i]] = len(path) - i
                computed[path[i]] = True

    total = 0
    for n in range(1, N + 1):
        if n < LIMIT:
            total += chain_len[n] if chain_len[n] > 0 else 1
        else:
            nxt = digit_fact_sum(n)
            total += 1 + (chain_len[nxt] if chain_len[nxt] > 0 else 1)
    return total

# Verification
# Known chains:
# 169 -> 363601 -> 1454 -> 169 (cycle of 3)
assert chain_length_brute(169) == 3
# 1 -> 1 (cycle of 1)
assert chain_length_brute(1) == 1
# 2 -> 2 (cycle of 1)
assert chain_length_brute(2) == 1
# 145 -> 145 (cycle of 1: 1!+4!+5! = 1+24+120 = 145)
assert chain_length_brute(145) == 1
# 871 -> 45361 -> 871 (cycle of length 2... let's verify)
assert digit_fact_sum(871) == 45361
assert digit_fact_sum(45361) == 871

# Compute answer (small demo)
lengths = [chain_length_brute(n) for n in range(1, 1001)]
total_1000 = sum(lengths)
print(f"Sum of L(n) for n=1..1000: {total_1000}")
