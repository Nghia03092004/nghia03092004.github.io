"""
Project Euler Problem 896: Divisible Ranges

A contiguous range [a, a+1, ..., a+n-1] of length n is a divisible range
if the integers can be arranged so that the k-th term is a multiple of k.

Find the 36th divisible range of length 36. Give the smallest number in the range.
"""

from scipy.optimize import linear_sum_assignment

def is_divisible_range(a, n):
    """Check if [a, a+n-1] is a divisible range using maximum bipartite matching."""
    # Use Hopcroft-Karp via networkx or manual implementation
    # We'll use a simple augmenting path approach

    # adj[k] = list of value indices that position k+1 can match (0-indexed)
    adj = [[] for _ in range(n)]
    for k in range(1, n + 1):
        first_mult = ((a + k - 1) // k) * k
        v = first_mult
        while v <= a + n - 1:
            adj[k - 1].append(v - a)
            v += k

    # Hopcroft-Karp
    match_l = [-1] * n
    match_r = [-1] * n

    def bfs():
        dist = [0] * n
        queue = []
        for u in range(n):
            if match_l[u] == -1:
                dist[u] = 0
                queue.append(u)
            else:
                dist[u] = float('inf')
        found = False
        qi = 0
        while qi < len(queue):
            u = queue[qi]; qi += 1
            for v in adj[u]:
                w = match_r[v]
                if w == -1:
                    found = True
                elif dist[w] == float('inf'):
                    dist[w] = dist[u] + 1
                    queue.append(w)
        return found, dist

    def dfs(u, dist):
        for v in adj[u]:
            w = match_r[v]
            if w == -1 or (dist[w] == dist[u] + 1 and dfs(w, dist)):
                match_l[u] = v
                match_r[v] = u
                return True
        dist[u] = float('inf')
        return False

    matching = 0
    while True:
        found, dist = bfs()
        if not found:
            break
        for u in range(n):
            if match_l[u] == -1:
                if dfs(u, dist):
                    matching += 1

    return matching == n

def solve():
    n = 36
    target = 36
    count = 0
    a = 1

    while True:
        if is_divisible_range(a, n):
            count += 1
            if count == target:
                print(a)
                return
        a += 1

if __name__ == "__main__":
    solve()
