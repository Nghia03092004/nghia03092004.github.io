"""
Problem 61: Cyclical Figurate Numbers

Find the sum of the only ordered set of six cyclic 4-digit numbers
for which each polygonal type (triangle through octagonal) is represented.
"""

from collections import defaultdict


def polygonal(s, n):
    """Return the n-th s-gonal number: P(s,n) = n*((s-2)*n - (s-4)) / 2."""
    return n * ((s - 2) * n - (s - 4)) // 2


def solve():
    # Generate 4-digit polygonal numbers for each type s in {3,...,8}
    poly = [[] for _ in range(6)]  # index 0..5 for types 3..8
    for s in range(3, 9):
        n = 1
        while True:
            val = polygonal(s, n)
            if val >= 10000:
                break
            if val >= 1000 and val % 100 >= 10:
                poly[s - 3].append(val)
            n += 1

    # Build adjacency: by_prefix[type][prefix] -> list of (suffix, value)
    by_prefix = [defaultdict(list) for _ in range(6)]
    for t in range(6):
        for v in poly[t]:
            p, s = divmod(v, 100)
            by_prefix[t][p].append((s, v))

    # DFS to find a 6-cycle using all six types
    def dfs(depth, used_mask, cur_suffix, start_prefix, chain):
        if depth == 6:
            if cur_suffix == start_prefix:
                return sum(chain)
            return None
        for t in range(6):
            if used_mask & (1 << t):
                continue
            for suf, val in by_prefix[t].get(cur_suffix, []):
                result = dfs(depth + 1, used_mask | (1 << t),
                             suf, start_prefix, chain + [val])
                if result is not None:
                    return result
        return None

    # Try starting with each type and each number
    for t in range(6):
        for v in poly[t]:
            p, s = divmod(v, 100)
            result = dfs(1, 1 << t, s, p, [v])
            if result is not None:
                return result


answer = solve()
assert answer == 28684
print(answer)
