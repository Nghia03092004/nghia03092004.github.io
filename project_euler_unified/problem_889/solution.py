"""
Problem 889: Reversal Sort (Pancake Sorting)
Prefix reversal sorting, pancake numbers, BFS on permutation graph.
"""

from itertools import permutations
from collections import deque

def prefix_reverse(perm, k):
    """Reverse the first k elements of perm."""
    return perm[:k][::-1] + perm[k:]

def pancake_bfs(n):
    """BFS from identity to find distances of all permutations of S_n."""
    identity = tuple(range(1, n + 1))
    dist = {identity: 0}
    queue = deque([identity])
    while queue:
        perm = queue.popleft()
        d = dist[perm]
        for k in range(2, n + 1):
            new_perm = prefix_reverse(perm, k)
            if new_perm not in dist:
                dist[new_perm] = d + 1
                queue.append(new_perm)
    return dist

def breakpoints(perm):
    """Count breakpoints: positions where |pi[i] - pi[i+1]| != 1."""
    n = len(perm)
    extended = [0] + list(perm) + [n + 1]
    b = sum(1 for i in range(len(extended) - 1) if abs(extended[i] - extended[i+1]) != 1)
    return b

# --- Verification ---
print("=== Pancake BFS ===")
for n in range(1, 7):
    dist = pancake_bfs(n)
    P_n = max(dist.values())
    total_dist = sum(dist.values())
    avg = total_dist / len(dist)
    # Distance distribution
    dist_counts = {}
    for d in dist.values():
        dist_counts[d] = dist_counts.get(d, 0) + 1
    print(f"  n={n}: P(n)={P_n}, avg={avg:.3f}, dist={dict(sorted(dist_counts.items()))}")

print("\n=== Breakpoint Lower Bound (n=4) ===")
dist4 = pancake_bfs(4)
for perm in sorted(dist4.keys())[:10]:
    d = dist4[perm]
    b = breakpoints(perm)
    lb = (b + 1) // 2
    print(f"  {perm}: d={d}, breakpoints={b}, lb={lb}, "
          f"{'OK' if d >= lb else 'FAIL'}")
    assert d >= lb

print("\n=== Known Pancake Numbers ===")
known = {1: 0, 2: 1, 3: 3, 4: 4, 5: 5, 6: 7}
for n, expected in known.items():
    dist = pancake_bfs(n)
    P_n = max(dist.values())
    print(f"  P({n}) = {P_n}, expected {expected}: {'OK' if P_n == expected else 'FAIL'}")
    assert P_n == expected

answer = max(pancake_bfs(6).values())
print(f"\nAnswer: P(6) = {answer}")

# --- 4-Panel Visualization ---
