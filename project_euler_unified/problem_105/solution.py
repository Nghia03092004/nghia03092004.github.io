"""
Problem 105: Special Subset Sums: Testing

From a file of 100 sets, find those that are special sum sets
and compute the sum of their S(A) values.

A special sum set satisfies:
1. All subset sums are distinct.
2. Larger subsets have larger sums (size-ordering property).

Approach: For each set, check Condition 2 in O(n), then Condition 1 in O(2^n).
"""

import os
import urllib.request

# ---------------------------------------------------------------------------
# Core functions
# ---------------------------------------------------------------------------
def is_special_sum_set(a) -> bool:
    """Check if sorted list a is a special sum set."""
    a = sorted(a)
    n = len(a)

    # Condition 2: sum of first k+1 elements > sum of last k elements
    for k in range(1, n // 2 + 1):
        if sum(a[:k + 1]) <= sum(a[n - k:]):
            return False

    # Condition 1: all 2^n - 1 nonempty subset sums must be distinct
    sums = set()
    for mask in range(1, 1 << n):
        s = sum(a[i] for i in range(n) if mask & (1 << i))
        if s in sums:
            return False
        sums.add(s)

    return True

def is_special_brute(a) -> bool:
    """Brute-force: check all pairs of disjoint subsets."""
    a = sorted(a)
    n = len(a)
    for b in range(1, 1 << n):
        for c in range(b + 1, 1 << n):
            if b & c:  # not disjoint
                continue
            sb = sum(a[i] for i in range(n) if b & (1 << i))
            sc = sum(a[i] for i in range(n) if c & (1 << i))
            nb = bin(b).count('1')
            nc = bin(c).count('1')
            if sb == sc:
                return False
            if nb > nc and sb <= sc:
                return False
            if nc > nb and sc <= sb:
                return False
    return True

# ---------------------------------------------------------------------------
# Data loading
# ---------------------------------------------------------------------------
def get_data():
    local_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), "p105_sets.txt")
    if os.path.exists(local_file):
        with open(local_file) as f:
            return f.read()
    url = "https://projecteuler.net/resources/documents/0105_sets.txt"
    try:
        response = urllib.request.urlopen(url)
        data = response.read().decode('utf-8')
        with open(local_file, 'w') as f:
            f.write(data)
        return data
    except Exception:
        raise RuntimeError("Could not download sets data. Place 'p105_sets.txt' locally.")

# ---------------------------------------------------------------------------
# Verify brute force matches optimized for small sets
# ---------------------------------------------------------------------------
test_sets = [
    [1, 2, 3, 5],       # special
    [1, 2, 3, 4],       # not special (1+4 = 2+3)
    [6, 9, 11, 12, 13], # special
]
for ts in test_sets:
    assert is_special_sum_set(ts) == is_special_brute(ts), f"Mismatch on {ts}"

# ---------------------------------------------------------------------------
# Main solve
# ---------------------------------------------------------------------------
def solve():
    data = get_data()
    total = 0
    special_sets = []

    for line in data.strip().split('\n'):
        line = line.strip()
        if not line:
            continue
        a = list(map(int, line.split(',')))
        if is_special_sum_set(a):
            s = sum(a)
            total += s
            special_sets.append((sorted(a), s))

    return total, special_sets

answer, special_sets = solve()
assert answer == 73702, f"Expected 73702, got {answer}"
print(answer)

# ---------------------------------------------------------------------------
# 4-Panel Visualization
# ---------------------------------------------------------------------------
