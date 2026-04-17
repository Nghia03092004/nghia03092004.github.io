"""
Problem 916: Graph Isomorphism Counting

Find the number of non-isomorphic simple graphs on 7 vertices.

Key ideas:
    - Burnside's lemma: orbit count = (1/|G|) * sum of fixed points.
    - G = S_n acts on graphs by permuting vertices.
    - A permutation pi fixes 2^{c(pi)} graphs, where c(pi) = number of
      orbits of pi on the edge set.
    - c(pi) depends only on the cycle type of pi.
    - For cycle type (l1, l2, ...):
        c = sum_{i<j} gcd(li, lj) + sum_i floor(li/2)

Methods:
    1. Direct Burnside: enumerate all n! permutations
    2. Cycle-type Burnside: group by conjugacy classes (partitions of n)
    3. Verification against OEIS A000088
"""

from itertools import permutations
from math import factorial, gcd
from collections import Counter

def count_graphs_direct(n: int) -> int:
    """Count non-isomorphic graphs on n vertices via direct Burnside."""
    edges = [(i, j) for i in range(n) for j in range(i + 1, n)]
    m = len(edges)
    edge_idx = {e: i for i, e in enumerate(edges)}

    total = 0
    for perm in permutations(range(n)):
        # Find orbits of perm acting on edges
        visited = [False] * m
        cycles = 0
        for i in range(m):
            if not visited[i]:
                cycles += 1
                j = i
                while not visited[j]:
                    visited[j] = True
                    u, v = edges[j]
                    pu, pv = perm[u], perm[v]
                    if pu > pv:
                        pu, pv = pv, pu
                    j = edge_idx[(pu, pv)]
        total += 2 ** cycles
    return total // factorial(n)

def partitions(n: int, max_val=None):
    """Generate all partitions of n (as sorted tuples, descending)."""
    if max_val is None:
        max_val = n
    if n == 0:
        yield ()
        return
    for first in range(min(n, max_val), 0, -1):
        for rest in partitions(n - first, first):
            yield (first,) + rest

def edge_orbits_from_cycle_type(cycle_type: tuple) -> int:
    """Number of edge orbits for a permutation with given cycle type.

    Formula: sum_{i<j} gcd(l_i, l_j) + sum_i floor(l_i / 2)
    """
    k = len(cycle_type)
    c = 0
    # Inter-cycle edges
    for i in range(k):
        for j in range(i + 1, k):
            c += gcd(cycle_type[i], cycle_type[j])
    # Intra-cycle edges
    for li in cycle_type:
        c += li // 2
    return c

def count_perms_with_cycle_type(n: int, cycle_type: tuple) -> int:
    """Number of permutations in S_n with given cycle type."""
    # n! / prod(l_i^a_i * a_i!) where a_i = multiplicity of cycle length l_i
    freq = Counter(cycle_type)
    denom = 1
    for length, count in freq.items():
        denom *= (length ** count) * factorial(count)
    return factorial(n) // denom

def count_graphs_cycle_type(n: int) -> int:
    """Count non-isomorphic graphs via cycle-type Burnside."""
    total = 0
    for part in partitions(n):
        c = edge_orbits_from_cycle_type(part)
        num_perms = count_perms_with_cycle_type(n, part)
        total += num_perms * (2 ** c)
    return total // factorial(n)

# Solve
answer = count_graphs_cycle_type(7)

# Verify with direct method for small n
for n in range(1, 7):
    assert count_graphs_direct(n) == count_graphs_cycle_type(n), \
        f"Mismatch at n={n}: {count_graphs_direct(n)} vs {count_graphs_cycle_type(n)}"

# OEIS A000088 verification
oeis = {0: 1, 1: 1, 2: 2, 3: 4, 4: 11, 5: 34, 6: 156, 7: 1044}
for n, expected in oeis.items():
    if n >= 1:
        assert count_graphs_cycle_type(n) == expected, f"n={n}: got {count_graphs_cycle_type(n)}"

print(answer)
